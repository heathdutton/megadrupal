<?php

/**
 * @file
 * Entity type breadcrumb path callbacks for Nuke Drupal Frontend module.
 */

/**
 * Implements hook_ndf_ENTITY_TYPE_breadcrumb_path().
 */
function ndf_ndf_taxonomy_term_breadcrumb_path($entity) {
  return 'admin/structure/taxonomy/' . $entity->vocabulary_machine_name;
}

/**
 * Implements hook_ndf_ENTITY_TYPE_breadcrumb_path().
 */
function ndf_ndf_node_breadcrumb_path($entity) {
  return 'admin/content';
}

/**
 * Implements hook_ndf_ENTITY_TYPE_breadcrumb_path().
 */
function ndf_ndf_file_breadcrumb_path($entity) {
  return 'admin/content/file';
}
