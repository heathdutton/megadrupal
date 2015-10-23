<?php
/**
 * @file
 *  Support Party attached entity destinations.
 */

/**
 * Destination class implementating migration into party attached entities (of
 * any type).
 */
class MigrateDestinationPartyAttachedEntity extends MigrateDestinationEntityAPI {

  var $data_set_name;
  var $data_set_info;
  var $data_set_controller;
  var $id_key;
  var $hats;
  var $override;

  /**
   * Basic initialisation.
   *
   * @param string $data_set_name
   *   The name of the data set to import to.
   * @param array $options
   *   Options (language, text_format) use for creating fields.
   */
  function __construct($data_set_name, array $options = array()) {
    $this->data_set_info = party_get_data_set_info($data_set_name);

    if (!$this->data_set_info) {
      throw new Exception("Data set {$data_set_name} does not exist.");
    }

    $this->data_set_name = $data_set_name;
    $this->hats = isset($options['hats']) ? $options['hats']: array();
    $this->id_key = isset($options['id_key']) ? $options['id_key']: NULL;
    $this->override = !empty($options['override']);

    parent::__construct($this->data_set_info['entity type'], $this->data_set_info['entity bundle'], $options);
  }

  /**
   * Import the entity. Hopefully this will preempt all the things to do with
   * ids.
   */
  public function import(stdClass $entity, stdClass $source_row) {
    // Get hold of our party.
    $migration = Migration::currentMigration();
    $id_key = isset($this->id_key) ? $this->id_key: key($migration->getMap()->getSourceKey());

    if (!isset($entity->{$this->id})) {
      // Attempt to load the party.
      if (!$party = entity_load_single('party', $source_row->{$id_key})) {
        // Party doesn't exist, so let's create it.
        $party = new Party();
        $party->is_new = TRUE;
        $party->pid = $source_row->{$id_key};
        $party->save();
      }

      // Load our data set and store against the source row as it gets passed
      // around.
      $source_row->data_set_controller = $party->getDataSetController($this->data_set_name);

      if ($this->override) {
        $ids = $source_row->data_set_controller->getEntityIds();
        if (!empty($ids)) {
          $source_row->migrate_map_destid1 =
            $entity->{$this->id} = reset($ids);
        }
      }
    }
    else {
      if (!$party = entity_load_single('party', $source_row->{$id_key})) {
        throw new MigrateException(t('Unable to find party for destination entity.'));
      }
    }

    // Assign any hats that are required by this migration.
    party_hat_hats_assign($party, $this->hats);

    return parent::import($entity, $source_row);
  }

  /**
   * Overrides MigrateDestinationEntity::prepare().
   *
   * Attach our entity to make sure no automated attaching happens on
   * controller save.
   */
  public function prepare($entity, stdClass $source_row) {
    $source_row->data_set_controller->attachEntity($entity);
    parent::prepare($entity, $source_row);
  }

  /**
   * Overrides MigrateDestinationEntity::complete().
   *
   * Make sure our entity gets attached.
   */
  public function complete($entity, $row) {
    $row->data_set_controller->save();
    parent::complete($entity, $row);
  }

  /**
   * Overrides MigrateDestinationEntityAPI::bulkRollback().
   *
   * Make sure the party attached entities table gets cleaned up.
   */
  public function bulkRollback(array $ids) {
    $result = parent::bulkRollback($ids);

    if ($result !== FALSE) {
      db_delete('party_attached_entity')
        ->condition('data_set', $this->data_set_name)
        ->condition('eid', $ids)
        ->execute();
    }

    return $result;
  }
}

