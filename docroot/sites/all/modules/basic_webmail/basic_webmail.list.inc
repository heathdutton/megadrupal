<?php
/**
 * @file
 * This file handles the message listing.
 */

/**
 * Display the listing of messages.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form values of the passed form.
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of 'INBOX' is used.
 *
 * @return array
 *   The formatted message listing.
 */
function basic_webmail_message_list($folder = 'INBOX') {
  if (empty($folder)) {
    $folder = 'INBOX';
  }
  return drupal_get_form('basic_webmail_mail_list_form', $folder);
}


/**
 * Display the listing of messages.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form values of the passed form.
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of 'INBOX' is used.
 *
 * @return array
 *   The formatted message listing.
 */
function basic_webmail_mail_list_form($form, &$form_state, $folder = 'INBOX') {
  // Initialize the form for dealing with messages in bulk.
  $form = array();

  global $user;
  if (empty($user->data['basic_webmail_username'])) {
    $form['error'] = array(
      '#type' => 'item',
      '#markup' => t('You have not configured your settings to use this webmail system. Edit your !account and fill in the necessary fields in the "Basic webmail account settings" group.', array('!account' => l('account settings', 'user'))),
    );
    return $form;
  }

  // Set the title of the page to the current mailbox, if there is one.
  if (!empty($folder)) {
    drupal_set_title(t('Current folder: @title', array('@title' => $folder)));
  }

  // Load the custom CSS file.
  drupal_add_css(drupal_get_path('module', 'basic_webmail') . '/basic_webmail.css');

  // Get the list of folders.
  $folder_list = _basic_webmail_get_folder_list($folder);

  // Show the current mailbox.
  $form['mailbox_title'] = array(
    '#type' => 'item',
    '#title' => t('Current mailbox: !mailbox', array('!mailbox' => $user->data['basic_webmail_username'])),
  );

  // Prepare the list of folders for display above the message list.
  if (!empty($folder_list)) {
    $form['folders'] = array(
      '#type' => 'item',
      '#title' => t('Other folders in this mailbox:'),
      '#markup' => _basic_webmail_prepare_folder_list($folder_list, $folder),
    );
  }

  $form['actions_title'] = array(
    '#type' => 'item',
    '#title' => t('Actions:'),
  );

  // Add a link to a blank composition form.
  $form['comoposition'] = array(
    '#type' => 'item',
    '#markup' => l(t('Compose new message'), 'basic_webmail/sendmail') . '<hr />',
  );

  // Create the table header for the message listing.
  $header = _basic_webmail_create_message_list_header();

  // Update the saved message information.
  _basic_webmail_update_stored_message_list($folder);

  // Get the message list.
  $message_list = _basic_webmail_get_message_list_data($header, $user->uid);

  $form['message_list_title'] = array(
    '#type' => 'item',
    '#title' => t('Messages in this folder:'),
  );

  // Add the message list to the form.
  $form['message_list'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $message_list,
    '#empty' => t('You have no email in this mailbox.'),
  );

  // Add the operation select list to the form.
  $form['operation'] = array(
    '#type' => 'select',
    '#title' => t('With checked'),
    '#default_value' => 'read',
    '#options' => array(
      'delete' => t('Delete'),
      'read' => t('Mark read'),
      'unread' => t('Mark unread'),
      'copy_to' => t('Copy'),
      'move_to' => t('Move'),
    ),
  );

  // Add the select list with the list of folders.
  $form['folder_name'] = array(
    '#type' => 'select',
    '#title' => t('The folder to copy or move to'),
    '#options' => $folder_list,
  );

  // Add the button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  // Save the current folder name for later use.
  $form['folder'] = array(
    '#type' => 'hidden',
    '#value' => $folder,
  );

  // Add the pager at the bottom of the form.
  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}


/**
 * Perform an action on a group of messages.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form values of the passed form.
 */
function basic_webmail_mail_list_form_submit($form, &$form_state) {
  // Find out what is to be done with the messages.
  $operation = $form_state['values']['operation'];
  // Filter out unselected messages.
  $messages = array_filter($form_state['values']['message_list']);

  if (isset($messages) && is_array($messages)) {
    // Connect to the server and retrieve a connection to the mailbox.
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    global $user;
    $imap_resource = _basic_webmail_connect_to_server($user->uid, $form_state['values']['folder']);
    if (empty($imap_resource)) {
      $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
      _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
      // @todo: figure out if this is necessary.
      // $form_state['redirect'] = $message;
    }

    switch ($operation) {
      case 'delete':
        _basic_webmail_mail_list_form_submit_delete($form_state, $messages, $imap_resource);
        break;

      case 'read':
        _basic_webmail_mail_list_form_submit_mark_read($form_state, $messages, $imap_resource);
        break;

      case 'unread':
        _basic_webmail_mail_list_form_submit_mark_unread($form_state, $messages, $imap_resource);
        break;

      case 'copy_to':
        _basic_webmail_mail_list_form_submit_copy_to($form_state, $messages, $imap_resource);
        break;

      case 'move_to':
        _basic_webmail_mail_list_form_submit_move_to($form_state, $messages, $imap_resource);
        break;
    }

    // Clean up.
    if (!imap_close($imap_resource)) {
      $message = 'There was an error closing the IMAP stream.';
      _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    }
  }

  // Send the user back to the page they were looking at.
  $form_state['redirect'] = _basic_webmail_get_return_destination();
}


/**
 *  Private helper functions that do the main work of the module.
 */


/**
 * Retrieves the list of folders.
 *
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of 'INBOX' is used.
 *
 * @return array
 *   The folder listing.
 */
function _basic_webmail_get_folder_list($folder = 'INBOX') {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if ($imap_resource === FALSE) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return FALSE;
  }

  // Get the list of folders from the server.
  $folders = imap_list($imap_resource, _basic_webmail_get_server_string(), '*');
  if (!empty($folders)) {
    natsort($folders);
    foreach ($folders as $folder_name) {
      $brief_folder_name = imap_utf7_decode(drupal_substr($folder_name, strpos($folder_name, '}') + 1, drupal_strlen($folder_name)));

      if ($brief_folder_name != $folder) {
        $folder_list[$brief_folder_name] = $brief_folder_name;
      }
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }

  return $folder_list;
}


/**
 * Prepares the list of folders for display.
 *
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of 'INBOX' is used.
 *
 * @return string
 *   The folder listing markup for displaying.
 */
function _basic_webmail_prepare_folder_list($folder_list, $folder = 'INBOX') {
  // Initialize the variable.
  $output = '<br />';

  // Iterate through the list of folders.
  foreach ($folder_list as $folder_name) {
    // Get the name for the current folder.
    $brief_folder_name = imap_utf7_decode(drupal_substr($folder_name, strpos($folder_name, '}'), drupal_strlen($folder_name)));
    // Add the folder to the list with a link.
    $output .= l($brief_folder_name, 'basic_webmail/list/' . $brief_folder_name) . '&nbsp; -- &nbsp;';
  }

  // Remove the final, unnecessary &nbsp; -- &nbsp;.
  $output = drupal_substr($output, 0, drupal_strlen($output) - 16);
  $output .= '<hr>';

  return $output;
}


/**
 * Create the table header for the message listing.
 *
 * @return array
 *   The table header for the message list.
 */
function _basic_webmail_create_message_list_header() {
  // Set the path to the icons.
  $icon_path = drupal_get_path('module', 'basic_webmail') . '/images/';

  // Specify the header information for the message list.
  return array(
    'message_unread' => array('data' => t('Read'), 'field' => 'message_unread'),
    'attachment_exists' => array('data' => t('Att.'), 'field' => 'attachment_exists'),
    'message_answered' => array('data' => t('Ans.'), 'field' => 'message_answered'),
    'message_subject' => array('data' => t('Subject'), 'field' => 'message_subject'),
    'from_address' => array('data' => t('From'), 'field' => 'from_address'),
    'message_date' => array(
      'data' => t('Date/Time'),
      'field' => 'message_date',
      'sort' => 'desc',
    ),
  );
}


/**
 * Updates the saved message list information.
 *
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of 'INBOX' is used.
 */
function _basic_webmail_update_stored_message_list($folder = 'INBOX') {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  global $user;
  $imap_resource = _basic_webmail_connect_to_server($user->uid, $folder);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  $mailboxes = imap_getsubscribed($imap_resource, _basic_webmail_get_server_string(), '*');
  if (empty($mailboxes[4])) {
    _basic_webmail_subscribe_to_mailboxes($user->uid);
  }

  // Remove the existing saved messages.
  db_delete('basic_webmail_messages')
    ->condition('user_id', $user->uid)
    ->execute();

  if (imap_num_msg($imap_resource) > 0) {
    // Retrieve and display the mail in the current folder.
    $threads = imap_thread($imap_resource);
    if ($threads === FALSE) {
      // Report the error.
      $message = t('There was an error getting the list of messages.');
      _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
      return;
    }
    else {
      // Save the message information in a temporary table.
      foreach ($threads as $key => $message_number) {
        $tree = explode('.', $key);
        if ($tree[1] == 'num' && $message_number > 0) {
          // Get the messages information from the header.
          $message_header = imap_headerinfo($imap_resource, $message_number);

          // Make sure the message is not marked as deleted.
          if ($message_header->Deleted == ' ') {
            // Set the read status.
            if ($message_header->Recent == 'N' || $message_header->Unseen == 'U') {
              $message_unread = 1;
            }
            else {
              $message_unread = 0;
            }

            // Process the message's subject for storage in the database.
            if (!empty($message_header->subject)) {
              $message_subject = _basic_webmail_process_subject($message_header->subject, $message_unread, $message_number, $folder);
            }
            else {
              $message_subject = _basic_webmail_process_subject('(No subject specified)', $message_unread, $message_number, $folder);
            }

            // Process the message sender for storage in the database.
            if (empty($message_header->from[0])) {
              $from_address = '';
            }
            else {
              $from_address = _basic_webmail_process_from_address($message_header->from[0]);
            }

            // Set the message's date.
            if (empty($message_header->date)) {
              $message_date = 0;
            }
            else {
              $message_date = strtotime($message_header->date);
            }

            // Check if message was answered.
            if ($message_header->Answered == 'A') {
              $message_answered = 1;
            }
            else {
              $message_answered = 0;
            }

            // Check for attachments.
            // Get the message parts list.
            $parts_list = _basic_webmail_get_parts_list($imap_resource, $message_number);
            if (!empty($parts_list)) {
              // Iterate over the parts list.
              foreach ($parts_list as $part_array) {
                if (!empty($part_array[0]) && !empty($part_array[1])) {
                  $attachment_exists = 1;
                  continue;
                }
                else {
                  $attachment_exists = 0;
                }
              }
            }

            // imap_uid($imap_resource, $message_number)
            $id = db_insert('basic_webmail_messages')
              ->fields(array(
                'user_id' => $user->uid,
                'message_number' => $message_number,
                'message_subject' => $message_subject,
                'from_address' => $from_address,
                'message_date' => $message_date,
                'message_unread' => $message_unread,
                'message_answered' => $message_answered,
                'attachment_exists' => $attachment_exists,
              ))
              ->execute();
          }
        }
      }
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }
}


/**
 * Retrieves the list of messages.
 *
 * @param array $header
 *   The original header that was created in
 *   _basic_webmail_create_message_list_header().
 * @param integer $user_id
 *   The ID for the user to gather messages for.
 *
 * @return array
 *   The list of messages.
 */
function _basic_webmail_get_message_list_data($header, $user_id) {
  // Initialize variables.
  $message_list = array();
  $icon_path = drupal_get_path('module', 'basic_webmail') . '/images/';

  // Specifiy the number of messages to display on a page.
  $row_limit = variable_get('basic_webmail_messages_per_page', 25);

  // Construct and execute the database query.
  $result = db_select('basic_webmail_messages', 'm')
    ->fields('m', array(
      'message_number',
      'message_unread',
      'attachment_exists',
      'message_answered',
      'message_subject',
      'from_address',
      'message_date',
    ))
    ->condition('user_id', $user_id, '=')
    ->extend('PagerDefault')
    ->limit($row_limit)
    ->extend('TableSort')
    ->orderByHeader($header)
    ->execute();

  // Iterate through the messages.
  foreach ($result as $message) {
    // Initialize variables.
    $message_unread = '';
    $message_attachment = '';
    $message_answered = '';
    $message_date = '(Invalid date.)';

    // If the message is unread, show the icon.
    if (!empty($message->message_unread)) {
      $message_unread = theme_image(array(
        'path' => $icon_path . 'mini-mail.png',
        'alt' => 'Generic icon indicating message is unread.',
        'title' => t('You have not read this message.'),
        'attributes' => array(),
      ));
    }
    elseif (empty($message->message_unread)) {
      $message_unread = theme_image(array(
        'path' => $icon_path . 'mini-mail-open.png',
        'alt' => 'Generic icon indicating message is read.',
        'title' => t('You have read this message.'),
        'attributes' => array(),
      ));
    }

    // If the message has an attachment, show the icon.
    if (!empty($message->attachment_exists)) {
      $message_attachment = theme_image(array(
        'path' => $icon_path . 'mini-doc.png',
        'alt' => 'Generic icon indicating message has an attachment.',
        'title' => t('This message has one or more attachments.'),
        'attributes' => array(),
      ));
    }

    // If the message has been answered, show the icon.
    if (!empty($message->message_answered)) {
      $message_answered = theme_image(array(
        'path' => $icon_path . 'mini-edit.png',
        'alt' => 'Generic icon indicating message has been answered.',
        'title' => t('You have replied to this message.'),
        'attributes' => array(),
      ));
    }

    // The date the message was sent.
    if ($message->message_date != 0) {
      $format_option = variable_get('basic_webmail_format_option', 'small');

      if ($format_option == 'custom') {
        $custom_format = variable_get('basic_webmail_custom_format', 'D, M j, Y - g:i:s a');
        $message->message_date = format_date($message->message_date, $format_option, $custom_format);
      }
      else {
        $message->message_date = format_date($message->message_date, $format_option);
      }

      $message_date = $message->message_date;
    }

    $message_list[$message->message_number] = array(
      'message_unread' => $message_unread,
      'attachment_exists' => $message_attachment,
      'message_answered' => $message_answered,
      'message_subject' => $message->message_subject,
      'from_address' => $message->from_address,
      'message_date' => $message_date,
    );
  }

  return $message_list;
}


/**
 * Retrieves the page the user was viewing when they chose their action.
 *
 * @return array
 *   The previous page the user was viewing when they took the action they
 *   chose. Basically, this uses the parameters of the drupal_goto function. For
 *   a list of possible options, see the url function.
 */
function _basic_webmail_get_return_destination() {
  $orig_path = drupal_get_destination();
  $orig_length = drupal_strlen($orig_path['destination']);
  $orig_start = strpos($orig_path['destination'], '=');
  $new_path = drupal_substr($orig_path['destination'], $orig_start, $orig_length);

  // Restore the converted characters.
  $new_path = str_replace('%25', '%', $new_path);
  $new_path = str_replace('%2F', '/', $new_path);
  $new_path = str_replace('%3F', '?', $new_path);
  $new_path = str_replace('%3D', '=', $new_path);
  $new_path = str_replace('%26', '&', $new_path);

  // Setup the return parameters.
  $query_point = strpos($new_path, '?');
  if ($query_point === FALSE) {
    $dest_path = $new_path;
    $dest_query = NULL;
  }
  else {
    $dest_path = drupal_substr($new_path, 0, strpos($new_path, '?'));
    $dest_query = drupal_substr($new_path, strpos($new_path, '?') + 1, drupal_strlen($new_path));
  }

  // Send the user back to the page they were looking at.
  return array($dest_path, array('query' => $dest_query));
}


/**
 * Deletes a group of messages.
 *
 * @param array $form_state
 *   The form values of the passed form.
 * @param array $messages
 *   The messasges to delete.
 * @param stream $imap_resource
 *   An IMAP stream, which is a connection to the mailbox on the server, and is
 *   created by calling _basic_webmail_connect_to_server.
 */
function _basic_webmail_mail_list_form_submit_delete($form_state, $messages, $imap_resource) {
  $error = 0;
  $message_count = 0;

  foreach ($messages as $message_number) {
    if ($form_state['values']['folder'] != 'INBOX.Trash') {
      if (!imap_mail_move($imap_resource, $message_number, 'INBOX.Trash')) {
        include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
        // Report the error.
        $message = 'There was an error deleting message #@message_number.';
        $message_parameters = array('@message_number' => $message_number);
        _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
        $error++;
      }
      else {
        $message_count++;
      }
    }
    else {
      if (!imap_delete($imap_resource, $message_number)) {
        include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
        // Report the error.
        $message = 'There was an error deleting message #@message_number.';
        $message_parameters = array('@message_number' => $message_number);
        _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
        $error++;
      }
      else {
        $message_count++;
      }
    }
  }

  if (!imap_expunge($imap_resource)) {
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    // Report the error.
    $message = 'There was an error expunging the mailboxes.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    $error++;
  }

  if (empty($error)) {
    drupal_set_message(t('@message_count messages were successfully deleted.', array('@message_count' => $message_count)));
  }
}


/**
 * Marks a group of messages as read.
 *
 * @param array $form_state
 *   The form values of the passed form.
 * @param array $messages
 *   The messasges to delete.
 * @param stream $imap_resource
 *   An IMAP stream, which is a connection to the mailbox on the server, and is
 *   created by calling _basic_webmail_connect_to_server.
 */
function _basic_webmail_mail_list_form_submit_mark_read($form_state, $messages, $imap_resource) {
  $error = 0;
  $message_count = 0;

  foreach ($messages as $message_number) {
    if (!imap_setflag_full($imap_resource, $message_number, '\\Seen')) {
      include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
      // Report the error.
      $message = 'There was an error setting the flag of message #@message_number.';
      $message_parameters = array('@message_number' => $message_number);
      _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
      $error++;
    }
    else {
      $message_count++;
    }
  }

  if (empty($error)) {
    drupal_set_message(t('@message_count messages were successfully marked as read.', array('@message_count' => $message_count)));
  }
}


/**
 * Marks a group of messages as unread.
 *
 * @param array $form_state
 *   The form values of the passed form.
 * @param array $messages
 *   The messasges to delete.
 * @param stream $imap_resource
 *   An IMAP stream, which is a connection to the mailbox on the server, and is
 *   created by calling _basic_webmail_connect_to_server.
 */
function _basic_webmail_mail_list_form_submit_mark_unread($form_state, $messages, $imap_resource) {
  $error = 0;
  $message_count = 0;

  foreach ($messages as $message_number) {
    if (!imap_clearflag_full($imap_resource, $message_number, '\\Seen')) {
      include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
      // Report the error.
      $message = 'There was an error clearing the flag of message #@message_number.';
      $message_parameters = array('@message_number' => $message_number);
      _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
      $error++;
    }
    else {
      $message_count++;
    }
  }

  if (empty($error)) {
    drupal_set_message(t('@message_count messages were successfully marked as unread.', array('@message_count' => $message_count)));
  }
}


/**
 * Copies a group of messages to another folder.
 *
 * @param array $form_state
 *   The form values of the passed form.
 * @param array $messages
 *   The messasges to delete.
 * @param stream $imap_resource
 *   An IMAP stream, which is a connection to the mailbox on the server, and is
 *   created by calling _basic_webmail_connect_to_server.
 */
function _basic_webmail_mail_list_form_submit_copy_to($form_state, $messages, $imap_resource) {
  $error = 0;
  $message_count = 0;

  foreach ($messages as $message_number) {
    if (!imap_mail_copy($imap_resource, $message_number, $form_state['values']['folder_name'])) {
      include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
      // Report the error.
      $message = 'There was an error copying message #@message_number.';
      $message_parameters = array('@message_number' => $message_number);
      _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
      $error++;
    }
    else {
      $message_count++;
    }
  }

  if (!imap_expunge($imap_resource)) {
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    // Report the error.
    $message = 'There was an error expunging the mailboxes.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    $error++;
  }

  if (empty($error)) {
    drupal_set_message(t('@message_count messages were successfully copied to @folder_name.', array('@message_count' => $message_count, '@folder_name' => $form_state['values']['folder_name'])));
  }
}


/**
 * Moves a group of messages to another folder.
 *
 * @param array $form_state
 *   The form values of the passed form.
 * @param array $messages
 *   The messasges to delete.
 * @param stream $imap_resource
 *   An IMAP stream, which is a connection to the mailbox on the server, and is
 *   created by calling _basic_webmail_connect_to_server.
 */
function _basic_webmail_mail_list_form_submit_move_to($form_state, $messages, $imap_resource) {
  $error = 0;
  $message_count = 0;

  foreach ($messages as $message_number) {
    if (!imap_mail_move($imap_resource, $message_number, $form_state['values']['folder_name'])) {
      include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
      // Report the error.
      $message = 'There was an error moving message #@message_number.';
      $message_parameters = array('@message_number' => $message_number);
      _basic_webmail_report_error($message, $message_parameters, WATCHDOG_ERROR, TRUE);
      $error++;
    }
    else {
      $message_count++;
    }
  }

  if (!imap_expunge($imap_resource)) {
    include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
    // Report the error.
    $message = 'There was an error expunging the mailboxes.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
    $error++;
  }

  if (empty($error)) {
    drupal_set_message(t('@message_count messages were successfully moved to @folder_name.', array('@message_count' => $message_count, '@folder_name' => $form_state['values']['folder_name'])));
  }
}


/**
 * Checks if the standard mailboxes exist and creates them, if necessary.
 *
 * This function checks to see if the standard mailboxes exit or not. If they
 * do not exist, this function will also create them. This fucntion will then
 * subscribe to the mailboxes.
 *
 * @param integer $user_id
 *   The user ID of the mailbox to subscribe to.
 */
function _basic_webmail_subscribe_to_mailboxes($user_id) {
  // Connect to the server and retrieve a connection to the mailbox.
  include_once drupal_get_path('module', 'basic_webmail') . '/basic_webmail.common.inc';
  $imap_resource = _basic_webmail_connect_to_server($user_id);
  if (empty($imap_resource)) {
    $message = 'There was an error connecting to the mail server. Contact the system administrator and/or check the logs for more information.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return;
  }

  $mailboxes = array(
    'INBOX',
    'INBOX.Drafts',
    'INBOX.Junk',
    'INBOX.Sent',
    'INBOX.Trash',
  );
  foreach ($mailboxes as $mailbox) {
    if (!imap_status($imap_resource, _basic_webmail_get_server_string() . $mailbox, SA_ALL)) {
      if (!imap_createmailbox($imap_resource, _basic_webmail_get_server_string() . $mailbox)) {
        $message = 'There was an error creating to the !mailbox mailbox.';
        $message_parameters = array('!mailbox' => $mailbox);
        _basic_webmail_report_error($message, $message_parameters, WATCHDOG_WARNING);
      }
    }

    if (!imap_subscribe($imap_resource, _basic_webmail_get_server_string() . $mailbox)) {
      $message = 'There was an error subscribing to the !mailbox mailbox.';
      $message_parameters = array('!mailbox' => $mailbox);
      _basic_webmail_report_error($message, $message_parameters, WATCHDOG_WARNING);
    }
  }

  // Clean up.
  if (!imap_close($imap_resource)) {
    $message = 'There was an error closing the IMAP stream.';
    _basic_webmail_report_error($message, array(), WATCHDOG_WARNING);
  }
}


/**
 * Processes a messasge's subject for storage in the database.
 *
 * @param string $subject
 *   The subject of the message.
 * @param integer $new_or_unread
 *   Indicates whether the message is new or unread or neither. If the message
 *   is new or unread, this is TRUE.
 * @param integer $message_number
 *   The number of the message in the mailbox.
 * @param string $folder
 *   The folder the message is in.
 *
 * @return string
 *   The subject of the message processed as a link for display and for storage
 *   in the database.
 */
function _basic_webmail_process_subject($subject, $new_or_unread, $message_number, $folder) {
  $number_characters = variable_get('basic_webmail_subject_characters', 40);

  if ($subject != '(No subject specified)') {
    $subject = _basic_webmail_prepare_email_string($subject);

    // If the link to the subject is longer than the database field will allow,
    // shorten the displayed subject so it fits, accounting for the ellipsis.
    $link_len = drupal_strlen($folder) + drupal_strlen($message_number) + 55;

    // Check to see if the admin specified a maximum character length for the
    // displayed subject.
    if (!empty($number_characters)) {
      // Make sure that length is not longer than the database field will allow.
      if (255 - $link_len < $number_characters) {
        $subject = truncate_utf8($subject, 255 - $link_len, TRUE, TRUE);
      }
      else {
        $subject = truncate_utf8($subject, $number_characters, TRUE, TRUE);
      }
    }
    elseif (drupal_strlen($subject) + $link_len > 255) {
      $subject = truncate_utf8($subject, 255 - $link_len, TRUE, TRUE);
    }
  }

  // Create a link to the message.
  $message_subject_link = l($subject, 'basic_webmail/view/' . $folder . '/' . $message_number);

  // Set the message's read status.
  if (!empty($new_or_unread)) {
    $message_subject = '<strong>' . $message_subject_link . '</strong>';
  }
  else {
    $message_subject = $message_subject_link;
  }

  return $message_subject;
}


/**
 * Processes the address of the messasge sender for storage in the database.
 *
 * @param object $from
 *   The sender of the message.
 *
 * @return string
 *   The address of the sender processed as a link for display and for storage
 *   in the database.
 */
function _basic_webmail_process_from_address($from) {
  if (empty($from->mailbox) && empty($from->host)) {
    // If the sender's mailbox and host are blank set a generic from name.
    $from_address = '(Unknown)';
  }
  elseif (!empty($from->personal)) {
    $link_name = _basic_webmail_prepare_email_string($from->personal);

    $from_address = l($link_name, 'basic_webmail/sendmail/' . $link_name . ' <' . $from->mailbox . '@' . $from->host . '>');

    if (drupal_strlen($from_address) > 255) {
      // Make the display name only the length of the available field size,
      // accounting for the ellipsis.
      $display_name = truncate_utf8($link_name, drupal_strlen($from_address) - 258, TRUE, TRUE);
      $from_address = l($display_name, 'basic_webmail/sendmail/' . $link_name . ' <' . $from->mailbox . '@' . $from->host . '>');
    }
  }
  else {
    $from_address = l($from->mailbox . '@' . $from->host, 'basic_webmail/sendmail/' . $from->mailbox . '@' . $from->host);
  }

  return $from_address;
}
