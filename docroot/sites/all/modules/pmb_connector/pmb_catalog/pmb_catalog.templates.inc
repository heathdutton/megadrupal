<?php

/**
 * @file
 * Manages PMB catalog templates.
 */

/**
 * Get locations template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - locations_and_sections: locations_and_sections array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_locations($variables) {
  $locations_and_sections = $variables['locations_and_sections'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.locations.tpl.php');
  return $template;
}

/**
 * Get location template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - location: location array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_location($variables) {
  $location = $variables['location'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.location.tpl.php');
  return $template;
}

/**
 * Get section template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - location: location array;
 *   - section: section array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_section($variables) {
  $location = $variables['location'];
  $section = $variables['section'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.section.tpl.php');
  return $template;
}

/**
 * Get shelves template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - shelves: shelves array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_shelves($variables) {
  $shelves = $variables['shelves'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.shelves.tpl.php');
  return $template;
}

/**
 * Get shelf template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - shelf: shelf array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_shelf($variables) {
  $shelf = $variables['shelf'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.shelf.tpl.php');
  return $template;
}

/**
 * Get block_shelf template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - shelf: shelf array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_block_shelf($variables) {
  $shelf = $variables['shelf'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.block_shelf.tpl.php');
  return $template;
}

/**
 * Get thesauri template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - thesauri: thesauri array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_thesauri($variables) {
  $thesauri = $variables['thesauri'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.thesauri.tpl.php');
  return $template;
}

/**
 * Get category template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - category: category array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_category($variables) {
  $category = $variables['category'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.category.tpl.php');
  return $template;
}

/**
 * Get serials template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - serials: serials array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_catalog_serials($variables) {
  $serials = $variables['serials'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_catalog') . '/templates/pmb_catalog.serials.tpl.php');
  return $template;
}
