<?php
/**
 * @file
 * Environment Field Access (Fields Helper).
 */

/**
 * Class EnvironmentFieldAccessFields.
 */
class EnvironmentFieldAccessFields {

  private $fields = [];
  private $instances = [];

  /**
   * Set field definitions.
   *
   * @param array[] $fields
   *   An associative array where keys - field names and values - definition.
   */
  public function __construct(array $fields) {
    $this->fields = $fields;
  }

  /**
   * Create fields.
   *
   * @throws \FieldException
   *   When cannot create a field.
   */
  public function create() {
    foreach ($this->fields as $name => $data) {
      if (!db_table_exists("field_data_$name")) {
        field_create_field($data + [
          'default' => '',
          'not null' => TRUE,
          'field_name' => $name,
        ]);
      }
    }
  }

  /**
   * Completely delete fields.
   *
   * This function deletes tables: "field_data_NAME" and "field_revision_NAME"
   * and entries in "field_config" and "field_config_instances".
   */
  public function delete() {
    foreach (array_keys($this->fields) as $name) {
      // Delete tables.
      foreach (['data', 'revision'] as $table_type) {
        $table = "field_{$table_type}_{$name}";

        if (db_table_exists($table)) {
          db_drop_table($table);
        }
      }

      // Delete entries.
      foreach (['config', 'config_instance'] as $table_type) {
        db_delete("field_$table_type")
          ->condition('field_name', $name)
          ->execute();
      }
    }
  }

  /**
   * Attach existing fields into entity.
   *
   * @param string $entity_type
   *   Entity machine name.
   * @param string $bundle_name
   *   Entity bundle name.
   *
   * @throws \FieldException
   *   When instance cannot be created.
   */
  public function attach($entity_type, $bundle_name) {
    $attached_fields = field_info_instances($entity_type, $bundle_name);

    foreach ($this->fields as $field_name => $data) {
      if (empty($attached_fields[$field_name]) && field_info_field($field_name)) {
        field_create_instance($data + [
          'bundle' => $bundle_name,
          'field_name' => $field_name,
          'entity_type' => $entity_type,
        ]);
      }
    }
  }

  /**
   * Get field instances.
   *
   * @return array[]
   *   Field instances.
   */
  public function &getInstances() {
    if (empty($this->instances)) {
      $query = db_select('field_config_instance', 'fci')
        ->fields('fci', ['field_name', 'data'])
        ->condition('field_name', array_keys($this->fields))
        ->execute()
        ->fetchAllKeyed();

      $this->instances = array_map('unserialize', $query);
    }

    return $this->instances;
  }

  /**
   * Save field instances.
   *
   * @throws \Exception
   * @throws \InvalidMergeQueryException
   */
  public function saveInstances() {
    foreach ($this->instances as $field_name => $data) {
      db_merge('field_config_instance')
        ->fields(['data' => serialize($data)])
        ->condition('field_name', $field_name)
        ->execute();
    }
  }

}
