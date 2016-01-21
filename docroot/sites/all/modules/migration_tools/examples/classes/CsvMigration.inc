<?php

/**
 * @file
 * Defines CsvMigration class.
 */

/**
 * Class CsvMigration.
 *
 * Parent class for all migrations with CSV data source.
 *
 * @package migration_tools
 */
abstract class CsvMigration extends MTBaseMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_file, $options = array()) {
    parent::__construct($arguments);
    ini_set('auto_detect_line_endings', TRUE);

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceCSV($source_file, $this->csvcolumns(), $options, $this->fields());
  }

  /**
   * Returns an array of columns.
   *
   * @return array
   *   A multi dimensional array keyed by number,
   *   e.g., $col[0] = array('machine_name' => 'Title');
   */
  abstract public function csvcolumns();

  /**
   * Returns an array of fields.
   *
   * @return array
   *   An array of field titles, keyed by field machine name.
   */
  abstract public function fields();
}

/**
 * Class MTCsvToNodeMigration.
 *
 * Parent class for all migrations with CSV data source.
 *
 * @package migration_tools
 */
abstract class MTCsvToNodeMigration extends CsvMigration {

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments, $source_file, $options = array()) {
    parent::__construct($arguments, $source_file, $options);
  }

  /**
   * {@inheritdoc}
   */
  public function prepare($entity, $row) {
    migration_tools_prepare_workbench_settings($entity, $row);
  }

  /**
   * {@inheritdoc}
   */
  public function complete($entity, $row) {
    if (isset($entity->nid) && $entity->nid) {
      migration_tools_complete_workbench_settings($entity, $row);
    }
  }

}
