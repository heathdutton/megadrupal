<?php

/**
 * @file
 * Manages PMB search templates.
 */

/**
 * Get pmb_catalog.search_form template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: form array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_search_base_form($variables) {
  $form = $variables['form'];

  $template = '';
  include(drupal_get_path('module', 'pmb_search') . '/templates/pmb_search.base_form.tpl.php');
  return $template;
}

/**
 * Get pmb_catalog.search_result template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - search_terms: search_terms array;
 *   - search_fields: search_fields array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_search_base_result($variables) {
  $search_terms = $variables['search_terms'];
  $search_fields = $variables['search_fields'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_search') . '/templates/pmb_search.base_result.tpl.php');
  return $template;
}

/**
 * Get pmb_catalog.search_external_form template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: form array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_search_external_form($variables) {
  $form = $variables['form'];

  $template = '';
  include(drupal_get_path('module', 'pmb_search') . '/templates/pmb_search.external_form.tpl.php');
  return $template;
}

/**
 * Get pmb_catalog.search_external_result template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - search_terms: search_terms array;
 *   - search_sources: search_sources array;
 *   - search_fields: search_fields array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_search_external_result($variables) {
  $search_terms = $variables['search_terms'];
  $search_sources = $variables['search_sources'];
  $search_fields = $variables['search_fields'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_search') . '/templates/pmb_search.external_result.tpl.php');
  return $template;
}

/**
 * Get pmb_catalog.search_advanced_form template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: form array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_search_advanced_form($variables) {
  $form = $variables['form'];

  $template = '';
  include(drupal_get_path('module', 'pmb_search') . '/templates/pmb_search.advanced_form.tpl.php');
  return $template;
}
