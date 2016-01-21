<?php
/**
 * @file
 * ns_media.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function ns_media_defaultconfig_taxonomy_default_vocabularies() {
  return array(
    'ns_media_file_category' => array(
      'name' => 'File category',
      'machine_name' => 'ns_media_file_category',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'ns_media_tag' => array(
      'name' => 'File Tag',
      'machine_name' => 'ns_media_tag',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
