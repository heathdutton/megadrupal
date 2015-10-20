<?php

/**
 * @file
 * Manages PMB reader templates.
 */

/**
 * Get reader account template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_account($variables) {
  $reader = $variables['reader'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.account.tpl.php');
  return $template;
}

/**
 * Get reader loans template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - loans: loans array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_loans($variables) {
  $reader = $variables['reader'];
  $loans = $variables['loans'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.loans.tpl.php');
  return $template;
}

/**
 * Get reader suggestions template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - suggestions: suggestions array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_suggestions($variables) {
  $reader = $variables['reader'];
  $suggestions = $variables['suggestions'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.suggestions.tpl.php');
  return $template;
}

/**
 * Get reader reservations template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - reservations reservations array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reservations($variables) {
  $reader = $variables['reader'];
  $reservations = $variables['reservations'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.reservations.tpl.php');
  return $template;
}

/**
 * Get reader reading lists template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - reading_lists: reading_lists array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reading_lists($variables) {
  $reader = $variables['reader'];
  $reading_lists = $variables['reading_lists'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.reading_lists.tpl.php');
  return $template;
}

/**
 * Get reader reading lists template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - reading_lists: reading_lists array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reading_list($variables) {
  $reader = $variables['reader'];
  $reading_list = $variables['reading_list'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.reading_list.tpl.php');
  return $template;
}

/**
 * Get reader reading lists public template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - reading_lists: reading_lists array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reading_lists_public($variables) {
  $reader = $variables['reader'];
  $reading_lists = $variables['reading_lists'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.reading_lists_public.tpl.php');
  return $template;
}

/**
 * Get reader reading list public template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - reader: reader array;
 *   - reading_lists: reading_lists array;
 *   - notices: notices array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reading_list_public($variables) {
  $reader = $variables['reader'];
  $reading_list = $variables['reading_list'];
  $notices = $variables['notices'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.reading_list_public.tpl.php');
  return $template;
}

/**
 * Get reader cart form template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: form array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_cart_form($variables) {
  $form = $variables['form'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.form_cart.tpl.php');
  return $template;
}

/**
 * Get reader reading list form template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: form array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_reader_reading_list_form($variables) {
  $form = $variables['form'];

  $template = '';
  include(drupal_get_path('module', 'pmb_reader') . '/templates/pmb_reader.form_reading_list.tpl.php');
  return $template;
}
