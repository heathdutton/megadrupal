<?php

/**
 * @file
 * Plugin that outputs the page title with safe markup.
 */

$plugin = array(
  'title' => t('Page title with safe markup'),
  'single' => TRUE,
  'icon' => 'icon_page.png',
  'description' => t('Outputs the page title that allows some safe HTML tags in it.'),
  'category' => t('Page elements'),
);

function ns_core_page_title_markup_content_type_render($subtype, $conf, $panel_args) {
  // Get the raw page title.
  $title = drupal_set_title();
  // If that didn't work, let's get it from the menu.
  if (!isset($title) && function_exists('menu_get_active_title')) {
    // Get the raw active menu title.
    $title = menu_get_active_title();
  }

  $block = new stdClass();
  $block->content = theme('ns_core_page_title', array('title' => ns_core_filter_xss($title)));
  return $block;
}

function ns_core_page_title_markup_content_type_admin_info($subtype, $conf) {
  $block->title = t('Page title with safe markup');
  $block->content = t('Outputs the page title that allowes some safe HTML tags in it.');
  return $block;
}
