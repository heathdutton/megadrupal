<?php
/**
 * @file
 * This file handles the viewing of a message.
 */

/**
 * Display the message.
 *
 * @param folder
 *   The name of the folder the message is in. If no folder is specified, it
 *   defaults to 'INBOX'.
 * @param message_number
 *   The ID of the message to delete.
 *
 * @return
 *   The formatted message.
 */
function basic_webmail_message($folder, $message_number) {
  $output = '';

  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Show the message.
  // Start with the the message header.
  $message_header = imap_headerinfo($imap_resource, $message_number);
  if (empty($message_header)) {
    $message = 'There was an error retrieving the message header.';
    _basic_webmail_report_error($message);
    return $message;
  }

  // Set the title of the page.
  $subject = !empty($message_header->subject) ? _basic_webmail_prepare_email_string($message_header->subject) : '(No subject given.)';
  drupal_set_title(t('@title', array('@title' => $subject)));

  // Offer a link back to where we came from.
  $output .= '<br />';
  $output .= l(t('Return to message listing'), 'basic_webmail/' . $folder);
  $output .= '<hr>';
  $output .= '<table>';


  // Start processing the header of the message.
  // Retrieve and display who the message is from.
  if (!empty($message_header->from[0]->personal)) {
    $output .= '<tr><td>From:</td><td>' . l(_basic_webmail_prepare_email_string($message_header->from[0]->personal), 'basic_webmail/sendmail/' . _basic_webmail_prepare_email_string($message_header->from[0]->personal) . ' <' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host . '>') . '</td></tr>';
  }
  else {
    $output .= '<tr><td>From:</td><td>' . l($message_header->from[0]->mailbox . '@' . $message_header->from[0]->host, 'basic_webmail/sendmail/' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host) . '</td></tr>';
  }

  // Retrieve and display who the message is to.
  $toaddresses = '';
  for ($i = 0; $i < count($message_header->to); $i++) {
    if (!empty($message_header->to[$i]->personal)) {
     $toaddresses .= l(_basic_webmail_prepare_email_string($message_header->to[$i]->personal), 'basic_webmail/sendmail/' . _basic_webmail_prepare_email_string($message_header->to[$i]->personal) . ' <' . $message_header->to[$i]->mailbox . '@' . $message_header->to[$i]->host . '>') . ', ';
    }
    else {
      $toaddresses .= l($message_header->to[$i]->mailbox . '@' . $message_header->to[$i]->host, 'basic_webmail/sendmail/' . $message_header->to[$i]->mailbox . '@' . $message_header->to[$i]->host) . ', ';
    }
  }
  // Drop the final comma and space.
  $output .= '<tr><td>To:</td><td>' . drupal_substr($toaddresses, 0, drupal_strlen($toaddresses) - 2) . '</td></tr>';

  // Retrieve and display who the message was CC'd to.
  if (!empty($message_header->ccaddress)) {
    $ccaddresses = '';
    for ($i = 0; $i < count($message_header->cc); $i++) {
      if (!empty($message_header->cc[$i]->personal)) {
        $ccaddresses .= l(_basic_webmail_prepare_email_string($message_header->cc[$i]->personal), 'basic_webmail/sendmail/' . _basic_webmail_prepare_email_string($message_header->cc[0]->personal) . ' <' . $message_header->cc[$i]->mailbox . '@' . $message_header->cc[$i]->host . '>') . ', ';
      }
      else {
        $ccaddresses .= l($message_header->cc[$i]->mailbox . '@' . $message_header->cc[$i]->host, 'basic_webmail/sendmail/' . $message_header->cc[$i]->mailbox . '@' . $message_header->cc[$i]->host) . ', ';
      }
    }

    // Drop the final comma and space.
    $output .= '<tr><td>CC:</td><td>' . drupal_substr($ccaddresses, 0, drupal_strlen($ccaddresses) - 2) . '</td></tr>';
  }

  // Retrieve and display the date of the message.
  $output .= '<tr><td>Date:</td><td>' . $message_header->date . '</td></tr>';

  // Close out the header.
  $output .= '</table>';
  $output .= '<hr>';


  // Start processing the body of the message.
  // Get the message parts list.
  $parts_list = _basic_webmail_get_parts_list($imap_resource, $message_number);

  // Initialize variable.
  $message_body = '';
  $message_self_link = '';
  $attachment_links = '';
  $reply_part = '';

  if (!empty($parts_list)) {
    // Iterate over the parts list.
    foreach ($parts_list as $part_id => $part_array) {
      // Process attachements, first, since PHP sometimes thinks a PDF file is
      // an HTML body part.
      // For attachments we really want them available whether the part being
      // viewed is HTML or plain text.
      if (!empty($part_array[0]) && !empty($part_array[1])) {
        $attachment = _basic_webmail_process_attachment($imap_resource, $message_number, $part_id, $part_array[1]);
        $attachment_links .= l($attachment[1], $attachment[2]) . '&nbsp; -- &nbsp;';
      }

      // Process a plain text part.
      elseif (arg(5) != 'html' && drupal_strtoupper($part_array[0]) == 'PLAIN') {
        if (!empty($message_body)) {
          // If there is at least one other part, add a separator between the
          // parts.
          $message_body .= '<hr width="75%">';
        }

        $message_body .= check_markup(_basic_webmail_decode_body_part($imap_resource, $message_number, $part_id), 'plain_text');

        if (empty($reply_part)) {
          $reply_part = $part_id;
        }
      }

      // Create a link to an HTML part.
      elseif (arg(5) != 'html' && drupal_strtoupper($part_array[0]) == 'HTML') {
        $message_self_link .= '<hr>' . l(t('View this message as a web page.'), 'basic_webmail/view/' . $folder . '/' . $message_number . '/message/html/' . $part_id);
      }

      // Process an HTML part.
      elseif (arg(5) == 'html' && drupal_strtoupper($part_array[0]) == 'HTML' && empty($part_array[0]->ifdparameters)) {
        if (!empty($message_body)) {
          // If there is at least one other part, add a separator between the
          // parts.
          $message_body .= '<hr width="75%">';
        }

        // Get the HTML body part.
        $html_body_part = _basic_webmail_decode_body_part($imap_resource, $message_number, arg(6) ? arg(6) : $part_id);
        if (!empty($html_body_part)) {
          // Get the list of allowed tags.
          $allowed_tags = preg_split('/\s+|<|>/', variable_get('basic_webmail_allowed_tags', '<a> <cite> <code> <dd> <dl> <dt> <em> <li> <ol> <strong> <ul>'), -1, PREG_SPLIT_NO_EMPTY);
          // Filter the body part and add it to the message.
          $message_body .= filter_xss($html_body_part, $allowed_tags);
        }

        if (empty($reply_part)) {
          $reply_part = $part_id;
        }

        $message_self_link .= '<hr>' . l(t('View the original message.'), 'basic_webmail/view/' . $folder . '/' . $message_number);
      }
    }
  }

  // Add the formatted message body to the output string.
  $output .= utf8_decode($message_body);

  // Add links to the attachments.
  if (!empty($attachment_links)) {
    $output .= '<hr>Attachment(s):<br /><br />';
    $output .= drupal_substr($attachment_links, 0, drupal_strlen($attachment_links) - 16);
  }

  // Add the link to the other part.
  $output .= $message_self_link;

  // Add a separator.
  $output .= '<hr>';

  // Add some action links for this message.
  // Links for all folders.
  $delete_link = l(t('Delete'), 'basic_webmail/delete/' . $folder . '/' . $message_number);
  $copy_link = l(t('Copy'), 'basic_webmail/copy/' . $folder . '/' . $message_number);
  $move_link = l(t('Move'), 'basic_webmail/move/' . $folder . '/' . $message_number);
  if ($folder == 'INBOX.Drafts') {
    // Links for the Drafts folder.
    $continue_link = l(t('Continue composition'), 'basic_webmail/sendmail/continue/' . $folder . '/' . $message_number . '/' . $reply_part);
    $output .= $continue_link . '&nbsp; -- &nbsp;' . $copy_link . '&nbsp; -- &nbsp;' . $move_link . '&nbsp; -- &nbsp;' . $delete_link;
  }
  else {
    // Links for the other folders.
    $reply_link = l(t('Reply'), 'basic_webmail/sendmail/reply/' . $folder . '/' . $message_number . '/' . $reply_part);
    $reply_to_all_link = l(t('Reply to all'), 'basic_webmail/sendmail/replytoall/' . $folder . '/' . $message_number . '/' . $reply_part);
    $forward_link = l(t('Forward'), 'basic_webmail/sendmail/forward/' . $folder . '/' . $message_number . '/' . $reply_part);
    $output .= $reply_link . '&nbsp; -- &nbsp;' . $reply_to_all_link . '&nbsp; -- &nbsp;' . $forward_link . '&nbsp; -- &nbsp;' . $copy_link . '&nbsp; -- &nbsp;' . $move_link . '&nbsp; -- &nbsp;' . $delete_link;
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  return $output;
}

/**
 *
 * /
function _basic_webmail_process_html_body_part($form, &$form_state, $folder, $message_number) {
      if (!empty($message_body)) {
        // If there is at least one other part, add a separator between the
        // parts.
        $message_body .= '<hr width="75%">';
      }

      // Get the HTML body part.
      $html_body_part = _basic_webmail_decode_body_part($imap_resource, $message_number, arg(6) ? arg(6) : $part_id);
      if (!empty($html_body_part)) {
        // Get the list of allowed tags.
        $allowed_tags = preg_split('/\s+|<|>/', variable_get('basic_webmail_allowed_tags', '<a> <cite> <code> <dd> <dl> <dt> <em> <li> <ol> <strong> <ul>'), -1, PREG_SPLIT_NO_EMPTY);
        // Filter the body part and add it to the message.
        $message_body .= filter_xss($html_body_part, $allowed_tags);
      }

      if (empty($reply_part)) {
        $reply_part = $part_id;
      }

      $message_self_link .= '<hr>' . l(t('View the original message.'), 'basic_webmail/view/' . $folder . '/' . $message_number);
}

/**
 *
 * /
function _basic_webmail_process_plain_text_body_part($form, &$form_state, $folder, $message_number) {
      if (!empty($message_body)) {
        // If there is at least one other part, add a separator between the
        // parts.
        $message_body .= '<hr width="75%">';
      }

      $message_body .= check_markup(_basic_webmail_decode_body_part($imap_resource, $message_number, $part_id), 'plain_text');

      if (empty($reply_part)) {
        $reply_part = $part_id;
      }
}


/**
 * Post a message delete confirmation form.
 *
 * @param message_number
 *   The ID of the message to delete.
 * @param folder
 *   The name of the folder the message is in. If no folder is specified, the
 *   default of 'INBOX' is used.
 *
 * @return
 *   The results of the confirmation question.
 */
function _basic_webmail_delete_message_form($form, &$form_state, $folder, $message_number) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Get the message header.
  $message_header = imap_headerinfo($imap_resource, $message_number);
  if (empty($message_header)) {
    $message = 'There was an error retrieving the message header.';
    _basic_webmail_report_error($message);
    return $message;
  }

  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';

  // Retrieve who the message is from.
  if (!empty($message_header->from[0]->personal)) {
    $from_address = _basic_webmail_prepare_email_string($message_header->from[0]->personal) . ' <' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host . '>';
  }
  else {
    $from_address = $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host;
  }

  // Retrieve the message date.
  if (empty($message_header->date)) {
    $message_date = 0;
  }
  else {
    $message_date = $message_header->date;
  }

  // Retrieve the message subject.
  if (empty($message_header->subject)) {
    $message_subject = '(No subject given.)';
  }
  else {
    $message_subject = _basic_webmail_prepare_email_string($message_header->subject);
  }

  // Fill in the form fields.
  $form['message_number'] = array(
    '#type' => 'hidden',
    '#value' => $message_number,
  );

  $form['folder'] = array(
    '#type' => 'hidden',
    '#value' => $folder,
  );

  // Fill in the rest of the confirm_form() variables.
  $question = t('Delete message?');
  $path = 'basic_webmail/view/' . $folder . '/' . $message_number;
  $description = t("Are you sure you want to delete the message from !from_address, sent on !message_date with the subject of '!message_subject'?", array('!from_address' => $from_address, '!message_date' => $message_date, '!message_subject' => $message_subject));
  $yes = 'Yes';
  $no = 'No';
  $name = 'confirm_message_deletion';

  // Call the function with the data.
  $confirmation_result = confirm_form(
    $form,         // Additional elements to inject into the form, for example hidden elements.
    $question,     // The question to ask the user (e.g. "Are you sure you want to delete the block <em>foo</em>?").
    $path,         // The page to go to if the user denies the action. Can be either a drupal path, or an array with the keys 'path', 'query', 'fragment'.
    $description, // Additional text to display (defaults to "This action cannot be undone.").
    $yes,          // A caption for the button which confirms the action (e.g. "Delete", "Replace", ...).
    $no,           // A caption for the link which denies the action (e.g. "Cancel").
    $name         // The internal name used to refer to the confirmation item.
  );

  // Clean up.
  if (!imap_close($imap_resource)) {
    $messsage = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  return $confirmation_result;
}

/**
 * Delete a specified message.
 *
 * @param form_id
 *   The ID of the form created in the form generation function.
 * @param form_values
 *   The contents of the form fields.
 */
function _basic_webmail_delete_message_form_submit($form, &$form_state) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $form_state['values']['folder']);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  if ($form_state['values']['folder'] != 'INBOX.Trash') {
    // If the message is located in a folder other than INBOX.Trash, move it to
    // INBOX.Trash.
    $result = imap_mail_move($imap_resource, $form_state['values']['message_number'], 'INBOX.Trash');
  }
  else {
    // Otherwise, delete the message.
    $result = imap_delete($imap_resource, $form_state['values']['message_number']);
  }

  if (empty($result)) {
    // Report the error.
    $message = 'There was an error deleting the message.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
  }
  else {
    drupal_set_message(t('The message was deleted.'));
    if (!imap_expunge($imap_resource)) {
      // Report the error.
      $message = 'There was an error expunging the mailboxes.';
      _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  $form_state['redirect'] = 'basic_webmail/' . $form_state['values']['folder'];
}


/**
 * Post a message copy form.
 *
 * @param message_number
 *   The ID of the message to delete.
 * @param folder
 *   The name of the folder the message is in. If no folder is specified, the
 *   default of 'INBOX' is used.
 *
 * @return
 *   The results of the confirmation question.
 */
function _basic_webmail_copy_message_form($form, &$form_state, $folder, $message_number) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Get the message header.
  $message_header = imap_headerinfo($imap_resource, $message_number);
  if (empty($message_header)) {
    $message = 'There was an error retrieving the message header.';
    _basic_webmail_report_error($message);
    return $message;
  }

  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';

  // Retrieve who the message is from.
  if (!empty($message_header->from[0]->personal)) {
    $from_address = _basic_webmail_prepare_email_string($message_header->from[0]->personal) . ' <' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host . '>';
  }
  else {
    $from_address = $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host;
  }

  // Retrieve the message date.
  if (empty($message_header->date)) {
    $message_date = 0;
  }
  else {
    $message_date = $message_header->date;
  }

  // Retrieve the message subject.
  if (empty($message_header->subject)) {
    $message_subject = '(No subject given.)';
  }
  else {
    $message_subject = _basic_webmail_prepare_email_string($message_header->subject);
  }

  // Get the list of folders from the server.
  $folders = imap_list($imap_resource, _basic_webmail_get_server_string(), '*');
  if (!empty($folders)) {
    natsort($folders);
    foreach ($folders as $folder_name) {
      $brief_folder_name = imap_utf7_decode(drupal_substr($folder_name, strpos($folder_name, '}') + 1, drupal_strlen($folder_name)));

      if ($brief_folder_name != $folder) {
        $folder_names[$brief_folder_name] = $brief_folder_name;
      }
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  $form['basic_webmail_to_folder'] = array(
    '#type' => 'select',
    '#title' => t('The folder to which you want to copy the message'),
    '#options' => $folder_names,
  );

  // Fill in the form fields.
  $form['basic_webmail_message_number'] = array(
    '#type' => 'hidden',
    '#value' => $message_number,
  );

  $form['basic_webmail_from_folder'] = array(
    '#type' => 'hidden',
    '#value' => $folder,
  );

  // Fill in the rest of the confirm_form() variables.
  $question = t('Copy message?');
  $path = 'basic_webmail/view/' . $folder . '/' . $message_number;
  $description = t("Copy message from !from_address, sent on !message_date with the subject of '!message_subject'?", array('!from_address' => $from_address, '!message_date' => $message_date, '!message_subject' => $message_subject));
  $yes = 'Copy';
  $no = 'Cancel';
  $name = 'confirm_message_copy';

  // Call the function with the data.
  $confirmation_result = confirm_form($form, $question, $path, $description, $yes, $no, $name);

  return $confirmation_result;
}

/**
 * Copy a specified message.
 *
 * @param form_id
 *   The ID of the form created in the form generation function.
 * @param form_values
 *   The contents of the form fields.
 */
function _basic_webmail_copy_message_form_submit($form, &$form_state) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $form_state['values']['basic_webmail_from_folder']);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Copy the message.
  if (!imap_mail_copy($imap_resource, $form_state['values']['basic_webmail_message_number'], $form_state['values']['basic_webmail_to_folder'])) {
    // Report the error.
    $message = 'There was an error copying message #@message_number.';
    $message_parameters = array('@message_number' => $message_number);
    _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
  }
  else {
    drupal_set_message(t('The message was copied.'));
    if (!imap_expunge($imap_resource)) {
      // Report the error.
      $message = 'There was an error expunging the mailboxes.';
      _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  $form_state['redirect'] = 'basic_webmail/' . $form_state['values']['basic_webmail_from_folder'];
}


/**
 * Post a message move form.
 *
 * @param message_number
 *   The ID of the message to delete.
 * @param folder
 *   The name of the folder the message is in. If no folder is specified, the
 *   default of 'INBOX' is used.
 *
 * @return
 *   The results of the confirmation question.
 */
function _basic_webmail_move_message_form($form, &$form_state, $folder, $message_number) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Get the message header.
  $message_header = imap_headerinfo($imap_resource, $message_number);
  if (empty($message_header)) {
    $message = 'There was an error retrieving the message header.';
    _basic_webmail_report_error($message);
    return $message;
  }

  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';

  // Retrieve who the message is from.
  if (!empty($message_header->from[0]->personal)) {
    $from_address = _basic_webmail_prepare_email_string($message_header->from[0]->personal) . ' <' . $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host . '>';
  }
  else {
    $from_address = $message_header->from[0]->mailbox . '@' . $message_header->from[0]->host;
  }

  // Retrieve the message date.
  if (empty($message_header->date)) {
    $message_date = 0;
  }
  else {
    $message_date = $message_header->date;
  }

  // Retrieve the message subject.
  if (empty($message_header->subject)) {
    $message_subject = '(No subject given.)';
  }
  else {
    $message_subject = _basic_webmail_prepare_email_string($message_header->subject);
  }

  // Get the list of folders from the server.
  $folders = imap_list($imap_resource, _basic_webmail_get_server_string(), '*');
  if (!empty($folders)) {
    natsort($folders);
    foreach ($folders as $folder_name) {
      $brief_folder_name = imap_utf7_decode(drupal_substr($folder_name, strpos($folder_name, '}') + 1, drupal_strlen($folder_name)));

      if ($brief_folder_name != $folder) {
        $folder_names[$brief_folder_name] = $brief_folder_name;
      }
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  $form['basic_webmail_to_folder'] = array(
    '#type' => 'select',
    '#title' => t('The folder to which you want to move the message'),
    '#options' => $folder_names,
  );

  // Fill in the form fields.
  $form['basic_webmail_message_number'] = array(
    '#type' => 'hidden',
    '#value' => $message_number,
  );

  $form['basic_webmail_from_folder'] = array(
    '#type' => 'hidden',
    '#value' => $folder,
  );

  // Fill in the rest of the confirm_form() variables.
  $question = t('Move message?');
  $path = 'basic_webmail/view/' . $folder . '/' . $message_number;
  $description = t("Move message from !from_address, sent on !message_date with the subject of '!message_subject'?", array('!from_address' => $from_address, '!message_date' => $message_date, '!message_subject' => $message_subject));
  $yes = 'Move';
  $no = 'Cancel';
  $name = 'confirm_message_move';

  // Call the function with the data.
  $confirmation_result = confirm_form($form, $question, $path, $description, $yes, $no, $name);

  return $confirmation_result;
}

/**
 * Move a specified message.
 *
 * @param form_id
 *   The ID of the form created in the form generation function.
 * @param form_values
 *   The contents of the form fields.
 */
function _basic_webmail_move_message_form_submit($form, &$form_state) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $form_state['values']['basic_webmail_from_folder']);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  // Move the message.
  if (!imap_mail_move($imap_resource, $form_state['values']['basic_webmail_message_number'], $form_state['values']['basic_webmail_to_folder'])) {
    // Report the error.
    $message = 'There was an error moving message #@message_number.';
    $message_parameters = array('@message_number' => $message_number);
    _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
  }
  else {
    drupal_set_message(t('The message was moved.'));
    if (!imap_expunge($imap_resource)) {
      // Report the error.
      $message = 'There was an error expunging the mailboxes.';
      _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  $form_state['redirect'] = 'basic_webmail/' . $form_state['values']['basic_webmail_from_folder'];
}


/**
 * Decode the body part for the specified message number and message part ID.
 *
 * @param resource $imap_resource
 *   The IMAP stream, created by calling _basic_webmail_connect_to_server().
 * @param integer $message_number
 *   The number of the message to get the information for.
 * @param string $part_id
 *   The ID of the part to get the information for.
 *
 * @return string
 *   The message body, decoded, if necessary.
 */
function _basic_webmail_decode_body_part($imap_resource, $message_number, $part_id) {
  $message_body_structure = imap_bodystruct($imap_resource, $message_number, $part_id);
  if (empty($message_body_structure)) {
    $message = "There was an error retrieving the message's body structure.";
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return FALSE;
  }
  else {
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';

    switch ($message_body_structure->encoding) {
      case 3:
        return imap_base64(_basic_webmail_mail_fetchpart($imap_resource, $message_number, $part_id));
        break;

      case 4:
        return utf8_encode(quoted_printable_decode(_basic_webmail_mail_fetchpart($imap_resource, $message_number, $part_id)));
        break;

      default:
        return _basic_webmail_mail_fetchpart($imap_resource, $message_number, $part_id);
        break;
    }
  }
}
