<?php

/**
 * @file
 * Admin form settings.
 */

/**
 * Form API callback.
 */
function whatsapp_settings_form($form, &$form_state) {
  // Data for the WhatsApp registration process.
  $phone = variable_get('whatsapp_sitewide_phone', '');
  $identifier = variable_get('whatsapp_sitewide_identifier', '');
  $form['registration_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration Info'),
    '#description' => t('Information about the account to send messages from when using the sitewide account.'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($phone) && !empty($identifier),
  );
  $form['registration_info']['whatsapp_sitewide_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Telephone number'),
    '#description' => t("Telephone number including the country code without '+' or '00'. For instance: 34 666 555 444."),
    '#element_validate' => array('whatsapp_phone_validate'),
    '#default_value' => $phone,
    '#required' => TRUE,
  );
  $form['registration_info']['whatsapp_sitewide_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('IMEI'),
    '#description' => t('This is tipically the !IMEI number. If you are using an iOS device you should input your WLAN MAC address.', array('!IMEI' => l(t('IMEI'), 'http://en.wikipedia.org/wiki/IMEI'))),
    '#element_validate' => array('whatsapp_identifier_validate'),
    '#default_value' => $identifier,
    '#required' => TRUE,
  );
  $form['registration_info']['whatsapp_sitewide_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('This is the username displayed by WhatsApp clients.'),
    '#default_value' => variable_get('whatsapp_sitewide_username', variable_get('site_name')),
  );
  $form['registration_info']['whatsapp_sitewide_status'] = array(
    '#type' => 'textfield',
    '#title' => t('Status'),
    '#description' => t('Real time indication of what you are doing, so that you can keep up to date with your contacts.'),
    '#default_value' => variable_get('whatsapp_sitewide_status', 'Using Drupal WhatsApp!'),
  );
  // If we have all the info we need, show next part of the form.
  if (!empty($phone) && !empty($identifier)) {
    $wc = new WhatsappController();
    $rest = $wc->checkCredentials();
    if (!empty($rest)) {
      foreach ($rest as $key => $value) {
        if ($key == 'expiration' || $key == 'price_expiration') {
          $value = format_date($value, 'long');
        }
        $rows[] = array(
          'data' => array(
            array('data' => $key, 'header' => TRUE),
            array('data' => $value),
          ),
        );
      }
      $form['account_information'] = array(
       '#theme' => 'table',
       '#rows' => $rows,
       '#caption' => t('Account information fetched from WhatsApp'),
      );
    }
  }

  return system_settings_form($form);
}

/**
 * Element validate function.
 */
function whatsapp_phone_validate($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    $value = preg_replace("/[^\d]/", '', $element['#value']);
    $wc = new WhatsappUtils();
    $dissectPhone = $wc->dissect($value);
    if (strlen($value) > 15) {
      form_error($element, t('This phone number has too many digits.'));
    }
    else if (strlen($value) < 6) {
      form_error($element, t('This phone number has too few digits.'));
    }
    else if (!$dissectPhone) {
      form_error($element, t('This phone number is not valid.'));
    }
    else {
      $form_state['values']['whatsapp_sitewide_phone'] = $value;
      if ($element['#value'] != $value) {
        drupal_set_message(t("The phone number will be saved as %phone to fit WhatsApp's numbering format.", array('%phone' => $value)));
      }
    }
  }
}

/**
 * Element validate function.
 */
function whatsapp_identifier_validate($element, &$form_state, $form) {
  $identifier = preg_replace("/\s+/", '', strtoupper($element['#value']));
  if ((!preg_match('/^([A-F0-9]{2}:){5}[A-F0-9]{2}$/i', $identifier)) && (!ctype_digit($identifier) || strlen($identifier) != 15)) {
    form_error($element, t('This IMEI/WLAN MAC is not valid.'));
  }
  $form_state['values']['whatsapp_sitewide_identifier'] = $identifier;
}

/**
 * Form API callback.
 */
function whatsapp_account_registration_form($form, &$form_state) {
  $form['account_reg'] = array(
    '#type' => 'fieldset',
    '#title' => t('WhatsApp account registration'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['account_reg']['options'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['account_reg']['options']['request'] = array(
    '#type' => 'fieldset',
    '#title' => t('Request a registration code'),
  );
  $form['account_reg']['options']['request']['method'] = array(
    '#type' => 'select',
    '#title' => t('Select method for request a registration code from WhatsApp'),
    '#options' => array(
      '' => t('- Select -'),
      'sms' => t('Send me a SMS'),
      'voice' => t('Call me'),
    ),
  );
  $form['account_reg']['options']['request']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('whatsapp_account_registration_form_request_submit'),
    '#value' => t('Request code'),
  );
  $form['account_reg']['options']['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Register an account using the code'),
  );
  $form['account_reg']['options']['register']['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification code'),
    '#description' => t("To complete your account registration, please enter the 6-digit activation code."),
    '#size' => 10,
    '#maxlength' => 6,
  );
  $form['account_reg']['options']['register']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('whatsapp_account_registration_form_register_submit'),
    '#value' => t('Validate code'),
  );

  $form['#validate'][] = 'whatsapp_account_registration_form_validate';

  $phone = variable_get('whatsapp_sitewide_phone', '');
  $identifier = variable_get('whatsapp_sitewide_identifier', '');

  $wc = new WhatsappController();
  $rest = $wc->checkCredentials();

  if (empty($phone) && empty($identifier)) {
    $form['account_reg']['options']['request']['submit']['#disabled'] = TRUE;
    $form['account_reg']['options']['register']['submit']['#disabled'] = TRUE;
    drupal_set_message(t('First you must !configure WhatsApp.', array('!configure' => l(t('configure'), 'admin/config/services/whatsapp'))), 'error');
  }
  else if (isset($rest->param)) {
    $form['account_reg']['options']['request']['submit']['#disabled'] = TRUE;
    $form['account_reg']['options']['register']['submit']['#disabled'] = TRUE;
    drupal_set_message(t('An error occurred: %reason%param', array('%reason' => $rest->reason, '%param' => isset($rest->param)?': ' . $rest->param : '')), 'error');
    drupal_set_message(t('First you must !configure WhatsApp with a valid values.', array('!configure' => l(t('configure'), 'admin/config/services/whatsapp'))), 'error');
  }
  else if ($rest->status == 'ok') {
    $form['account_reg']['options']['request']['submit']['#disabled'] = TRUE;
    $form['account_reg']['options']['register']['submit']['#disabled'] = TRUE;
    drupal_set_message(t('Your credentials are good. No need a registration code.'));
  }


  return $form;
}

/**
 * Form validation callback for a WhatsApp account registration form.
 */
function whatsapp_account_registration_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Request code') && $form_state['values']['method'] == '') {
    form_set_error('method', t('You must select a valid method.'));
  }
  else if($form_state['values']['op'] == t('Validate code') && strlen($form_state['values']['code']) < 6) {
    form_set_error('code', t('This activation code has too few digits.'));
  }
}

/**
 * Form submit callback for a request a registration code form.
 */
function whatsapp_account_registration_form_request_submit($form, &$form_state) {

  $phone = variable_get('whatsapp_sitewide_phone', '');
  $identifier = variable_get('whatsapp_sitewide_identifier', '');

  $wc = new WhatsappController();
  $rest = $wc->requestCode($form_state['values']['method']);

  if ($rest->status == 'sent') {
    watchdog('whatsapp', 'Request a registration code from WhatsApp');
    drupal_set_message(t("WhatsApp sent you an %method with a code. To complete your account registration, please enter the 6-digit activation code.", array('%method' => $form_state['values']['method'] == 'sms' ? t('SMS') : t('Call'))));
  }
  else if ($rest->status == 'ok') {
    drupal_set_message(t('Your credentials are good. No need a registration code.'));
  }
  else {
    watchdog('whatsapp', 'An error occurred requesting the registration code from WhatsApp: %reason%param', array('%reason' => $rest->reason, '%param' => isset($rest->param)?': ' . $rest->param : ''), WATCHDOG_ERROR);
    drupal_set_message(t('An error occurred requesting the registration code from WhatsApp: %reason%param', array('%reason' => $rest->reason, '%param' => isset($rest->param)?': ' . $rest->param : '')), 'error');
    if (isset($rest->retry_after)) {
      drupal_set_message(t('Retry again after %seconds seconds.', array('%seconds' => $rest->retry_after)), 'error');
    }
  }

  unset($form_state['values']['method']);
}

/**
 * Form submit callback for a registration code form.
 */
function whatsapp_account_registration_form_register_submit($form, &$form_state) {
  $wc = new WhatsappController();
  $rest = $wc->registerCode($form_state['values']['code']);

  if ($rest->status == 'ok') {
    watchdog('whatsapp', 'The registration code has been successfully completed');
    drupal_set_message(t("The registration code has been successfully completed."));
  }
  else {
    watchdog('whatsapp', 'An error occurred registering the registration code from WhatsApp: %reason%param', array('%reason' => $rest->reason, '%param' => isset($rest->param)?': ' . $rest->param : ''), WATCHDOG_ERROR);
    drupal_set_message(t('An error occurred registering the registration code from WhatsApp: %reason%param', array('%reason' => $rest->reason, '%param' => isset($rest->param)?': ' . $rest->param : '')), 'error');
  }

  unset($form_state['values']['code']);
}

/**
 * Form API callback.
 */
function whatsapp_test_message_form($form, &$form_state) {  
  $form['destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination'),
    '#description' => t('Destination telephone number with country code.'),
    '#element_validate' => array('whatsapp_destination_phone_validate'),
    '#required' => TRUE,
  );
  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Message type'),
    '#description' => t('Select the message type you want to send.'),
    '#options' => array(
      'text' => t('Text'),
//      'image' => t('Image'),
    ),
    '#default_value' => 'text',
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Text message'),
    '#description' => t('The text message that will be sent to the destination.'),
    '#states' => array(
      'visible' => array(
        ':input[name=type]' => array('value' => 'text'),
      ),
      'required' => array(
        ':input[name=type]' => array('value' => 'text'),
      ),
    ),
  );
  $form['image'] = array(
    '#type' => 'file',
    '#title' => t('Image file'),
    '#description' => t('The image that will be sent to the destination. Allowed extensions are: jpg jpeg gif png.'),
    '#states' => array(
      'visible' => array(
        ':input[name=type]' => array('value' => 'image'),
      ),  
      'required' => array(
        ':input[name=type]' => array('value' => 'image'),
      ),  
    ),
    '#element_validate' => array('whatsapp_message_image_validate'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  return $form;
}

/**
 * Element validate function.
 */
function whatsapp_destination_phone_validate($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    $value = preg_replace("/[^\d]/", '', $element['#value']);
    $wc = new WhatsappController();
    $dissectPhone = $wc->get_container('util')->dissect($value);
    if (strlen($value) > 15) {
      form_error($element, t('This phone number has too many digits.'));
    }
    else if (strlen($value) < 6) {
      form_error($element, t('This phone number has too few digits.'));
    }
    else if (!$dissectPhone) {
      form_error($element, t('This phone number is not valid.'));
    }
    else {
      $form_state['values']['destination'] = $value;
    }
  }
}

/**
 * Validation callback for image file.
 */
function whatsapp_message_image_validate($element, &$form_state, $form) {
  $file = file_save_upload('image', array(
    'file_validate_extensions' => array('jpg jpeg gif png'),
  ));
  // If the file passed validation:
  if ($file) {
    // Move the file, into the Drupal file system
    $uri = 'temporary://whatsapp';
    if (!file_prepare_directory($uri)) {
      drupal_mkdir($uri);
    }
    if ($file = file_move($file, $uri)) {
      // Save the file for use in the submit handler.
      $form_state['storage']['image'] = $file;
    }
    else {
      form_error($element, t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
}

/**
 * Submit callback to actually send the message.
 */
function whatsapp_test_message_form_submit($form, &$form_state) {
  $wc = new WhatsappController();
  $wc
    ->setDebug(TRUE)
    ->setCountryCode('ES')
    ->setLangCode('es')
    ->init();
  $success = FALSE;
  $wc
    ->setDestination($form_state['values']['destination']);
  switch ($form_state['values']['type']) {
    case 'image':
      if (isset($form_state['storage']) && isset($form_state['storage']['image'])) {
        $file = $form_state['storage']['image'];
        // We are done with the file, remove it from storage.
        unset($form_state['storage']['image']);
        $success = $wc->sendImage(image_load($file->uri));;
      }
      break;
    
    default:
      $success = $wc->sendMessage($form_state['values']['message']);
      break;
  }
  if ($success) {
    $wc->WaitforReceipt();
    drupal_set_message(t('Your message has been sent.'));
  }
}
