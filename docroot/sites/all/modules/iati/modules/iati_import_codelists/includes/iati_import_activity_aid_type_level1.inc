<?php
/**
 * @file.
 */
class AidTypeLevel1 extends XMLMigration {
  /**
   * Invoke the class constructor.
   */
  public function __construct() {
    parent::__construct();

    // Do some general administration.
    $this->description = t('Imports terms of Aid Type level 1.');

    // Instantiate the map.
    $fields = array(
      'code' => 'code',
      'name' => 'name',
      'description' => 'description',
      'parent' => 'parent',
    );
    $items_url = 'http://data.aidinfolabs.org/data/codelist/AidType/version/1.0/lang/en.xml';
    $item_xpath = '/codelist/AidType';  // relative to document
    $item_ID_xpath = 'code';          // relative to item_xpath
    $this->source = new MigrateSourceXML($items_url, $item_xpath, $item_ID_xpath, $fields);
    $this->destination = new MigrateDestinationTerm('activity_aid_type');
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
    $this->addFieldMapping('parent', 'parent');
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
