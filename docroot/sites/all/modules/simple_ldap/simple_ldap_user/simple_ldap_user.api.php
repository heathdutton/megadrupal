<?php
/**
 * @file
 * Describe hooks provided by the Simple LDAP User module.
 */

/**
 * SimpleLdapUser fingerprint.
 *
 * Variables exposed by __get() and __set()
 * ----------------------------------------
 * $attributes
 * $dn
 * $exists
 * $server
 *
 * Magic methods
 * -------------
 * __construct($name)
 * __get($name)
 * __set($name, $value)
 *
 * Public functions
 * ----------------
 * authenticate($password)
 * save()
 * delete()
 *
 * Public static methods
 * ---------------------
 * singleton($name)
 * filter()
 * reset()
 * hash($key, $value)
 */

/**
 * simple_ldap_user helper functions.
 *
 * simple_ldap_user_load_or_create_by_name($name)
 * simple_ldap_user_login_name_validate($form, &$form_state)
 * simple_ldap_user_sync_user_to_ldap($drupal_user)
 * simple_ldap_user_sync_user_to_drupal($drupal_user)
 * simple_ldap_user_variable_get($varuable)
 */

/**
 * Synchronizes a Drupal user to LDAP.
 *
 * This hook is called when simple_ldap_user needs to synchronize Drupal user
 * data to LDAP.
 *
 * This example sets the LDAP employeeType attribute to "full-time"
 *
 * @param StdClass $user
 *   The full Drupal user object that is being synchronized.
 */
function hook_sync_user_to_ldap($user) {
  $ldap_user = SimpleLdapUser::singleton($user->name);
  $ldap_user->employeeType = 'full-time';
  $ldap_user->save();
}

/**
 * Alter user data before saving to Drupal.
 *
 * This hook is called when simple_ldap_user is doing an account synchronization
 * from LDAP to Drupal, immediately before user_save() is called.
 *
 * @param array $edit
 *   Array of changes to apply to the Drupal user by user_save().
 * @param StdClass $drupal_user
 *   The Drupal user object to be saved.
 * @param SimpleLdapUser $ldap_user
 *   The SimpleLdapUser object that matches the Drupal user object.
 */
function hook_simple_ldap_user_to_drupal_alter($edit, $drupal_user, $ldap_user) {
}

/**
 * Alter user data before saving to LDAP.
 *
 * This hook is called when simple_ldap_user is doing an account synchronization
 * from Drupal to LDAP, immediately before SimpleLdapUser::save() is called.
 *
 * @param SimpleLdapUser $ldap_user
 *   The SimpleLdapUser object to be saved.
 * @param StdClass $drupal_user
 *   The Drupal user object that matches the SimpleLdapUser object.
 */
function hook_simple_ldap_user_to_ldap_alter($ldap_user, $drupal_user) {
}
