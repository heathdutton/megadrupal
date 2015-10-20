<?php

/**
 * @file
 * Contains menu callbacks for non-administrative pages. For example, the page
 * that asks a user to name their browser, and the page that lists all of a
 * user's browsers.
 */

/**
 * Menu callback for user/%user/browsers.
 *
 * Creates a table displaying the information for the current user's browsers.
 */
function login_notify_user_browsers($account) {
  $browsers = login_notify_browsers_load($account->uid);
  $current_browser = login_notify_browser_load_current($account);

  $header = array(
    array('data' => t('Name'), 'nowrap' => 'nowrap'),
    array('data' => t('IP Address'), 'nowrap' => 'nowrap'),
    array('data' => t('User Agent'), 'nowrap' => 'nowrap'),
    array('data' => t('Created'), 'nowrap' => 'nowrap'),
    array('data' => t('Last Access'), 'nowrap' => 'nowrap'),
    array('data' => t('Edit'), 'nowrap' => 'nowrap'),
    array('data' => t('Lock'), 'nowrap' => 'nowrap'),
    array('data' => t('Delete'), 'nowrap' => 'nowrap'),
  );
  $rows = array();
  foreach ($browsers as $browser) {
    $row = array(
      'name' => check_plain($browser['name']),
      'ip_address' => check_plain($browser['ip_address']),
      'user_agent' => check_plain($browser['user_agent']),
      'created' => array(
        'data' => date('r', $browser['created']),
        'nowrap' => 'nowrap',
      ),
      'access' => array(
        'data' => date('r', $browser['access']),
        'nowrap' => 'nowrap',
      ),
      'edit' => '<a href="' . url('user/' . $account->uid . '/browsers/browser/' . $browser['browser_id'] . '/edit') . '">' . t('edit') . '</a>',
      'lock' => '<a href="' . url('user/' . $account->uid . '/browsers/browser/' . $browser['browser_id'] . '/lock') . '">' . t('lock') . '</a>',
      'delete' => '<a href="' . url('user/' . $account->uid . '/browsers/browser/' . $browser['browser_id'] . '/delete') . '">' . t('delete') . '</a>',
    );
    if ($current_browser && $current_browser['token'] == $browser['token']) {
      $row['name'] = '<strong>' . $row['name'] . '</strong> [' . t('current') . ']';
    }
    if (!empty($browser['locked'])) {
      $row['name'] .= ' [' . t('locked') . ']';
      $row['lock'] = '<a href="' . url('user/' . $account->uid . '/browsers/browser/' . $browser['browser_id'] . '/unlock') . '">' . t('unlock') . '</a>';
    }
    $rows[] = $row;
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
  ));
}

/**
 * Menu form callback for user/%user/browsers/name.
 *
 * Asks the user to name their browser.
 *
 * @param object $account
 *   The current user.
 *
 * @see login_notify_user_browsers_name_form_validate()
 * @see login_notify_user_browsers_name_form_submit()
 */
function login_notify_user_browsers_name_form($form, &$form_state, $account) {
  $form_state['account'] = $account;

  $form = array();

  // This will only be available if they choose a name that already exists.
  // This is if, for example, it's the same browser but they lost the cookie
  // somehow. Otherwise they'd have to give it a different name, go in and
  // delete the old browser, and update the name of this browser.
  $form['remove'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove the previous browser and replace it with this one'),
    '#access' => FALSE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Browser Name'),
    '#maxlength' => LOGIN_NOTIFY_MAX_LENGTH_NAME,
    '#required' => TRUE,
    '#description' => t('Give this device a unique nickname.'),
  );

  $form['remember'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remember me'),
    '#default_value' => TRUE,
    '#description' => t('Remember this browser. Uncheck to require this browser to be renamed once your browser window is closed.'),
  );

  $form['bypass_user_agent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not require this or similar browsers to be named anymore'),
    // Blank user agents are not allowed to bypass the browser naming process.
    '#access' => !empty($_SERVER['HTTP_USER_AGENT']),
    '#description' => t('This will allow browsers with the following user agent to access your account without having to give the browser a unique name, but you will still be emailed when this happens.<br /><code>@useragent</code>', array(
      '@useragent' => isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '',
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // See if there was an error with the name in the #validate. If there was
  // then this form was rebuilt.
  if (!empty($form_state['name_error'])) {
    $form['remove']['#access'] = TRUE;
    form_set_error('name', t('You have already used this name. Do you want to remove the previous browser?'));
    // We have to add class="error" manually, it won't be done automatically by
    // the above form_set_error().
    $form['name']['#attributes']['class'][] = 'error';
  }

  return $form;
}

/**
 * Form validation handler for login_notify_user_browsers_name_form().
 *
 * Ensures that the user hasn't already used the browser name.
 *
 * @see login_notify_user_browsers_name_form()
 */
function login_notify_user_browsers_name_form_validate($form, &$form_state) {
  $account = $form_state['account'];
  $name = trim($form_state['values']['name']);
  $remove = !empty($form_state['values']['remove']);

  if (!empty($name)) {
    $current_token = NULL;
    // See if there's a current (invalid) browser cookie token.
    if (!empty($_COOKIE[LOGIN_NOTIFY_COOKIE_NAME])) {
      $current_token = $_COOKIE[LOGIN_NOTIFY_COOKIE_NAME];
    }
    // See if a browser with the same name already exists, unless they've
    // checked off "remove the previous browser".
    if (!$remove
        && ($brower = login_notify_browser_load_by_name($account->uid, $name))
        // We're allowed to reuse the same name if we're reusing the same
        // cookie token because that cookie token will be deleted from the DB
        // before inserting the new one.
        && $brower['token'] != $current_token) {
      // Don't show the error message here because we need the form to rebuild
      // so that we can show the Remove checkbox, and it won't rebuild if there
      // are errors.
      $form_state['name_error'] = TRUE;
    }
    else {
      $form_state['name_error'] = FALSE;
    }
  }
}

/**
 * Form submit handler for login_notify_user_browsers_name_form().
 *
 * Saves the browser record in the database with the chosen name, sets the
 * browser cookie, and sends an email to the user notifying them that a new
 * browser has accessed the site with their account.
 *
 * @see login_notify_user_browsers_name_form()
 */
function login_notify_user_browsers_name_form_submit($form, &$form_state) {
  // Make sure there wasn't an error with the name. Since we didn't set a form
  // error in the #validate it will still get to the #submit.
  if (!empty($form_state['name_error'])) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  $account = $form_state['account'];
  $name = trim($form_state['values']['name']);
  $remember = !empty($form_state['values']['remember']);
  $bypass_user_agent = !empty($form_state['values']['bypass_user_agent']);
  $remove = !empty($form_state['values']['remove']);

  $token = NULL;
  // See if there already is a token cookie that we can reuse (this will happen
  // if someone logs into the site will multiple accounts using the same
  // browser).
  if (!empty($_COOKIE[LOGIN_NOTIFY_COOKIE_NAME])
      // Make sure it's the correct length.
      && strlen($_COOKIE[LOGIN_NOTIFY_COOKIE_NAME]) == LOGIN_NOTIFY_TOKEN_LENGTH) {
    // Reuse the same cookie token.
    // We don't have to worry about the UID+token unique key here, that's
    // handled by login_notify_browser_save().
    $token = $_COOKIE[LOGIN_NOTIFY_COOKIE_NAME];
  }
  else {
    // Create a new cookie token.
    $token = user_password(LOGIN_NOTIFY_TOKEN_LENGTH);
  }

  if ($remove) {
    // Remove the existing browser, if there is one.
    if ($existing_browser = login_notify_browser_load_by_name($account->uid, $name)) {
      $existing_browser['deleted'] = TRUE;
      login_notify_browser_save($existing_browser);
    }
  }

  $browser = array(
    'uid' => $account->uid,
    'name' => $name,
    'token' => $token,
    'ip_address' => ip_address(),
    'user_agent' => isset($_SERVER['HTTP_USER_AGENT']) ? $_SERVER['HTTP_USER_AGENT'] : '',
    'bypass_user_agent' => $bypass_user_agent,
    'remember' => $remember,
    'access' => REQUEST_TIME,
  );
  login_notify_browser_save($browser);

  login_notify_cookie_set($account, $browser, $remember);

  drupal_set_message(t('You have named your browser %name.', array('%name' => $name)));

  // Send the email.
  login_notify_browser_email_send($account, $browser);

  $form_state['rebuild'] = FALSE;
}

/**
 * Menu form callback user/%user/browsers/browser/%login_notify_browser/edit.
 *
 * The form that allows a user to edit a browser.
 *
 * @param object $account
 *   The user account.
 * @param array $browser
 *   The browser record in the database.
 *
 * @see login_notify_user_browsers_edit_form_validate()
 * @see login_notify_user_browsers_edit_form_submit()
 */
function login_notify_user_browsers_edit_form($form, &$form_state, $account, $browser) {
  $form_state['account'] = $account;
  $form_state['browser'] = $browser;

  $form = array();

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Browser Name'),
    '#maxlength' => LOGIN_NOTIFY_MAX_LENGTH_NAME,
    '#required' => TRUE,
    '#description' => t('Give this device a unique nickname.'),
    '#default_value' => $browser['name'],
  );

  $form['bypass_user_agent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not require this or similar browsers to be named anymore'),
    // Blank user agents are not allowed to bypass the browser naming process.
    '#access' => !empty($browser['user_agent']),
    '#default_value' => !empty($browser['bypass_user_agent']),
    '#description' => t('This will allow browsers with the following user agent to access your account without having to give the browser a unique name, but you will still be emailed when this happens.<br /><code>@useragent</code>', array(
      '@useragent' => $browser['user_agent'],
    )),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation for login_notify_user_browsers_edit_form().
 *
 * Ensures that they haven't already used the browser name.
 *
 * @see login_notify_user_browsers_edit_form()
 */
function login_notify_user_browsers_edit_form_validate($form, &$form_state) {
  $browser = $form_state['browser'];

  if (!empty($form_state['values']['name'])) {
    $name = trim($form_state['values']['name']);
    // See if there's another browser with this name.
    $name_browser = login_notify_browser_load_by_name($browser['uid'], $name);
    if ($name_browser && $name_browser['browser_id'] != $browser['browser_id']) {
      form_set_error('name', t('You already have another browser with this name.'));
    }
  }
}

/**
 * Form submit for login_notify_user_browsers_edit_form().
 *
 * Saves the browser with the new name in the database.
 *
 * @see login_notify_user_browsers_edit_form()
 */
function login_notify_user_browsers_edit_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  $browser = $form_state['browser'];

  $name = trim($form_state['values']['name']);
  $bypass_user_agent = !empty($form_state['values']['bypass_user_agent']);
  $browser['name'] = $name;
  $browser['bypass_user_agent'] = $bypass_user_agent;
  // Save the browser.
  login_notify_browser_save($browser);

  drupal_set_message(t("You've saved the browser %name successfully.", array('%name' => $name)));

  $form_state['redirect'] = 'user/' . $account->uid . '/browsers';
}

/**
 * Menu form callback user/%user/browsers/browser/%login_notify_browser/delete.
 *
 * The form that allows a user to delete a browser.
 *
 * @param object $account
 *   The user account.
 * @param array $browser
 *   The browser record in the database.
 *
 * @see login_notify_user_browsers_delete_form_submit()
 */
function login_notify_user_browsers_delete_form($form, &$form_state, $account, $browser) {
  $form_state['account'] = $account;
  $form_state['browser'] = $browser;

  $form = array();

  $form['message']['#markup'] = '<p>' . t('Are you sure you want to delete the browser %name?', array('%name' => $browser['name'])) . '</p>'
    . '<p>' . t('Deleting the browser <strong>will not log them out</strong>, it will only require them to enter a new browser name.') . '</p>';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete browser'),
  );

  return $form;
}

/**
 * Form submit for login_notify_user_browsers_delete_form().
 *
 * 'Deletes' a browser from the database, which means simply setting the
 * 'deleted' field to 1, and not actually deleting a record from the table.
 *
 * @see login_notify_user_browsers_delete_form()
 */
function login_notify_user_browsers_delete_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  $browser = $form_state['browser'];

  // Delete the browser.
  $browser['deleted'] = TRUE;
  login_notify_browser_save($browser);

  drupal_set_message(t("You've deleted the browser %name successfully.", array('%name' => $browser['name'])));

  $form_state['redirect'] = 'user/' . $account->uid . '/browsers';
}

/**
 * Menu form callback user/%user/browsers/browser/%login_notify_browser/lock.
 *
 * The form that allows a user to lock a browser out of their account.
 *
 * @param object $account
 *   The user account.
 * @param array $browser
 *   The browser record from the database.
 *
 * @see login_notify_user_browsers_lock_form_submit()
 */
function login_notify_user_browsers_lock_form($form, &$form_state, $account, $browser) {
  $form_state['account'] = $account;
  $form_state['browser'] = $browser;

  $form = array();

  $form['message']['#markup'] = '<p>' . t('Are you sure you want to lock the browser %name?', array('%name' => $browser['name'])) . '</p>'
    . '<p>' . t('Locking the browser will <strong>log them out</strong> and force them to log back in, requiring the username and password.') . '</p>';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lock browser'),
  );

  return $form;
}

/**
 * Form submit handler for login_notify_user_browsers_lock_form().
 *
 * Marks the browser as locked in the database. If the browser tries to access
 * the site from this account again they will be forcefully logged out.
 *
 * Important note: this isn't completely secure. If an attacker knows that
 * their browser has been or could be locked, they could delete their browser
 * cookie. This would force them to name their browser again before they could
 * do anything, which would send the user another notification that a new
 * browser has accessed the site.
 *
 * For complete security, an option to prevent unrecognized browsers from
 * accessing the site using your account would be required.
 *
 * @see login_notify_user_browsers_lock_form()
 */
function login_notify_user_browsers_lock_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  $browser = $form_state['browser'];

  // Lock the browser.
  $browser['locked'] = TRUE;
  login_notify_browser_save($browser);

  drupal_set_message(t("You've locked the browser %name successfully.", array('%name' => $browser['name'])));

  $form_state['redirect'] = 'user/' . $account->uid . '/browsers';
}

/**
 * Menu form callback user/%user/browsers/browser/%login_notify_browser/unlock.
 *
 * The form that allows a user to unlock a browser.
 *
 * @param object $account
 *   The user account.
 * @param array $browser
 *   The browser record from the database.
 *
 * @see login_notify_user_browsers_unlock_form_submit()
 */
function login_notify_user_browsers_unlock_form($form, &$form_state, $account, $browser) {
  $form_state['account'] = $account;
  $form_state['browser'] = $browser;

  $form = array();

  $form['message']['#markup'] = '<p>' . t('Are you sure you want to unlock the browser %name?', array('%name' => $browser['name'])) . '</p>'
    . '<p>' . t('Unlocking the browser will allow it to access your account once again.') . '</p>';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unlock browser'),
  );

  return $form;
}

/**
 * Form submit handler for login_notify_user_browsers_unlock_form().
 *
 * @see login_notify_user_browsers_unlock_form()
 */
function login_notify_user_browsers_unlock_form_submit($form, &$form_state) {
  $account = $form_state['account'];
  $browser = $form_state['browser'];

  // Unlock the browser.
  $browser['locked'] = FALSE;
  login_notify_browser_save($browser);

  drupal_set_message(t("You've unlocked the browser %name successfully.", array('%name' => $browser['name'])));

  $form_state['redirect'] = 'user/' . $account->uid . '/browsers';
}

/**
 * Handler for secure user/%user/browsers/browser/%login_notify_browser/unlock.
 *
 * When the above URL is called with the query parameters 'timestamp' and
 * 'key' this function is called directly from our hook_init().
 *
 * This unlocks a browser if the timestamp and key are correct. It works even
 * if the user is logged out, which is why it's called in hook_init() rather
 * than going through the menu normally.
 *
 * @see login_notify_init()
 */
function login_notify_user_browsers_unlock_secure($account, $browser, $timestamp, $key) {
  global $user;

  $current = REQUEST_TIME;
  // Check the timestamp, it must be less than X seconds ago.
  if ($timestamp < $current && $timestamp >= $current - LOGIN_NOTIFY_UNLOCK_TIMEOUT
      // Make sure the key is valid.
      && $key == login_notify_browser_unlock_key($account, $browser, $timestamp)) {
    // Valid unlock URL.
    // Unlock the browser.
    $browser['locked'] = FALSE;
    login_notify_browser_save($browser);

    drupal_set_message(t("You've unlocked the browser %name successfully.", array('%name' => $browser['name'])));

    // If they're not logged in, redirect to the login page.
    if (empty($user->uid)) {
      drupal_goto('user');
    }
    // Otherwise, redirect to the browsers page.
    else {
      drupal_goto('user/' . $user->uid . '/browsers');
    }
  }
  else {
    drupal_set_message(t('Browser unlock URL is invalid.'), 'error');
  }
}
