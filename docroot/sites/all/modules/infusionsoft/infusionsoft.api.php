<?php
/**
 * @file
 * Developer hooks for infusionsoft module.
 */

/**
 * Alter Hook to modify the infusionsoft conatct information
 * before it is sent to Infusionsoft.  See contact info
 * data format in 'infusionsoft_contact' type
 * @see infusionsoft_datatypes_info()
 *
 * @param array $contact
 *   An array of key value pairs of the infusionsoft_contact
 *   data type
 * @param StdClass $account
 *   A Drupal User
 */
function hook_infusionsoft_contact_alter(&$contact, $account) {
  if ($account->some_param) {
    $contact['Company'] = my_function_get_user_company($account);
  }
}
