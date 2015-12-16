<?php
/**
 * @file
 * Provides API documentation.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Alter the field suppression during field storage.
 *
 * @param $skip_fields
 *   An array keyed by field IDs whose data will not be loaded or saved
 *   depending on $hook.
 * @param $hook
 *   The field storage hook being invoked hook_field_storage_pre_*(), either
 *   load, insert, or update.
 * @param $fields
 *   List of fields as returned by field_info_fields().
 * @see hook_field_storage_pre_load()
 * @see hook_field_storage_pre_insert()
 * @see hook_field_storage_pre_update()
 * @see field_info_fields()
 */
function hook_field_suppress_alter(&$skip_fields, $hook, $fields) {
  if ($hook == 'insert') {
    // Allow field data for some_field to be stored on insert, but not update
    // if the field_suppress setting is set to 'always'.
    unset($skip_fields[$fields['some_field']['id']]);
  }
}
