<?php

/**
 * @file
 * Hooks provided by Kaltura module.
 */

/**
 * Returns the URL the user will be redirected to "after AddEntry in the CW".
 *
 * Only first module implemented this hook will be counted.
 *
 * @return string
 *   A string containing a URL.
 *
 * @see kaltura_cw_destination()
 *
 * @todo Provide a better description.
 */
function hook_kaltura_cw_destination() {
  return url('kaltura/entries');
}

/**
 * Executes any additional tasks after "notification is received".
 *
 * @param array $notification_data
 *   Associative array as it is contained in each element of $data property of
 *   object of KalturaNotificationClient class. Keys include:
 *   - notification_id.
 *   - notification_type.
 *
 * @see kaltura_notification_handler()
 * @see \KalturaNotificationClient
 *
 * @todo Provide a better description.
 * @todo Describe all possible elements of $notification_data array.
 */
function hook_kaltura_notification_handler(array $notification_data) {
  // @todo Add an example.
}

/**
 * Acts on Kaltura Media Entries being loaded from the database.
 *
 * This hook is invoked during Kaltura Media Entry loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param array $entities
 *   An array of Kaltura Media Entry entities being loaded, keyed by id.
 *
 * @see hook_entity_load()
 */
function hook_kaltura_entry_load(array $entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
 * Responds when a Kaltura Media Entry is inserted.
 *
 * This hook is invoked after the Kaltura Media Entry is inserted into the
 * database.
 *
 * @param Entity $entity
 *   The Kaltura Media Entry that is being inserted.
 *
 * @see hook_entity_insert()
 */
function hook_kaltura_entry_insert(Entity $entity) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('kaltura_entry', $entity),
      'extra' => print_r($entity, TRUE),
    ))
    ->execute();
}

/**
 * Acts on a Kaltura Media Entry being inserted or updated.
 *
 * This hook is invoked before the Kaltura Media Entry is saved to the database.
 *
 * @param Entity $entity
 *   The Kaltura Media Entry that is being inserted or updated.
 *
 * @see hook_entity_presave()
 */
function hook_kaltura_entry_presave(Entity $entity) {
  ++$entity->kaltura_views;
}

/**
 * Responds to a Kaltura Media Entry being updated.
 *
 * This hook is invoked after the Kaltura Media Entry has been updated in the
 * database.
 *
 * @param Entity $entity
 *   The Kaltura Media Entry that is being updated.
 *
 * @see hook_entity_update()
 */
function hook_kaltura_entry_update(Entity $entity) {
  db_update('mytable')
    ->fields(array('extra' => print_r($entity, TRUE)))
    ->condition('id', entity_id('kaltura_entry', $entity))
    ->execute();
}

/**
 * Responds to Kaltura Media Entry deletion.
 *
 * This hook is invoked after the Kaltura Media Entry has been removed from the
 * database.
 *
 * @param Entity $entity
 *   The Kaltura Media Entry that is being deleted.
 *
 * @see hook_entity_delete()
 */
function hook_kaltura_entry_delete(Entity $entity) {
  db_delete('mytable')
    ->condition('pid', entity_id('kaltura_entry', $entity))
    ->execute();
}

/**
 * Acts on a Kaltura Media Entry given the custom metadata.
 *
 * Modules implementing this hook may attach fields to the Kaltura Media Entry
 * object based on provided metadata before the object is saved to the database.
 *
 * @param Entity $entity
 *   The Kaltura Media Entry that is being inserted or updated.
 * @param array $metadata
 *   Each array's element corresponds to metadata from one profile: key is the
 *   profile ID and value is an array with the following elements:
 *   - fields: Array with metadata fields, where each key is the field's
 *     system name and each value is an indexed array of field's values.
 *   - metadata: KalturaMetadata object as returned from the service.
 *   - metadata_profile: KalturaMetadataProfile object as returned from the
 *     service.
 *
 * @see kaltura_save_entry_metadata()
 * @see kaltura_kaltura_save_entry_metadata()
 */
function hook_kaltura_save_entry_metadata(Entity $entity, array $metadata) {
  // Remove previously set field values so if they were removed at KMC then
  // the changes will be reflected locally.
  $entity->field_mymodule_data = array();

  foreach ($metadata as $profile_id => $profile_meta) {
    $profile = $profile_meta['metadata_profile'];
    $fields  = $profile_meta['fields'];

    // Check the profile because fields with the same name may exist in multiple
    // profiles.
    if ($profile->systemName === 'SomeEntryInfo') {
      // Attach metadata field 'LoremIpsum' as field_mymodule_data to the
      // entity.
      if (!empty($fields['LoremIpsum'])) {
        foreach ($fields['LoremIpsum'] as $item) {
          // Kaltura Media Entry entity is not language-aware so it is safe and
          // rational to use LANGUAGE_NONE for its fields.
          $entity->field_mymodule_data[LANGUAGE_NONE][]['value'] = $item;
        }
      }
    }
  }
}
