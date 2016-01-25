<?php

namespace Drupal\quizz_migrate;

use MigrateDestinationEntity;
use stdClass;

class BaseEntityDestination extends MigrateDestinationEntity {

  protected $entity_type;
  protected $base_table;
  protected $bundle;
  protected static $pk_name;
  protected $disable_import_complete = FALSE;

  public function __construct($options = array()) {
    $this->bundle = isset($options['bundle']) ? $options['bundle'] : (isset($this->bundle) ? $this->bundle : NULL);
    parent::__construct($this->entity_type, $this->bundle, $options);
  }

  static public function getKeySchema() {
    return array(
        static::$pk_name => array(
            'type'        => 'int',
            'unsigned'    => TRUE,
            'description' => 'ID or VID of entity',
        ),
    );
  }

  public function fields($migration = NULL) {
    $fields = array();

    $schema = drupal_get_schema($this->base_table);
    foreach ($schema['fields'] as $name => $info) {
      $fields[$name] = isset($info['description']) ? $info['description'] : $name;
    }

    if ($this->bundle) {
      $entity_type = str_replace(' ', '_', ucwords(str_replace('_', ' ', $this->entity_type)));
      $fields += migrate_handler_invoke_all($entity_type, 'fields', $this->entity_type, $this->bundle, $migration);
      $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entity_type, $this->bundle, $migration);
    }

    return $fields;
  }

  public function bulkRollback(array $ids) {
    migrate_instrument_start($this->entityType . '_delete_multiple');
    $this->prepareRollback($ids);
    $this->doBulkRollBack($ids);
    $this->completeRollback($ids);
    migrate_instrument_stop($this->entityType . '_delete_multiple');
  }

  protected function doBulkRollBack($ids) {
    entity_delete_multiple($this->entityType, $ids);
  }

  public function import(stdClass $entity, stdClass $row) {
    $entity = entity_create($this->entityType, (array) $entity);
    unset($entity->is_new);
    return $this->doImport($entity, $row);
  }

  protected function doImport($entity, $row) {
    $this->prepare($entity, $row);
    $entity->save();

    if (!$this->disable_import_complete) {
      $this->complete($entity, $row);
    }

    return array($entity->{static::$pk_name});
  }

}
