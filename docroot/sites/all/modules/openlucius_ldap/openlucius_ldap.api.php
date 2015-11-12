<?php
/**
 * @file
 * Hooks provided by the openlucius_ldap module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Openlucius LDAP hook which allows other modules to change the search query.
 *
 * @param array $variables
 *   An array of strings to be used for searching in LDAP.
 *
 * @return array
 *   Returns the altered variables.
 */
function hook_openlucius_ldap_get_ldap_search_query($variables) {
}

/**
 * Openlucius LDAP hook for ldap login triggers.
 *
 * @param array $result
 *   The result obtained from LDAP.
 *
 * @return mixed
 *   Returns the value after other modules have mingled with it.
 *
 * @see openlucius_ldap_user_login()
 */
function hook_openlucius_ldap_login_trigger($result) {

}

/**
 * Hook to allow other modules to alter the fields.
 *
 * @param array $fields
 *   The user fields array to be altered.
 * @param array $data
 *   The LDAP data of a user.
 *
 * @return array
 *   The fields array to be used for user creation.
 */
function hook_openlucius_ldap_pre_create_user($fields, $data) {

}

/**
 * Hook to allow other modules to alter the fields before saving the user.
 *
 * @param array $fields
 *   The user fields array to be altered.
 * @param array $data
 *   The LDAP data of a user.
 *
 * @return array
 *   The fields array to be used for user creation.
 */
function openlucius_ldap_pre_create_user($fields, $data) {
  if (count(module_implements('openlucius_ldap_pre_create_user')) > 0) {

    // Expose the LDAP result to other modules to react upon.
    $fields = module_invoke_all('openlucius_ldap_pre_create_user', $fields, $data);
  }

  return $fields;
}

/**
 * Hook to allow other modules to alter the user_wrapper before saving.
 *
 * @param \EntityMetadataWrapper $user_wrapper
 *   The EntityMetadataWrapper to be altered or used.
 * @param array $data
 *   The LDAP data which was used to create the entity.
 *
 * @return \EntityMetadataWrapper
 *   The EntityMetadataWrapper after usage.
 */
function hook_openlucius_ldap_post_create_user(\EntityMetadataWrapper $user_wrapper, $data) {
  if (count(module_implements('openlucius_ldap_post_create_user')) > 0) {

    // Expose the LDAP result to other modules to react upon.
    $user_wrapper = module_invoke_all('openlucius_ldap_post_create_user', $user_wrapper, $data);
  }

  return $user_wrapper;
}

/**
 * Alter for the user wrapper after all fields have been set (before saving).
 */
function hook_openlucius_ldap_after_create_new_user_alter(&$user_wrapper, &$user) {

}

/**
 * @} End of "addtogroup hooks".
 */
