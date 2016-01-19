<?php
/**
 * @file
 * mvpcreator_blog.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function mvpcreator_blog_taxonomy_default_vocabularies() {
  return array(
    'mvpcreator_blog_tags' => array(
      'name' => 'Tags',
      'machine_name' => 'mvpcreator_blog_tags',
      'description' => 'Keywords connecting blog posts on this site.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
