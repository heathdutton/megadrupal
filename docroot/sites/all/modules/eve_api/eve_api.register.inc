<?php
/**
 * @file
 * Functions related to altering the registration method on the site.
 */

/**
 * Create callback for standard ctools registration wizard.
 *
 * @param string $step
 *   Indicates what step of the registration process is being called
 */
function eve_api_register_wizard($step = 'enter_api') {
  // Include required ctools files.
  ctools_include('wizard');
  ctools_include('object-cache');

  $form_info = array(
    // Specify unique form id for this form.
    'id' => 'multistep_registration',
    // Specify the path for this form. It is important to include space for the
    // $step argument to be passed.
    'path' => "user/register/%step",
    // Show bread crumb trail.
    'show trail' => TRUE,
    'show back' => FALSE,
    'show return' => FALSE,
    // Callback to use when the 'next' button is clicked.
    'next callback' => 'eve_api_subtask_next',
    // Callback to use when entire form is completed.
    'finish callback' => 'eve_api_subtask_finish',
    // Callback to use when user clicks final submit button.
    'return callback' => 'eve_api_subtask_finish',
    // Callback to use when user cancels wizard.
    'cancel callback' => 'eve_api_subtask_cancel',
    // Specify the order that the child forms will appear in, as well as their
    // page titles.
    'order' => array(
      'enter_api' => t('Enter EVE API'),
      'register' => t('Register'),
    ),
    // Define the child forms. Be sure to use the same keys here that were user
    // in the 'order' section of this array.
    'forms' => array(
      'enter_api' => array('form id' => 'eve_api_enter_api_form'),
      'register' => array('form id' => 'user_register_form'),
    ),
  );

  // Make cached data available within each step's $form_state array.
  $form_state['signup_object'] = eve_api_get_page_cache('signup');

  // Return the form as a Ctools multi-step form.
  $output = ctools_wizard_multistep_form($form_info, $step, $form_state);

  return $output;
}

/**
 * Retrieves an object from the cache.
 *
 * @param string $name
 *   The name of the cached object to retrieve.
 */
function eve_api_get_page_cache($name) {
  ctools_include('object-cache');
  $cache = ctools_object_cache_get('eve_api', $name);

  // If the cached object doesn't exist yet, create an empty object.
  if (!$cache) {
    $cache = new stdClass();
    $cache->locked = ctools_object_cache_test('eve_api', $name);
  }

  return $cache;
}

/**
 * Creates or updates an object in the cache.
 *
 * @param string $name
 *   The name of the object to cache.
 *
 * @param object $data
 *   The object to be cached.
 */
function eve_api_set_page_cache($name, $data) {
  ctools_include('object-cache');
  ctools_object_cache_set('eve_api', $name, $data);
}

/**
 * Removes an item from the object cache.
 *
 * @param string $name
 *   The name of the object to destroy.
 */
function eve_api_clear_page_cache($name) {
  ctools_include('object-cache');
  ctools_object_cache_clear('eve_api', $name);
}

/**
 * Callback executed when the 'next' button is clicked.
 */
function eve_api_subtask_next(&$form_state) {
  // Store submitted data in a ctools cache object, namespaced 'signup'.
  eve_api_set_page_cache('signup', $form_state['values']);
}

/**
 * Callback executed when the 'cancel' button is clicked.
 */
function eve_api_subtask_cancel(&$form_state) {
  // Clear our ctools cache object. It's good housekeeping.
  eve_api_clear_page_cache('signup');
}

/**
 * Callback executed when the entire form submission is finished.
 */
function eve_api_subtask_finish(&$form_state) {
  // Clear our Ctool cache object.
  eve_api_clear_page_cache('signup');

  // Redirect the user to the front page.
  drupal_goto('<front>');
}

/**
 * Implements hook_form_FORM_ID_alter() for user_register_form().
 */
function eve_api_form_user_register_form_alter(&$form, &$form_state) {
  global $user;

  unset($form['actions']);

  $account = $form['#user'];
  $register = ((int) $form['#user']->uid > 0 ? FALSE : TRUE);

  $admin = user_access('administer users');

  $form['account']['#type'] = 'fieldset';
  $form['account']['#title'] = t('Blue Status Verified');
  $form['account']['#description'] = t('Please select your main character from the drop down box. This character will be used to identify yourself throughout the site and various tools.');

  $form['#validate'] = array(
    'eve_api_user_register_form_validate',
    'user_account_form_validate',
    'user_validate_picture',
    'user_register_validate',
  );

  $form['#submit'] = array(
    'user_register_submit',
    'eve_api_user_register_form_submit',
    'ctools_wizard_submit',
  );

  if (!is_array($form_state['signup_object'])) {
    form_set_error('keyID', t('Unknown Error, please try again.'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  $key_id = (int) $form_state['signup_object']['keyID'];
  $v_code = (string) $form_state['signup_object']['vCode'];

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('Unknown Error, please try again.'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  $whitelist = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $whitelist[] = (int) $character['characterID'];
    }
  }

  $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
    ':characterIDs' => $whitelist,
  ));

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
  $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

  if ($result->rowCount()) {
    if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
      form_set_error('keyID', t('Unknown Error, please try again.'));
      form_set_error('vCode');
      drupal_goto('user/register');
    }
  }
  else {
    if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
      form_set_error('keyID', t('Unknown Error, please try again.'));
      form_set_error('vCode');
      drupal_goto('user/register');
    }
  }

  if (!eve_api_verify_blue($characters)) {
    form_set_error('keyID', t('Unknown Error, please try again.'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  if (eve_api_characters_exist($characters)) {
    form_set_error('keyID', t('Unknown Error, please try again.'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  if (!variable_get('eve_api_require_blue', FALSE)) {
    $valid_characters_list = eve_api_list_valid_characters($characters);
  }
  else {
    $valid_characters_list = eve_api_list_valid_characters($characters, FALSE);
  }

  $form['account']['name'] = array(
    '#type' => 'select',
    '#title' => t('Select your Main Character'),
    '#options' => drupal_map_assoc($valid_characters_list),
    '#description' => t('Detected valid Main Characters.'),
    '#required' => TRUE,
    '#empty_option' => t('- Select a New Character -'),
    '#attributes' => array('class' => array('username')),
    '#access' => ($register || ($user->uid == $account->uid && user_access('change own username')) || $admin),
    '#weight' => -10,
  );
}

/**
 * Form validation handler for user_register_form().
 *
 * @see eve_api_user_register_form_submit()
 */
function eve_api_user_register_form_validate(&$form, &$form_state) {
  $key_id = (int) $form_state['signup_object']['keyID'];
  $v_code = (string) $form_state['signup_object']['vCode'];

  $result = db_query('SELECT apiID FROM {eve_api_keys} WHERE keyID = :keyID AND vCode =:vCode', array(
    ':keyID' => $key_id,
    ':vCode' => $v_code,
  ));

  if ($result->rowCount()) {
    form_set_error('keyID', t('API Key already exists!'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('Unknown Error, please try again.'));
    form_set_error('vCode');
    drupal_goto('user/register');
  }

  $whitelist = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $whitelist[] = (int) $character['characterID'];
    }
  }

  $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
    ':characterIDs' => $whitelist,
  ));

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
  $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

  if ($result->rowCount()) {
    if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
      form_set_error('keyID', t('Your account has been whitelisted, please ensure that the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked. Only (Public Information -> (Characterinfo and FacWarStats), (Private Information) -> (CharacterSheet)) are required.'));
      form_set_error('vCode');
    }
  }
  else {
    if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
      form_set_error('keyID', t('Please ensure that all boxes are highlighted and selected for the API, the "Character" drop down box is set to "All", the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked.'));
      form_set_error('vCode');
    }
  }

  if (!eve_api_verify_blue($characters) && !variable_get('eve_api_require_blue', FALSE)) {
    form_set_error('keyID', t('No characters associated with your key are currently blue.'));
    form_set_error('vCode');
  }

  if ($chars = eve_api_characters_exist($characters)) {
    form_set_error('keyID', t('Characters on this key have already been registered. Characters registered: @chars', array('@chars' => implode(", ", $chars))));
    form_set_error('vCode');
  }

  $selected = array();

  foreach ($characters['characters'] as $character) {
    $selected[] = $character['characterName'];
  }

  if (!in_array((string) $form_state['values']['name'], $selected)) {
    form_set_error('name', t('You must select a main character!'));
  }
}

/**
 * Form submission handler for user_login_form().
 *
 * @see eve_api_user_register_form_validate()
 */
function eve_api_user_register_form_submit(&$form, &$form_state) {
  $account = $form_state['user'];
  $uid = (int) $account->uid;
  $key_id = (int) $form_state['signup_object']['keyID'];
  $v_code = (string) $form_state['signup_object']['vCode'];

  $character = eve_api_create_key($account, $key_id, $v_code);

  if (isset($character['not_found']) || $character == FALSE) {
    db_update('users')->fields(array(
        'characterID' => 0,
        'name' => $account->name,
        'signature' => '',
      ))->condition('uid', $uid, '=')->execute();

    $default_role = user_role_load(variable_get('eve_api_unverified_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', $default_role->rid);

    drupal_set_message(t('Registration partially failed, please verify API Key when activated.'), 'error');
    return;
  }

  $queue = DrupalQueue::get('eve_api_cron_api_user_sync');
  $queue->createItem(array(
    'uid' => $uid,
    'runs' => 1,
  ));

  $character_name = (string) $character['characterName'];
  $character_id = (int) $character['characterID'];
  $corporation_name = (string) $character['corporationName'];
  $character_is_blue = FALSE;

  if ($corporation_role = user_role_load_by_name($corporation_name)) {
    user_multiple_role_edit(array($uid), 'add_role', (int) $corporation_role->rid);

    $alliance_role = user_role_load(variable_get('eve_api_alliance_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', (int) $alliance_role->rid);
  }
  elseif (eve_api_verify_blue($character)) {
    $character_is_blue = TRUE;

    $blue_role = user_role_load(variable_get('eve_api_blue_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', (int) $blue_role->rid);
  }
  else {
    $default_role = user_role_load(variable_get('eve_api_unverified_role', 2));
    user_multiple_role_edit(array($uid), 'add_role', $default_role->rid);
  }

  module_invoke_all('eve_api_user_update', array(
    'account' => $account,
    'character' => $character,
    'character_is_blue' => $character_is_blue,
  ));

  db_update('users')->fields(array(
      'characterID' => $character_id,
      'name' => $character_name,
      'signature' => '',
    ))->condition('uid', $uid, '=')->execute();
}

/**
 * Form constructor for the landing page for the user registration process.
 *
 * @see eve_api_enter_api_form_validate()
 * @see eve_api_enter_api_form_submit()
 *
 * @ingroup forms
 */
function eve_api_enter_api_form($form, &$form_state) {
  $form['verify']['info'] = array(
    '#type' => 'item',
    '#title' => t('Verify Blue Status'),
    '#description' => t('Please enter your EVE API Key in order to verify you qualify for an account. Please ensure your key matches the options listed below.'),
    '#weight' => 0,
  );

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? t('You may set an expiry, but the preferred method is to tick the box "No Expiry".') : t('Please tick the box "No Expiry".');
  $allow_type = variable_get('eve_api_require_type', TRUE) ? t('Please set this to "All".') : t('You may select your main character or "All".');

  $form['verify']['help_character'] = array(
    '#type' => 'item',
    '#title' => t('Character:'),
    '#description' => $allow_type,
    '#weight' => 10,
  );

  $form['verify']['help_type'] = array(
    '#type' => 'item',
    '#title' => t('Type:'),
    '#description' => t('Please set this to "Character".'),
    '#weight' => 20,
  );

  $form['verify']['help_expire'] = array(
    '#type' => 'item',
    '#title' => t('Expires:'),
    '#description' => $allow_expires,
    '#weight' => 30,
  );

  $form['verify']['help_mask'] = array(
    '#type' => 'item',
    '#title' => t('Access Mask:'),
    '#description' => t('Click <a href="@mask" target="_blank">here</a> to create a new key with the correct access mask pre-selected.', array('@mask' => 'http://community.eveonline.com/support/api-key/CreatePredefined?accessMask=' . variable_get('eve_api_access_mask', 268435455))),
    '#weight' => 40,
  );

  $form['verify']['keyID'] = array(
    '#type' => 'textfield',
    '#title' => t('Key ID'),
    '#description' => t('Please enter your Key ID.'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#weight' => 50,
  );

  $form['verify']['vCode'] = array(
    '#type' => 'textfield',
    '#title' => t('Verification Code'),
    '#description' => t('Please enter your Verification Code.'),
    '#required' => TRUE,
    '#size' => 80,
    '#maxlength' => 64,
    '#weight' => 60,
  );

  if (!variable_get('eve_api_enable', FALSE)) {
    drupal_set_message(t('Registrations temporarily disabled.'), 'error', FALSE);
    $form['buttons']['next']['#disabled'] = TRUE;
  }

  $form['#validate'][] = 'eve_api_enter_api_form_validate';

  return $form;
}

/**
 * Form validation handler for eve_api_enter_api_form().
 *
 * @see eve_api_enter_api_form_submit()
 */
function eve_api_enter_api_form_validate($form, &$form_state) {
  $key_id = (int) $form_state['values']['keyID'];
  $v_code = (string) $form_state['values']['vCode'];

  if (empty($key_id) || empty($v_code) || preg_match('/[^a-z0-9]/i', $v_code) || preg_match('/[^0-9]/', $key_id) || strlen($key_id) > 15 || strlen($v_code) > 64 || strlen($v_code) < 20) {
    form_set_error('keyID', t('Invalid input, please try again.'));
    form_set_error('vCode');
    return;
  }

  $result = db_query('SELECT apiID FROM {eve_api_keys} WHERE keyID = :keyID AND vCode =:vCode', array(
    ':keyID' => $key_id,
    ':vCode' => $v_code,
  ));

  if ($result->rowCount()) {
    form_set_error('keyID', t('API Key already exists!'));
    form_set_error('vCode');
    return;
  }

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    form_set_error('keyID', t('There was an error with the API.'));
    form_set_error('vCode');
  }
  else {
    $whitelist = array();

    if (!empty($characters)) {
      foreach ($characters['characters'] as $character) {
        $whitelist[] = (int) $character['characterID'];
      }
    }

    $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
      ':characterIDs' => $whitelist,
    ));

    $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
    $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

    if ($result->rowCount()) {
      if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
        form_set_error('keyID', t('Your account has been whitelisted, please ensure that the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked. Only (Public Information -> (Characterinfo and FacWarStats), (Private Information) -> (CharacterSheet)) are required.'));
        form_set_error('vCode');
      }
    }
    else {
      if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
        form_set_error('keyID', t('Please ensure that all boxes are highlighted and selected for the API, the "Character" drop down box is set to "All", the "Type" drop down box is set to "Character", and that the "No Expiry" checkbox is ticked.'));
        form_set_error('vCode');
      }
    }

    if (!eve_api_verify_blue($characters) && !variable_get('eve_api_require_blue', FALSE)) {
      form_set_error('keyID', t('No characters associated with your key are currently blue to this alliance.'));
      form_set_error('vCode');
    }

    if ($chars = eve_api_characters_exist($characters)) {
      form_set_error('keyID', t('Characters on this key have already been registered. Characters registered: @chars', array('@chars' => implode(", ", $chars))));
      form_set_error('vCode');
    }
  }
}
