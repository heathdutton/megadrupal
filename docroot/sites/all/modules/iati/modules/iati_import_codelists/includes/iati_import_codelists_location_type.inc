<?php 
/**
 * @file.
 * Import the code list for location type.
 */
class Locationtype extends XMLMigration {
  /**
   * Call the class constuctor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports codelists for the location.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
    );
    $items_url = array (
    		'http://data.aidinfolabs.org/data/codelist/LocationType/version/1.0/lang/en.xml',
    		str_replace('includes', 'data', dirname(__FILE__)) . '/location_type_continent.xml',
    );
    $item_xpath = '/codelist/LocationType';  // relative to document
    $item_ID_xpath = 'LocationType';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('location_type');
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
    $this->addFieldMapping('field_iati_location', 'code')->xpath('code');
    $this->addFieldMapping('name', 'name')->xpath('name');
  }
}
