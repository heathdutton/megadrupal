<?php
/**
 * @file
 * Set up for the pages migration.
 */
function typo3_pages_migrate_install() {
  typo3_pages_migrate_content_type();
}

function typo3_pages_migrate_uninstall() {
  typo3_pages_migrate_content_type_delete();
}

/**
 *
 * Create content type "Typo3 Page".
 */
function typo3_pages_migrate_content_type() {
  $types = array(
    array(
      'type' => 'typo3_page',
      'name' => st('Typo3 Pages'),
      'base' => 'node_content',
      'description' => st("Pages from typo3."),
      'custom' => 1,
      'modified' => 1,
      'locked' => 1,
    ),
  );

  foreach ($types as $type) {
    $type = node_type_set_defaults($type);
    node_type_save($type);
    node_add_body_field($type);
  }
}


function typo3_pages_migrate_content_type_delete() {
  $bundle = 'typo3_page';
  node_type_delete($bundle);
}
