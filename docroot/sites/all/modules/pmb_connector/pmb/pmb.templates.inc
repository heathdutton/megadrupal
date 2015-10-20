<?php

/**
 * @file
 * Manages PMB generic templates.
 */

/**
 * Get PMB pager template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - current_page
 *   - page_count
 *   - tags
 *   - quantity
 *   - link_generator_callback
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_pager($variables) {
  $current_page = $variables['current_page'];
  $page_count = $variables['page_count'];
  $tags = $variables['tags'];
  $quantity = $variables['quantity'];
  $link_generator_callback = $variables['link_generator_callback'];

  $template = '';
  include(drupal_get_path('module', 'pmb') . '/templates/pmb.pager.tpl.php');
  return $template;
}

/**
 * Get PMB block pager template.
 *
 * @param array $variables
 *   An associative array containing:
 *   - current_page
 *   - page_count
 *   - tags
 *   - id
 *   - link_generator_callback
 * @return
 *   A themed HTML string.
 *
 * @ingroup themeable
 */
function theme_pmb_block_pager($variables) {
  $current_page = $variables['current_page'];
  $page_count = $variables['page_count'];
  $tags = $variables['tags'];
  $id = $variables['id'];
  $link_generator_callback = $variables['link_generator_callback'];

  $template = '';
  include(drupal_get_path('module', 'pmb') . '/templates/pmb.block_pager.tpl.php');
  return $template;
}
