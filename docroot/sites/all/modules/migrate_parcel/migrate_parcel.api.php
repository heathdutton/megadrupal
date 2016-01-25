<?php

/**
 * @file
 * Contains hook definitions for the Migrate Export module.
 */

/**
 * Alter field definitions for entities.
 *
 * @param $fields
 *  An array of fields as returned by Migration destination class. Keys are
 *  field machine names, values are UI descriptions.
 * @param $entity_type
 *  The type of the entity to get fields for.
 * @param $bundle_name
 *  The bundle machine name to get fields for.
 *
 * @return
 *  A flat array of field names.
 */
function hook_migrate_parcel_fields_alter(&$fields, $entity_type, $bundle_name) {
  if ($entity_type == 'taxonomy_term') {
    // Unset stuff we can't handle.
    unset($fields['pathauto']);
  }
}
