<?php

/**
 * @file
 * Manages PMB view templates.
 */

/**
 * Get pmb_view.notice_diplay template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - display_type: display mode (medium_line, small_line);
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_notice_display($variables) {
  $notice = $variables['notice'];
  $display_type = $variables['display_type'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.notice_display.tpl.php');
  return $template;
}

/**
 * Get pmb_view.notice template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_notice($variables) {
  $notice = $variables['notice'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.notice.tpl.php');
  return $template;
}

/**
 * Get pmb_view.notice_info template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - info: optional info to display.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_notice_info($variables) {
  $notice = $variables['notice'];
  $info = $variables['info'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.notice_info.tpl.php');
  return $template;
}

/**
 * Get pmb_view.notice_external template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_notice_external($variables) {
  $notice = $variables['notice'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.notice_external.tpl.php');
  return $template;
}

/**
 * Get pmb_view.serial_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_serial($variables) {
  $notice = $variables['notice'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.serial.tpl.php');
  return $template;
}

/**
 * Get pmb_view.bulletin_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - bulletin: pmb_unimarc bulletin array;
 *   - serial: pmb_unimarc serial array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_bulletin($variables) {
  $bulletin = $variables['bulletin'];
  $serial = $variables['serial'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.issue.tpl.php');
  return $template;
}

/**
 * Get pmb_view.notice_external_diplay template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - notice: pmb_unimarc notice array;
 *   - display_type: display mode (medium_line, small_line);
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_notice_external_display($variables) {
  $notice = $variables['notice'];
  $display_type = $variables['display_type'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.notice_external_display.tpl.php');
  return $template;
}

/**
 * Get pmb_view.author_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - author: author array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_author($variables) {
  $author = $variables['author'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.author.tpl.php');
  return $template;
}

/**
 * Get pmb_view.publisher_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - publisher: publisher array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_publisher($variables) {
  $publisher = $variables['publisher'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.publisher.tpl.php');
  return $template;
}

/**
 * Get pmb_view.serie_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - serie: serie array;
 *   - parent_publisher: publisher array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_serie($variables) {
  $serie = $variables['serie'];
  $parent_publisher = $variables['parent_publisher'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.serie.tpl.php');
  return $template;
}

/**
 * Get pmb_view.subserie_view template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - subserie: sub-serie array;
 *   - parent_serie: parent_serie array;
 *   - parameters: optional parameters array.
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_view_subserie($variables) {
  $subserie = $variables['subserie'];
  $parent_serie = $variables['parent_serie'];
  $parameters = $variables['parameters'];

  $template = '';
  include(drupal_get_path('module', 'pmb_view') . '/templates/pmb_view.subserie.tpl.php');
  return $template;
}
