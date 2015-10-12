<?php


/**
 * @file
 *
 * This file contain the settings and forms.
 *
 */

/**
 * Setting about Sina Weibo connection under user profile.
 */
function sinaweibo_user_settings($account) {
  $weibo_account = db_select('weibo_account')
	->fields('weibo_account')
	->condition('uid', $account->uid)
	->execute()
	->fetchAssoc();
  $output = '';
  if (!$weibo_account || empty($weibo_account)) {
    $output .= drupal_render(drupal_get_form('sinaweibo_user_enable_form', $account));
  }
  else {
    $output .= t('Your account is associated with <b>@name</b>. ', array('@name' => $weibo_account['screen_name']));
    $output .= t('You can sign in with your Sina Weibo account.');
    $output .= drupal_render(drupal_get_form('sinaweibo_user_disable_form', $account));
  }
  return $output;
}

/**
 * Connect the account with Sina Weibo.
 */
function sinaweibo_user_enable_form($form, $form_state, $account) {
  $form = array();
  $form['#action'] = url('weibo/redirect', array('query' => array('token' => drupal_get_token('weibo/connect'))));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Connect with Sina Weibo'),
    '#suffix' => '<p class="description">' . t('You will be taken to Sina Weibo website in order to complete the process.') . '</p>',
  );
  return $form;
}

/**
 * Disconnect the account with Sina Weibo.
 */
function sinaweibo_user_disable_form($form, $form_state, $account) {
  $form = array();
  $form['#user'] = $account;
  $form['#action'] = url('weibo/disconnect', array('query' => array('token' => drupal_get_token('weibo/disconnect/'. $account->uid))));
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Disconnect this account'),
  );
  return $form;
}

/**
 * Sina Weibo consumer key and consumer secret set up.
 */
function sinaweibo_settings_form($form, &$form_state) {

  $form['weibo_consumer_key'] = array(
    '#title' => t('Consumer Key'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('weibo_consumer_key', NULL),
  );

  $form['weibo_consumer_secret'] = array(
    '#title' => t('Consumer Secret'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('weibo_consumer_secret', NULL),
  );

  $form['weibo_post_message'] = array(
    '#title' => t('Post a message to weibo when connect'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => variable_get('weibo_post_message', NULL),
  );

  $form['weibo_enable_signup'] = array(
    '#title' => t('Enable signup with weibo'),
    '#type' => 'checkbox',
    '#required' => FALSE,
    '#default_value' => variable_get('weibo_enable_signup', NULL),
  );

  $form['weibo_button_url'] = array(
    '#title' => t('Sign in button image URL'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('weibo_button_url', 'http://www.sinaimg.cn/blog/developer/wiki/240.png'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Save the Sina Weibo settings.
 */
function sinaweibo_settings_form_submit($form, &$form_state) {
  variable_set('weibo_consumer_key', $form_state['values']['weibo_consumer_key']);
  variable_set('weibo_consumer_secret', $form_state['values']['weibo_consumer_secret']);
  variable_set('weibo_post_message', $form_state['values']['weibo_post_message']);
  variable_set('weibo_enable_signup', $form_state['values']['weibo_enable_signup']);
  variable_set('weibo_button_url', $form_state['values']['weibo_button_url']);

  drupal_set_message(t('Your configuration have been saved.'));
}
