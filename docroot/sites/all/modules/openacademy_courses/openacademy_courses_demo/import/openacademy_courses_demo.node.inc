<?php

/**
 * @file
 * Migrations for Basic Nodes used in Openacademy Courses Demo.
 */

class OpenacademyCoursesDemoNode extends OpenacademyDemoMigration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import Courses nodes.');

    $this->dependencies = array('OpenacademyCoursesDemoTerm');

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

    $import_path = drupal_get_path('module', 'openacademy_courses_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_courses_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1, 'embedded_newlines' => TRUE));

    $this->destination = new MigrateDestinationNode('openacademy_course');

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
      ->defaultValue(drupal_get_path('module', 'openacademy_courses_demo') . '/import/images');

    // Image Alt
    $this->addFieldMapping('field_featured_image:alt', 'image_alt');

    // Course type
    $this->addFieldMapping('field_course_type', 'course_type')
       ->separator(', ');
    $this->addFieldMapping('field_course_type:create_term')
      ->defaultValue(TRUE);

    // Course term
    $this->addFieldMapping('field_course_term', 'course_term')
       ->separator(', ');
    $this->addFieldMapping('field_course_term:create_term')
      ->defaultValue(TRUE);

    // Body
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');

    // Location
    $this->addFieldMapping('field_course_location', 'location');
    
    // Course credits
    $this->addFieldMapping('field_course_credits', 'credits');
    
    // Course number
    $this->addFieldMapping('field_course_number', 'number');
    
    // Course time
    // TODO: We need a field handler for office_hours.
    //$this->addFieldMapping('field_course_time', 'time');
    
    // People
    if (module_exists('openacademy_people_demo')) {
      $this->addFieldMapping('field_course_people', 'people')
        ->separator(', ')
        ->sourceMigration('OpenacademyPeopleDemoNode');
    }
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('image_alt', 'Image alt');
    $columns[3] = array('course_type', 'Course type');
    $columns[4] = array('course_term', 'Course term');
    $columns[5] = array('body', 'Body');
    $columns[6] = array('location', 'Location');
    $columns[7] = array('credits', 'Course credits');
    $columns[8] = array('number', 'Course number');
    //$columns[9] = array('time', 'Course time');
    $columns[9] = array('people', 'People');
    return $columns;
  }

}
