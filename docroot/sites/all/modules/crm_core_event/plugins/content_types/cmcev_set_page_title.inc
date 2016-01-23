<?php
/**
 * @file
 * Set page title for page.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Set page title'),
  'description' => t('Set page title for page.'),
  'category' => t('CRM Core Event'),
  'render callback' => 'cmcev_set_page_title_render',
  'admin info' => 'cmcev_set_page_title_admin_info',
  'defaults' => array(
    'height' => '',
    'color_series' => '',
    'display_labels' => '',
    'include_legend' => '0',
  ),
  'all contexts' => TRUE,
);

/**
 * Admin info.
 */
function cmcev_set_page_title_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Set page title');
  $block->content = t('Set page title for page.');

  return $block;
}

/**
 * Render callback.
 */
function cmcev_set_page_title_render($subtype, $conf, $panel_args, $context = NULL) {

  $title = NULL;
  // Argument is node ID.
  if (is_numeric($panel_args[0])) {
    $nid = (int) $panel_args[0];
    $node = node_load($nid);
    if ($node && $node->type == 'cmcev_event') {
      $title = t('Event summary for %event', array('%event' => $node->title));
    }
  }
  else {
    $title = t('Source summary for %source', array('%source' => $panel_args[0]));
  }

  drupal_set_title($title, PASS_THROUGH);


  // Return everything.
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  return $block;
}
