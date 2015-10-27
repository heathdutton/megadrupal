<?php

/**
 * @file
 * Page callback for gmail contact invite page.
 */

/**
 * Invite page for gmail.
 *
 * @return string
 */
function gmail_contact_invite_page() {
  if (isset($_GET["code"])) {
    // Store auth code in session.
    $_SESSION['gmail_auth_code'] = $_GET['code'];

    // Redirect to gmail invite form.
    drupal_goto('gmail-invite');
  }
  else {
    return t('You are not supposed to be on this page.');
  }

}

/**
 * @param $form
 * @param $form_state
 * @return mixed
 */
function gmail_contact_invite_form($form, &$form_state) {
  // Use the contact options from session if this form is not rendered
  // initially. E.g, this form is rendered after validation fails.
  if (isset($_SESSION['gmail_contact_options'])) {
    $options = $_SESSION['gmail_contact_options'];
  }
  else {
    if (!isset($_SESSION['gmail_auth_code'])) {
      $form['text'] = array(
        '#markup' => t('You are not supposed to be on this page.'),
      );
      return $form;
    }

    $auth_code = $_SESSION['gmail_auth_code'];
    $xmlresponse = gmail_contact_get_gmail_contacts($auth_code);
    $contacts = gmail_contact_parse_gmail_contacts($xmlresponse);

    /*$contacts = array(
      array('email' => '22@e.c', 'name' => 'Rulin er'),
      array('email' => '22@e.cs', 'name' => 'Bulin er'),
      array('email' => '22@e.ce', 'name' => 'Aulin aa'),
      array('email' => '22@e.cess', 'name' => 'amlin ac'),
    );*/

    // Display message to user if no contacts found.
    if (empty($contacts)) {
      $message = t("It seems your account doesn't have contacts at this moment.");
      if (variable_get('gmail_contact_name_required', '')) {
        $message .= t("<br>Or all your contacts don't have name associated.
        Please note if one contact doesn't have name, it will not display here.");
      }
      $form['message'] = array(
        '#markup' => $message,
      );

      return $form;
    }

    $options = array();
    $uc = variable_get('gmail_contact_capitalize_name', '');
    foreach ($contacts as $contact) {
      if ($uc) {
        $options[$contact['email']] = ucfirst($contact['name']);
      }
      else {
        $options[$contact['email']] = $contact['name'];
      }
    }

    $sort = variable_get('gmail_contact_sort', '');
    if ($sort) {
      asort($options);
    }

    // Store options in session, so we can use it later, especially when
    // validation fails.
    //
    $_SESSION['gmail_contact_options'] = $options;
  }

  $form['contacts'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function gmail_contact_invite_form_validate($form, &$form_state) {

  $emails = array();
  $all_contacts = $form_state['values']['contacts'];
  foreach ($all_contacts as $contact) {
    if ($contact) {
      $emails[] = $contact;
    }
  }

  if (empty($emails)) {
    $msg = t('You should select at least one contact to proceed.');
    form_set_error('contacts', $msg);
    return;
  }

  // Save valid emails.
  $form_state['emails'] = $emails;
}

function gmail_contact_invite_form_submit($form, &$form_state) {
  global $user, $language;

  $site_mail = variable_get('site_mail', $user->mail);
  $from = variable_get('gmail_contact_email_address', $site_mail);

  if (module_exists('token')) {
    $from = token_replace($from);
  }

  $emails = $form_state['emails'];

  // Add those email work to drupal queue.
  if (variable_get('gmail_contact_queue_send', '')) {
    $queue = DrupalQueue::get('gmail_contact_invites');
  }

  foreach ($emails as $email) {

    if (variable_get('gmail_contact_queue_send', '')) {
      // Use drupal queue to send emails.
      // Set variables, and add email work to drupal queue.
      $vars = array();
      $vars['email'] = $email;
      $vars['from'] = $from;
      $queue->createItem($vars);
    }
    else {
      $result = drupal_mail('gmail_contact', 'invite', $email, $language, array(), $from, TRUE);
      if ($result) {
        watchdog('gmail_contact', 'Successfully send e-mail %to).', array('%to' => $email));
      }
    }
  }

  if (variable_get('gmail_contact_queue_send', '')) {
    $message = t('Your invitation will be sent shortly.');
  }
  else {
    $message = t('Your invitation has been sent successfully!');
  }
  drupal_set_message($message);

  $form_state['redirect'] = "<front>";

  // Remove contact options from session, once one request is done. We should
  // not leave these data in session for long time.
  if (isset($_SESSION['gmail_contact_options'])) {
    unset($_SESSION['gmail_contact_options']);
  }
}
