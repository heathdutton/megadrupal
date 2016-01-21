<?php

/**
 * @file
 * Functions and forms for the frontend part of newsletter module.
 */


/*
 * Menu callback; Lets users manage their subscriptions.
 */
function newsletter_manage_subscriptions_form($form, &$form_state, $user = NULL) {
  if (!isset($user)) {
    global $user;
    $subscriber = entity_get_controller('newsletter_subscriber')->create();
    $subscriber->email = isset($_GET['email']) ? check_plain($_GET['email']) : @$user->mail;

  }
  elseif (arg(0) == 'newsletter' && arg(1) == 'edit') {
    unset($user);
    global $user;
    $subscribers = newsletter_subscriber_load(array(), array('hash' => arg(2)));
    $subscriber = !empty($subscribers) ? array_pop($subscribers) : entity_get_controller('newsletter_subscriber')->create();
  }
  else {
    $subscribers = isset($user->uid) ? newsletter_subscriber_load(array(), array('uid' => $user->uid)) : array();
    $subscriber = !empty($subscribers) ? array_pop($subscribers) : entity_get_controller('newsletter_subscriber')->create();
  }
  newsletter_add_js();

  $show_email = variable_get('newsletter_show_email', TRUE);

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => isset($user->uid) ? $user->uid : NULL,
  );
  $form['nsid'] = array(
    '#type' => 'hidden',
    '#value' => isset($subscriber->nsid) ? $subscriber->nsid : NULL,
  );
  if ($show_email) {
    $form['mail'] = array(
      '#attributes' => array('class' => array('container-inline')),
      '#type' => 'fieldset',
      '#title' => t('E-mail'),
      '#required' => TRUE,
      '#weight' => -10,
    );
  }
  $form['mail']['email'] = array(
    '#type' => $show_email ? 'textfield' : 'hidden',
    '#default_value' => !empty($subscriber->email) ? $subscriber->email : @$user->mail,
    '#required' => TRUE,
    '#size' => 20,
  );
  $personal_info_form = variable_get('newsletter_personal_info_form');
  if ( in_array('show', $personal_info_form) || in_array('require', $personal_info_form) ) {
    $form['info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Personal info'),
      '#weight' => 5,
    );
  }

  foreach ($personal_info_form as $info_type => $value) {
    if ($value != 'hidden') {
      switch ($info_type) {
        case 'firstname':
          $form['info']['firstname'] = array(
            '#type' => 'textfield',
            '#title' => t('First name'),
            '#size' => 40,
            '#required' => (bool) ($value == 'require'),
            '#default_value' => $subscriber->firstname,
          );
          break;
        case 'lastname':
          $form['info']['lastname'] = array(
            '#type' => 'textfield',
            '#title' => t('Last name'),
            '#size' => 40,
            '#required' => (bool) ($value == 'require'),
            '#default_value' => $subscriber->lastname,
          );
          break;
        case 'gender':
          $form['info']['gender'] = array(
            '#type' => 'select',
            '#title' => t('Gender'),
            '#options' => array(
              'male' => t('Male'),
              'female' => t('Female'),
              'other' => t('Other'),
            ),
            '#empty_option' =>  t('- Select -'),
            '#required' => (bool) ($value == 'require'),
            '#default_value' => $subscriber->gender,
          );
          break;
        case 'receive_format':
          $form['info']['receive_format'] = array(
            '#type' => 'select',
            '#title' => t('Preferred format'),
            '#options' => array(
              'html' => t('HTML'),
              'plain' => t('Plain Text'),
            ),
            '#empty_option' =>  t('- Select -'),
            '#required' => (bool) ($value == 'require'),
            '#default_value' => $subscriber->receive_format,
          );
          break;
      }
    }
  }
  field_attach_form('newsletter_subscriber', $subscriber, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($subscriber->nsid) ?  t('Update subscription') : t('Subscribe'),
    '#weight' => 10,
  );

  if (isset($subscriber->nsid)) {
    $form['unsubscribe'] = array(
      '#type' => 'link',
      '#title' => t('Unsubscribe'),
      '#href' => 'newsletter/unsubscribe/' . $subscriber->hash,
      '#weight' => 10,
    );
  }

  return $form;
}

function newsletter_manage_subscriptions_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t("This e-mail doesn't exist."));
  }
}

function newsletter_manage_subscriptions_form_submit($form, &$form_state) {
  $needs_confirm = variable_get('newsletter_send_confirm');
  $subscriber = (object) $form_state['values'];
  if (empty($form_state['values']['nsid'])) {
    unset($subscriber->nsid);
  }
  else {
    $needs_confirm = FALSE;
  }
  if (empty($form_state['values']['uid'])) {
    $subscriber->uid = NULL;
  }

  field_attach_submit('newsletter_subscriber', $subscriber, $form, $form_state);
  entity_get_controller('newsletter_subscriber')->save($subscriber, $needs_confirm);
}

/**
 * Confirms a subscription to a newsletter list.
 */
function newsletter_confirm($subscriber) {

  if ($subscriber && !$subscriber->confirmed) {
    $subscriber->confirmed = 1;
    $subscriber->confirmation_timestamp = REQUEST_TIME;
    entity_get_controller('newsletter_subscriber')->save($subscriber);

    if (variable_get('newsletter_send_welcome', FALSE)) {
      newsletter_create()->sendBasic(2, $subscriber->email);
    }
    return t('You have successfully confirmed your subscription!');
  }
  return t('The hash you provided is invalid or outdated');
}

/**
 * Unsubscribes a subscriber from newsletter.
 */
function newsletter_unsubscribe($form, &$form_state, $subscriber) {

  if ($subscriber) {
    $form['#subscriber'] = $subscriber;
    return confirm_form($form, t('Are you sure you want to unsubscribe from our newsletters?'), '<front>');
  }

  drupal_set_message(t('The hash you provided is invalid or outdated'), 'warning');
  drupal_goto();
}

function newsletter_unsubscribe_submit($form, &$form_state) {
  $subscriber = $form['#subscriber'];
  if (variable_get('newsletter_send_unsubscribe', FALSE)) {
    newsletter_create()->sendBasic(3, $subscriber->email);
  }
  entity_get_controller('newsletter_subscriber')->delete($subscriber->nsid);
  drupal_set_message(t('You have successfully unsubscribed from our newsletter'));
  $form_state['redirect'] = '<front>';
}

/*
 * Updates the opened newsletters.
 */
function newsletter_statistics_open($subscriber) {

  if (isset($_GET['source']) && $_GET['source'] == 'newsletter' && $subscriber) {
    $newsletter = newsletter_newsletter_load($_GET['nnid']);
    $newsletter->opens++;

    entity_get_controller('newsletter_newsletter')->save($newsletter);
  }
}
