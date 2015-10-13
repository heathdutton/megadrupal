<?php
/**
 * @file
 * Cheetahmail user pages
 */

require_once 'cheetahmail.api.inc';

/**
 * Builder cheetahmail_subscribe_form Form.
 */
function cheetahmail_subscribe_form() {
  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#description' => 'Your email address for receiving Newsletters',
    '#size' => '20',
    '#required' => TRUE,
  );

  $form['actions']['subscribe'] = array(
    '#type' => 'submit',
    '#value' => 'Subscribe',
  );

  return $form;
}

/**
 * Validation callback for cheetahmail_subscribe_form Form.
 */
function cheetahmail_subscribe_form_validate(&$form, &$form_state) {
  $valid_email = $form_state['values']['email'];
  if (!valid_email_address($valid_email)) {
    form_set_error('email', 'Please, provide a valid email address.');
  }
}

/**
 * Submit callback for cheetahmail_subscribe_form Form.
 */
function cheetahmail_subscribe_form_submit(&$form, &$form_state) {
  $post_data = $form_state['values'];

  $subscriber_id = cheetahmail_api_get_helper()->ems_subscribers_FindByEmail($post_data['email']);
  
  if(is_array($subscriber_id))
  {
	$subscriber_id = array_shift($subscriber_id);
  }  
  
  
  if (!empty($subscriber_id) && EMS_CHEETAHMAIL_UNREGISTERED != $subscriber_id) {
    // Existing Email.
    $subscriber_info = cheetahmail_api_get_helper()->ems_subscribers_Get($subscriber_id);

    if (isset($subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) &&
      EMS_CHEETAHMAIL_SUBSCRIBED == $subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) {
      drupal_set_message(t("The email address `%email` is already subscribed.", array('%email' => $post_data['email'])), $type = 'warning');
      return;
    }
    else {
      // Change status of the existing subcriber
      // from Unsubscribed -> to Subscriber here.
      $result = cheetahmail_api_get_helper()->ems_subscribers_Update($subscriber_id, array(array(EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID, EMS_CHEETAHMAIL_SUBSCRIBED)));
      drupal_set_message(t('The email address `%email` successfully subscribed.', array('%email' => $post_data['email'])), $type = 'status');
      $sendemail_result = cheetahmail_api_get_helper()->ems_chronocontact_SendMail($chrono_id = cheetahmail_api_get_helper()->_get_subscribing_Chrono(), $subscriber_id);
    }
  }
  else {
    // New email.
    $subscriber_id = cheetahmail_api_get_helper()->ems_subscribers_Add($post_data['email']);
    if (empty($subscriber_id)) {
      drupal_set_message(t("Unknown error: Can't subscribe email address `%email`. Please try again later.", array('%email' => $post_data['email'])), $type = 'error');
    }
    else {
      $sendemail_result = cheetahmail_api_get_helper()->ems_chronocontact_SendMail($chrono_id = cheetahmail_api_get_helper()->_get_subscribing_Chrono(), $subscriber_id);
      drupal_set_message(t('The email address `%email` successfully subscribed.', array('%email' => $post_data['email'])), $type = 'status');
    }
  }

  return;
}

/**
 * Builder cheetahmail_subscribe_form Form.
 */
function cheetahmail_unsubscribe_form() {
  $form = array();

  $email = empty($_GET['email']) ? '' : $_GET['email'];

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#description' => 'Your email address for unsubscribe Newsletters',
    '#size' => '20',
    '#required' => TRUE,
    '#default_value' => $email,
  );

  $form['actions']['unsubscribe'] = array(
    '#type' => 'submit',
    '#value' => 'Unsubscribe',
    '#submit' => array('cheetahmail_unsubscribe_form_submit'),
  );

  $form['#validate'][] = 'cheetahmail_subscribe_form_validate';
  return $form;
}

/**
 * Submit Callback for cheetahmail_unsubscribe_form form
 */
function cheetahmail_unsubscribe_form_submit(&$form, &$form_state) {
  $post_data = $form_state['values'];

  $subscriber_id = cheetahmail_api_get_helper()->ems_subscribers_FindByEmail($post_data['email']);
  
  if(is_array($subscriber_id))
  {
	$subscriber_id = array_shift($subscriber_id);
  }    
  
  
  if (!empty($subscriber_id) && EMS_CHEETAHMAIL_UNREGISTERED != $subscriber_id) {
    // Existing Email.
    $subscriber_info = cheetahmail_api_get_helper()->ems_subscribers_Get($subscriber_id);
    if (isset($subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) &&
      EMS_CHEETAHMAIL_UNSUBSCRIBED == $subscriber_info->ArrayOfString[EMS_CHEETAHMAIL_SUBSCRIBER_STATUS_FIELD_ID]->string[1]) {
      drupal_set_message(t('The email address `%email` is already unsubscribed.', array('%email' => $post_data['email'])), $type = 'warning');
      return;
    }
    else {
        // Change status of the existing subcriber
        // from Unsubscribed -> to Subscriber here.
        $result = cheetahmail_api_get_helper()->ems_subscribers_Unsubscribe($subscriber_id);
        drupal_set_message(t('The email address `%email` successfully unsubscribed.', array('%email' => $post_data['email'])), $type = 'status');
        $sendemail_result = cheetahmail_api_get_helper()->ems_chronocontact_SendMail($chrono_id = cheetahmail_api_get_helper()->_get_unsubscribing_Chrono(), $subscriber_id);
        return;
    }
  }
  else {
      // New email.
      drupal_set_message(t("The email address `%email` isn't subscribed at all.", array('%email' => $post_data['email'])), $type = 'status');
  }

  return;
}
