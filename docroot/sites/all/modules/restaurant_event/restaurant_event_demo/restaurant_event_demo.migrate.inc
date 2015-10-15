<?php
/**
 * @file
 * Migration file for Restaurant Event module
 */

/**
 * Implements hook_migrate_api().
 */
function restaurant_event_demo_migrate_api() {
  return array(
    'api' => 2,
    'field handlers' => array(
      'DateMigrateFieldHandler',
    ),
    'migrations' => array(
      'RestaurantEventMigrateEvent' => array(
        'class_name' => 'RestaurantEventMigrateEventMigration',
      ),
    ),
  );
}

// Migration class
class RestaurantEventMigrateEventMigration extends Migration {

  public function __construct() {
    parent::__construct();
    $this->description = t('Import event nodes.');

    // Create a map object for tracking the relationships between source rows
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

    // Create a MigrateSource object.
    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'restaurant_event_demo') . '/import/events.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode('event', array('text_format' => 'panopoly_html_text'));

    // Fields Mapping
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');

    $this->addFieldMapping('field_event_categories', 'category')->separator(', ');
    $this->addFieldMapping('field_event_categories:create_term')
      ->defaultValue(TRUE);

    $this->addFieldMapping('field_event_featured_image', 'images');
    $this->addFieldMapping('field_event_featured_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_event_featured_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'restaurant_event_demo') . '/import/images');

    $this->addFieldMapping('uid', 'uid')->defaultValue(1);

    $this->addFieldMapping('field_event_featured', 'featured');
    $this->addFieldMapping('field_event_date', 'date');
  }

  function csvcolumns() {
    // "Title", "Images", "Tags", "Body"
    $columns[0] = array('title', 'Title');
    $columns[1] = array('images', 'Images');
    $columns[2] = array('category', 'Category');
    $columns[3] = array('body', 'Body');
    $columns[4] = array('featured', 'Featured');
    $columns[5] = array('date', 'Date');
    return $columns;
  }

  function prepareRow($row) {
    // Set date as the current date.
    $row->date = time();
    // $row->date = '2014-04-04 10:10:10';
  }
}
