<?php
/**
 * @file
 * Migration file for Restaurant Blog module
 */

/**
 * Implements hook_migrate_api().
 */
function restaurant_blog_demo_migrate_api() {
  return array(
    'api' => 2,
    'migrations' => array(
      'RestaurantBlogMigrateBlogPost' => array(
        'class_name' => 'RestaurantBlogMigrateBlogPostMigration',
      ),
    ),
  );
}

// Migration class
class RestaurantBlogMigrateBlogPostMigration extends Migration {

  public function __construct() {
    parent::__construct();
    $this->description = t('Import blog posts nodes.');

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
    $this->source = new MigrateSourceCSV(drupal_get_path('module', 'restaurant_blog_demo') . '/import/blog_posts.csv', $this->csvcolumns(), array('header_rows' => 1));

    $this->destination = new MigrateDestinationNode('blog_post', array('text_format' => 'panopoly_html_text'));

    // Fields Mapping
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('body', 'body');

    $this->addFieldMapping('field_blog_post_tags', 'tags')->separator(', ');
    $this->addFieldMapping('field_blog_post_tags:create_term')
      ->defaultValue(TRUE);

    $this->addFieldMapping('field_blog_post_categories', 'category')->separator(', ');
    $this->addFieldMapping('field_blog_post_categories:create_term')
      ->defaultValue(TRUE);

    $this->addFieldMapping('field_blog_post_featured_image', 'images');
    $this->addFieldMapping('field_blog_post_featured_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this->addFieldMapping('field_blog_post_featured_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'restaurant_blog_demo') . '/import/images');

    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
  }

  function csvcolumns() {
    // "Title", "Images", "Tags", "Body"
    $columns[0] = array('title', 'Title');
    $columns[1] = array('images', 'Images');
    $columns[2] = array('tags', 'Tags');
    $columns[3] = array('category', 'Category');
    $columns[4] = array('body', 'Body');
    return $columns;
  }
}
