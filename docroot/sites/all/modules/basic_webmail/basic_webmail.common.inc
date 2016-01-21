<?php
/**
 * @file
 * Common functions that are used by much of the module, but do not need to be
 * included in basic_webmail.module.
 */

/**
 * Connect to the mail server.
 *
 * @param integer $user_id
 *   The user ID of the mailbox to subscribe to.
 * @param string $folder
 *   The name of the folder to open. If no folder is specified, the default
 *   of INBOX is used.
 *
 * @return stream
 *   An IMAP stream, which is the mailbox connection, if successful, or FALSE,
 *   if not.
 */
function _basic_webmail_connect_to_server($user_id, $folder = 'INBOX') {
  // Retrieve the user object for the user accessing the mail server.
  $user = user_load($user_id);
  if (empty($user->data['basic_webmail_username']) || empty($user->data['basic_webmail_password'])) {
    $message = '!name has not configured their !user_settings_link.';
    $message_parameters = array(
      '!name' => $user->name,
      '!user_settings_link' => l(t('mail settings'), 'user/' . $user->uid . '/edit'),
    );
    _basic_webmail_report_error($message, $message_parameters);
    return FALSE;
  }

  // Get the users email login and password.
  $username = $user->data['basic_webmail_username'];
  $password = _basic_webmail_decrypt_password($user->data['basic_webmail_password']);

  $mailbox = _basic_webmail_get_server_string() . $folder;

  // Make the actual connection.
  if (version_compare(phpversion(), '5.2.0', '>=')) {
    $imap_resource = imap_open($mailbox, $username, $password, 0, 1);
  }
  else {
    $imap_resource = imap_open($mailbox, $username, $password, 0);
  }

  if ($imap_resource == FALSE) {
    $message = 'There was an error accessing the remote server.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
  }

  return $imap_resource;
}


/**
 * Create the server connection string.
 *
 * @return string
 *   The string representing the server connection information.
 */
function _basic_webmail_get_server_string() {
  // Initialize variable.
  $server_address = variable_get('basic_webmail_server_address', '');

  // Sanity check -- make sure the admin has done the configuration.
  if (empty($server_address)) {
    $message = 'The !server_settings for Basic webmail have not yet been configured. Please do that before checking your e-mail.';
    $message_parameters = array(
      '!server_settings' => l(t('server settings'), 'admin/settings/basic_webmail')
    );
    _basic_webmail_report_error($message, $message_parameters);
    return FALSE;
  }

  // Prepare the flags for connecting to the server with.
  $imap_flags = '';
  if (variable_get('basic_webmail_secure_log_in', 0) == 1) {
    $imap_flags .= '/secure';
  }
  if (variable_get('basic_webmail_use_ssl', 0) == 1) {
    $imap_flags .= '/ssl';
  }
  if (variable_get('basic_webmail_validate_cert', 0) == 1) {
    $imap_flags .= '/validate-cert';
  }
  else {
    $imap_flags .= '/novalidate-cert';
  }
  if (variable_get('basic_webmail_use_tls', 0) == 1) {
    $imap_flags .= '/tls';
  }
  else {
    $imap_flags .= '/notls';
  }

  // Return the configured settings and create a connection string.
  return '{' . $server_address . ':' . variable_get('basic_webmail_server_port', '143') . $imap_flags . '}';
}


/**
 * Reports error messages.
 *
 * @todo
 *   Handle t().
 *
 * @param string $message
 *   The additional text to be saved and/or displayed.
 * @param boolean $display
 *   Indicates whether to display the error message to the user or not.
 * @param boolean $log
 *   Indicates whether to save the error message to the logs or not.
 * @param integer $severity
 *   The severity of the message, as per RFC 3164. See
 *   http://api.drupal.org/api/drupal/includes--bootstrap.inc/group/logging_severity_levels/7
 *   for more information.
 */
function _basic_webmail_report_error($message, $message_parameters = array(), $severity = WATCHDOG_ERROR, $display = FALSE, $log = TRUE) {
  if ($severity != WATCHDOG_INFO && $severity != WATCHDOG_NOTICE) {
    // Get the most recent IMAP error message.
    $last_error = imap_last_error();
    if ($last_error === FALSE) {
      // There was no recent IMAP error message, so add a generic message.
      $message_parameters['!last_error'] = 'There was no additional information provided by the system.';
    }
    else {
      $message_parameters['!last_error'] = $last_error;
    }
  }

  // The message is to be displayed to the user.
  if ($display) {
    if ($severity == WATCHDOG_DEBUG || $severity == WATCHDOG_INFO || $severity == WATCHDOG_NOTICE) {
      $dsm_severity = 'status';
    }
    elseif ($severity == WATCHDOG_WARNING) {
      $dsm_severity = 'warning';
    }
    else {
      $dsm_severity = 'error';
    }

    if (!empty($message_parameters['!last_error'])) {
      drupal_set_message(t($message . ' !last_error', $message_parameters), $dsm_severity);
    }
    else {
      drupal_set_message(t($message, $message_parameters), $dsm_severity);
    }
  }

  // The message is to be saved in the system logs.
  if ($log) {
    if (!empty($message_parameters['!last_error'])) {
      watchdog('basic_webmail', $message . ' !last_error', $message_parameters, $severity);
    }
    else {
      watchdog('basic_webmail', $message, $message_parameters, $severity);
    }
  }
}


/**
 * Processes email strings.
 *
 * This function takes a string from an email (subject, user name, etc.) and
 * processes it thus:
 * - It converts MIME-encoded text to UTF-8.
 * - It converts it to plain text for displaying in HTML.
 *
 * @param string $string
 *   The string to process.
 * @param string $char_set
 *   The character set for MIME decoding.
 *
 * @return
 *   The processed string.
 */
function _basic_webmail_prepare_email_string($string, $char_set = 'UTF-8') {
  // Convert the MIME-encoded string to UTF-8.
  $string = _basic_webmail_decode_mime_str($string, $char_set);
  // Convert the string to plain-text for display as HTML.
  $string = check_plain($string);
  return $string;
}


/**
 * Converts MIME-encoded text to UTF-8.
 *
 * This function is necessary, because there is a bug in some versions of PHP
 * that returns strings converted with the imap_utf8() function in all upper
 * case letters.  http://bugs.php.net/bug.php?id=44098
 *
 * This function was taken from here and modified:
 * http://us2.php.net/manual/en/function.imap-utf8.php#92026
 *
 * This was fixed in PHP version 5.3.2.
 *
 * @param string $string
 *   The string to convert,
 * @param string $char_set
 *   The character set to convert the string to. If not specified, it defaults
 *   to UTF-8.
 *
 * @return string
 *   The converted string.
 */
function _basic_webmail_decode_mime_str($string, $char_set = 'UTF-8') {
  $new_string = '';
  $elements = imap_mime_header_decode($string);

  for ($i = 0; $i < count($elements); $i++) {
    if ($elements[$i]->charset == 'default') {
      $elements[$i]->charset = 'iso-8859-1';
    }

    $new_string .= iconv($elements[$i]->charset, $char_set, $elements[$i]->text);
  }

  return $new_string;
}


/**
 * Gets a list of all the possible parts of a message.
 *
 * Much of this function was taken from
 * http://us2.php.net/manual/en/function.imap-fetchstructure.php#51497
 *
 * @todo This function needs to be rethought through and done better. It works
 * for now, so I'm leaving it until later.
 *
 * @param resource $imap_resource
 *   The IMAP stream, created by calling _basic_webmail_connect_to_server().
 * @param integer $message_number
 *   The number of the message to get the information for.
 *
 * @return array
 *   All of the available message parts, or an empty array, if there is an
 *   error. The array is structured thusly:
 *     $parts_list[$part_id] => array(
 *       '$part_type',
 *       '$part_structure'
 *     );
 */
function _basic_webmail_get_parts_list($imap_resource, $message_number) {
  // Initialize the return value.
  $parts_list = array();

  // Retrieve the message structure.
  $message_structure = imap_fetchstructure($imap_resource, $message_number);
  if (empty($message_structure)) {
    $message = 'There was an error retrieving the message structure.';
    _basic_webmail_report_error($message, array(), WATCHDOG_ERROR, TRUE);
    return array();
  }

  if ($message_structure->type == 0) {
    // This message has a simple structure, i.e.: there is only one part.
    $parts_list[1] = array($message_structure->subtype);
  }
  else {
    // This message has a complex structure, i.e.: there are multiple parts.
    // Pull the parts from the message structure.
    $parts = $message_structure->parts;
    $endwhile = FALSE;
    // Stack while parsing message.
    $stack = array();
    // Incrementer.
    $i = 0;

    // Do while the marker is false.
    while (!$endwhile) {
      // If there is no part for this item...
      if (empty($parts[$i])) {
        // If the number of items on the stack are one or more...
        if (count($stack) > 0) {
          // Process the subparts.
          // Decrement the stack count by one.
          $stack_count = count($stack) - 1;
          // Overwrite the parts list with what is on the stack.
          $parts = $stack[$stack_count]['p'];
          // Set the incrementer.
          $i = $stack[$stack_count]['i'] + 1;
          // Remove the last item from the stack.
          array_pop($stack);
        }
        else {
          // End the process.
          $endwhile = TRUE;
        }
      }

      if (empty($endwhile)) {
        // Create message part first (example '1.2.3').
        $partstring = '';

        // Fill in the parts string.
        foreach ($stack as $s) {
          $partstring .= ($s['i'] + 1) . '.';
        }

        // Finish it.
        $partstring .= ($i + 1);

        // Determine attachment status.
        // If ifdisposition == 1 (and disposition == 'attachment'), then there's
        // one or more attachments.
        // PHP reports some attachments (specifically PDF files) as an HTML body
        // type, so checking type is not reliable.
        if (!empty($parts[$i])) {
          if (!empty($parts[$i]->ifdisposition) && !empty($parts[$i]->dparameters[0]->value)) {
            // One or more attachments exists.
            $parts_list[$partstring] = array(drupal_strtoupper($parts[$i]->subtype), $parts[$i]);
          }
          else {
            // No attachments exist.
            $parts_list[$partstring] = array(drupal_strtoupper($parts[$i]->subtype));
          }
        }
      }

      // If there are subparts.
      if (!empty($parts[$i]->parts)) {
        // Add to the stack, filling it with the subparts and the increment
        // number.
        $stack[] = array(
          'p' => $parts,
          'i' => $i,
        );
        // Reset the parts array with the subparts for continued processing.
        $parts = $parts[$i]->parts;
        // Reset the incrementer.
        $i = 0;
      }
      else {
        // Bump the incrementer.
        $i++;
      }
    }
  }

  return $parts_list;
}


/**
 * Processes the attachment for the specified message number and message part.
 *
 * @param resource $imap_resource
 *   An IMAP stream returned by imap_open().
 * @param integer $message_number
 *   The number of the message to get the information for.
 * @param part_id
 *   The ID of the part to get the information for.
 * @param object $part_structure
 *   The structure definitaion of the part of the message that represents the
 *   attachment.
 *
 * @return array
 *   All of the available message parts, or FALSE if there is an error.
 */
function _basic_webmail_process_attachment($imap_resource, $message_number, $part_id, $part_structure) {
  $file_name = $part_structure->dparameters[0]->value;
  $file_data = _basic_webmail_mail_fetchpart($imap_resource, $message_number, $part_id);

  // If the attachment is encoded, decode it.
  if ($part_structure->encoding == 3) {
    // Decode the base64 encoded file.
    $file_data = base64_decode($file_data);
  }
  elseif ($part_structure->encoding == 4) {
    // Decode the quoted printable encoded file.
    $file_data = quoted_printable_decode($file_data);
  }

  // Create the destination path variable.
  $file_dest = variable_get('basic_webmail_attachment_location', file_default_scheme() . '://attachments');
  // Add the user's login ID to the path.
  global $user;
  $file_dest .= '/' . $user->name;
  // Create any necessary directories.
  file_prepare_directory($file_dest, FILE_CREATE_DIRECTORY);

  // Don't show any directory creation messages to the user.
  if (isset($_SESSION['messages']) && isset($_SESSION['messages']['status'])) {
    foreach ($_SESSION['messages']['status'] as $key => $drupal_message) {
      if (drupal_substr($drupal_message, 0, 13) == 'The directory' && drupal_substr($drupal_message, drupal_strlen($drupal_message) - 17, 17) == 'has been created.') {
        unset($_SESSION['messages']['status'][$key]);
      }
    }
  }

  // Save the file.
  $file_object = file_save_data($file_data, $file_dest . '/' . $file_name, FILE_EXISTS_REPLACE);
  // Create a URL, with which to link to the file.
  $file_url = file_create_url($file_object->uri);

  // Return the necessary data.
  return array($file_dest, $file_name, $file_url);
}


 /**
 * Encrypts a user's password.
 *
 * If neither of the Encryption (http://drupal.org/project/encrypt) or AES
 * encryption (http://drupal.org/project/aes) modules are installed, this
 * function will simply return the value passed in.
 *
 * @param string $password
 *   The password to encrypt.
 *
 * @return
 *   The encrypted password, if the option to encrypt is enabled and an
 *   encryption module is installed, or the passed in value, if not.
 */
function _basic_webmail_encrypt_password($password) {
  if (variable_get('basic_webmail_use_encryption', 0) == 1) {
    if (module_exists('encrypt')) {
      // Encrypt the password.
      $password = encrypt($password);
    }
    elseif (module_exists('aes')) {
      // Encrypt the password.
      $password = aes_encrypt($password);
    }
  }

  return $password;
}


/**
 * Decrypts a user's password.
 *
 * If neither of the Encryption (http://drupal.org/project/encrypt) or AES
 * encryption (http://drupal.org/project/aes) modules are installed, this
 * function will simply return the value passed in.
 *
 * @param string $password
 *   The password to decrypt.
 *
 * @return
 *   The decrypted password, if the option to encrypt is enabled and an
 *   encryption module is installed, or the passed in value, if not.
 */
function _basic_webmail_decrypt_password($password) {
  if (variable_get('basic_webmail_use_encryption', 0) == 1) {
    if (module_exists('encrypt')) {
      // Decrypt the password.
      $password = decrypt($password);
    }
    elseif (module_exists('aes')) {
      // Decrypt the password.
      $password = aes_decrypt($password);
    }
  }

  return $password;
}


/** *******************************************************
 *
 *  The next four functions are from
 *  http://us3.php.net/manual/en/function.imap-fetchbody.php#31685
 *  with modifications for Drupal Coding Standards:
 *  http://drupal.org/coding-standards
 *  and what is already here in this module.
 *
 *  ******************************************************/

/**
 * Returns the body of a part of a message.
 *
 * @param resource $imap_resource
 *   An IMAP stream returned by imap_open().
 * @param integer $message_number
 *   The number of the message.
 * @param string $part
 *   The message part of which the body is returned.
 *
 * @return string
 *   The message body for the part of the message indicated.
 */
function _basic_webmail_mail_fetchpart($imap_resource, $message_number, $part) {
  $parts = _basic_webmail_mail_fetchparts($imap_resource, $message_number);
  $part_numbers = explode(".", $part);
  $current_part = $parts;

  while (list($key, $val) = each($part_numbers)) {
    $current_part = $current_part[$val];
  }

  if ($current_part != '') {
    return $current_part;
  }
  else {
    return FALSE;
  }
}

/**
 * Returns the bodies of all parts of an email.
 *
 * The structure of the array corresponds to the structure that is available
 * with imap_fetchstructure().
 *
 * @param resource $imap_resource
 *   An IMAP stream returned by imap_open().
 * @param integer $message_number
 *   The number of the message.
 *
 * @return array
 *   The bodies of the subparts of the message.
 */
function _basic_webmail_mail_fetchparts($imap_resource, $message_number) {
  $parts = array();
  $header = imap_fetchheader($imap_resource, $message_number);
  $body = imap_body($imap_resource, $message_number, FT_INTERNAL);
  $i = 1;

  $new_parts = _basic_webmail_mail_mimesplit($header, $body);
  if (!empty($new_parts)) {
    while (list($key, $val) = each($new_parts)) {
      $parts[$i] = _basic_webmail_mail_mimesub($val);
      $i++;
    }
  }
  else {
    $parts[$i] = $body;
  }

  return $parts;
}

/**
 * Splits a mulitpart mime message body into its respective parts.
 *
 * @param string $header
 *   The header of the message.
 * @param string $body
 *   The body of the message.
 *
 * @return array
 *   The message body's parts, or FALSE, if no parts are found.
 */
function _basic_webmail_mail_mimesplit($header, $body) {
  // Initialize the message parts variable.
  $parts = array();
  // Matches any character zero or more times, followed by 'boundary="' (case
  // insensitive), followed by any character other than '"' one or more times,
  // followed by '"'.
  $pattern = '~.*boundary="[^"]+"~i';
  // Search the message header for the message part boundary definition, which
  // is what $pattern describes, and save the results in $regs.
  if (preg_match($pattern, $header, $regs)) {
    // I can't seem to figure out how to get the previous match to work 100%
    // with PCRE as it was originally intended to work in POSIX, so I've
    // created this interim step, which ends up producing the desired results.
    $interim_pattern = '~"[^"]+"~';
    preg_match($interim_pattern, $regs[0], $interim_results);
    // Save the actual boundary from the results.
    $boundary = drupal_substr($interim_results[0], 1, drupal_strlen($interim_results[0]) - 2);

    // Matches zero or more characters that are not '\r\n' before and after
    // what is in '$boundary'.
    $delimiter_reg = '~[^\r\n]*' . $boundary . '[^\r\n]*~';

    // Search $body for any instance of the boundary, which is what
    // $delimiter_reg describes, and save the results in $results.
    if (preg_match($delimiter_reg, $body, $results)) {
      // Break out the results into the relevant pieces.
      $delimiter = $results[0];
      $parts = explode($delimiter, $body);
      $parts = array_slice($parts, 1, -1);
    }

    // Deliver the payload.
    return $parts;
  }
  // No parts were found, so return FALSE.
  else {
    return FALSE;
  }
}

/**
 * Returns the subparts of the given part.
 *
 * @param string $part
 *   The message part in which to look for subparts.
 *
 * @return array
 *   The subparts of the given part. If no subparts are found, returns the body
 *   of the current part.
 */
function _basic_webmail_mail_mimesub($part) {
  $i = 1;
  $head_delimiter = "\r\n\r\n";
  $del_length = drupal_strlen($head_delimiter);

  // get head & body of the current part
  $end_of_head = strpos($part, $head_delimiter);
  $head = drupal_substr($part, 0, $end_of_head);
  $body = drupal_substr($part, $end_of_head + $del_length, drupal_strlen($part));

  // check whether it is a message according to rfc822
  if (stristr($head, "Content-Type: message/rfc822")) {
    $part = drupal_substr($part, $end_of_head + $del_length, drupal_strlen($part));
    $return_parts[1] = _basic_webmail_mail_mimesub($part);
    return $return_parts;
  }
  // if no message, get subparts and call function recursively
  elseif ($sub_parts = _basic_webmail_mail_mimesplit($head, $body)) {
    // got more subparts
    while (list($key, $val) = each($sub_parts)) {
      $return_parts[$i] = _basic_webmail_mail_mimesub($val);
      $i++;
    }

    return $return_parts;
  }
  else {
    return $body;
  }
}
// End of four imap_fetchbody functions.


/**
 * Possibly unneedded functions follow.
 */

/**
 * Checks for the existence of a specified mailbox.
 *
 * @param resource $imap_resource
 *   An IMAP stream returned by imap_open().
 * @param string $mailbox
 *   The name of the mailbox to check for.
 *
 * @return boolean
 *   Returns TRUE if the mailbox exists, and FALSE if it does not exist.
 */
function _basic_webmail_mailbox_exists($imap_resource, $mailbox) {
  $mailbox_exists = FALSE;

  $mailboxes = imap_getmailboxes($imap_resource, _basic_webmail_get_server_string(), '*');
  foreach ($mailboxes as $mailbox_object) {
    if (drupal_substr($mailbox_object->name, strpos($mailbox_object->name, '}') + 1, drupal_strlen($mailbox_object->name)) == $mailbox) {
      $mailbox_exists = TRUE;
    }
  }

  return $mailbox_exists;
}
