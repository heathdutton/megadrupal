<?php

/**
 * @file
 * Dotmailer pages file - includes mostly callbacks from hook_menu()
 */

/**
 * Confirm form for resetting the Dotmailer details stored for the API
 */
function dotmailer_reset($form, &$form_state) {
  return confirm_form($form, t('Are you sure that you wish to remove the current dotMailer API details?'), '<front>');
}

/**
 * Submit function for dotmailer_reset()
 */
function dotmailer_reset_submit() {
  for ($i = 1; $i <= 5; $i++) {
    variable_del('dotmailer_username'.$i);
    variable_del('dotmailer_password'.$i);
    variable_del('dotmailer_active_lists'.$i);
  }
  variable_del('dotmailer_active_lists');
  drupal_goto('admin/config/dotmailer');
}

/**
 * User edit form for managing subscriptions
 */
function dotmailer_subscribe_auth_form($form, &$form_state, $account) {
  $form = _dotmailer_subscribe_auth_form_elements($account) + $form;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Generates the form elements required to edit your subscriptions
 *
 * @param type $account
 *
 * @return array
 */
function _dotmailer_subscribe_auth_form_elements($account) {
  $form = array();

  $q = _dotmailer_api_object();
  if (!$q) {
    return $form;
  }

  if ($account->uid) {
    $form['uid'] = array(
      '#type' => 'value',
      '#value' => $account->uid,
    );
  }
  else {
    // user registration or admin creating new user
    $account->roles = array(2 => 'authenticated');
  }
  foreach (_dotmailer_get_available_books($account) as $book) {
    $is_subscribed = FALSE;
    // ignore required books, handled by hook_user
    if ($book->booktype !== DOTMAILER_BOOKTYPE_REQUIRED) {
      if ($account->uid) {
        $is_subscribed = _dotmailer_is_subscribed($book->Id, $account->mail, $q);
        $default_value = $is_subscribed['subscribed'];
      }
      else {
        $default_value = $book->booktype == DOTMAILER_BOOKTYPE_OPTOUT ? TRUE : FALSE;
      }
      // wrap in a div
      $form['wrapper' . $book->Id] = array(
        '#prefix' => '<div class="dotmailer-newsletter-wrapper">',
        '#suffix' => '</div>',
      );
      $form['wrapper' . $book->Id]['dotmailer_book_' . $book->Id] = array(
        '#type' => 'checkbox',
        '#title' => t('Subscribe to the @newsletter newsletter', array('@newsletter' => (trim($book->title) !== '') ? $book->title : $book->Name)),
        '#default_value' => $default_value,
        '#description' => $book->description,
        '#attributes' => array('class' => array('dotmailer-newsletter-checkbox-' . $book->Id)),
      );
    }
  }
  return $form;
}

/**
 * Submit handler to add users to books when editing/creating a user
 */
function dotmailer_subscribe_auth_form_submit($form, &$form_state) {
  $q = _dotmailer_api_object();
  if (!$q) {
    return;
  }

  if ($form_state['values']['uid']) {
    $account = user_load($form_state['values']['uid']);
  }
  else {
    $account = $form_state['user'];
  }

  $books = _dotmailer_get_available_books($account);
  foreach ($books as $book) {
    // ignore required books, they are handled via hook_user
    if ($book->booktype == DOTMAILER_BOOKTYPE_REQUIRED) {
      continue;
    }

    $is_subscribed = _dotmailer_is_subscribed($book->Id, $account->mail, $q);
    $selected = $form_state['values']['dotmailer_book_' . $book->Id];

    // unsubscribe a subscribed user who unchecked the box for this newsletter
    if ($is_subscribed['subscribed'] && !$selected) {
      _dotmailer_unsubscribe_user($book, $account->mail, TRUE, $q, FALSE);
    }
    elseif ($selected) {
      // subscribe the user if they are not previously subscribed or update existing subscriptions
      $datafields = _dotmailer_load_contact_datalabels($account->uid, $book->Id);
      _dotmailer_subscribe_user($book, $account->mail, $datafields, $is_subscribed, TRUE, $q);
    }
  }
}

/**
 * Return all available books a given user can subscribe to
 * @todo Rewrite this for anon. Currently, this only submits the
 * first form, whatever button you click.
 * @global <type> $user
 *
 * @return <type> page content of all available books for a given user
 */
function dotmailer_subscribe_page() {
  global $user;
  $output = '';
  if ($user->uid) {
    return drupal_get_form('dotmailer_subscribe_auth_form', $user);
  }

  $output = array();
  $q = _dotmailer_api_object();
  if ($q) {
    $books = _dotmailer_get_available_books();
    foreach ($books as $book) {
      $output[] = array(
        '#type' => 'fieldset',
        '#title' => $book->Name,
        'form' => drupal_get_form('dotmailer_subscribe_anon_form', $book, $q),
      );
    }
  }
  return $output;
}

/**
 * Return a form for a given dotmailer address book
 *
 * @param <type> $form_state
 * @param <type> $book book object
 * @param <type> $q domtailer object
 *
 * @return <type> form array
 */
function dotmailer_subscribe_anon_form(&$form, &$form_state, $book, $q) {
  $form['description'] = array(
    '#value' => $book->description,
  );
  $form['book'] = array(
    '#type' => 'value',
    '#value' => $book,
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#size' => 35,
  );
  // Loop through the datafields and add them to the form
  foreach ((array)dotmailer_listcontactdatalabels($book->Username, $book->Password)->ContactDataLabel as $datafield) {
    if ($datafield->IsPrivate != TRUE) {
      $form[$datafield->Name] = array(
        '#title' => check_plain(drupal_ucfirst(drupal_strtolower($datafield->Name))),
        '#type' => 'textfield',
        '#size' => 20,
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sign Up!'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Submit handler for stand alone form
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function dotmailer_subscribe_anon_form_submit($form, &$form_state) {
  $success = FALSE;
  $values = $form_state['values'];
  $uservars = array();
  // Loop through the available datafields
  foreach ((array)dotmailer_listcontactdatalabels($form_state['values']['book']->Username, $form_state['values']['book']->Password)->ContactDataLabel as $datafield) {
    if ($datafield->IsPrivate != TRUE && isset($values[$datafield->Name])) {
      $uservars[$datafield->Name]['data'] = $values[$datafield->Name];
      // At moment just strings are used, can use dates for dob?
      $uservars[$datafield->Name]['type'] = 'string';
    }
  }
  $success = _dotmailer_subscribe_user($values['book'], $values['email'], $uservars, array('subscribed' => FALSE));

  if ($success) {
    $msg = variable_get('dotmailer_subscription_success_message', t('Thank you, you have been successfully subscribed.'));
  }
  else {
    $msg = variable_get('dotmailer_subscription_failure_message', t('We were unable to subscribe you at this time. Please try again later.'));
  }
  drupal_set_message(check_plain($msg));
}

/**
 * Dotmailer unsubscribe form
 *
 * @param <type> $form_values
 *
 * @return <type>
 */
function dotmailer_unsubscribe_form($form_values = array()) {
  $form = array();
  $form['intro'] = array(
    '#type' => 'markup',
    '#value' => t('Use this form to unsubscribe from all of our newsletters.'),
  );
  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unsubscribe'),
  );
  return $form;
}

/**
 * Validate unsubscribe form
 *
 * @todo $book is not defined - where does it come from?
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function dotmailer_unsubscribe_form_validate($form, &$form_state) {
  if (!$form_state['values']['email']) {
    form_set_error('email', t('Please enter your email address.'));
  }
}

/**
 * Submit handler for unsubscribe form
 *
 * @todo $book is not defined - where does it come from?
 *
 * @param <type> $form
 * @param <type> $form_state
 */
function dotmailer_unsubscribe_form_submit($form, &$form_state) {
  $q = _dotmailer_api_object();
  if ($q) {
    $success = _dotmailer_total_unsubscribe_user($form_state['values']['email'], TRUE, $q);
    if (!$success) {
      drupal_set_message(variable_get('dotmailer_unsubscription_failure_message', t('We were unable to unsubscribe you at this time. Please try again later.')));
    }
  }
}

