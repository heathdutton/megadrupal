<?php

/**
 * @file
 * Base Migrations for Taxonomy Terms used in Open Academy Core Demo.
 */

class OpenacademyCoreDemoSpotlight extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import spotlight items.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uuid' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationEntityAPI::getKeySchema('fieldable_panels_pane', 'spotlight')
    );

    $import_path = drupal_get_path('module', 'openacademy_core_demo') . '/import/data/';
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_core_demo.spotlight.csv', $this->csvcolumns(), array('header_rows' => 1));
    $this->destination = new MigrateDestinationEntityAPI('fieldable_panels_pane', 'spotlight');

    $this->addFieldMapping('title', 'title');

    $this->addFieldMapping('field_basic_spotlight_items', 'title');
    $this->addFieldMapping('field_basic_spotlight_items:description', 'description');
    $this->addFieldMapping('field_basic_spotlight_items:fid', 'image');
    $this->addFieldMapping('field_basic_spotlight_items:link', 'link');
  }

  protected function csvcolumns() {
    $columns[0] = array('uuid', 'UUID');
    $columns[1] = array('title', 'Title');
    $columns[2] = array('description', 'Description');
    $columns[3] = array('image', 'Image');
    $columns[4] = array('link', 'Link');
    return $columns;
  }

  public function prepareRow($current_row) {
    dsm($current_row);
    return TRUE;
  }
  
}
