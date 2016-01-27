<?php
/**
 * @file
 * drustack_forum.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function drustack_forum_taxonomy_default_vocabularies() {
  return array(
    'forums' => array(
      'name' => 'Forums',
      'machine_name' => 'forums',
      'description' => 'Forum navigation vocabulary',
      'hierarchy' => 1,
      'module' => 'forum',
      'weight' => -10,
    ),
  );
}
