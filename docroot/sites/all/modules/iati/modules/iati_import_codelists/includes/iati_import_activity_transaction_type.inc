<?php 
/**
 * Import code lists for Transaction type
 */
class TransactionType extends XMLMigration {
 /**
  * Call to the class constructor.
  */
  public function __construct() {
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports terms of Transaction Type.');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'description' => 'description',
    );
    $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/TransactionType/version/1.01/lang/en.xml';
    $item_xpath = '/codelist/TransactionType';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('transaction_type');
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
  }
}