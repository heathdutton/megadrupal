<?php

/**
 * @file
 * Migrations for Basic Nodes used in Openacademy People Demo.
 */

class OpenacademyPeopleDemoNode extends OpenacademyDemoMigration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import People nodes.');

    $this->dependencies = array('OpenacademyPeopleDemoTermType', 'OpenacademyPeopleDemoTermTags');

    // if (module_exists('openacademy_publications_demo')) {
    //   $this->dependencies[] = 'OpenacademyPublicationsDemoNode';
    // }
    // 
    // if (module_exists('openacademy_courses_demo')) {
    //   $this->dependencies[] = 'OpenacademyCoursesDemoNode';
    // }
    
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

    $import_path = drupal_get_path('module', 'openacademy_people_demo') . '/import/data/';

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_people_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1, 'embedded_newlines' => TRUE));

    $this->destination = new MigrateDestinationNode('openacademy_person');

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
      ->defaultValue(drupal_get_path('module', 'openacademy_people_demo') . '/import/images');

    // Image Alt
    $this->addFieldMapping('field_featured_image:alt', 'image_alt');

    // Person type
    $this->addFieldMapping('field_person_type', 'person_type')
       ->separator(', ');

    // Keywords (tags)
    $this->addFieldMapping('field_tags', 'keywords')
       ->separator(', ');
    $this->addFieldMapping('field_tags:create_term')
      ->defaultValue(TRUE);

    // Biography
    $this->addFieldMapping('body', 'biography');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');

    // Position
    $this->addFieldMapping('field_person_position', 'position');

    // Publications
    if (module_exists('openacademy_publications_demo')) {
      $this->addFieldMapping('field_person_publications', 'publications')
        ->separator(', ')
        ->sourceMigration('OpenacademyPublicationsDemoNode');
    }

    // Courses
    if (module_exists('openacademy_courses_demo')) {
      $this->addFieldMapping('field_person_courses', 'courses')
        ->separator(', ')
        ->sourceMigration('OpenacademyCoursesDemoNode');
    }

    // Email
    $this->addFieldMapping('field_person_email', 'email');

    // Office
    $this->addFieldMapping('field_person_office', 'office');

    // Phone
    $this->addFieldMapping('field_person_phone', 'phone');

    // Website
    $this->addFieldMapping('field_person_website', 'website');
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('image_alt', 'Image alt');
    $columns[3] = array('person_type', 'Person type');
    $columns[4] = array('keywords', 'Keywords');
    $columns[5] = array('biography', 'Biography');
    $columns[6] = array('position', 'Position');
    $columns[7] = array('publications', 'Publications');
    $columns[8] = array('courses', 'Courses');
    $columns[9] = array('email', 'Email');
    $columns[10] = array('office', 'Office');
    $columns[11] = array('phone', 'Phone');
    $columns[12] = array('website', 'Website');
    return $columns;
  }

}
