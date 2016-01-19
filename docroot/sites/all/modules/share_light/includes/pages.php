<?php

use \Drupal\share_light\Email;

/**
 * Get all default values as array.
 */
function share_light_email_defaults() {
  $defaults = array(
    'share_light_email_page_title' => variable_get('share_light_email_page_title', t('Forward this page to a friend')),
    'share_light_email_page_noindex' => variable_get('share_light_email_page_noindex', TRUE),
    'share_light_email_page_instructions' => variable_get('share_light_email_page_instructions', ''),
    'share_light_email_page_redirect' => variable_get('share_light_email_page_redirect', ''),
    'share_light_email_message_edit' => variable_get('share_light_email_message_edit', FALSE),
    'share_light_email_message_subject' => variable_get('share_light_email_message_subject', t('[share:sender] has forwarded a page to you from [site:name]')),
    'share_light_email_message_message' => variable_get('share_light_email_message_message', t('[share:sender] thought you would like to see this page from the [site:name] web site.')),
    'share_light_email_message_footer' => variable_get('share_light_email_message_footer', t('Visit our site on [share:url].')),
    'share_light_email_flood_control' => variable_get('share_light_email_flood_control', 100),
    'share_light_email_flood_error' => variable_get('share_light_email_flood_error', t("You can't send more than !number messages per hour. Please try again later.")),
    'share_light_email_flood_clicks' => variable_get('share_light_email_flood_clicks', 3),
  );
  return $defaults;
}

function share_light_email_settings($node = NULL) {
  $settings = share_light_email_settings_from_node($node) + share_light_email_defaults();
  return $settings;
}

function share_light_email_settings_from_node($node = NULL) {
  if ($node) {
    $sql = 'SELECT * FROM {share_light_email_settings} WHERE nid=:nid';
    $settings = array();
    if ($row = db_query($sql, array(':nid' => $node->nid))->fetch()) {
      foreach ($row as $key => $value) {
        if (!empty($value) || $key == 'message_edit') {
          $settings['share_light_email_' . $key] = $value;
        }
      }
      return $settings;
    }
  }
  return array();
}

/**
 * Page callback for node/%/share.
 *
 * Display a page specific share page.
 */
function share_light_node_email_page($node) {
  // Tell SEO to ignore this page (but don't generate the meta tag for an overlay)
  if (variable_get('share_light_page_noindex', 1)) {
    $element = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' =>  'robots',
        'content' => 'noindex, nofollow',
      )
    );
    drupal_add_html_head($element, 'share_light_email_noindex');
  }

  $defaults = share_light_email_settings($node);
  drupal_set_title(token_replace($defaults['share_light_email_page_title'], array('node' => $node)));
  $form = drupal_get_form('share_light_node_email_form', $node, $defaults);
  $form['#attributes']['class'][] = 'share-page';
  return $form;
}

/**
 * Form call-back: The share form.
 */
function share_light_node_email_form($form, &$form_state, $node, $defaults) {
  $form_state['node'] = $node;
  $form_state['defaults'] = &$defaults;

  $tokenData = array(
    'node' => $node,
  );
  $edit_message = !empty($defaults['share_light_email_message_edit']);
  
  $form['instructions'] = array(
    '#type' => 'item',
    '#markup' => check_plain(token_replace($defaults['share_light_email_page_instructions'])),
  );
  $form['firstname'] = array(
    '#type' => 'textfield',
    '#title' => t('First name'),
    '#size' => 58,
    '#maxlength' => 254,
    '#required' => TRUE,
  );
  $form['lastname'] = array(
    '#type' => 'textfield',
    '#title' => t('Last name'),
    '#size' => 58,
    '#maxlength' => 254,
    '#required' => TRUE,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Email'),
    '#size' => 58,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['recipients'] = array(
    '#type' => 'textarea',
    '#title' => t('Send To'),
    '#cols' => 50,
    '#rows' => 5,
    '#description' => t('Enter multiple addresses on separate lines or separate them with commas.'),
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => token_replace($defaults['share_light_email_message_subject'], $tokenData),
    '#description' => '',
    '#disabled' => !$edit_message,
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#cols' => 50,
    '#rows' => 5,
    '#default_value' => token_replace($defaults['share_light_email_message_message'], $tokenData),
    '#description' => '',
    '#disabled' => !$edit_message,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Message'),
  );

  return $form;
}

/*
 * Create a receipients list and token
 */
function share_light_recipient_list($form_state) {
  // Process variables for use as tokens.
  $recipients = str_replace(array("\r\n", "\n", "\r"), ',', $form_state['values']['recipients']);
  $r = array();
  foreach (explode(',', $recipients) as $recipient) {
    if (trim($recipient) != '') {
      $r[trim($recipient)] = TRUE;
    }
  }
  $recipients = array_keys($r);
  $token = array(
    'sender' => implode(' ', array($form_state['values']['firstname'], $form_state['values']['lastname'])),
    'recipients' => $recipients,
  );
  return array('recipients' => $recipients, 'token' => $token);
}

/**
 * Get share URL.
 */
function share_light_email_url($node) {
  $query = array('s' => 'share_email');
  $path = 'node/' . $node->nid;

  if (!empty($_GET['hash'])) {
    $get = array();
    if (isset($_GET['path'])) {
      $get['path'] = $_GET['path'];
    }
    if (isset($_GET['query'])) {
      $get['query'] = $_GET['query'];
    }
    if ($_GET['hash'] == Email::signQuery($get)) {
      if (!empty($get['path'])) {
        $path = $get['path'];
      }
      if (!empty($get['query'])) {
        $query += $get['query'];
      }
    }
  }

  return url($path, array('query' => $query, 'absolute' => TRUE));
}

/**
 * Validation callback function for share_light_node_email_form().
 */
function share_light_node_email_form_validate($form, &$form_state) {
  // normalize address entries
  $tokens = share_light_recipient_list($form_state);
  $recipient_addresses = $tokens['recipients'];

  $bad_items = array('Content-Type:', 'MIME-Version:', 'Content-Transfer-Encoding:', 'bcc:', 'cc:');
  $bad_string = FALSE;
  foreach ($bad_items as $item) {
    if (preg_match("/$item/i", $form_state['values']['email'])) {
      $bad_string = TRUE;
    }
  }
  if (strpos($form_state['values']['email'], "\r") !== FALSE || strpos($form_state['values']['email'], "\n") !== FALSE || $bad_string == TRUE) {
    form_set_error('email', t('Header injection attempt detected.  Do not enter line feed characters into the from field!'));
  }
  if (user_validate_mail($form_state['values']['email'])) {
    form_set_error('email', t('Your Email address is invalid.'));
  }
  if (!$form_state['values']['firstname']) {
    form_set_error('firstname', t('You must enter your first name.'));
  }
  if (!$form_state['values']['lastname']) {
    form_set_error('lastname', t('You must enter your last name.'));
  }
  if (!$form_state['values']['subject']) {
    form_set_error('subject', t('You must enter a subject.'));
  }
  if (!$form_state['values']['message']) {
    form_set_error('subject', t('You must enter message.'));
  }
  if (empty($recipient_addresses)) {
    form_set_error('recipients', t('You did not enter any recipients.'));
  }
  else {
    foreach ($recipient_addresses as $address) {
      if ((user_validate_mail($address)) && ($address != '')) {
        form_set_error('recipients', t('One of your Recipient addresses is invalid:') . '<br />' . check_plain($address));
      }
    }
  }
  if (!user_access('override flood control')) {
    // Check if it looks like we are going to exceed the flood limit.
    // It is important to ensure that the number of e-mails to be sent count against the threshold.
    if (!flood_is_allowed('share_light_email', variable_get('share_light_email_flood_control', 10) - count($recipient_addresses) + 1)) {
      form_set_error('recipients', check_plain(t(variable_get('share_light_email_flood_error', "You can't send more than !number messages per hour. Please try again later."), array('!number' => variable_get('share_light_email_flood_control', 10)))));
    }
  }
}


/**
 * Send share email
 */
function share_light_node_email_form_submit($form, &$form_state) {
  $node = $form_state['node'];
  $defaults = &$form_state['defaults'];
  $values = &$form_state['values'];
  $recipient_list = share_light_recipient_list($form_state);
  $recipients = $recipient_list['recipients'];
  $tokens['share'] = $recipient_list['token'];
  $tokens['node'] = $node;

  // generate name from firstname and lastname, name used in different occasions in this file
  $values['sender'] = $tokens['share']['sender'];
  $values['footer'] = token_replace($defaults['share_light_email_message_footer'], $tokens);
  $tokens['share']['url'] = $values['url'] = share_light_email_url($node);

  $message = token_replace(theme('share_light_message_body', array(
    'node' => $node,
    'values' => $values,
  )), $tokens);
  $email['message'] = $message;

  // Subject
  $email['subject'] = token_replace(check_plain($values['subject']), $tokens);

  // we want to From: and Reply-to: to be the senders email
  $email['from'] = $values['sender'] . ' <' . $values['email'] . '>';
  $email['headers']['Reply-To'] = $values['sender'] . ' <' . $values['email'] . '>';

  $mail_params = array(
    'message' => $email['message'],
    'subject' => $email['subject'],
    'headers' => $email['headers'],
    'node' => $node,
    'email' => $email,
  );
  $mail_params['headers']['Content-Type'] = 'text/html; charset=UTF-8';
  $mail_params['plaintext'] = NULL;
  $mail_params['plain'] = FALSE;

  foreach ($recipients as $to) {
    drupal_mail('share_light', 'share_light_page', trim($to), $GLOBALS['language'], $mail_params, $email['from']);
    // Flood control
    flood_register_event('share_light_email');
  }

  module_invoke_all('share_light_email_send', $node, $values);

  if ($redirect = variable_get('share_light_page_redirect')) {
    $form_state['redirect'] = $redirect;
  } else {
    $form_state['redirect'] = 'node/' . $node->nid;
    drupal_set_message(t('Thank you for sharing. You are awesome!'));
  }
}
