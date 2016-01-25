<?php 
/**
 * Import codelists for Loan Repayment Period
 */
class LoanRepaymentPeriod extends XMLMigration {
  /**
   * Instantiate the constructor
   */
  public function __construct() {
    parent::__construct();
    // Do some general administration.
    $this->description = t('Imports terms for Loan Repayment Period.');
    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
    );
    $items_url = 'http://codelists103.archive.iatistandard.org/data/codelist/LoanRepaymentPeriod/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/LoanRepaymentPeriod';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('loan_repayment_period');
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
  }
}