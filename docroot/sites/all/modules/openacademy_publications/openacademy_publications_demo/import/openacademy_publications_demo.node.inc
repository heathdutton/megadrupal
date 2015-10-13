<?php

/**
 * @file
 * Migrations for Basic Nodes used in Openacademy Publications Demo.
 */

class OpenacademyPublicationsDemoNode extends OpenacademyDemoMigration {

  public function __construct($arguments = array()) {
    parent::__construct($arguments = array());
    $this->description = t('Import Publication nodes.');

    $this->dependencies = array('OpenacademyPublicationsDemoTerm', 'OpenacademyPublicationsDemoBiblio');
    
    //if (module_exists('openacademy_people_demo')) {
    //  $this->dependencies[] = 'OpenacademyPeopleDemoNode';
    //}
    
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
    $this->source = new MigrateSourceCSV($import_path . 'openacademy_publications_demo.node.csv', $this->csvcolumns(), array('header_rows' => 1, 'embedded_newlines' => TRUE));

    $this->destination = new MigrateDestinationNode('openacademy_publication');

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
      ->defaultValue(drupal_get_path('module', 'openacademy_publications_demo') . '/import/images');

    // Image Alt
    $this->addFieldMapping('field_featured_image:alt', 'image_alt');

    // Categories (tags)
    $this->addFieldMapping('field_tags', 'tags')
       ->separator(', ');
    $this->addFieldMapping('field_tags:create_term')
      ->defaultValue(TRUE);

    // Action
    $this->addFieldMapping('field_publication_action', 'action');

    // Action title
    $this->addFieldMapping('field_publication_action:title', 'action_title');

    // Citation
    $this->addFieldMapping('field_publication_citation', 'citation')
      ->sourceMigration('OpenacademyPublicationsDemoBiblio');

    // Description
    $this->addFieldMapping('body', 'description');
    $this->addFieldMapping('body:format')->defaultValue('panopoly_wysiwyg_text');

    // Author
    if (module_exists('openacademy_people_demo')) {
      $this->addFieldMapping('field_publication_author', 'author')
        ->sourceMigration('OpenacademyPeopleDemoNode');
    }

    // Publisher
    $this->addFieldMapping('field_publication_publisher', 'publisher');

    // Published date
    $this->addFieldMapping('field_publication_published', 'published_date');
  }

  protected function csvcolumns() {
    $columns[0] = array('title', 'Title');
    $columns[1] = array('image', 'Image');
    $columns[2] = array('image_alt', 'Image alt');
    $columns[3] = array('tags', 'Tags');
    $columns[4] = array('action', 'Action');
    $columns[5] = array('action_title', 'Action title');
    $columns[6] = array('citation', 'Citation');
    $columns[7] = array('description', 'Description');
    $columns[8] = array('author', 'Author');
    $columns[9] = array('publisher', 'Publisher');
    $columns[10] = array('published_date', 'Published date');
    return $columns;
  }

}
