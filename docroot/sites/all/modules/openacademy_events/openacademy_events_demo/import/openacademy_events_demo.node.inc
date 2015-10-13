<?php

/**
 * @file
 * Migrations for Basic Nodes used in Openacademy Events Demo.
 */

class OpenacademyEventsDemoNode extends OpenacademyDemoMigration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import Events nodes.');

    $this->dependencies = array('OpenacademyEventsDemoTerm');

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

    $import_path = drupal_get_path('module', 'openacademy_events_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_events_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1, 'embedded_newlines' => TRUE));

    $this->destination = new MigrateDestinationNode('openacademy_event');

    $this->addFieldMapping('uid')->defaultValue(1);
    $this->addFieldMapping('status')->defaultValue(1);
    $this->addFieldMapping('language')->defaultValue(LANGUAGE_NONE);

    // Title
    $this->addFieldMapping('title', 'title');

    // Image
    $this->addFieldMapping('field_featured_image', 'image');
    $this->addFieldMapping('field_featured_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_featured_image:source_dir', 'source_dir')
      ->defaultValue(drupal_get_path('module', 'openacademy_events_demo') . '/import/images');

    // Image Alt
    $this->addFieldMapping('field_featured_image:alt', 'image_alt');

    // Keywords (tags)
    $this->addFieldMapping('field_tags', 'tags')
       ->separator(', ');
    $this->addFieldMapping('field_tags:create_term')
      ->defaultValue(TRUE);

    // Event type
    $this->addFieldMapping('field_event_type', 'event_type')
       ->separator(', ');

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');
    
    // Event date
    $this->addFieldMapping('field_event_date', 'date');

    // Event address
    $this->addFieldMapping('field_event_address', 'address');
    
    // Event location
    $this->addFieldMapping('field_event_location', 'location');
    $this->addFieldMapping('field_event_location:format')->defaultValue('panopoly_wysiwyg_text');

    // More info
    $this->addFieldMapping('field_event_moreinfo', 'moreinfo');
    $this->addFieldMapping('field_event_moreinfo:title', 'moreinfo_title');
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('image_alt', 'Image alt');
    $columns[3] = array('tags', 'Tags');
    $columns[4] = array('event_type', 'Event type');
    $columns[5] = array('body', 'Body');
    $columns[6] = array('date', 'Date');
    $columns[7] = array('address', 'Address');
    $columns[8] = array('location', 'Location');
    $columns[9] = array('moreinfo', 'Info link');
    $columns[10] = array('moreinfo_title', 'Info link title');
    return $columns;
  }

}
