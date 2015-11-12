<?php 
/**
 * Import codelists for Sector
 */
class SectorCodeLists extends XMLMigration {
  /**
   * Instantiate the constructor
   */
  public function __construct() {
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports terms for Sector.');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'description' => 'description',
      'category' => 'category',
      'category-name' => 'category-name',
      'category-description' => 'category-description',
    );
    $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/Sector/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/Sector';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('sector');
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
    $this->addFieldMapping('description', 'description')->xpath('description');
    $this->addFieldMapping('field_category', 'category')->xpath('category');
    $this->addFieldMapping('field_category_name', 'category-name')->xpath('category-name');
    $this->addFieldMapping('field_category_description', 'field_category_description')->xpath('category-description');
  }
}