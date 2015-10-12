<?php
/**
 * @file
 * Hooks provided by the File Entity Perms module.
 */

/**
 * Declare that your module provides permissioning types.
 */
function hook_file_entity_perms_info($owner, $api) {
  return array(
    'role' => array(
      'title' => 'Role',
    ),
  );
}

/**
 * Define form elements for the permission section of the entity form.
 *
 * @param $form
 *   The specific types section of the form.
 *
 * @param $permission
 *   The current permissions for this permissioning type.
 */
function hook_file_entity_perms_TYPE_form(&$form, $permission) {
  // If no permissions were passed then set it to an empty array.
  $permission = is_null($permission) ? array() : $permission;

  // Select role permissions.
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => $permission,
    '#options' => user_roles(TRUE),
    '#description' => t('Roles that can view this file.'),
  );
}

/**
 * Process the values on form submission.
 *
 * @param $fid
 *   The file id.
 *
 * @param $values
 *   The form values for this permission type.
 */
function hook_file_entity_perms_TYPE_form_submit($fid, $values) {
  // Set our permissions.
  file_entity_perms_set_permission($fid, 'role', $values['roles']);
}

/**
 * Determine access rights for a file of a specific type.
 *
 * @param $file
 *   The file object we are testing.
 *
 * @param $account
 *   The user account to test.
 *
 * @param $permission
 *   The current permissions of that file.
 *
 * @return
 *   FILE_ENTITY_ACCESS_ALLOW if the operation is to be allowed;
 *   FILE_ENTITY_ACCESS_DENY if the operation is to be denied;
 *   FILE_ENTITY_ACCESS_IGNORE to not affect this operation at all.
 */
function hook_file_entity_perms_TYPE_access($file, $account, $permission) {
  // We need to get the keys of the users roles so it will be in the format
  // that we save the roles.
  $user_roles = array_keys($account->roles);

  // Find out if we have any matching roles.
  $matching_roles = array_intersect($permission, $user_roles);

  // If any of our roles match then allow access.
  if (!empty($matching_roles)) {
    return FILE_ENTITY_ACCESS_ALLOW;
  }
  else {
    return FILE_ENTITY_ACCESS_DENY;
  }
}


/**
 * Process the values when a permission is set.
 *
 * @param $fid
 *   The file id.
 *
 * @param $type
 *   The permissioning type.
 *
 * @param $permission
 *   The permissions being set.
 */
function hook_file_entity_perms_set_permission($fid, $type, $permission) {

}

/**
 * Process the values when a permission is deleted.
 *
 * @param $fid
 *   The file id.
 *
 * @param $type
 *   The permissioning type.
 */
function hook_file_entity_perms_delete_permission($fid, $type) {

}
