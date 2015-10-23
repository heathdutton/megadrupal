<?php

/**
 * @file
 * Base Migrations for Taxonomy Terms used in Open Academy People Demo.
 */

class OpenacademyPeopleDemoTermType extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import taxonomy terms.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $import_path = drupal_get_path('module', 'openacademy_people_demo') . '/import/data/';
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_people_demo.term.type.csv', $this->csvcolumns(), array('header_rows' => 1));
    $this->destination = new MigrateDestinationTerm('openacademy_person_type');
    $this->addFieldMapping('name', 'name');
  }

  protected function csvcolumns() {
    $columns[0] = array('name', 'Name');
    return $columns;
  }
}


class OpenacademyPeopleDemoTermTags extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import taxonomy terms.');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationTerm::getKeySchema()
    );

    $import_path = drupal_get_path('module', 'openacademy_people_demo') . '/import/data/';
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_people_demo.term.tags.csv', $this->csvcolumns(), array('header_rows' => 1));
    $this->destination = new MigrateDestinationTerm('openacademy_tags');
    $this->addFieldMapping('name', 'name');
  }

  protected function csvcolumns() {
    $columns[0] = array('name', 'Name');
    return $columns;
  }
}
