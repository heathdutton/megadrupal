<?php
/**
 * @file.
 */
class OrganisationRole extends XMLMigration {
  /**
   * Invoke the class constructor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of type Organisation Role.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'description' => 'description',
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/OrganisationRole/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/OrganisationRole';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('iati_roles');
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
    $this->addFieldMapping('description', 'description')->xpath('description');
    $this->addFieldMapping('name', 'name')->xpath('name');
  }
}
