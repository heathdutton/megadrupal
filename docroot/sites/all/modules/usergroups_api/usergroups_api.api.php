<?php

/**
 * @file
 * Hooks provided by the Usergroups API module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Retrieve a list of groups defined.
 *
 * @return
 *   An array of associative arrays. Each inner array has elements:
 *   - "name": The internal name of the group. No more than 48 characters.
 *   - "title": The human-readable, localized name of the group.
 *   - "weight": An integer specifying the groups's sort ordering.
 *   - "description": (optional) A short description of what the group
 *      contains.
 *   - "include": (optional) The path to the file to include before calling
 *     additional hooks related to this group. Path should be relative to
 *     drupal root.
 *   - "hidden": (optional) Whether this group is hidden in lists and other
 *     places. If the list of groups is quite large, it may be a good idea
 *     to set this to TRUE.
 */
function hook_usergroups_api_groups() {
  return array(
    'user' => array(
      'name' => 'user',
      'title' => t('User'),
      'description' => t('The specific user. Each user is its own group.'),
      'include' => drupal_get_path('module', 'usergroups_api') .'/modules/user.usergroups.inc',
      'module' => 'user',
      'hidden' => TRUE,
      'weight' => -100,
    ),
    'role' => array(
      'name' => 'role',
      'title' => t('Role'),
      'description' => t('User is part of a group if they have the specific role.'),
      'description' => drupal_get_path('module', 'usergroups_api') .'/modules/user.usergroups.inc',
      'module' => 'user',
      'weight' => -99,
    ),
  );
}

/**
 * @return
 *   An associative array with each group defined by the module.
 *   The key should be the unique id and each element the group
 *   title.
 */
function hook_usergroups_api_GROUP_NAME_options() {
  $groups = array(
    1 => t('My cool group'),
    3 => t('My cool group 3'),
  );
  return $groups;
}

/**
 * Optional hook. If your module defines a large number of groups
 * or hook_usergroups_api_GROUP_NAME_options() takes a long time
 * to process, this hook will be called to determine the title
 * of a specific group.
 *
 * @param $id
 *   A group id defined by your module.
 * @return
 *   The title for a specific group.
 */
function hook_usergroups_api_GROUP_NAME_options_title($id) {
  if ($id == 1) {
    return t('My cool group');
  }
  else {
    return t('My cool group 3');
  }
}

/**
 * Determine the specific groups that a user belongs to.
 *
 * @param $account
 *   A user object to load the specific groups for.
 * @return
 *   An associative array of groups that the user is a member of.
 */
function hook_usergroups_api_GROUP_NAME_specific_groups($id) {
  $groups = array(
    1 => t('My cool group'),
  );
  return $groups;
}

/**
 * Define the form item that will be used for adding new users to the usergroups
 * options form.
 * This hook is not required. If not defined, a simple multiple select form item
 * will be generated with the hook_usergroups_api_GROUP_NAME_options() as options.
 *
 * @param $form
 *   The $form array. Use this if you have more than one field you need.
 * @param $form_state
 *   The standard $form_state array for any form.
 * @param $options
 *   A keyed array of options for the form. Possible keys:
 *    'whitelist' - Array of group types to include in the options form. If
 *                  no groups are set then all groups are allowed (unless
 *                  excluded by the 'blacklist').
 *    'blacklist' - Array of group types to exclude from the options form.
 *    'fieldset' - The name of the fieldset to put the form inside of. If set
 *                 to FALSE, then the options will be placed directly in the
 *                 form. Default value: 'usergroups_api'
 *    'fieldset_description' - The description to put inside of the fieldset
 *                             when using the fieldset.
 *    'multiples_per_group' - Allow the user to select multiple group ids in
 *                            each group.
 *    'multiple_groups' - Allow the user to select multiple group types to add
 *                        groups from.
 *    'defaults' - An array of default values of selected groups. If multiple
 *                 group types are allowed, a single array will be fine as the
 *                 values will be processed internally.
 *
 * @return
 *   A form item array that will be placed in the new form.
 */
function hook_usergroups_api_GROUP_NAME_options_form($form, $form_state, $options) {
  $form_item = array(
    '#type' => 'textfield',
    '#description' => t('Begin entering the user\'s username that you would like to select.'),
    '#autocomplete_path' => 'usergroups_api/user/autocomplete',
  );
  if ($options['multiples_per_group']) {
    $form_item['#description'] .= ' ' . t('Separate multiple usernames with a comma.');
  }

  // Convert the uids to usernames.
  $names = array();
  if (isset($options['defaults_per_type']['original']['user'])) {
    foreach ($options['defaults_per_type']['original']['user'] as $uid) {
      $names[] = _user_usergroups_api_user_get_username($uid);
    }
  }
  $form_item['#default_value'] = implode(',', $names);
  return $form_item;
}

/**
 * Validation of the form items added for this group type.
 * This hook is not required. By default, the values from the $form_state under
 * 'usergroups_api_group_GROUP_NAME' will be used. It is assumed that this will
 * be an array of rekeyed group ids.
 *
 * @param $form
 *   The $form array. Use this if you have more than one field you need. The
 *   $options array is found here: $form['#usergroups_api'].
 * @param $form_state
 *   The standard $form_state array for any form.
 *
 * @return
 *   An array of groups that were selected for this group type by the user.
 *
 * @see hook_usergroups_api_GROUP_NAME_options_form()
 */
function hook_usergroups_api_GROUP_NAME_options_form_validate($form, $form_state) {
  $users = $groups = array();
  $names_entered = explode(',', $form_state['values']['usergroups_api_group_user']);
  foreach ($names_entered as $name) {
    $name = trim($name);
    // Only allow a username once and do not allow blank usernames.
    if (!$name || isset($users[$name])) {
      continue;
    }
    $users[$name] = _user_usergroups_api_user_get_uid($name);
    // As long as the user was found, add them to the list of groups.
    if ($users[$name]) {
      $groups[$users[$name]] = $users[$name];
    }
  }
  return $groups;
}
