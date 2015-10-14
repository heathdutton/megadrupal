<?php

/**
 * @file
 * Documentation of Web Taxonomy hooks.
 */

/**
 * Web Taxonomy offers a CTools based plugin API.
 *
 * @defgroup pluginapi Plugin API
 * @{
 */

/**
 * A hook_web_taxonomy_info() implementation declares available web taxonomies
 * and the classes that handle them. For an example look at
 * drupaldocweb_web_taxonomy_info().
 *
 * @see drupaldocweb_web_taxonomy_info()
 */
function hook_web_taxonomy_info() {
  $info = array();
  $path = drupal_get_path('module', 'drupaldocweb') . '/plugins';
  $info['drupal_full_projects'] = array(
    'name' => 'Drupal - Full Projects',
    'description' => 'Modules, themes, and installation profiles with version numbers.',
    'handler' => array(
      'parent' => 'WebTaxonomy',
      'class' => 'DrupalProjectsWebTaxonomy',
      'file' => 'DrupalProjectsWebTaxonomy.inc',
      'path' => $path,
    ),
  );
  return $info; 
}

/**
 * @}
 */
