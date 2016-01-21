<?php
/**
 * @file.
 */
class FinancialTypeLevel1 extends XMLMigration {
  /**
   * Invoke the class constructor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of Financial Type leve 1.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'parent' => 'parent'
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/FinanceType/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/FinanceType';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('activity_finance_type');
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
    $this->addFieldMapping('parent', 'parent');
    // $this->addFieldMapping('category', 'category')->xpath('category');
  }
  /**
   * Construct the machine name from the source file name.
   */
  public function prepareRow($row) {
    if (!empty($row->xml->code) && !empty($row->xml->category)) {
      $term = db_select(str_replace('1', '0', $this->getMap()->getMapTable()), 'mt')
      ->fields('mt', array('destid1'))
      ->condition('sourceid1', $row->xml->category)
      ->execute()
      ->fetchAssoc();
      if ($term === FALSE) {
        drupal_set_message(t('Could not find the top level sectors. Please load them before loading the sub-sectors.'));
        return FALSE;
      }
      $row->parent = $term['destid1'];
    }
    else {
      drupal_set_message(t('There is no sub-sector provided.'));
      return FALSE;
    }
  }
}
