<?php

/**
 * @file
 * Migrations for Biblio content used in Openacademy Publications Demo.
 */

class OpenacademyPublicationsDemoBiblio extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import Publication biblio nodes.');

    // Create a map object for tracking the relationships between source rows.
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $import_path = drupal_get_path('module', 'openacademy_publications_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_publications_demo.biblio.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode('biblio');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);
    
    // Title
    $this->addFieldMapping('title', 'title');
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('biblio_year', 'Year');
    $columns[2] = array('biblio_type', 'Type');
    $columns[3] = array('biblio_publisher', 'Publisher');
    return $columns;
  }

}
