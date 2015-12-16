<?php

/**
 * @file
 * Migrations for Basic Nodes.
 */

class PanopolyEventDemoNode extends Migration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import nodes.');

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

    $import_path = drupal_get_path('module', 'panopoly_event_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'panopoly_event_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode('panopoly_event');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('field_featured_image', 'image');
    $this->addFieldMapping('field_featured_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_featured_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'panopoly_event_demo') . '/import/images');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');
    $this->addFieldMapping('field_featured_status', 'featured');
    $this->addFieldMapping('created', 'created');
    //"Date","DateEnd","Registration","Paid"
    $this->addFieldMapping('field_date', 'date');
    $this->addFieldMapping('field_date:to', 'dateend');
    $this->addFieldMapping('field_registration', 'registration');
    //$this->addFieldMapping('field_paid_event', 'paid');

  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('body', 'Body');
    $columns[3] = array('featured', 'Featured');
    $columns[4] = array('created', 'Created');
    //"Date","DateEnd","Registration","Paid"
    $columns[5] = array('date', 'Date');
    $columns[6] = array('dateend', 'DateEnd');
    $columns[7] = array('registration', 'Registration');
    // $columns[8] = array('paid', 'Paid');
    return $columns;
  }

}
