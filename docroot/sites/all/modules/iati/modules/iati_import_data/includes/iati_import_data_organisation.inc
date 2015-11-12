<?php 
/**
 * @file.
 * Import the data for the organisation.
 */
class DataOrganisation extends XMLMigration {
  /**
   * Call to the class constructor.
   */
  public function __construct(array $arguments) {
    $this->arguments = $arguments;
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports data for the organization.');

    // Instantiate the map.
    $fields = array(
      'iati-identifier' => 'iati-identifier',
      'name' => 'name',
      'default-currency' => 'default-currency'
    );
    //$items_url = 'http://projects.dfid.gov.uk/iati_files/organisation.xml';
//    $items_url = 'http://projects.dfid.gov.uk/iati_files/organisation.xml';
    $item_xpath = '/iati-organisations/iati-organisation';  // relative to document
    $item_ID_xpath = 'iati-identifier';          // relative to item_xpath
  if (isset($arguments['source_file']) && !empty($arguments['source_file'])) {
    	$this->source = new MigrateSourceXML($arguments['source_file'], $item_xpath, $item_ID_xpath, $fields);
    }
    else {
    	$this->source = new MigrateSourceXML("https://www.irishaid.ie/media/irishaid/allwebsitemedia/30whatwedo/iati-12-13-data.xml", $item_xpath, $item_ID_xpath, $fields);
    }
    $this->destination = new MigrateDestinationNode('iati_organisation');
    // Instantiate the map.
    $this->map = new MigrateSQLMap($this->machineName,
    array(
      'iati-identifier' => array(
        'type' => 'varchar',
        'length' => 64,
        'not null' => TRUE,
        'description' => 'iati-identifier',
        'alias' => 'c',
      ),
    ),
    MigrateDestinationNode::getKeySchema()
    );
    // Instantiate the field mapping.
    $this->addFieldMapping('field_iati_identifier', 'iati-identifier')->xpath('iati-identifier');
    $this->addFieldMapping('title', 'name')->xpath('name');
    $this->addFieldMapping('field_iati_default_currency', 'default-currency')->xpath('@default-currency');
  }

  /**
   * Construct the machine name from the source file name.
   */
  /* protected function generateMachineName($class_name = NULL) {
    return 'dataorganisation' . str_replace('-', '', drupal_strtolower(pathinfo($this->arguments['source_file'], PATHINFO_FILENAME)));
  } */
}
