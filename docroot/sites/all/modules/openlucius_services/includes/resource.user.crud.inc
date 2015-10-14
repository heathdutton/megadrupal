<?php
/**
 * @file
 * This file contains the user services.
 */

/**
 * Function to create a user from an array of data.
 *
 * @param array $data
 *   The submitted data to be used for user creation.
 *
 * @return array
 *   Returns a message containing the id, the message and the path to the user.
 * @throws \Exception
 *   Throws an exception when invalid.
 */
function _openlucius_services_create_user(array $data) {

  // Check if the user may create users.
  if (!user_access("administer users")) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_CREATE_ACCESS_DENIED);
  }

  // Fetch creation requirements for this type.
  $requirements = _openlucius_services_entity_requirements('user');

  // Check if the requirements are set.
  foreach ($requirements as $requirement) {
    if (empty($data[$requirement])) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => $requirement));
    }
  }

  // Validate the input data before creating the account.
  _openlucius_services_user_input_validation($data, TRUE);

  // Set a random password if this was not set.
  if (!isset($data['password'])) {
    $data['password'] = user_password(8);
  }

  // Load the role.
  $role = user_role_load($data['role']);

  // Set up the user fields.
  $fields = array(
    'name'   => check_plain($data['name']),
    'mail'   => $data['mail'],
    'pass'   => $data['password'],
    'status' => 1,
    'init'   => 'email address',
    'roles'  => array(
      DRUPAL_AUTHENTICATED_RID => 'authenticated user',
      $role->rid               => $role->name,
    ),
  );

  // The first parameter is left blank so a new user is created.
  $account = user_save('', $fields);

  // Use the metadata wrapper for easy access.
  $wrapper = entity_metadata_wrapper('user', $account);

  foreach ($data as $field => $value) {
    if (in_array($field, array('groups', 'phone', 'body', 'labels', 'teams'))) {

      // Fetch the drupal field.
      $drupal_field = openlucius_services_field_mapping('user', $field);

      // Set the field on the entity.
      _openlucius_services_user_set_field($wrapper, $drupal_field, $data);
    }
  }

  // Save the values.
  $wrapper->save();

  // Send the e-mail through the user module.
  drupal_mail('user', 'register_no_approval_required', $data['mail'], NULL, array('account' => $account), variable_get('site_mail', 'info@luciuswebsystem.nl'));

  return openlucius_services_message($account->uid, t("User successfully created"), url('user/dashboard/' . $account->uid, array('absolute' => TRUE)));
}

/**
 * Function to update a user using an array of data.
 *
 * @param int $uid
 *   The user id for the user to be altered.
 * @param array $data
 *   The submitted data to be used for user creation.
 *
 * @return array
 *   Returns a message containing the id, the message and the path to the user.
 * @throws \Exception
 *   Throws an exception when invalid.
 */
function _openlucius_services_update_user($uid, array $data) {
  global $user;

  // Verify that the submitted $uid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($uid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($uid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Check if the user may create users.
  if (!user_access("administer users") && $user->uid !== $uid) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EDIT_ACCESS_DENIED);
  }

  // Validate the input.
  _openlucius_services_user_input_validation($data);

  // Load the user.
  $account = user_load($uid);

  // Use the metadata wrapper for easy access.
  $wrapper = entity_metadata_wrapper('user', $account);

  foreach ($data as $field => $value) {

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping('user', $field);

    // Set the field on the entity.
    _openlucius_services_user_set_field($wrapper, $drupal_field, $data);
  }

  // Save the values.
  $wrapper->save();

  // Update the password without the wrapper
  // @see https://www.drupal.org/node/2224645.
  if (isset($data['password'])) {
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    $hashed_password = user_hash_password($data['password']);

    // Reload the account after field updates.
    $account = user_load($account->uid);

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping('user', 'password');

    // Set the hashed password.
    $account->{$drupal_field} = $hashed_password;

    // Save the user object.
    user_save($account);
  }

  return openlucius_services_message($uid, t("User successfully updated"), url('user/dashboard/' . $uid, array('absolute' => TRUE)));
}

/**
 * Function to activate a user or multiple users.
 *
 * @param mixed $data
 *   Either an integer if a single user has to be activated or an array if more
 *   than one user needs to be activated.
 *
 * @return array
 *   Returns a message containing the amount of successful activations.
 */
function _openlucius_services_activate_user($data) {

  // Check if the user may administer users.
  if (!user_access("administer users")) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EDIT_ACCESS_DENIED);
  }

  // Initiate counters for checking if accounts have been activated.
  $success = $impossible = $failures = 0;

  // Alter for single update.
  if (gettype($data) != 'array') {
    $data = array('users' => array(intval($data)));
  }

  // Process users.
  return _openlucius_services_batch_activate_deactivate_users($data, 1, $success, $impossible, $failures);
}

/**
 * Function to deactivate a user or multiple users.
 *
 * @param mixed $data
 *   Either an integer if a single user has to be deactivated
 *   or an array if more than one user needs to be deactivated.
 *
 * @return array
 *   Returns a message containing the amount of successful de-activations.
 */
function _openlucius_services_block_user($data) {

  // Check if the user may administer users.
  if (!user_access("administer users")) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EDIT_ACCESS_DENIED);
  }

  // Initiate counters for checking if accounts have been deactivated and such.
  $success = $impossible = $failures = 0;

  // Alter for single update.
  if (gettype($data) != 'array') {
    $data = array('users' => array(intval($data)));
  }

  // Process users.
  return _openlucius_services_batch_activate_deactivate_users($data, 0, $success, $impossible, $failures);
}

/**
 * Function for activating / blocking multiple users.
 *
 * @param array $data
 *   The data to be processed.
 * @param int $status
 *   The status the user should receive.
 * @param int $success
 *   The amount of successful activations / de-activations.
 * @param int $impossible
 *   The amount of impossible updates
 *   (for example: status 0 will not be blocked)
 * @param int $failures
 *   The amount of failed updates.
 *
 * @return array
 *   Returns a message containing the amount of failures.
 * @throws \Exception
 *   Throws an exception if the parameters are wrong.
 */
function _openlucius_services_batch_activate_deactivate_users(array $data, $status, &$success, &$impossible, &$failures) {
  // Try to activate the array of users.
  // Taken from @see user_user_operations_unblock().
  $accounts = user_load_multiple($data['users']);
  foreach ($accounts as $account) {

    // Skip unblocking user if they are already unblocked, super or admin.
    if ($account !== FALSE && $account->status != $status && $account->uid != 1 && !user_has_role(OPENLUCIUS_CORE_OPENLUCIUS_ADMIN_ROLE, $account)) {
      user_save($account, array('status' => $status));
      $success++;
    }
    elseif ($account->status == $status) {
      $impossible++;
    }
    else {
      $failures++;
    }
  }

  $parameters = array(
    '@success'  => $success,
    '@active'   => $impossible,
    '@failures' => $failures,
  );

  // Send message.
  if ($status == 1) {
    return openlucius_services_message(NULL, t("Successfully activated: @success \n skipped: @active active accounts \n  failed to activate: @failures", $parameters), NULL);
  }
  else {
    return openlucius_services_message(NULL, t("Successfully deactivated: @success \n skipped: @active inactive accounts \n  failed to deactivate: @failures", $parameters), NULL);
  }
}

/**
 * Function to add / remove a user from teams.
 *
 * @param int $uid
 *   The user to be altered.
 * @param array $data
 *   The data which is used for the team actions.
 *
 * @return array
 *   Returns an array containing the OpenLucius message.
 * @throws Exception
 *   Throws an exception if an error occurs.
 */
function _openlucius_services_user_team_actions($uid, array $data) {

  // Check if the user may administer users.
  if (!user_access("administer users")) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EDIT_ACCESS_DENIED);
  }

  // Fetch the menu item.
  $menu = menu_get_item();

  // Check if we have the groups parameter.
  if (empty($data['teams'])) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => 'teams'));
  }

  // Check if the type is correct.
  if (gettype($data['teams']) != 'array') {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($data['teams']),
      '@requirement' => 'array',
      '@for'         => 'teams',
    ));
  }

  // Fetch the account.
  $account = user_load($uid);

  // Fetch the wrapper for easy access.
  $wrapper = entity_metadata_wrapper('user', $account);

  // Fetch the drupal field.
  $drupal_field = openlucius_services_field_mapping('user', 'teams');

  // Fetch the user teams.
  $teams = openlucius_core_fetch_user_teams($uid);

  if ($menu['page_arguments'][1] == 'add_user_to_teams') {
    // Merge and get unique items to prevent double groups.
    $data['teams'] = array_unique(array_merge($teams, $data['teams']));
  }
  else {
    $data['teams'] = array_values(array_diff($teams, $data['teams']));
  }

  // Set the fields.
  _openlucius_services_user_set_field($wrapper, $drupal_field, $data);

  // Save the changes.
  $wrapper->save();

  // Send message to user.
  return openlucius_services_message($uid, t('Successfully updated user'), url('user/dashboard/' . $uid, array('absolute' => TRUE)));
}

/**
 * Function to add / remove a user from groups.
 *
 * @param int $uid
 *   The user to be altered.
 * @param array $data
 *   The data which is used for the groups actions.
 *
 * @return array
 *   Returns an array containing the OpenLucius message.
 * @throws Exception
 *   Throws an exception if an error occurs.
 */
function _openlucius_services_user_group_actions($uid, array $data) {

  // Check if the user may administer users.
  if (!user_access("administer users")) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EDIT_ACCESS_DENIED);
  }

  // Fetch the menu item.
  $menu = menu_get_item();

  // Check if we have the groups parameter.
  if (empty($data['groups'])) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => 'groups'));
  }

  // Check if the type is correct.
  if (gettype($data['groups']) != 'array') {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($data['groups']),
      '@requirement' => 'array',
      '@for'         => 'groups',
    ));
  }

  // Fetch the account.
  $account = user_load($uid);

  // User not found.
  if (empty($account)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_NOT_FOUND, array('@uid' => $uid));
  }

  // Fetch the wrapper for easy access.
  $wrapper = entity_metadata_wrapper('user', $account);

  // Fetch the drupal field.
  $drupal_field = openlucius_services_field_mapping('user', 'groups');

  // Fetch the user groups.
  $user_groups = openlucius_core_fetch_user_groups($account->uid);

  if ($menu['page_arguments'][1] == 'add_user_to_groups') {
    // Merge and get unique items to prevent double groups.
    $data['groups'] = array_unique(array_merge($user_groups, $data['groups']));
  }
  else {
    $data['groups'] = array_values(array_diff($user_groups, $data['groups']));
  }

  // Set the fields.
  _openlucius_services_user_set_field($wrapper, $drupal_field, $data);

  // Save the changes.
  $wrapper->save();

  // Send message to user.
  return openlucius_services_message($uid, t('Successfully updated user'), url('user/dashboard/' . $uid, array('absolute' => TRUE)));
}

/**
 * Function to set fields using the given data.
 *
 * @param \EntityMetadataWrapper $wrapper
 *   The entity metadata wrapper which is used for manipulating the fields.
 * @param string $field
 *   The field to be set.
 * @param null $data
 *   The data to use for setting the fields.
 */
function _openlucius_services_user_set_field(\EntityMetadataWrapper &$wrapper, $field, $data = NULL) {
  switch ($field) {

    case 'name':
      $wrapper->{$field}->set($data['name']);
      break;

    case 'mail':
      $wrapper->{$field}->set($data['mail']);
      break;

    // TODO fix password update @see https://www.drupal.org/node/2224645.
    // case 'password':
    // require_once DRUPAL_ROOT .
    // '/' . variable_get('password_inc', 'includes/password.inc');
    // $hashed_password = user_hash_password($data['password']);
    // $wrapper->{$field}->set($hashed_password);
    // break;
    case 'field_groups':
      $wrapper->{$field}->set($data['groups']);
      break;

    case 'field_user_phone':
      $wrapper->{$field}->set($data['phone']);
      break;

    case 'field_user_about_me':
      $wrapper->{$field}->set($data['body']);
      break;

    case 'field_user_tags':
      $wrapper->{$field}->set($data['labels']);
      break;

    case 'field_user_teams':
      $wrapper->{$field}->set($data['teams']);
      break;

    default:
      // Check if there's a module ready to process the data.
      if (count(module_implements('_openlucius_services_user_set_field')) > 0) {
        module_invoke_all('_openlucius_services_user_set_field', $wrapper, $field, $data);
      }
      // Return error as this field cannot yet be processed.
      else {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NOT_IMPLEMENTED_YET, array(
          "@type"   => $field,
          '@method' => '_openlucius_services_user_set_field',
          '@see'    => 'includes/resource.user.crud.inc',
        ));
      }
      break;
  }
}

/**
 * Function for validating the input for the user.
 *
 * @param array $data
 *   The input to be validated.
 * @param bool $new
 *   Whether this is a new account or an existing one.
 */
function _openlucius_services_user_input_validation($data, $new = FALSE) {
  global $user;

  // Check if the username is set.
  if (isset($data['name'])) {

    // Check if the user exists, if it does throw error.
    $account = user_load_by_name($data['name']);
    if (!empty($account)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EXISTS, array(
        '@type'  => 'name',
        '@value' => $data['name'],
      ));
    }
    else {
      $validated = user_validate_name($data['name']);
      if (!empty($validated)) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_DEFAULT, array('message' => $validated));
      }
    }
  }

  // Check if the mail is set.
  if (isset($data['mail'])) {

    // Check if the email address is correct.
    if (!valid_email_address($data['mail'])) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_INCORRECT_FORMAT, array(
        "@parameter" => $data['mail'],
        '@format'    => 'email@example.com',
      ));
    }

    // Try to load the user by using the mail to see if another account is
    // using this address, if it does throw error.
    $account = user_load_by_mail($data['mail']);
    if (!empty($account)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_EXISTS, array(
        '@type'  => 'mail',
        '@value' => $data['mail'],
      ));
    }
  }

  // Check if a role was entered.
  if (isset($data['role'])) {

    // Check if the role exists.
    $role = user_role_load($data['role']);
    if (empty($role)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_ROLE_NOT_FOUND, array('@rid' => $data['role']));
    }
    else {

      // Check if this user may pass this role to another user.
      if (user_has_role(OPENLUCIUS_CORE_OPENLUCIUS_MANAGER_ROLE) && $data['role'] == OPENLUCIUS_CORE_OPENLUCIUS_ADMIN_ROLE) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_ROLE_NOT_ALLOWED, array('@role' => $role->name));
      }
    }
  }

  if (isset($data['groups'])) {
    // Check if the groups are of the correct type.
    if (is_array($data['groups'])) {
      foreach ($data['groups'] as $group) {

        // Check if the nid is numeric.
        if (!is_numeric($group)) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
            '@input'       => gettype($group),
            '@requirement' => 'integer',
            '@for'         => 'group',
          ));
        }
        // Verify that this node is indeed a group.
        else {
          $type = openlucius_core_get_type_by_nid($group);
          if ($type != 'ol_group') {
            $reverse = openlucius_services_map_type($type, TRUE);
            openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_WRONG_TYPE, array(
              '@type'  => $reverse,
              '@type2' => 'group',
            ));
          }
        }
        // Verify that the creator of this user is in the group
        // he / she intends to give to the new user.
        if (!openlucius_core_user_in_group($group, $user->uid)) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_GROUP_ACCESS_DENIED);
        }
      }
    }
    // Return Error because the type is incorrect.
    else {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
        '@input'       => gettype($data['groups']),
        '@requirement' => 'array',
        '@for'         => 'groups',
      ));
    }
  }

  // Check if we have labels.
  if (isset($data['labels'])) {

    // Check if we have an array.
    if (is_array($data['labels'])) {
      foreach ($data['labels'] as $label) {
        $term = taxonomy_term_load($label);

        // Check if the term exists.
        if (empty($term)) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NON_EXISTING_TERM, array("@type" => "labels"));
        }
        // Check if the term is of the correct vocabulary.
        elseif ($term->vocabulary_machine_name != 'user_tags') {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_VOCABULARY);
        }
      }
    }
    else {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
        '@input'       => gettype($data['labels']),
        '@requirement' => 'array',
        '@for'         => 'labels',
      ));
    }
  }

  // Check if teams are set.
  if (isset($data['teams'])) {

    // Check if the teams are of the correct type.
    if (is_array($data['teams'])) {
      foreach ($data['teams'] as $team) {

        // Check if the nid is numeric.
        if (!is_numeric($team)) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
            '@input'       => gettype($team),
            '@requirement' => 'integer',
            '@for'         => 'item in teams',
          ));
        }
        // Verify that this node is indeed a team.
        else {
          $type = openlucius_core_get_type_by_nid($team);
          if ($type != 'team') {
            $reverse = openlucius_services_map_type($type, TRUE);
            openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_WRONG_TYPE, array(
              '@type'  => $reverse,
              '@type2' => 'team',
            ));
          }
        }
      }
    }
    // Return Error because the type is incorrect.
    else {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
        '@input'       => gettype($data['teams']),
        '@requirement' => 'array',
        '@for'         => 'teams',
      ));
    }
  }
}
