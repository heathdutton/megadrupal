<?php

class FieldIndex extends Entity {

  public $id;
  public $name;
  public $label;
  public $published;
  public $status;
  public $entity_type;
  public $bundle;
  public $field_name;
  public $column_names;

  /**
   * Returns whether the quiz type is locked, thus may not be deleted or renamed.
   *
   * Quiz types provided in code are automatically treated as locked, as well
   * as any fixed quiz type.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

  public function getTable() {
    $field = field_info_field($this->field_name);
    return _field_sql_storage_tablename($field);
  }

}

class FieldIndexController extends EntityAPIControllerExportable {

  /**
   * Get entity types which support fields, use SQL storage
   */
  public function getEntityTypes() {
    $info = array();

    foreach (entity_get_info() as $type_name => $entity_info) {
      if (empty($entity_info['fieldable'])) {
        continue;
      }

      if ($this->isSQLEntityType($type_name)) {
        $info[$type_name] = $entity_info;
      }
    }

    return $info;
  }

  protected function isSQLEntityType($type_name) {
    foreach (field_info_instances($type_name) as $field_instances) {
      if (!empty($field_instances)) {
        foreach (array_keys($field_instances) as $field_name) {
          $field_info = field_info_field($field_name);
          if ('field_sql_storage' === $field_info['storage']['module']) {
            return TRUE;
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   * @param FieldIndex $field_index
   */
  public function save($field_index, DatabaseTransaction $transaction = NULL) {
    $return = parent::save($field_index, $transaction);

    // Find table
    if ($field = field_info_field($field_index->field_name)) {
      $table = $field_index->getTable();

      // Find columns
      $columns = array();
      foreach ($field_index->column_names as $column_name) {
        $columns[] = _field_sql_storage_columnname($field_index->field_name, $column_name);
      }

      // Create index
      $index_name = 'field_index__' . str_replace(' ', '_', strtolower($field_index->name));
      if (db_index_exists($table, $index_name)) {
        db_drop_index($table, $index_name);
      }

      if ($field_index->published) {
        db_add_index($table, $index_name, $columns);
      }
    }

    return $return;
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    foreach (entity_load($this->entityType, $ids) as $field_index) {
      $table = $field_index->getTable();
      $index_name = 'field_index__' . str_replace(' ', '_', strtolower($field_index->name));
      if (db_index_exists($table, $index_name)) {
        db_drop_index($table, $index_name);
      }
    }
    parent::delete($ids, $transaction);
  }

}
