<?php

/**
 * @file
 * Migrations for Basic Nodes used in Openacademy News Demo.
 */

class OpenacademyNewsDemoNode extends OpenacademyDemoMigration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import News nodes.');

    $this->dependencies = array('OpenacademyNewsDemoTerm');
    
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

    $import_path = drupal_get_path('module', 'openacademy_news_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_news_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1, 'embedded_newlines' => TRUE));

    $this->destination = new MigrateDestinationNode('openacademy_news');

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
      ->defaultValue(drupal_get_path('module', 'openacademy_news_demo') . '/import/images');

    // Image Alt
    $this->addFieldMapping('field_featured_image:alt', 'image_alt');

    // Keywords (tags)
    $this->addFieldMapping('field_tags', 'tags')
       ->separator(', ');
    $this->addFieldMapping('field_tags:create_term')
      ->defaultValue(TRUE);

    // Date
    $this->addFieldMapping('created', 'date');

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('image_alt', 'Image alt');
    $columns[3] = array('tags', 'Tags');
    $columns[4] = array('date', 'Date');
    $columns[5] = array('body', 'Body');
    return $columns;
  }

}
