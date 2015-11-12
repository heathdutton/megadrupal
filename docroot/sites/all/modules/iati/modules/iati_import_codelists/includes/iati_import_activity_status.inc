<?php 
/**
 * @file.
 * Import the code list for activity aid type.
 */
class ActivityStatus extends XMLMigration {
  /**
   * Invoke the class constructor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of activity status.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'language' => 'language',
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/ActivityStatus/version/1.0/lang/en.xmlv.xml';
    $item_xpath = '/codelist/ActivityStatus';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('activity_status');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'code' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'code',
          'alias' => 'c',
        ),
      ),
    MigrateDestinationTerm::getKeySchema()
    );
    // Instantiate the field mapping.
    $this->addFieldMapping('field_iati_code', 'code')->xpath('code');
    $this->addFieldMapping('name', 'name')->xpath('name');
    $this->addFieldMapping('field_langauge', 'language')->xpath('language');
  }
}
