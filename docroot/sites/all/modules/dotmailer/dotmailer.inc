<?php

/**
 * @file
 *  This include provides all the joining functions
 *  required for using the Dotmailer API with Drupal.
 *
 * TODO - A cleanup of the dotmailer.class.php file would probably mean that we
 *        could remove this file.
 */

/**
 * Get a dotmailer API object for communication with the dotmailer server
 *
 * @return object
 */
function _dotmailer_api_object() {
  module_load_include('php', 'dotmailer', 'dotmailer.class');
  return new dotMAPI();
}

/**
 * Returns username and password details for dotMailer.
 *
 * @return array
 */
function _dotmailer_credentials($username, $password) {
  $username = variable_get($username);
  $password = variable_get($password);
  if ($username && !$password) {
    return FALSE;
  }
  return array(
    'username' => $username,
    'password' => $password,
  );
}

/**
 * We check the return value from an API function and if there
 * is a Soap fault error then add this to watchdog and null the
 * result value
 *
 * @param object $r
 *
 * @return
 */
function _dotmailer_checkreturn($r) {

  if ($r instanceof SoapFault) {
    $message = $r->getMessage();
    // If the contact just isn't in the system then don't error them
    if (strpos($message, 'ERROR_CONTACT_NOT_FOUND') !== FALSE) {
      return FALSE;
    }
    else {
      watchdog('dotmailer', $message, array(), WATCHDOG_ERROR);
      drupal_set_message(t('There was an error updating your subscription, consult the log for more information.'), 'error');
    }
    return NULL;
  }
  return $r;
}

/**
 * Obtain a full contact by email address. This will return a full
 * dotmailer contact, with the flag to include an array of address
 * books values is requested
 *
 * @param object $email
 * @param object $inc_address
 *
 * @return object $result
 */
function dotmailer_getcontactbyemail($username, $password, $email, $inc_address = TRUE) {
  $c = _dotmailer_api_object();
  $result = $c->getContactByEmail(_dotmailer_credentials($username, $password), $email, $inc_address);
  return _dotmailer_checkreturn($result);
}

/**
 * Obtain a list of address books for the account
 *
 * @return object $result
 */
function dotmailer_listaddressbooks($username, $password) {
  $c = _dotmailer_api_object();
  $result = $c->listAddressBooks(_dotmailer_credentials($username, $password));
  if (empty($result)) {
    return array();
  }
  return _dotmailer_checkreturn($result);
}

/**
 * Obtain a list of all contacts for the account
 *
 * NOTE NOT CURRENTLY USED
 *
 * @return
 */
function dotmailer_listcontacts() {
  throw new Exception('Not currently used');
  return FALSE; //$c->listContactsInAddressBook($login, 11550, 10, 0);
}

/**
 * Obtain a list of the datafields used across all the address books
 *
 * @return object $result
 */
function dotmailer_listcontactdatalabels($username, $password) {
  $c = _dotmailer_api_object();
  $result = $c->listContactDataLabels(_dotmailer_credentials($username, $password));
  return _dotmailer_checkreturn($result);
}

/**
 * Add a new contact to an address book, basically subscribing
 * the user from Drupal into Dotmailer
 *
 * @param object $book
 * @param object $email
 * @param object $data_fields
 * @param object $optin
 * @param object $email_type
 *
 * @return boolean $result
 */
function dotmailer_addcontacttoaddressbook($username, $password, $book_id, $email, $data_fields, $optin, $email_type) {
  $c = _dotmailer_api_object();
  $result = $c->addContactToAddressBook(_dotmailer_credentials($username, $password), $book_id, $email, $data_fields, $optin, $email_type);
  return _dotmailer_checkreturn($result);
}

/**
 * Updates the contact for a specific address book
 *
 * @param object $book_id
 * @param object $contact_id
 * @param object $email
 * @param object $data_fields
 * @param object $optin_type
 * @param object $email_type [optional]
 * @param object $audience_type [optional]
 *
 * @return boolean $result
 */
function dotmailer_updatecontact($username, $password, $book_id, $contact_id, $email, $data_fields, $optin_type, $email_type = 'Html', $audience_type = 'Unknown') {
  $c = _dotmailer_api_object();
  $result = $c->updateContact(_dotmailer_credentials($username, $password), $book_id, $contact_id, $email, $data_fields, $optin_type, $email_type, $audience_type);
  return _dotmailer_checkreturn($result);
}

/**
 * Removes a contact from an address book
 *
 * @param object $book_id
 * @param object $email
 * @param object $resubscribe [optional]
 * @param object $totalunsub [optional]
 *
 * @return boolean $result
 */
function dotmailer_removecontactfromaddressbook($username, $password, $book_id, $email, $resubscribe = FALSE, $totalunsub = FALSE) {
  $c = _dotmailer_api_object();
  $result = $c->removeContactFromAddressBook(_dotmailer_credentials($username, $password), $book_id, $email, $resubscribe, $totalunsub);
  return _dotmailer_checkreturn($result);
}

/**
 * Get all of the available books
 * Optionally filter by user
 *
 * @param object $user (optional)
 *
 * @return array $available_books
 */
function _dotmailer_get_available_books($user = NULL) {
  $address_books = variable_get('dotmailer_books', array());
  $available_books = array();
  if ($address_books) {
    foreach ($address_books as $address_book) {
      if ($user) {
        foreach (array_keys((array)$user->roles) as $rid) {
          if (!empty($address_book->roles[$rid])) {
            $available_books[] = $address_book;
            break;
          }
        }
      }
      else {
        $available_books[] = $address_book;
      }
    }
  }
  return $available_books;
}

/**
 * Get book based on ID
 *
 * @param object $book_id
 *
 * @return
 */
function _dotmailer_get_book($book_id) {
  $all_books = _dotmailer_get_available_books();
  if (!empty($all_books)) {
    foreach ($all_books as $book) {
      if ($book->Id == $book_id) {
        return $book;
      }
    }
  }
}

/**
 * Get all of the books that the given user must be subscribed to
 *
 * @return array $books
 */
function _dotmailer_get_required_books() {
  $data = variable_get('dotmailer_books');
  $address_books = array();
  if ($data) {
    foreach ($data as $address_book) {
      if ($address_book->booktype === DOTMAILER_BOOKTYPE_REQUIRED) {
        $address_books[] = $address_book;
      }
    }
  }
  return $address_books;
}

/**
 * Return a list of keys for profile fields that can be used
 * to match against dotmailer datafields
 *
 * @todo $register variable is unitialised
 */
function dotmailer_get_datafields_keys() {
  $out = array(0 => '<none>');

  $user_fields = array(
    'name'      => t('Username'),
    'mail'      => t('Email Address'),
    'uid'       => t('User ID'),
    'signature' => t("User's Signature"),
    'created'   => t("User's Creation Date"),
    'access'    => t("User's Last Access Date"),
  );

  foreach ($user_fields as $key => $field) {
    $out[$key] = t('User: !field', array('!field' => $field));
  }

  if (function_exists('_profile_get_fields')) {
    $categories = profile_categories();
    if (!empty($categories)) {
      foreach ($categories as $category) {
        $result = _profile_get_fields($category['name'], $register);
        while ($field = db_fetch_object($result)) {
          $out[$field->name] = t('Profile: !cat - !field', array('!cat' => $field->category, '!field' => $field->title));
        }
      }
    }
  }

  if (function_exists('token_get_book')) {
    $tokens = token_get_list(array('user', 'order'));
    if (is_array($tokens['user'])) {
      foreach ($tokens['user'] as $token => $name) {
        $out['token_' . $token] = t('Token: !field', array('!field' => $name));
      }
    }
  }
  return $out;
}

/**
 * Return a list of datafield values matched against the Drupal
 * profile field keys
 *
 * @deprecated because it does very strange things.
 *
 * @return array $out
 */
function dotmailer_get_datafield_values($user) {

  // Create an array copy of the user object
  $out = (array)$user;

  // Convert all fields to strings. Note that a few of these may become 'Array'
  // or 'Object' but we don't care - they will be ignored anyway.
  $out = array_map('strval', $out);

  // Convert date fields to ISO 8601
  foreach (array('login', 'access', 'created') as $key) {
    $out[$key] = date('c', $out[$key]);
  }

  $out = array_merge($out, _dotmailer_get_user_tokens($user));
  return $out;
}

/**
 * Get the user tokens for datafields
 * @todo Update for D7
 *
 * @return array $out
 */
function _dotmailer_get_user_tokens($user) {
  $out = array();
  if (function_exists('token_get_values')) {
    $vars = token_get_values('user', $user);
    foreach ($vars->tokens as $key => $value) {
      $out['token_' . $value] = $vars->values[$key];
    }
  }
  return $out;
}

/**
 * Get the relevent datafields for the given user for the given address book,
 * although they are generally the same throughout
 *
 * @param object $uid
 * @param object $book_id
 * @param object $all_datafields
 *
 * @return array $uservars
 */
function _dotmailer_load_contact_datalabels($uid, $book_id, $all_datafields = NULL) {
  $address_book = dotmailer_get_saved_address_book($book_id);
  if (!$all_datafields) {
    $all_datafields = dotmailer_listcontactdatalabels($address_book->Username, $address_book->Password);
  }

  $user = user_load($uid);

  $uservars = array();

  foreach ($all_datafields->ContactDataLabel as $datafield) {
    if (!empty($address_book->datafields[$datafield->Name])) {
      $token = $address_book->datafields[$datafield->Name];
      $string = token_replace($token, array('user' => $user), array('clear' => TRUE, 'sanitize' => FALSE));

      $uservars[$datafield->Name]['data'] = $string;
      // At moment just strings are used, can use dates for dob?
      $uservars[$datafield->Name]['type'] = 'string';
    }
  }

  return $uservars;
}

/**
 * Retrieves data about a saved address book or defines an empty object ready for
 * having settings stored.
 *
 * @param int|stdClass $address_book Either an address book object or just the ID
 *
 * @return stdClass
 */
function dotmailer_get_saved_address_book($address_book) {
  $address_books = variable_get('dotmailer_books', NULL);

  if (is_object($address_book)) {
    $id = $address_book->ID;
    $name = $address_book->Name;
  }
  else {
    $id = $address_book;
    $name = "";
  }

  if ($address_books && isset($address_books[$id])) {
    return $address_books[$id];
  }

  $book = (object)array(
    'Id' => $id,
    'Name' => $name,
    'title' => NULL,
    'weight' => NULL,
    'external_url' => NULL,
    'additional_recipients' => NULL,
    'roles' => NULL,
    'description' => NULL,
    'booktype' => NULL,
    'doublein' => NULL,
    'datafields' => array(),
  );
  return $book;
}

/**
 * Subscribe or update a user in a given book
 * @see dotmailer_add_contact()
 */
function _dotmailer_subscribe_user($book, $email, $datafields, $is_subscribed = array(), $message = TRUE, $q = NULL) {
  if ($q || $q = _dotmailer_api_object()) {
    // Convert doublein to opt-in type value
    switch ($book->doublein) {
      case 0:
        // Single opt-in type
        $optin_type = 1;
      break;
      case 1:
        // Double opt-in type
        $optin_type = 2;
      break;
    }

    if ($is_subscribed['subscribed']) {
      $action = 'updated in';
      $success = dotmailer_updatecontact($book->Username,$book->Password,$book->Id,$is_subscribed['contactId'], $email, $datafields, $book->booktype);
      if ($success && $message) {
        drupal_set_message(t('You have updated your settings in %book.', array('%book' => $book->Name)));
      }
    }
    else {
      $action = 'subscribed to';
      $success = dotmailer_addcontacttoaddressbook($book->Username, $book->Password, $book->Id, $email, $datafields, $optin_type, 'Html');

      if ($message && $success && 2 === $optin_type) {
        drupal_set_message(t('You have chosen to subscribe to %book. An email will be sent to your address. Click the link in the email to confirm the subscription.', array('%book' => $book->Name)));
      }
      elseif ($message && $success) {
        drupal_set_message(t('You have been subscribed to %book.', array('%book' => $book->Name)));
      }
    }
  }

  if ($success) {
    watchdog('dotmailer', '@email was @action book @book.', array('@action' => $action, '@email' => $email, '@book' => $book->Name), WATCHDOG_NOTICE);
  }
  else {
    watchdog('dotmailer', '@email has failed to subscribe to @book.', array('@email' => $email, '@book' => $book->Name), WATCHDOG_WARNING);
  }

  return $success;
}

/**
 * Unsubscribe the given user to the given list
 *
 * We have already checked to see if this contact is subscribed
 * for this book, so we don't need to check again
 *
 * @param object $book
 * @param object $mail
 * @param object $message [optional]
 * @param object $q [optional]
 * @param object $total_unsub
 *
 * @return
 */
function _dotmailer_unsubscribe_user($book, $mail, $message = TRUE, $q = NULL, $total_unsub = FALSE) {
  if ($q || $q = _dotmailer_api_object()) {
    $success = dotmailer_removecontactfromaddressbook($book->Username,$book->Password,$book->Id, $mail,
      FALSE, $total_unsub);
    if ($message && $success) {
      drupal_set_message(t('You have unsubscribed from %list.', array('%list' => $book->Name)));
    }
  }
  return $success;
}

/**
 * @todo Make this work with all accounts.
 *
 * @param $mail
 * @param bool $message
 * @param null $q
 * @return bool
 */
function _dotmailer_total_unsubscribe_user($mail, $message = TRUE, $q = NULL) {
  $success = FALSE;
  if ($q || $q = _dotmailer_api_object()) {
    for ($i = 1; $i <= 5; $i++) {
      $success = dotmailer_removecontactfromaddressbook('dotmailer_username' . $i,'dotmailer_password' . $i, NULL, $mail, FALSE, TRUE);
    }
    if ($message && $success) {
      drupal_set_message(t('You have now been fully unsubscribed from all lists.'));
    }
  }
  return $success;
}

/**
 * Check if the given user is subscribed to the given list, we
 * return both the result as a boolean and the contactId from
 * dotmailer to use in some functions (i.e. update)
 *
 * @param object $book_id
 * @param object $email
 * @param object $q [optional]
 *
 * @return array $is_subscribed
 */
function _dotmailer_is_subscribed($book_id, $email, $q = NULL) {
  $address_book = dotmailer_get_saved_address_book($book_id);
  $is_subscribed = array('subscribed' => FALSE, 'contactId' => -1);
  if ($q || $q = _dotmailer_api_object()) {
    $subinfo = dotmailer_getcontactbyemail($address_book->Username,
      $address_book->Password,$email, TRUE);

    if (is_array($subinfo)) {
      if (is_array($subinfo['addressBooks']) && array_key_exists($book_id, $subinfo['addressBooks'])) {
        if (isset($subinfo['emailId']) && $subinfo['emailId'] != '') {
          $is_subscribed['subscribed'] = TRUE;
          $is_subscribed['contactId'] = $subinfo['emailId'];
        }
      }
    }
  }
  return $is_subscribed;
}

function _dotmailer_ensure_required_subscriptions($account) {
  $required_address_books = _dotmailer_get_required_books();
  if ($required_address_books) {
    $q = _dotmailer_api_object();
    foreach ($required_address_books as $address_book) {

      $is_allowed = FALSE;
      foreach (array_keys($address_book->roles) as $role) {
        if (array_key_exists($role, $account->roles)) {
          $is_allowed = TRUE;
          break;
        }
      }

      $is_subscribed = _dotmailer_is_subscribed($address_book->Id, $account->mail, $q);

      if ($is_allowed && !$is_subscribed['subscribed']) {
        $datafields = _dotmailer_load_contact_datalabels($account->uid,
          $address_book->Id, dotmailer_listcontactdatalabels($address_book->Username, $address_book->Password));
        $result = _dotmailer_subscribe_user($address_book, $account->mail, $datafields, $is_subscribed, FALSE, $q);
      }
      elseif ($is_subscribed['subscribed']) {
        $result = _dotmailer_unsubscribe_user($address_book, $account->mail, FALSE, $q);
      }

      if (isset($result)) {
        watchdog('dotmailer', 'dotmailer: %email updated in address book %book',
          array('%email' => $account->mail, '%book' => $address_book->Name), WATCHDOG_INFO
        );
      }
    }
  }
}

