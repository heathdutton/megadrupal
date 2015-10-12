<?php
/**
 * @file
 * Code for pollim.migrate.inc.
 */

/**
 * Implements migrate hook_migrate_api().
 */
function pollim_migrate_api() {
  $api = array(
    'api' => 2,
  );

  return $api;
}


/**
 * Destination class implementing migration into pollim.
 */
class MigrateDestinationPollim extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'pollim_id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'description' => 'ID of destination pollim',
      ),
    );
  }

  /**
   * Return an options array for pollim destinations.
   *
   * @param string $language
   *   Default language for pollims created via this destination class.
   * @param string $textFormat
   *   Default text format for pollims created via this destination class.
   *
   * @return array
   *   Default option values.
   */
  static public function options($language, $textFormat) {
    return compact('language', 'textFormat');
  }

  /**
   * Basic initialization.
   *
   * @param string $bundle
   *   A.k.a. the pollim type (basic, advanced, etc.) of the pollim.
   * @param array $options
   *   Options applied to pollims.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct('pollim', $bundle, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function fields($migration = NULL) {
    $fields = array();
    // First the core (pollim table) properties.
    $fields['pollim_id'] = t('Pollim ID');
    $fields['name'] = t('Pollim: Name');
    $fields['type'] = t('Pollim: Bundle');
    $fields['created'] = t('Pollim: Created timestamp');
    $fields['changed'] = t('Pollim: Modified timestamp');
    $fields['language'] = t('Pollim: Language (fr, en, ...)');
    $fields['is_new'] = t('Option: Indicates a new pollim with the specified pollim_id should be created');

    // Then add in anything provided by handlers.
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('Pollim', 'fields', $this->entityType, $this->bundle, $migration);

    return $fields;
  }

  /**
   * Delete a batch of pollims at once.
   *
   * @param array $pollimIds
   *   List of pollim IDs to be deleted.
   */
  public function bulkRollback(array $pollimIds) {
    migrate_instrument_start('pollim_delete_multiple');
    $this->prepareRollback($pollimIds);
    pollim_delete_multiple($pollimIds);
    $this->completeRollback($pollimIds);
    migrate_instrument_stop('pollim_delete_multiple');
  }

  /**
   * Import a single pollim.
   *
   * @param stdClass $pollim
   *   Pollim object to build. Prefilled with any fields mapped in the Migration.
   * @param stdClass $row
   *   Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *   List of key fields (pollim_id only in this case) of the pollim that was
   *   saved if successful. FALSE on failure.
   *
   * @throws MigrateException
   */
  public function import(stdClass $pollim, stdClass $row) {
    // Updating previously-migrated content?
    $migration = Migration::currentMigration();
    if (isset($row->migrate_map_destid1)) {
      // Make sure is_new is off.
      $pollim->is_new = FALSE;
      if (isset($pollim->pollim_id)) {
        if ($pollim->pollim_id != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming pollim_id !pollim_id and map destination pollim_id !destid1 don't match",
            array('!pollim_id' => $pollim->pollim_id, '!destid1' => $row->migrate_map_destid1)));
        }
        else {
          $pollim->pollim_id = $row->migrate_map_destid1;
        }
      }
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($pollim->pollim_id)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination pollim_id provided'));
      }
      $oldPollim = pollim_load($pollim->pollim_id);
      if (empty($oldPollim)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but pollim !pollim_id does not exist',
          array('!pollim_id' => $pollim->pollim_id)));
      }
      if (!isset($pollim->created)) {
        $pollim->created = $oldPollim->created;
      }
    }
    elseif (!isset($pollim->type)) {
      // Default the type to our designated destination bundle (by doing this
      // conditionally, we permit some flexibility in terms of implementing
      // migrations which can affect more than one type).
      $pollim->type = $this->bundle;
    }

    // Set some required properties.
    if ($migration->getSystemOfRecord() == Migration::SOURCE) {
      if (empty($pollim->language)) {
        $pollim->language = $this->language;
      }

      // Apply defaults, allow standard entity prepare hooks to fire.
      if (isset($pollim->created)) {
        $pollim->created = MigrationBase::timestamp($pollim->created);
      }
      else {
        $pollim->created = REQUEST_TIME;
      }
      if (isset($pollim->changed)) {
        $changed = MigrationBase::timestamp($pollim->changed);
      }
    }

    // Invoke migration prepare handlers.
    $this->prepare($pollim, $row);

    // Trying to update an existing pollim.
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      // Incoming data overrides existing, so only copy non-existent fields.
      foreach (array_keys($oldPollim) as $field) {
        // An explicit NULL in the source data means to wipe to old value
        // (i.e., don't copy it over from $old_pollim)
        if (property_exists($pollim, $field) && $pollim->$field === NULL) {
          // Ignore this field.
        }
        elseif (!isset($pollim->$field)) {
          $pollim->$field = $oldPollim->$field;
        }
      }
    }

    if (isset($pollim->pollim_id) && !(isset($pollim->is_new) && $pollim->is_new)) {
      $updating = TRUE;
    }
    else {
      $updating = FALSE;
    }

    migrate_instrument_start('pollim_save');
    entity_save('pollim', $pollim);
    migrate_instrument_stop('pollim_save');

    if (isset($pollim->pollim_id)) {
      if ($updating) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }

      // Unfortunately, http://drupal.org/node/722688 was not accepted, so fix
      // the changed timestamp.
      if (isset($changed)) {
        db_update('pollim')
          ->fields(array('changed' => $changed))
          ->condition('pollim_id', $pollim->pollim_id)
          ->execute();
        $pollim->changed = $changed;
      }

      $return = array($pollim->pollim_id);
    }
    else {
      $return = FALSE;
    }

    $this->complete($pollim, $row);
    return $return;
  }
}
