<?php
/**
 * @file migrate_parcel.migrate.inc
 * Contains hooks and classes for migrate.
 */

/**
 * Implements hook_migrate_api().
 */
function migrate_parcel_migrate_api() {
  $api = array(
    'api' => 2,
  );
  return $api;
}

/**
 * Our main migration class.
 */
class MigrateExportMigration extends DynamicMigration {

  /**
   * @param $arguments
   *  An array of arguments. Expects:
   *  - machine_name: The machine name to use.
   *  - description: Description of the migration data.
   *  - source_file: The source CSV file.
   *  - entity: The entity type.
   *  - bundle: The bundle machine name.
   *  - mapping_defaults: (optional) An array of default values for mappings,
   *    keyed by destination field, where the value is the default to use.
   *  - dependencies: (optional) An array of migration machine names this
   *    migration depends on.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    $entity_type = $arguments['entity'];
    $bundle_name = $arguments['bundle'];

    parent::__construct();

    // Human-friendly description of this migration process.
    $this->description = $arguments['description'];

    // Get the destination class.
    $destination_class = $this->getDestinationClass($arguments['entity']);

    // Create a map object for tracking the relationships between source rows.
    if ($destination_class == 'MigrateDestinationEntityAPI') {
      $map_key_schema_destination = MigrateDestinationEntityAPI::getKeySchema($entity_type);
    }
    else {
      $map_key_schema_destination = $destination_class::getKeySchema();
    }

    if (isset($arguments['key'])) {
      $key = $arguments['key'];
      $map_key_schema_source = array(
        $key => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => "CSV Mapping",
        ),
      );
    }
    else {
      // If no key is given in the arguments, use the same map schema for both
      // source and destination: assume the CSV file has a column for the entity
      // primary key.
      $map_key_schema_source = $map_key_schema_destination;
    }

    $this->map = new MigrateSQLMap($this->machineName,
      $map_key_schema_source,
      $map_key_schema_destination
    );

    // Unlike normal Migration classes, the source of the data is not hard-coded
    // - it comes from the arguments
    $source_options = array('header_rows' => 1);
    $this->source = new MigrateSourceCSV($arguments['source_file'], array(), $source_options, array());

    // Set up our destination.
    // Parameters are different depending on the class used.
    if ($destination_class == 'MigrateDestinationEntityAPI') {
      $this->destination = new MigrateDestinationEntityAPI(
        $entity_type,
        $bundle_name
      );
    }
    else {
      $this->destination = new $destination_class(
        $bundle_name
      );
    }

    // Declare dependencies.
    if (isset($arguments['dependencies'])) {
      $this->dependencies = $arguments['dependencies'];
    }

    $destination_fields = $this->destination->fields();
    $source_fields = $this->source->fields();
    // Remove the primary key from the source fields so we don't attempt to
    // map into it.
    $entity_info = entity_get_info($entity_type);
    $primary_key = $entity_info['entity keys']['id'];
    unset($source_fields[$primary_key]);

    // The commont fields are the intersection between source and destination.
    $common_fields = array_intersect(array_keys($destination_fields), array_keys($source_fields));
    //dsm($source_fields, 's');
    //dsm($destination_fields, 'd');
    //dsm($common_fields, '$common_fields');

    // Add a mapping for all common fields.
    $this->addSimpleMappings($common_fields);

    if (isset($this->arguments['mapping_defaults'])) {
      foreach ($this->arguments['mapping_defaults'] as $field_name => $default_value) {
        $this->addFieldMapping($field_name, NULL)
          ->defaultValue($default_value)
          ->description(t('Default value from info file.'));
      }
    }
  }

  /**
   * Construct the machine name.
   */
  protected function generateMachineName($class_name = NULL) {
    return $this->arguments['machine_name'];
  }

  /**
   * Get the destination class.
   *
   * Static so it can be called from the export UI.
   *
   * @param $entity_type
   *  The type of entity to get the class for.
   *
   * @return
   *  A class name, either MigrateDestinationEntityAPI or something more
   *  specific if defined.
   */
  static function getDestinationClass($entity_type) {
    $lookup = array(
      'node' => 'MigrateDestinationNode',
      'taxonomy_term' => 'MigrateDestinationTerm',
    );
    if (isset($lookup[$entity_type])) {
      $class = $lookup[$entity_type];
    }
    else {
      $class = 'MigrateDestinationEntityAPI';
    }
    return $class;
  }

  /**
   * Get the destination object.
   *
   * Static so it can be called from the export UI.
   *
   * @param $destination_class
   *  The class to use. Use getDestinationClass() to obtain this.
   * @param $entity_type
   *  The type of entity to get the object for.
   * @param $bundle_name
   *  The bundle name to get the object for.
   *
   * @return
   *  A new destination object which can be set on the Migration class.
   */
  static function getDestinationObject($destination_class, $entity_type, $bundle_name) {
    if ($destination_class == 'MigrateDestinationEntityAPI') {
      $destination_object = new MigrateDestinationEntityAPI(
        $entity_type,
        $bundle_name
      );
    }
    else {
      $destination_object = new $destination_class(
        $bundle_name
      );
    }

    return $destination_object;
  }

}
