<?php 
/**
 * Import codelists for Organization Identifiers
 */
class OrganizationIdentifiers extends XMLMigration {
  /**
   * Instantiate the constructor
   */
  public function __construct() {
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports terms for Organization Identifiers.');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'abbreviation' => 'abbreviation',
    );
    $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/OrganisationIdentifier/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/OrganisationIdentifier';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('organisation_identifiers');
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
    $this->addFieldMapping('field_abbreviation', 'abbreviation')->xpath('abbreviation');
  }
}