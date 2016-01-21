<?php
/**
 * @file
 * Migration classes.
 */

/**
 * Implements hook_migrate_api().
 */
function migrate_csv_to_sql_example_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      MIGRATE_CSV_TO_SQL_EXAMPLE_MIGRATION_GROUP_NAME => array(
        'title' => t('Humans'),
      ),
    ),
  );
  return $api;
}

/**
 * An example base migration that uses the CSVToSqlImporter.
 */
class BaseExampleHumanMigration extends Migration {
  /**
   * @var CSVToSQLImporter
   */
  protected $csvToSqlInstance;
  /**
   * @var array
   */
  protected $csvFiles;

  /**
   * Constructor for base migration.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    // Avoid known line ending issue: "Invalid data value" at
    // drupal.org/node/1152158#InvalidDataValue.
    ini_set('auto_detect_line_endings', TRUE);

    $this->csvFiles = $arguments['csv_files'];

    // We instantiate a CSVToSqlImporter object, with settings where the CSV
    // file data should be imported.
    $this->initCSVtoSQLImporter();
  }

  /**
   * Initialize the CSV to SQL Importer.
   */
  public function initCSVtoSQLImporter() {
    // Get the CSV columns.
    $csv_columns = migrate_csv_to_sql_example_csv_columns();

    // Used to compute a hash, which is checked for uniqueness in the table.
    // In this example, a person with the same name and surname is considered
    // unique.
    $hash_fields = array(
      'name',
      'surname',
    );

    $table_name = MIGRATE_CSV_TO_SQL_EXAMPLE_HUMAN_TABLE_NAME;

    // For the sake of presentation, we use an additional static value field
    // stored in the SQL table generated for the CSV.
    $additional_sql_columns = array(
      MIGRATE_CSV_TO_SQL_EXAMPLE_ADDITIONAL_GENDER_FIELD_NAME,
    );

    $additional_sql_values = array(
      MIGRATE_CSV_TO_SQL_EXAMPLE_ADDITIONAL_GENDER_FIELD_NAME => 'Unknown',
    );

    // The filter predicate is used to skip importing of certain CSV rows, based
    // on certain conditions.
    // This will work only for PHP 5.3+.
    // In our case we consider that any row that has an empty name or surname
    // should be skipped. This way we will skip the completely empty rows
    // and the bogus Total count rows.
    $filter_predicate = function ($row) {
      return empty($row['name']) || empty($row['surname']);
    };

    // We have to skip the first line containing the header columns.
    $csv_source_options = array(
      'header_rows' => 1,
      'filter_predicate' => $filter_predicate,
    );

    $arguments = array(
      'csv_source_options' => $csv_source_options,
      'additional_sql_columns' => $additional_sql_columns,
      'additional_sql_values'  => $additional_sql_values,
    );

    $this->csvToSqlInstance = new CSVToSQLImporter(
      $this->csvFiles(),
      $csv_columns,
      $table_name,
      $hash_fields,
      $arguments
    );
  }

  /**
   * Updates the source count by running the CSV to SQL importer.
   *
   * We usually call this in the constructor of the Migration class.
   *
   * @throws Exception
   */
  public function updateSourceCount() {
    // If no count has been cached yet, import CSV into the DB.
    $source = $this->source;
    /* @var $source MigrateSourceSQLOnSteroids */
    if (!$source->countIsCached()) {
      $this->csvToSqlInstance->importCSVToSQL();
    }
  }

  /**
   * Import the CSV data to the SQL table, before doing the actual migration.
   *
   * Every time before the migration is ran, we import the CSV data into the DB
   * to make sure we have the latest data for migration.
   *
   * @throws Exception
   */
  protected function preImport() {
    parent::preImport();

    // Before importing sync all CSV data into the SQL tables.
    $this->csvToSqlInstance->importCSVToSQL();
  }

  /**
   * Returns the array of CSV files.
   */
  public function csvFiles() {
    return $this->csvFiles;
  }
}

/**
 * Example human migration class.
 */
class ExampleHumanMigration extends BaseExampleHumanMigration {
  /**
   * Constructs migration instance.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('Import human data.');

    $bundle = MIGRATE_CSV_TO_SQL_EXAMPLE_HUMAN_NODE_TYPE;

    // Select the columns we want to use for migration, from the DB table
    // containing the CSV data.
    $columns = array(
      'name',
      'surname',
      'age',
      'children_count',
      'gender',
    );

    // Because the imported data was denormalized (people can own multiple pets)
    // we want to group by the rest of the fields, so we don't get duplicates.
    // This is why using a SQL source is better than a raw CSV file, we get the
    // full power of SQL queries to manipulate our source data.
    $query
      = db_select(MIGRATE_CSV_TO_SQL_EXAMPLE_HUMAN_TABLE_NAME, 'h')
      ->fields('h', $columns)
      ->groupBy('name')
      ->groupBy('surname')
      ->groupBy('age')
      ->groupBy('children_count')
      ->groupBy('gender')
      ->groupBy('title');

    // Select only the first pet the human owns.
    $query->addExpression('MIN(h.owns)', 'first_owned');

    // Build a title from the name and surname, to be used as the node title.
    $query->addExpression("CONCAT_WS(' ', name, surname)", 'title');

    // Wrap the top query as a sub-query, so that it is map joinable.
    $additional_columns = array('first_owned', 'title');
    $wrapping_columns = array_merge($columns, $additional_columns);
    $wrapping_query = db_select($query, 'x')->fields('x', $wrapping_columns);

    // Create the source object.
    $this->source = new MigrateSourceSQLOnSteroids($wrapping_query);

    // Update source count because data might not have been imported from CSV
    // to SQL yet. This uses a method implemented only in the derived class,
    // which is why we use it.
    $this->updateSourceCount();

    // We migrate into nodes.
    $this->destination = new MigrateDestinationNode($bundle);

    // The unique key in our case will be the name and surname.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => t('Name'),
        ),
        'surname' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => t('Surname'),
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // Mapped fields.
    $this->addFieldMapping('title', 'title')
      ->defaultValue('')
      ->description(t('First Name and Surname'));

    $this->addFieldMapping('field_human_first_name', 'name')
      ->defaultValue('')
      ->description(t('Name'));

    $this->addFieldMapping('field_human_last_name', 'surname')
      ->defaultValue('')
      ->description(t('Surname'));

    $this->addFieldMapping('field_human_age', 'age')
      ->defaultValue('0')
      ->description(t('Age'));

    $this->addFieldMapping('field_human_owned_pets', 'first_owned')
      ->defaultValue('')
      ->description(t('Owned pets'));

    $this->addFieldMapping('field_human_child_count', 'children_count')
      ->defaultValue('0')
      ->description(t('Child count'));

    $this->addFieldMapping('field_human_gender', 'gender')
      ->defaultValue('Unknown')
      ->description(t('Gender'));
  }
}
