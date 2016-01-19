<?php
/**
 * @file
 * Contains the CSV to SQL Importer.
 */

/**
 * Allows multiple CSV files into a SQL table, to be used for Migration Sources.
 */
class CSVToSQLImporter {
  protected $csvFiles;
  protected $csvColumnsInfo;
  protected $csvSourceOptions = array();
  protected $csvSourceFields = array();
  protected $tableName;
  protected $hashFields;
  protected $additionalSqlColumns = array();
  protected $additionalSqlValues = array();

  protected static $displayFunction;

  /**
   * Constructs an instance to import CSV files into a single SQL table.
   *
   * @param array $csv_files
   *   An array of csv files with the following keys:
   *     path - a stream wrapper string to the CSV file.
   *     name - the name of the csv file with the extension.
   * @param array $csv_columns_info
   *   An array of CSV columns info as requested by the MigrateSourceCSV class.
   * @param string $table_name
   *   A string with the table name where the data should be inserted.
   * @param array $hash_fields
   *   An array of sql field names, which should be used to compute the hash of
   *   a CSV row, to know whether to insert it or not.
   * @param array $arguments
   *   An array of additional parameters with the following keys:
   *     csv_source_options - an array of options to send to the
   *       MigrateSourceCSV constructor.
   *     csv_source_fields - an array of fields to send to the
   *       MigrateSourceCSV constructor.
   *     additional_sql_columns - a list of additional sql columns to add into
   *       the DB table.
   *     additional_sql_values - a list of additional static sql values to add
   *       into the the DB table.
   *     displayFunction - a function name to display error messages.
   */
  public function __construct(
    array $csv_files,
    array $csv_columns_info,
    $table_name,
    array $hash_fields,
    $arguments = array()
  ) {

    $this->csvFiles = $csv_files;
    $this->csvColumnsInfo = $csv_columns_info;
    $this->tableName = $table_name;
    $this->hashFields = $hash_fields;

    if (isset($arguments['csv_source_options'])) {
      $this->csvSourceOptions = $arguments['csv_source_options'];
    }

    if (isset($arguments['csv_source_fields'])) {
      $this->csvSourceFields = $arguments['csv_source_fields'];
    }

    if (isset($arguments['additional_sql_columns'])) {
      $this->additionalSqlColumns = $arguments['additional_sql_columns'];
    }

    if (isset($arguments['additional_sql_values'])) {
      $this->additionalSqlValues = $arguments['additional_sql_values'];
    }

    // Display function copied from Migration class.
    self::$displayFunction = 'drupal_set_message';
    if (isset($arguments['displayFunction'])) {
      self::$displayFunction = $arguments['displayFunction'];
    }
  }

  /**
   * Imports the CSV files into the given SQL table.
   *
   * @throws Exception
   */
  public function importCSVToSQL() {
    // Go through each given csv file.
    foreach ($this->csvFiles as $csv_file) {
      // Instantiate the CSV source.
      $csv_source = new MigrateSourceCSVOnSteroids(
        $csv_file['path'],
        $this->csvColumnsInfo(),
        $this->csvSourceOptions,
        $this->csvSourceFields
      );

      try {
        // Start reading the CSV source from the start.
        $csv_source->performRewind();
      }
      catch (Exception $e) {
        self::displayMessage(
          t('Migration failed with source plugin exception: %e, in %file:%line',
            array(
              '%e' => $e->getMessage(),
              '%file' => $e->getFile(),
              '%line' => $e->getLine(),
            )
          )
        );
        throw $e;
      }

      // Wrap everything into a transaction, in case of an exception.
      $transaction = db_transaction();
      try {

        // Prepare to insert a row of CSV data.
        $insert = db_insert($this->tableName());
        $columns = $this->prepareSqlFields();
        $insert->fields($columns);

        // Get rows one by one.
        while ($data_row = $csv_source->getNextRow()) {
          $data_row_array = (array) $data_row;

          // Preprocess the CSV row values.
          $data_row_array = $this->preprocessSqlValues($data_row_array, $csv_file['name']);

          // Check whether the CSV row already exists in the DB.
          $hash_exists = $this->hashExists($data_row_array['hash']);

          // If not, insert the CSV row into the DB.
          if (!$hash_exists) {
            $insert->values($data_row_array)->execute();
          }
        }
      }
      catch (Exception $e) {
        self::displayMessage(
          t('Migration failed with source plugin exception: %e, in %file:%line',
            array(
              '%e' => $e->getMessage(),
              '%file' => $e->getFile(),
              '%line' => $e->getLine(),
            )
          )
        );

        $transaction->rollback();
        throw $e;
      }
    }
  }

  /**
   * Checks whether a row exists in the DB with the given hash.
   */
  public function hashExists($hash) {
    // Check whether the hash is present in the DB.
    $result = db_select($this->tableName(), 'l')
      ->fields('l', array('hash'))
      ->condition('hash', $hash)
      ->execute()
      ->fetchField();
    return $result;
  }

  /**
   * Adds additional sql values to the insertion query.
   *
   * Also computes a hash values based on a set of sql fields.
   */
  public function preprocessSqlValues($values, $csv_file_name) {
    // Add the csv file name as a column.
    $values['file_name'] = $csv_file_name;

    foreach ($this->additionalSqlValues as $key => $additional_value) {
      $values[$key] = $additional_value;
    }

    // Compute hash of the csv row.
    $relevant_hash_values = array();
    foreach ($this->hashFields as $hash_field) {
      $relevant_hash_values[] = $values[$hash_field];
    }
    $values['hash'] = md5(serialize(($relevant_hash_values)));

    return $values;
  }

  /**
   * Returns a list of SQL fields to be used for insertion into the SQL table.
   */
  public function prepareSqlFields() {
    $column_names = $this->csvColumnsNames();
    $columns = array();

    foreach ($this->additionalSqlColumns as $additional_column) {
      $columns[] = $additional_column;
    }
    $columns[] = 'file_name';
    $columns[] = 'hash';
    $columns[] = 'csvrownum';

    foreach ($column_names as $column_name) {
      $columns[] = $column_name;
    }

    return $columns;
  }

  /**
   * Returns info about the CSV columns.
   */
  public function csvColumnsInfo() {
    return $this->csvColumnsInfo;
  }

  /**
   * Returns just the CSV column names.
   */
  public function csvColumnsNames() {
    $columns_info = $this->csvColumnsInfo();
    $columns = array();
    foreach ($columns_info as $column) {
      $columns[] = $column[0];
    }
    return $columns;
  }

  /**
   * Returns table name where to insert the CSV data.
   */
  public function tableName() {
    return $this->tableName;
  }

  /**
   * Output the given message appropriately (drush_print/drupal_set_message).
   *
   * @param string $message
   *   The message to output.
   * @param int $level
   *   Optional message severity as understood by drupal_set_message and
   *   drush_log
   *   (defaults to 'error').
   */
  static public function displayMessage($message, $level = 'error') {
    call_user_func(self::$displayFunction, $message, $level);
  }

}
