<?php
/**
 * @file.
 */
class SectorLevel0 extends XMLMigration {
  /**
   * Invoke the class constructor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of Activity Sector 0.');

    // Instantiate the map.
    $fields = array(
      'category' => 'category',
      'category-name' => 'category-name',
      'category-description' => 'category-description',
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/Sector/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/Sector';  // relative to document
    $item_ID_xpath = 'category';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('iati_activity_sector');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'category' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'category',
          'alias' => 'c',
        ),
      ),
    MigrateDestinationTerm::getKeySchema()
    );
    // Instantiate the field mapping.
    $this->addFieldMapping('field_iati_code', 'category')->xpath('category');
    $this->addFieldMapping('name', 'category-name')->xpath('category-name');
    $this->addFieldMapping('description', 'category-description')->xpath('category-description');
  }
}
