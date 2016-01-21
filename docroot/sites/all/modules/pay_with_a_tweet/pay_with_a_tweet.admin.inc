<?php

/**
 * @file
 * Admin page callbacks for the Pay with a Tweet module.
 *
 * @ingroup pay_with_a_tweet
 */

/**
 * Menu callback to show the list of pay_with_a_tweet entities availables.
 */
function pay_with_a_tweet_info() {

  $settings = variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config());

  $table = array('header' => array(), 'rows' => array(), 'empty' => '');

  $table['header'] = array(
    'id' => array('data' => t('ID'), 'field' => 'n.pid', 'sort' => 'desc'),
    'title' => array('data' => t('Title'), 'field' => 'n.title'),
    'message' => t('Message'),
    'author' => t('Author'),
    'image' => t('Image'),
    'download' => t('Download'),
    'changed' => array('data' => t('Updated'), 'field' => 'n.changed'),
    'token' => t('Token'),
    'operations' => t('Operations'),
  );

  $table['empty'] = t('There are no downloads available. !create', array('!create' => l(t('Create a download'), 'admin/pay_with_a_tweet/add')));
  $query = db_select('pay_with_a_tweet', 'n')->extend('PagerDefault')->extend('TableSort');
  $pids = $query
      ->fields('n', array('pid'))
      ->limit(50)
      ->orderByHeader($table['header'])
      ->execute()
      ->fetchCol();
  $destination = drupal_get_destination();

  if (!empty($pids) && is_array($pids)) {
    $downloads = pay_with_a_tweet_load_multiple($pids);
    foreach ($downloads as $download) {
      $image = !empty($download->image) ? file_load($download->image) : FALSE;
      $file = !empty($download->download) ? file_load($download->download) : FALSE;
      $author = $download->uid > 0 ? user_load($download->uid) : NULL;

      $table['rows'][$download->pid] = array(
        'id' => $download->pid,
        'title' => check_plain($download->title),
        'message' => check_plain($download->message),
        'author' => !empty($author) ? l($author->name, 'user/' . $author->uid) : ' ',
        'image' => $image ? theme('image', array('path' => $image->uri, 'attributes' => array('style' => 'max-width:90px'))) : '',
        'download' => $file ? $file->filename : '',
        'changed' => format_date($download->changed, 'long'),
        'token' => '[pay_with_a_tweet:pid:' . $download->pid . ']',
        'operations' => l(t('edit'), 'admin/pay_with_a_tweet/' . $download->pid . '/edit', array('query' => $destination)) . ' | '
        . l(t('delete'), 'admin/pay_with_a_tweet/' . $download->pid . '/delete', array('query' => $destination)),
      );

      if ($settings['own_blocks'] == '1') {
        $table['rows'][$download->pid]['operations'] .= ' | '
          . l(t('block'), 'admin/structure/block/manage/pay_with_a_tweet/pay-with-a-tweet-own-' . $download->pid . '/configure');
      }
    }
  }
  $return = theme('table', $table);
  return $return;
}

/**
 * Menu callback to configure pay_with_a_tweet module settings.
 */
function pay_with_a_tweet_config_form($form, $form_state) {
  global $base_url;

  $settings = variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config());

  $form['twitter_app'] = array(
    '#type' => 'fieldset',
    '#title' => t('Your Twitter App info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['twitter_app']['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Key'),
    '#required' => TRUE,
    '#default_value' => $settings['consumer_key'],
  );

  $form['twitter_app']['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Secret'),
    '#required' => TRUE,
    '#default_value' => $settings['consumer_secret'],
  );

  $form['twitter_app']['callback'] = array(
    '#markup' => t('Your callback URL is %url', array('%url' => $base_url . '/pay_with_a_tweet/callback')),
  );

  $form['pwt_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pay with a Tweet Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pwt_config']['own_blocks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a block for each download'),
    '#default_value' => $settings['own_blocks'],
  );

  $form['pwt_config']['blocks'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of extra blocks'),
    '#maxlength' => 3,
    '#size' => 3,
    '#required' => TRUE,
    '#default_value' => $settings['blocks'],
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Edit'));
  return $form;
}

/**
 * Save the pay_with_a_tweet module settings.
 */
function pay_with_a_tweet_config_form_submit($form, $form_state) {

  $pay_with_a_tweet_submission = array(
    'own_blocks' => $form_state['values']['own_blocks'],
    'blocks' => intval($form_state['values']['blocks']),
    'consumer_key' => $form_state['values']['consumer_key'],
    'consumer_secret' => $form_state['values']['consumer_secret'],
  );

  $settings = array_merge(variable_get(PAY_WITH_A_TWEET, _pay_with_a_tweet_config()), $pay_with_a_tweet_submission);
  variable_set(PAY_WITH_A_TWEET, $settings);
  drupal_set_message(t('Configuration changed successfully'), 'status');
}

/**
 * Menu callback to create a pay_with_a_tweet entity.
 */
function pay_with_a_tweet_add() {
  $pay_with_a_tweet = (object) array(
    'pid' => '',
    'type' => 'pay_with_a_tweet',
    'title' => '',
    'message' => '',
    'download' => '',
    'image' => '',
    'uid' => 0,
    'created' => 0,
    'changed' => 0,
  );

  return drupal_get_form('pay_with_a_tweet_form', $pay_with_a_tweet);
}

/**
 * Create/Update pay_with_a_tweet entity form.
 */
function pay_with_a_tweet_form($form, &$form_state, $pay_with_a_tweet) {
  global $user;

  $pay_with_a_tweet->name = user_load($pay_with_a_tweet->uid);
  if (empty($pay_with_a_tweet->pid)) {
    $pay_with_a_tweet->name = $user;
  }

  $form['pay_with_a_tweet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pay with a tweet'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['pay_with_a_tweet']['pid'] = array(
    '#type' => 'value',
    '#value' => $pay_with_a_tweet->pid,
  );

  $form['pay_with_a_tweet']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $pay_with_a_tweet->title,
    '#required' => TRUE,
  );

  $form['pay_with_a_tweet']['message'] = array(
    '#title' => t('Message'),
    '#type' => 'textarea',
    '#description' => t('The comment will be unpublished if it contains any of the phrases above. Use a case-sensitive, comma-separated list of phrases. Example: funny, bungee jumping, "Company, Inc."'),
    '#default_value' => $pay_with_a_tweet->message,
    '#required' => TRUE,
  );

  $form['pay_with_a_tweet']['image'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#description' => t('The uploaded image will be displayed on this page using the image style choosen below.'),
    '#default_value' => !empty($pay_with_a_tweet->image) ? $pay_with_a_tweet->image : '',
    '#upload_location' => 'public://pay_with_a_tweet/images/',
  );

  $form['pay_with_a_tweet']['download'] = array(
    '#title' => t('Download'),
    '#type' => 'managed_file',
    '#description' => t('The uploaded file will be displayed on this page using the image style choosen below.'),
    '#default_value' => !empty($pay_with_a_tweet->download) ? $pay_with_a_tweet->download : '',
    '#upload_location' => 'public://pay_with_a_tweet/downloads/',
    '#required' => TRUE,
  );

  // Node author information for administrators.
  $form['author'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authoring information'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => array('node-form-author'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'node') . '/node.js',
        array(
          'type' => 'setting',
          'data' => array('anonymous' => variable_get('anonymous', t('Anonymous'))),
        ),
      ),
    ),
  );

  $form['author']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored by'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => !empty($pay_with_a_tweet->name->name) ? $pay_with_a_tweet->name->name : '',
    '#weight' => -1,
    '#description' => t('Leave blank for %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous')))),
  );

  $array_date = array(
    '%time' => !empty($pay_with_a_tweet->changed) ? format_date($pay_with_a_tweet->changed, 'custom', 'Y-m-d H:i:s O') : format_date(time(), 'custom', 'Y-m-d H:i:s O'),
    '%timezone' => !empty($pay_with_a_tweet->date) ? format_date($pay_with_a_tweet->date, 'custom', 'O') : format_date(time(), 'custom', 'O'),
  );

  $form['author']['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Authored on'),
    '#maxlength' => 25,
    '#description' => t('Format: %time. The date format is YYYY-MM-DD and %timezone is the time zone offset from UTC. Leave blank to use the time of form submission.', $array_date),
    '#default_value' => !empty($pay_with_a_tweet->changed) ? format_date($pay_with_a_tweet->changed, 'custom', 'Y-m-d H:i:s O') : format_date(time(), 'custom', 'Y-m-d H:i:s O'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  /* @TODO future support to extra fields */
  field_attach_form('pay_with_a_tweet', $pay_with_a_tweet, $form, $form_state);

  return $form;
}

/**
 * Validate a Create/Update pay_with_a_tweet entity form.
 */
function pay_with_a_tweet_form_validate($form, &$form_state) {
  $pay_with_a_tweet_submission = (object) $form_state['values'];
  /* @TODO future support to extra fields */
  field_attach_form_validate('pay_with_a_tweet', $pay_with_a_tweet_submission, $form, $form_state);
}

/**
 * Save a Create/Update pay_with_a_tweet entity form.
 */
function pay_with_a_tweet_form_submit($form, &$form_state) {
  $pay_with_a_tweet_submission = (object) $form_state['values'];
  if (!empty($pay_with_a_tweet_submission->pid)) {
    $pay_with_a_tweet = pay_with_a_tweet_load($pay_with_a_tweet_submission->pid);
    $pay_with_a_tweet->is_new = FALSE;
  }
  else {
    $pay_with_a_tweet = new stdClass();
    $pay_with_a_tweet->pid = '';
    $pay_with_a_tweet->is_new = TRUE;
  }
  $author = user_load_by_name($pay_with_a_tweet_submission->name);
  $date = date_create($pay_with_a_tweet_submission->date);

  $pay_with_a_tweet->uid = intval($author->uid);
  $pay_with_a_tweet->title = $pay_with_a_tweet_submission->title;
  $pay_with_a_tweet->message = $pay_with_a_tweet_submission->message;
  $pay_with_a_tweet->download = $pay_with_a_tweet_submission->download;
  $pay_with_a_tweet->image = $pay_with_a_tweet_submission->image;
  $pay_with_a_tweet->changed = $date->getTimestamp();

  if (!isset($pay_with_a_tweet->created) || empty($pay_with_a_tweet->created)) {
    $pay_with_a_tweet->created = $pay_with_a_tweet->changed;
  }
  /* @TODO future support to extra fields */
  field_attach_submit('pay_with_a_tweet', $pay_with_a_tweet_submission, $form, $form_state);
  $pay_with_a_tweet_saved = pay_with_a_tweet_save($pay_with_a_tweet);

  if ($pay_with_a_tweet->is_new || $pay_with_a_tweet->image != $pay_with_a_tweet_saved->image) {
    if (!empty($pay_with_a_tweet_saved->image)) {
      $file = file_load($pay_with_a_tweet_saved->image);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
    }
    if (!$pay_with_a_tweet->is_new) {
      $file = file_load($pay_with_a_tweet->image);
      file_usage_delete($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
      file_delete($file);
    }
  }

  if ($pay_with_a_tweet->is_new || $pay_with_a_tweet->download != $pay_with_a_tweet_saved->download) {
    if (!empty($pay_with_a_tweet_saved->download)) {
      $file = file_load($pay_with_a_tweet_saved->download);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
    }
    if (!$pay_with_a_tweet->is_new) {
      $file = file_load($pay_with_a_tweet->download);
      file_usage_delete($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
      file_delete($file);
    }
  }

  if (empty($pay_with_a_tweet->pid)) {
    drupal_set_message(t('Download %title created!', array('%title' => $pay_with_a_tweet_saved->title)), 'status');
  }
  else {
    drupal_set_message(t('Download %title changed!', array('%title' => $pay_with_a_tweet_saved->title)), 'status');
  }
  $form_state['redirect'] = "admin/pay_with_a_tweet";
}

/**
 * Delete pay_with_a_tweet entity form.
 */
function pay_with_a_tweet_delete_form($form, &$form_state, $pay_with_a_tweet) {
  global $user;

  $form['pay_with_a_tweet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pay with a tweet'),
    '#collapsible' => FALSE,
    '#description' => 'texto borrar',
  );

  $form['pay_with_a_tweet']['pid'] = array(
    '#type' => 'value',
    '#value' => $pay_with_a_tweet->pid,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/pay_with_a_tweet'),
  );

  return $form;
}

/**
 * Delete pay_with_a_tweet entity form submit.
 */
function pay_with_a_tweet_delete_form_submit($form, &$form_state) {

  $pay_with_a_tweet = pay_with_a_tweet_load($form_state['values']['pid']);
  if (pay_with_a_tweet_delete($pay_with_a_tweet)) {
    if (!empty($pay_with_a_tweet->image)) {
      $file = file_load($pay_with_a_tweet->image);
      if (!empty($file)) {
        file_usage_delete($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
        file_delete($file);
      }
    }

    if (!empty($pay_with_a_tweet->download)) {
      $file = file_load($pay_with_a_tweet->download);
      if (!empty($file)) {
        file_usage_delete($file, 'file', 'pay_with_a_tweet', $pay_with_a_tweet->pid);
        file_delete($file);
      }
    }
    drupal_set_message(t('Download %title deleted!', array('%title' => $pay_with_a_tweet->title)), 'status');
  }
  else {
    drupal_set_message(t('Error deleting %title', array('%title' => $pay_with_a_tweet->title)), 'error');
  }

  $form_state['redirect'] = "admin/pay_with_a_tweet";
}
