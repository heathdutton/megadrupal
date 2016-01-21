<?php
/**
 * @file
 * This file handles the composing of a message.
 */

/**
 * Construct the form for composing an email message.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The state of the form.
 * @param string $send_mail_type
 *   Whether the message is new, a reply or a forward. Possible values:
 *   - "new": All form fields are left blank.
 *   - "reply": The To form field is filled in with the from address of the
 *     original message. The Subject and Body form fields are filled in with
 *     the corresponding fields from the original message.
 *   - "replytoall": The To form field is filled in with the from address of
 *     the original message. The CC form field is filled in with all of the
 *     addresses in the to and cc fields of the original message. The Subject
 *     and Body form fields are filled in with the corresponding fields from
 *     the original message.
 *   - "forward": The To, CC and BCC form fields are blank. The Subject and
 *     Body form fields are filled in with the corresponding fields from the
 *     original message.
 *   - "continue": The contents of the form are filled in by the message in
 *     the Drafts folder that is being continued.
 *   - An email address: The To form field gets filled in with the value in
 *     $send_mail_type, and all other form fields are left blank.
 * @param string $send_mail_folder
 *   The folder the message is in.
 * @param integer $send_mail_message_number
 *   The ID of the message being replied to or forwarded, blank otherwise.
 *
 * @return array
 *   The completed form definition.
 */
function _basic_webmail_send_mail_form($form, &$form_state, $send_mail_type = 'new', $send_mail_folder = '', $send_mail_message_number = '') {
  $send_mail_to = '';
  $send_mail_cc = '';
  $send_mail_bcc = '';
  $send_mail_subject = '';
  $send_mail_body = '';
  $attachments = array();

  // Check to see if this is a reply or forward.
  if ($send_mail_type == 'reply' || $send_mail_type == 'replytoall' || $send_mail_type == 'forward' || $send_mail_type == 'continue') {
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    // Make sure we have a message id.
    if (empty($send_mail_message_number)) {
      $message = 'There was an attempt to reply to or forward a message w/o including the message id.';
      _basic_webmail_report_error($message);
    }
    // Get the message information for use in filling the form.
    else {
      // Connect to the server and retrieve a connection to the mailbox.
      global $user;
      $imap_resource = _basic_webmail_connect_to_server($user->uid, $send_mail_folder);
      if (empty($imap_resource)) {
        $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
        _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
        return;
      }

      // Show the message.

      // Get the message header.
      $message_header = imap_headerinfo($imap_resource, $send_mail_message_number);
      if (empty($message_header)) {
        $message = 'There was an error retrieving the message header.';
        _basic_webmail_report_error($message);
      }

      // Get the message body.
      // @todo verify the arg is not 6.
      $message_body = imap_fetchbody($imap_resource, $send_mail_message_number, arg(5));
      if (empty($message_body)) {
        $message = 'There was an error retrieving the message body.';
        _basic_webmail_report_error($message);
      }

      include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';

      // Set the title of the page.
      drupal_set_title(t('@title', array('@title' => _basic_webmail_prepare_email_string($message_header->subject))));

      // Add a link back to the listing.
      if (empty($send_mail_message_number)) {
        $form['basic_webmail_send_mail_list_link'] = array(
          '#value' => l(t('Return to message listing'), 'basic_webmail/' . $send_mail_folder) . '<hr>',
        );
      }
      else {
        $form['basic_webmail_send_mail_list_link'] = array(
          '#value' => l(t('Return to message listing'), 'basic_webmail/' . $send_mail_folder) . '&nbsp; -- &nbsp;' . l(t('Return to message'), 'basic_webmail/view/' . $send_mail_message_number . '/' . $send_mail_folder) . '<hr>',
        );
      }

      // Set defaults based on $send_mail_type.
      if ($send_mail_type == 'reply' || $send_mail_type == 'replytoall' || $send_mail_type == 'forward' || $send_mail_type == 'continue') {
        // Get the message parts list.
        $parts_list = _basic_webmail_get_parts_list($imap_resource, $send_mail_message_number);

        if (module_exists('mimemail')) {
          // Iterate over the parts list.
          foreach ($parts_list as $part_id => $part_array) {
            // If there's an attachment, give the user the option of including
            // it in their message.
            if (!empty($part_array[0]) && !empty($part_array[1])) {
              $attachment = _basic_webmail_process_attachment($imap_resource, $send_mail_message_number, $part_id, $part_array[1]);
              $attachments[$attachment[0] . '/' . $attachment[1]] = $attachment[1];
            }
          }
        }
      }

      // Set defaults based on $send_mail_type.
      if ($send_mail_type == 'reply' || $send_mail_type == 'replytoall') {
        if (!empty($message_header->Msgno)) {
          $form['basic_webmail_message_number'] = array(
            '#type' => 'hidden',
            '#value' => $message_header->Msgno,
          );
        }
        if (!empty($message_header->message_id)) {
          $form['basic_webmail_in_reply_to'] = array(
            '#type' => 'hidden',
            '#value' => $message_header->message_id,
          );
        }

        // Set the subject of the message.
        $send_mail_subject = 'Re: ' . _basic_webmail_prepare_email_string($message_header->subject);

        // Set the "To" of the message.
        if (!empty($message_header->from[0]->personal)) {
          $send_mail_to = _basic_webmail_prepare_email_string($message_header->from[0]->personal) . ' <' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host . '>';
        }
        else {
          $send_mail_to = $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host;
        }

        // Set the body of the message.
        if ($send_mail_type == 'reply') {
          $send_mail_body = "\n\n\nOn " . $message_header->date . ", you wrote:\n";
        }
        else {
          $send_mail_body = "\n\n\nOn " . $message_header->date . ', ' . $send_mail_to . " wrote:\n";
        }

        $temp_send_mail_body = explode("\n", utf8_encode(quoted_printable_decode($message_body)));
        $send_mail_body .= '> ' . implode("\n> ", $temp_send_mail_body);
        $send_mail_body .= "\n";

        // Add the sender's signature, if one exists.
        global $user;
        if (!empty($user->signature)) {
          $send_mail_body .= "\n-- \n" . $user->signature;
        }
      }


      // Set defaults based on $send_mail_type.
      if ($send_mail_type == 'replytoall') {
        // Set the cc of the message.
        // Make sure we don't have any bad information by setting it to an empty
        // string first.
        $send_mail_cc = '';

        if (!empty($message_header->to)) {
          // Add all addresses in the to field.
          for ($i = 0; $i < count($message_header->to); $i++) {
            if (!empty($message_header->to[$i]->personal)) {
              $send_mail_cc .= _basic_webmail_prepare_email_string($message_header->to[$i]->personal) . ' <' . $message_header->to[$i]->mailbox . '@' . $message_header->to[$i]->host . '>, ';
            }
            else {
              $send_mail_cc .= $message_header->to[$i]->mailbox . '@' . $message_header->to[$i]->host . ', ';
            }
          }
        }

        // Add all addresses in the cc field.
        if (!empty($message_header->ccaddress)) {
          for ($i = 0; $i < count($message_header->cc); $i++) {
            if (!empty($message_header->cc[$i]->personal)) {
              $send_mail_cc .= _basic_webmail_prepare_email_string($message_header->cc[$i]->personal) . ' <' . $message_header->cc[$i]->mailbox . '@' . $message_header->cc[$i]->host . '>, ';
            }
            else {
              $send_mail_cc .= $message_header->cc[$i]->mailbox . '@' . $message_header->cc[$i]->host . ', ';
            }
          }
        }

        // Remove the final ", " from the string.
        $send_mail_cc = drupal_substr($send_mail_cc, 0, drupal_strlen($send_mail_cc) - 2);
      }


      // Set defaults based on $send_mail_type.
      if ($send_mail_type == 'forward') {
        // Set the subject of the message.
        $send_mail_subject = 'Fw: ' . _basic_webmail_prepare_email_string($message_header->subject);

        // Set the body of the message.
        $send_mail_body = "\n\n\n------- Forwarded Message\n\n";
        $send_mail_body .= $message_body;
        $send_mail_body .= "\n\n------- End of Forwarded Message\n\n";

        // Add the sender's signature, if one exists.
        global $user;
        if (!empty($user->signature)) {
          $send_mail_body .= "\n-- \n" . $user->signature;
        }
      }


      // Set defaults based on $send_mail_type.
      if ($send_mail_type == 'continue') {
        $form['basic_webmail_message_number'] = array(
          '#type' => 'hidden',
          '#value' => $message_header->Msgno,
        );

        // Set the "To" of the message.
        if (!empty($message_header->toaddress)) {
          $send_mail_to = $message_header->toaddress;
        }
        // Set the cc of the message.
        if (!empty($message_header->ccaddress)) {
          $send_mail_cc = $message_header->ccaddress;
        }
        // Set the bcc of the message.
        if (!empty($message_header->bccaddress)) {
          $send_mail_bcc = $message_header->bccaddress;
        }
        // Set the subject of the message.
        if (!empty($message_header->subject)) {
          $send_mail_subject = _basic_webmail_prepare_email_string($message_header->subject);
        }
        // Fill in the body of the message.
        $send_mail_body = $message_body;
      }


      // Clean up.
      if (!imap_close($imap_resource)) {
        $message = 'There was an error closing the IMAP stream.';
        _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
      }
    }
  }
  else {
    // Set the title of the page.
    drupal_set_title(t('New mail message'));

    // If this is true, $send_mail_type holds the email address to send the
    // message to.
    if ($send_mail_type != 'new') {
      // Set the To field.
      $send_mail_to = $send_mail_type;
    }
  }

  $form['basic_webmail_send_mail_to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => $send_mail_to,
    '#maxlength' => 1024,
    '#description' => t('One or more recepients of this message. Separate the addresses with a comma (,).'),
    '#required' => TRUE,
  );
  if (user_access("access users' email addresses")) {
    $form['basic_webmail_send_mail_to']['#autocomplete_path'] = 'basic_webmail/autocomplete';
  }

  $form['basic_webmail_send_mail_cc'] = array(
    '#type' => 'textfield',
    '#title' => t('CC'),
    '#default_value' => $send_mail_cc,
    '#maxlength' => 1024,
    '#description' => t('One or more email addresses to receive a carbon copy. Separate the addresses with a comma (,).'),
  );
  if (user_access("access users' email addresses")) {
    $form['basic_webmail_send_mail_cc']['#autocomplete_path'] = 'basic_webmail/autocomplete';
  }

  $form['basic_webmail_send_mail_bcc'] = array(
    '#type' => 'textfield',
    '#title' => t('BCC'),
    '#default_value' => $send_mail_bcc,
    '#maxlength' => 1024,
    '#description' => t('One or more email addresses to receive a blind carbon copy. These email addresses will not be visible in the message. Separate the addresses with a comma (,).'),
  );
  if (user_access("access users' email addresses")) {
    $form['basic_webmail_send_mail_bcc']['#autocomplete_path'] = 'basic_webmail/autocomplete';
  }

  $form['basic_webmail_send_mail_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $send_mail_subject,
    '#maxlength' => 1024,
  );

/*
  $form['basic_webmail_send_mail_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $send_mail_body,
    '#rows' => 15,
  );
*/
  // Add the field for specifying the body and text format of the message.
  // Get the HTML input format setting and the corresponding name.
  if (module_exists('mimemail')) {
    // Get the admin specified default text format.
    $default_filter_format = _basic_webmail_compute_default_filter_format();

    // Check if the user is allowed to override the text format.
    if (user_access('basic webmail override text format')) {
      // The user is allowed to override the text format, so display a
      // textarea that has the text format selection field attached.
      $form['basic_webmail_send_mail_body']['message'] = array(
        '#type' => 'text_format',
        '#title' => t('Message'),
        '#format' => !empty($default_filter_format) ? $default_filter_format : NULL,
        '#rows' => 15,
        '#default_value' => $send_mail_body,
      );
    }
    else {
      // The user is not allowed to override the text format, so display a
      // textarea that does not have the text format selection field
      // attached.
      $form['basic_webmail_send_mail_body']['message'] = array(
        '#type' => 'textarea',
        '#title' => t('Message'),
        '#rows' => 15,
        '#default_value' => $send_mail_body,
      );
      // Save the text format information for later use.
      $form['basic_webmail_send_mail_body_filter_format'] = array(
        '#type' => 'value',
        '#value' => $default_filter_format,
      );
      // Display text format information to the user.
      $form['basic_webmail_send_mail_body_filter_format_info'] = array(
        '#type' => 'item',
        '#description' => t('The message will be sent using the %filter text format. More information about what is available is on the <a href="@formats_descriptions">Compose tips</a> page.', array('%filter' => $default_filter_format, '@formats_descriptions' => url('filter/tips'))),
      );
    }

    // If the user has access, add the field for specifying the attachment.
    if (user_access('basic webmail include attachments')) {
      // If any exists, include attachments from message as checkboxes here.
      if (!empty($attachments)) {
        $form['basic_webmail_attachments'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Attachments from original message'),
          '#options' => $attachments,
          '#description' => t('Check any attachments from the original you want to include in this message.'),
        );
      }

      // Add fields to the form for the user to add attachments.
      for ($i = 1; $i <= variable_get('basic_webmail_number_attachments', 3); $i++) {
        $form['basic_webmail_attachment_' . $i] = array(
          '#type' => 'file',
          '#title' => t('Attachment #!number', array('!number' => $i)),
        );
      }
    }
  }
  else {
    // Mime Mail is not installed, so display a textarea that does not have
    // the text format selection field attached, as this message will only
    // be sent as plain text.
    $form['basic_webmail_send_mail_body']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#rows' => 15,
      '#default_value' => $send_mail_body,
    );
    // Save the text format information for later use.
    $form['basic_webmail_send_mail_body_filter_format'] = array(
      '#type' => 'value',
      '#value' => filter_format_load('plain_text'),
    );
    // Display text format information to the user.
    $form['basic_webmail_send_mail_body_filter_format_info'] = array(
      '#type' => 'item',
      '#description' => '<p>' . t('The message will be sent as plain text.') . '</p>',
    );
  }

  $form['basic_webmail_send_mail_self'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send a copy to myself.'),
  );

  $form['_basic_webmail_send_mail_form_send'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  $form['_basic_webmail_send_mail_form_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save in INBOX.Drafts'),
  );

  $form['#attributes']['enctype'] = 'multipart/form-data';

  return $form;
}



/**
 * Move some of the form prep functionality out to separate functions, here.
 */



/**
 * Prepare the message.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The state of the form.
 */
function _basic_webmail_send_mail_form_submit($form, &$form_state) {
  // The headers and body of the message to send
  $params = array();
  // The headers of the message to save.
  $message_envelope = array();
  // The body of the message to save.
  $message_body = array();

  // Set the sender's email address and name.
  global $user;
  $params['from'] = _basic_webmail_prepare_addresses($user->data['basic_webmail_username']);
  $message_envelope['from'] = $params['from'] ;

  // Set the recipients.
  $params['to'] = _basic_webmail_prepare_addresses($form_state['values']['basic_webmail_send_mail_to']);
  $message_envelope['to'] = $params['to'];

  // Set the CC field, if it's not empty.
  if (!empty($form_state['values']['basic_webmail_send_mail_cc'])) {
    $params['headers']['Cc'] = _basic_webmail_prepare_addresses($form_state['values']['basic_webmail_send_mail_cc']);
    $message_envelope['cc'] = $params['headers']['Cc'];
  }

  // Set the BCC field, if it's not empty.
  if (!empty($form_state['values']['basic_webmail_send_mail_bcc'])) {
    $params['headers']['Bcc'] = _basic_webmail_prepare_addresses($form_state['values']['basic_webmail_send_mail_bcc']);
    $message_envelope['bcc'] = $params['headers']['Bcc'];
  }

  // Set the subject.
  $params['subject'] = $form_state['values']['basic_webmail_send_mail_subject'];
  $message_envelope['subject'] = $params['subject'];

  // Set the input filter format of the message body.
  $format = _basic_webmail_compute_default_filter_format();
  if (user_access('basic webmail override text format')) {
    // Allow the form data to determine the message format, if the user has the
    // appropriate permissions.
    if (is_array($form_state['values']['message'])) {
      $format = $form_state['values']['message']['format'];
    }
    else {
      $format = $form_state['values']['basic_webmail_send_mail_body_filter_format']->format;
    }
  }

  // Check for attachements.
  $has_attachments = _basic_webmail_attachments_exist($form_state['values']);

  // Set the message body.
  _basic_webmail_prepare_body_to_send($params, $form_state['values'], $format);
  _basic_webmail_prepare_body_to_save($message_body, $form_state['values']['message']['value'], $format, $has_attachments);


  // Add the attachments, if any exist.
  if (module_exists('mimemail')) {
    // Check for the exsistance of attachments that were added from a message
    // being replied to or forwarded.
    if (!empty($form_state['values']['basic_webmail_attachments'])) {
      foreach ($form_state['values']['basic_webmail_attachments'] as $attachment) {
        if (!empty($attachment)) {
          $params['attachments'] = _basic_webmail_prepare_attachments_to_send($form_state['values']['basic_webmail_attachments']);
          _basic_webmail_prepare_attachments_to_save($message_body, $form_state['values']['basic_webmail_attachments']);
          break;
        }
      }
    }

    // Check for the exsistance of any new attachments.
    for ($i = 1; $i <= variable_get('basic_webmail_number_attachments', '3'); $i++) {
      if (!empty($_FILES['files'])) {
        if ($_FILES['files']['error']['basic_webmail_attachment_' . $i] == 0) {
          $params['attachments'] = _basic_webmail_prepare_attachments_to_send();
          _basic_webmail_prepare_attachments_to_save($message_body);
          break;
        }
      }
    }
  }

  // Set some other header items.
  _basic_webmail_prepare_headers($params, $message_envelope, $format,
    !empty($form_state['values']['basic_webmail_in_reply_to']) ? $form_state['values']['basic_webmail_in_reply_to'] : '');

  // Get the message number from the form, if one exists.
  $message_number = (!empty($form_state['values']['basic_webmail_message_number'])) ? $form_state['values']['basic_webmail_message_number'] : 0;

  // Check to see which button was clicked.
  if ($form_state['clicked_button']['#value'] == 'Send') {
    $reply = (!empty($form_state['values']['basic_webmail_in_reply_to'])) ? $form_state['values']['basic_webmail_in_reply_to'] : FALSE;

    // Send the message to the recipient(s).
    _basic_webmail_send_message($params, 'message', language_default(), $message_envelope, $message_body, $message_number, $reply);

    // Check to see if a copy is to be sent to the sender.
    if (!empty($form_state['values']['basic_webmail_send_mail_self'])) {
      // Reset the To field to that of the From field.
      $params['to'] = $params['from'];

      // If a CC field is present, remove it, or those recipients will get the
      // message twice.
      if (!empty($params['headers']['Cc'])) {
        unset($params['headers']['Cc']);
        unset($message_envelope['cc']);
      }

      // If a BCC field is present, remove it, or those recipients will get the
      // message twice.
      if (!empty($params['headers']['Bcc'])) {
        unset($params['headers']['Bcc']);
        unset($message_envelope['bcc']);
      }

      global $language;

      // Send a copy of the essage to the sender.
      _basic_webmail_send_message($params, 'copy_to_self', $language, $message_envelope, $message_body);
    }
  }
  else {
    // Save the message to the Drafts folder.
    _basic_webmail_save_message($message_envelope, $message_body, $message_number, 'INBOX.Drafts', '\\Draft');
  }

  // Where the user goes after the message is sent.
  if (arg(4)) {
    // The original message.
    drupal_goto('/basic_webmail/view/' . arg(3) . '/' . arg(4));
  }
  elseif (arg(3)) {
    // The original folder.
    drupal_goto('/basic_webmail/' . arg(3));
  }
  else {
    // The main message list.
    drupal_goto('/basic_webmail');
  }
}

/**
 * Figures out the default filter format for the current user.
 *
 * @return string
 *   Either the user's selected format, the admin-specified one, or the
 *   system default depending on which of those the user is allowed to use.
 */
function _basic_webmail_compute_default_filter_format() {
  global $user;

  $user_default = filter_default_format($user);
  $specified_filter_format = variable_get('basic_webmail_text_format', $user_default);

  // The admin has not specified one, there wouldn't be a match for the
  // default.
  if (!empty($specified_filter_format)) {
    // Get all the text formats the user is allowed access to.
    $filter_formats = filter_formats($user);
    // Cycle through the list of possible text formats.
    foreach ($filter_formats as $filter_format) {
      // If the current filter format matches the admin specified one...
      if ($filter_format->format == $specified_filter_format['format'] || $filter_format->format == $specified_filter_format) {
        return $filter_format->format;
      }
    }
  }

  // If nothing was selected above, set the default to the Drupal system
  // default.
  return filter_fallback_format();
}

/**
 * Figures out the default filter format for the current user.
 *
 * @param array $form_values
 *   The values returned from the message sending form.
 *
 * @return boolean
 *   If attachments have been included, this function returns TRUE, if not,
 *   FALSE.
 */
function _basic_webmail_attachments_exist($form_values) {
  // Check for the exsistance of attachments that were added from a message
  // being replied to or forwarded.
  if (!empty($form_values['basic_webmail_attachments'])) {
    foreach ($form_values['basic_webmail_attachments'] as $attachment) {
      if (!empty($attachment)) {
        return TRUE;
      }
    }
  }

  // Check for the exsistance of any new attachments.
  for ($i = 1; $i <= variable_get('basic_webmail_number_attachments', '3'); $i++) {
    if (!empty($_FILES['files'])) {
      if ($_FILES['files']['error']['basic_webmail_attachment_' . $i] == 0) {
        return TRUE;
      }
    }
  }

  return FALSE;
}


/**
 * Sends the message to the recipients.
 *
 * @param array $params
 *   The parameters for the Drupal mail message.
 * @param string $message_key
 *   The ID or key of this message, so that Drupal can distinguish it from other
 *   messages.
 * @param array $language_info
 *   The language information the message is to be created with.
 * @param array $message_envelope
 *   An associative array of headers fields. See the documentation block for
 *   _basic_webmail_save_message() for more information.
 * @param array $message_body
 *   An indexed array of message bodies. See the documentation block for
 *   _basic_webmail_save_message() for more information.
 * @param boolean $copy_to_self
 *   Whether to send a copy to one's self or not.
 * @param integer $message_number
 *   If this is a continuation, this is the number that was assigned to this
 *   message.
 * @param boolean $reply_to
 *   Whether this message is in replay to another message or not.
 */
function _basic_webmail_send_message($params, $message_key, $language_info, $message_envelope, $message_body, $message_number = 0, $reply_to = FALSE) {
  // Send the message to the recipients.
  $results = drupal_mail('basic_webmail', $message_key, $params['to'], $language_info, $params, $params['from'], TRUE);

  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  // If the sending was a success, perform some additional steps.
  if ($results['result'] != FALSE) {
    // Complete the envelope of the copy for the Sent folder.
    $message_envelope['date'] = date('r');
    // Save a copy to the Sent folder.
    _basic_webmail_save_message($message_envelope, $message_body);

    // If this is a reply, mark the replied to message as answered.
    if (!empty($reply_to)) {
      // Connect to the server and retrieve a connection to the mailbox.
      global $user;
      $imap_resource = _basic_webmail_connect_to_server($user->uid);
      if (empty($imap_resource)) {
        // Report the error.
        $message = 'There was an error connecting to the server.';
        _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
      }

      // Mark the replied to message as answered.
      if (!imap_setflag_full($imap_resource, $message_number, '\\Answered')) {
        // Report the error.
        $message = 'There was an error marking the replied to message as answered.';
        _basic_webmail_report_error($message);
      }
    }

    // Report to the user.
    $message = 'Your message was successfully sent.';
    _basic_webmail_report_error($message, array(), WATCHDOG_INFO, TRUE, FALSE);
  }
  else {
    // Report the error.
    $message = 'There was an error sending your message.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
  }
}


/**
 * Saves the message to the specified folder.
 *
 * @param array $message_envelope
 *   An associative array of headers fields. Valid keys are: "remail",
 *   "return_path", "date", "from", "reply_to", "in_reply_to", "subject", "to",
 *   "cc", "bcc", "message_id", and "custom_headers" (which is an associative
 *   array of other headers).
 * @param array $message_body
 *   An indexed array of message bodies. A body is an associative array which
 *   can consist of the following keys: "type", "encoding", "charset",
 *   "type.parameters", "subtype", "id", "description", "disposition.type",
 *   "disposition", "contents.data", "lines", "bytes" and "md5".
 * @param integer $message_number
 *   If this is a continuation, this is the number that was assigned to this
 *   message.
 * @param string $folder
 *   The mail folder to save this message into. This function defaults to
 *   'INBOX.Sent' for sending messages, but when saving a draft copy to continue
 *   later, it should be 'INBOX.Drafts'.
 * @param string $read
 *   Indicates the status of this message as it will be in the folder after
 *   saving. This defaults to '\\Seen' for copies of sent messages. Possible
 *   other options are '\\Answered', '\\Flagged', '\\Deleted', and '\\Draft'.
 *   (The extra '\' is to escape the one that is needed.)
 */
function _basic_webmail_save_message($message_envelope, $message_body, $message_number = 0, $folder = 'INBOX.Sent', $read = '\\Seen') {
  // Complete the envelope of the copy for the Sent folder.
  $message_envelope['date'] = date('r');
  // Create the message string to store.
  $message_to_save = imap_mail_compose($message_envelope, $message_body);
  if (empty($message_to_save)) {
    // Report the error.
    $message = 'There was an error creating the copy of the message for the sent folder.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
  }
  else {
    // Connect to the server and retrieve a connection to the mailbox.
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    global $user;
    $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
    if (empty($imap_resource)) {
      // Report the error.
      $message = 'There was an error connecting to the server.';
      _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    }

    if ($folder == 'INBOX.Drafts') {
      // Place the message in the Drafts folder.
      // First make sure this isn't a re-saving of a previously saved message.
      if (imap_num_msg($imap_resource) > 0) {
        $threads = imap_thread($imap_resource);
        if (!empty($threads)) {
          foreach ($threads as $message_no) {
            if (!empty($message_number) && $message_no == $message_number) {
              // Remove the old copy.
              if (!imap_delete($imap_resource, $message_number)) {
                // Report the error.
                $message = 'There was an error deleting message #@message_number from the INBOX.Drafts folder.';
                $message_parameters = array('@message_number' => $message_number);
                _basic_webmail_report_error($message, $message_parameters);
              }

              // Really remove the old copy.
              if (!imap_expunge($imap_resource)) {
                // Report the error.
                $message = 'There was an error expunging the mailboxes.';
                _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
              }
            }
          }
        }
      }
    }

    // Place the message in the folder.
    if (!imap_append($imap_resource, _basic_webmail_get_server_string() . $folder, $message_to_save, $read)) {
      // Report the error.
      $message = 'There was an error saving the message to the folder.';
      _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    }
    else {
      if ($folder == 'INBOX.Drafts') {
        // Report to the user.
        $message = 'Your message was successfully saved in the INBOX.Drafts folder.';
        _basic_webmail_report_error($message, array(), WATCHDOG_INFO, TRUE, FALSE);
      }
    }

    // Clean up.
    if (!imap_close($imap_resource)) {
      $message = 'There was an error closing the IMAP stream.';
      _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    }
  }
}


/**
 * Takes given email addresses and adds the user name, if it exists, to them.
 *
 * @param string $email_addresses
 *   One or more email addresses, separated by commas.
 *
 * @return string
 *   The email address with their corresponding user name, if one exists.
 */
function _basic_webmail_prepare_addresses($email_addresses) {
  $addresses = array();

  // Check to see if there are multiple addresses in the string.
  if (strpos($email_addresses, ',')) {
    // Seperate the multiple addresses into an array of individual items.
    $addresses = explode(',', $email_addresses);

    $results = '';
    foreach ($addresses as $address) {
      // For each item in the list, attempt to add the user's name.
      $results .= _basic_webmail_get_user_name_from_mail(trim($address)) . ', ';
    }
  }
  else {
    // There is only one address. Attempt to add the user's name to it.
    $results = _basic_webmail_get_user_name_from_mail(trim($email_addresses)) . ', ';
  }

  // Remove any extra white space.
  $results = trim($results);

  // If there is a trailing comma, remove it.
  if (drupal_substr($results, drupal_strlen($results) - 1, 1) == ',') {
    $results = drupal_substr($results, 0, drupal_strlen($results) - 1);
  }

  return $results;
}


/**
 * Takes a given email address and adds the user name, if it exists, to it.
 *
 * @param string $email_addresses
 *   The email address.
 *
 * @return string
 *   The email address with its corresponding user name, if one exists.
 */
function _basic_webmail_get_user_name_from_mail($address) {
  // Search the user list for anyone with the given email address.
  $user_name = db_query("SELECT name FROM {users} WHERE mail = :mail", array(':mail' => $address))->fetchField();

  // If something was found...
  if (!empty($user_name)) {
    // Return the new email address, including the user's name.
    $address_parts = explode('@', $address);
    return imap_rfc822_write_address($address_parts[0], $address_parts[1], $user_name);
  }
  else {
    // Otherwise, return the supplied email address.
    return $address;
  }
}


/**
 * Prepares the message's header.
 *
 * @param array $params
 *   The message parameters that are being filled in for the message to send.
 * @param array $message_envelope
 *   The message headers that are being filled in for the message to save.
 * @param string $format
 *   The input format filter the body was created with.
 * @param string $reply
 *   If this message is in reply to another, this is the other message.
 *
 * @return array
 *   The prepared header.
 */
function _basic_webmail_prepare_headers(&$params, &$message_envelope, $format, $reply_to = '') {
  // If the message body was set to plain text, force it, even if Mime Mail is
  // installed and enabled.
  if (!empty($format) && $format == 'plain_text') {
    $params['plain'] = TRUE;
    $params['headers']['Content-Type'] = 'text/plain; charset=UTF-8; format=flowed';
  }
  elseif (module_exists('mimemail')) {
    // Mime Mail requires this header or it will filter all text.
    $params['headers']['Content-Type'] = 'text/html; charset=UTF-8';
  }
  else {
    $params['headers']['Content-Type'] = 'text/plain; charset=UTF-8; format=flowed';
  }

  // These need to be set, as $message_envelope will not go through Drupal's
  // regular mail processing.
  $message_envelope['return_path'] = $message_envelope['from'];
  $message_envelope['reply_to'] = $message_envelope['from'];

  // If this is in reply to another message, 'in_reply_to' needs to set this, so
  // that message threading can work correctly.
  if (!empty($reply_to)) {
    $message_envelope['in_reply_to'] = $reply_to;
    $params['headers']['in_reply_to'] = $reply_to;
  }
}


/**
 * Prepares the body for the message to send.
 *
 * @param array $params
 *   The message to save, as it currently exists.
 * @param array $form_values
 *   The values returned from the message sending form.
 * @param string $format
 *   The input text format the message was created with.
 */
function _basic_webmail_prepare_body_to_send(&$params, $form_values, $format) {
  $params['body'] = array();

  // The message has been specified to be plain text.
  if (!empty($format) && $format == 'plain_text') {
    // Add in the actual message.
    if (is_array($form_values['message'])) {
      $params['body'][] = wordwrap(check_plain($form_values['message']['value']));
    }
    else {
      $params['body'][] = wordwrap(check_plain($form_values['message']));
    }
  }
  elseif (module_exists('mimemail')) {
    global $language;

    // Add in the actual message.
    if (is_array($form_values['message'])) {
      $params['body'][] = check_markup($form_values['message']['value'], $format, ($language) ? $language->language : LANGUAGE_NONE);
    }
    else {
      $params['body'][] = check_markup($form_values['message'], $format, ($language) ? $language->language : LANGUAGE_NONE);
    }
  }
  // Mime Mail is not installed, so this message will be converted as plain
  // text.
  else {
    // Add in the actual message.
    if (is_array($form_values['message'])) {
      $params['body'][] = wordwrap(check_plain($form_values['message']['value']));
    }
    else {
      $params['body'][] = wordwrap(check_plain($form_values['message']));
    }
  }
}


/**
 * Prepares the body for the message to save.
 *
 * @param array $message_body
 *   The message to save, as it currently exists.
 * @param string $form_body
 *   The message body returned from the message sending form.
 * @param string $format
 *   The input text format the message was created with.
 * @param boolean $has_attachments
 *   An indicator as to whether the message has attachments or not.
 */
function _basic_webmail_prepare_body_to_save(&$message_body, $form_body, $format, $has_attachments) {
  // Add the body's MIME "header".
  // @TODO: The attachment check should be done first, but the resultant message
  // is not always quite right. It just may be the way Thunderbird is displaying
  // draft messages.
  if ($has_attachments) {
    // If there are one or more attachments to the message, this type is set,
    // regardless of how many and what types of body parts there are.
    $body_part = array();
    $body_part['type']    = TYPEMULTIPART;
    $body_part['subtype'] = 'mixed';
    $message_body[] = $body_part;
  }
  elseif ($format != 'plain_text') {
    // If there is an HTML body part, this type is set.
    $body_part = array();
    $body_part['type']    = TYPEMULTIPART;
    $body_part['subtype'] = 'alternative';
    $message_body[] = $body_part;
  }

  // Add the body part(s).
  if ($format == 'plain_text') {
    // This message only has one plain text body part.
    $body_part = array();
    $body_part['type'] = TYPETEXT;
    $body_part['description'] = 'message';
    $body_part['subtype'] = 'plain';
    $body_part['contents.data'] = wordwrap($form_body) . "\n\n";
    $message_body[] = $body_part;
  }
  else {
    // This message has an HTML body part, and will therefore, also have both an
    // HTML body part,
    // Add the plain text body part.
    $body_part = array();
    $body_part['type'] = TYPETEXT;
    $body_part['description'] = 'message';
    $body_part['subtype'] = 'plain';
    $body_part['contents.data'] = wordwrap(drupal_html_to_text($form_body)) . "\n\n";
    $message_body[] = $body_part;

    // Add the HTML body part.
    $body_part = array();
    $body_part['type'] = TYPETEXT;
    $body_part['description'] = 'message';
    $body_part['subtype'] = 'html';
    $body_part['contents.data'] = check_markup($form_body, $format) . "\n\n";
    $message_body[] = $body_part;
  }
}


/**
 * Processes attachments for use with Basic webmail.
 *
 * @param array $included_attachments
 *   The attachments, if any, that are included as part of a reply or forward.
 *
 * @return array
 *   The attachment information, as Mime Mail expects it.
 */
function _basic_webmail_prepare_attachments_to_send($included_attachments = array()) {
  $files = array();

  // Loop through each possible attachment.
  for ($i = 1; $i <= variable_get('basic_webmail_number_attachments', '3'); $i++) {
    // Check to see if an attachment exists.
    if ($_FILES['files']['size']['basic_webmail_attachment_' . $i] > 0) {
      $files[] = array(
        'filepath' => $_FILES['files']['tmp_name']['basic_webmail_attachment_' . $i],
        'uri' => $_FILES['files']['tmp_name']['basic_webmail_attachment_' . $i],
        'filename' => $_FILES['files']['name']['basic_webmail_attachment_' . $i],
        'filemime' => $_FILES['files']['type']['basic_webmail_attachment_' . $i],
      );
    }
  }

  if (!empty($included_attachments)) {
    foreach ($included_attachments as $key => $attachment) {
      if (!empty($attachment)) {
        $files[] = array(
          'filepath' => drupal_realpath($attachment),
          'uri' => $attachment,
          'filename' => drupal_basename($attachment),
          'filemime' => file_get_mimetype($attachment),
        );
      }
    }
  }

  return $files;
}


/**
 * Prepare the attachments to save with the message.
 *
 * @param array $message_body
 *   The message to save, as it currently exists.
 * @param array $included_attachments
 *   The attachments, if any, that are included as part of a reply or forward.
 *
 * @return array
 *   The attachment information, as Mime Mail expects it.
 */
function _basic_webmail_prepare_attachments_to_save(&$message_body, $included_attachments = array()) {
  if (!empty($included_attachments)) {
    // Add any attachments that were included from a message that is being
    // replied to or forwarded.
    foreach ($included_attachments as $key => $attachment) {
      if ($attachment) {
        $message_body[] = _basic_webmail_process_attachment_to_save(
          mime_content_type($attachment),
          basename($attachment),
          chunk_split(base64_encode(file_get_contents($attachment)))
        );
      }
    }
  }

  // Add any additional attachments.
  for ($i = 1; $i <= variable_get('basic_webmail_number_attachments', '3'); $i++) {
    if ($_FILES['files']['error']['basic_webmail_attachment_'. $i] == 0) {
      $message_body[] = _basic_webmail_process_attachment_to_save(
        $_FILES['files']['type']['basic_webmail_attachment_'. $i],
        basename($_FILES['files']['name']['basic_webmail_attachment_'. $i]),
        chunk_split(base64_encode(file_get_contents($_FILES['files']['tmp_name']['basic_webmail_attachment_'. $i])))
      );
    }
  }
}

/**
 * Processes an attachment to save with the message.
 *
 * @param string $content_type
 *   The MIME type of the file to attach to the message.
 * @param string $file_name
 *   The s name of the file to attach to the message.
 * @param  $file_contents
 *   The contents of the file to attach to the message.
 *
 * @return array
 *   The attachment information, ready to be added to a message body for
 *   imap_mail_compose().
 */
function _basic_webmail_process_attachment_to_save($content_type, $file_name, $file_contents) {
  $attachment = array();

  switch (substr($content_type, 0, strpos($content_type, '/'))) {
    case 'audio':
      $attachment['type'] = TYPEAUDIO;
      break;
    case 'image':
      $attachment['type'] = TYPEIMAGE;
      break;
    case 'text':
      $attachment['type'] = TYPETEXT;
      break;
    case 'video':
      $attachment['type'] = TYPEVIDEO;
      break;
    default:
      $attachment['type'] = TYPEAPPLICATION;
      break;
  }

  $attachment['encoding'] = ENCBASE64;
  $attachment['subtype'] = substr($content_type, strpos($content_type, '/') + 1);
  $attachment['description'] = $file_name;
  $attachment['disposition.type'] = 'attachment';
  $attachment['disposition'] = array('filename' => $attachment['description']);
  $attachment['type.parameters'] = array('name' => $attachment['description']);
  $attachment['contents.data'] = $file_contents;

  return $attachment;
}


/**
 * Automatically fills in email addresses of the current users.
 *
 * Copied and modified from taxonomy_autocomplete() in
 * modules/taxonomy/taxonomy.pages.inc
 *
 * @param string $addresses
 *   A comma-separated list of email addresses entered in the autocomplete form
 *   element. Only the last addresses is used for autocompletion.
 */
function basic_webmail_autocomplete($addresses) {
  // The user enters a comma-separated list of email addresses. Only the last
  // address is autocompleted.
  $address_array = drupal_explode_tags($addresses);

  // Get the last address.
  $last_address = trim(drupal_strtolower(array_pop($address_array)));

  if ($last_address != '') {
    // Search the users table for all email addresses which are similar to the
    // last item in the list.
    $query = db_select('users', 'u')
      ->fields('u', array('mail'))
      ->condition('u.status', 1)
      ->condition('u.mail', '%' . db_like($last_address) . '%', 'LIKE')
      ->range(0, 10);

    // Do not select any addresses that are already entered.
    if (!empty($address_array)) {
      $query->condition('u.mail', $address_array, 'NOT IN');
    }

    // Execute the query and get the results.
    $results = $query
      ->execute();

    // Convert what's left of the array to a string.
    $prefix = count($address_array) ? drupal_implode_tags($address_array) . ', ' : '';

    $matches = array();

    // Iterate through the list of similar email addresses.
    foreach ($results as $value) {
      $n = $value->mail;
      // Add the email address to the list of possible matches.
      $matches[$prefix . $n] = $value->mail;
    }
  }

  // Send the results back.
  drupal_json_output($matches);
}
