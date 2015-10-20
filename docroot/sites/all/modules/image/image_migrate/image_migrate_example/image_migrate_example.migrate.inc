<?php

/**
 * @file image_migrate_example.migrate.inc
 * Sample migration from D6 to D7.
 */

/**
 * Implements hook_migrate_api().
 */
function image_migrate_example_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'imagetest' => array(
        'title' => t('Sample Drupal 6 to Drupal 7 migration with Image migration.'),
      ),
    ),
    'migrations' => array(),
  );

  // This portion is copied from migrate_d2d_example_migrate_api()

  $common_arguments = array(
    'source_connection' => 'drupal_6_image',
    'source_version' => 6,
    'group_name' => 'example_image',
  );

  // Register the user migration.
  // We just use the migrate_d2d D6 migration class as-is.
  $api['migrations']['User'] = $common_arguments + array(
    'description' => t('Migration of users from Drupal 6'),
    'class_name' => 'DrupalUser6Migration',
  );

  // File migration.
  $api['migrations']['File'] = $common_arguments + array(
    'description' => t('Migration of files from Drupal 6'),
    'class_name' => 'DrupalFile6Migration',
    'source_dir' => '/path/to/source/site/',
    'user_migration' => 'User',
  );

  // Node migrations.
  $node_arguments = array(
    // Define a migration for the image nodes.
    'Image' => array(
      'class_name' => 'DrupalNode6Migration',
      'description' => t('Migration of image nodes from Drupal 6'),
      'source_type' => 'image',
      'destination_type' => 'image',
    ),
    // If using image_attach, define a migration for each node type which
    // attaches images.
    'Article' => array(
      'class_name' => 'DrupalNode6Migration',
      'description' => t('Migration of article nodes from Drupal 6'),
      'source_type' => 'story',
      'destination_type' => 'article',
    ),
  );

  // Tell the node migrations where the users are coming from, so they can
  // set up the dependency and resolve D6->D7 uids.
  $common_node_arguments = $common_arguments + array(
    'user_migration' => 'User'
  );

  foreach ($node_arguments as $migration_name => $arguments) {
    $arguments = array_merge_recursive($arguments, $common_node_arguments);
    $api['migrations'][$migration_name] = $arguments;
  }

  // These migrations are for image module's data.

  // Image migrations
  $api['migrations']['ImageFile'] = $common_arguments + array(
    'description' => t('Migration of images from Drupal 6'),
    'class_name' => 'ImageNodeDrupalMigration',
    // You need to create this field on the image node type.
    'image_field' => 'field_image_file',
    'image_node_type' => 'image',
    'file_migration' => 'File',
    'image_node_migration' => 'Image',
  );

  // Image attach migration.
  $api['migrations']['ImageAttach'] = $common_arguments + array(
    'description' => t('Migration of attached images from Drupal 6'),
    'class_name' => 'ImageAttachImageNodeDrupalMigration',
    'image_node_migration' => 'Image',
    'attaching_node_migration' => 'Article',
    'attaching_node_type' => 'article',
    // You need to create this field on the attaching node type.
    'attaching_entityreference_field' => 'field_attached_images',
  );

  return $api;
}
