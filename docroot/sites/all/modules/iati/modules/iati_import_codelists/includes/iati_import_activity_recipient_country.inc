<?php
/**
 * Imports codelists for recipient country
 */
class RecipientCountry extends XMLMigration {
 /**
  * Call to the class constructor.
  */
 public function __construct() {
  parent::__construct();
  // Do some general administration.
  $this->description = t('Imports terms for the recipient country.');

  // Instantiate the map.
  $fields = array(
    'code' => 'code',
    'name' => 'name',
    'language' => 'language',
  );
  $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/Country/version/1.01/lang/en.xml';
  $item_xpath = '/codelist/Country';  // relative to document
  $item_ID_xpath = 'code';          // relative to item_xpath
  $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
  $this->destination = new MigrateDestinationTerm('activity_recipient_country');
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
  $this->addFieldMapping('field_langauge', 'langauge')->xpath('language');
  $this->addFieldMapping('name', 'name')->xpath('name');
  $this->addFieldMapping('field_iati_code', 'field_iati_code')->xpath('description');
 }
}