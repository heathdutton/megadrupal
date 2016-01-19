<?php 
/**
 * @file.
 * Import the code list for Acivity Collaboration Type.
 */
class CollaborationType extends XMLMigration {
  /**
   * Constructor for the class.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of the Acivity Collaboration Type.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'description' => 'description',
      'language' => 'language',
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/CollaborationType/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/CollaborationType';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('activity_collaboration_type');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName, array(
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
    $this->addFieldMapping('description', 'description')->xpath('description');
    $this->addFieldMapping('language', 'language')->xpath('language');
  }
}
