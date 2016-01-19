<?php 
/**
 * @file.
 * Import the location data for coubtries(level 1).
 */
class LocationLevel1 extends XMLMigration {
  /**
   * Call the class constuctor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports codes for admin boundaries level 1.');
    $this->softDependencies = array('LocationLevel0');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
    );
   // $items_url = 'http://datadev.aidinfolabs.org/data/codelist/Country/version/1.01/lang/en.xml';
     $items_url = str_replace('includes', 'data', dirname(__FILE__)) . '/countrycodelists.xml';
    $item_xpath = '/codelist/Country';  // relative to document
    $item_ID_xpath = 'Country';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('iati_admin_boundaries');
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
    $this->addFieldMapping ('field_level')->defaultValue('2');
    
    
  }
}
