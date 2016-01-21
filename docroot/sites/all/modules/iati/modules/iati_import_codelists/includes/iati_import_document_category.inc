<?php 
/**
 * Import codelists for Document Category
 */
class DocumentCategory extends XMLMigration {
  /**
   * Instantiate the constructor
   */
  public function __construct() {
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports terms for Activity Document Category.');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'category' => 'category',
      'category-name' => 'category-name',
    );
    $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/DocumentCategory/version/1.02/lang/en.xml';
    $item_xpath = '/codelist/DocumentCategory';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('document_category');
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
    $this->addFieldMapping('field_category', 'category')->xpath('category');
    $this->addFieldMapping('field_category_name', 'category-name')->xpath('category-name');
  }
}