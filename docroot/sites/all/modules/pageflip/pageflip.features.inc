<?php
/**
 * @file
 * pageflip.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pageflip_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function pageflip_node_info() {
  $items = array(
    'pageflip_book' => array(
      'name' => t('PageFlip Book'),
      'base' => 'node_content',
      'description' => t('A comic book, magazine, or other type of book.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'pageflip_chapter' => array(
      'name' => t('PageFlip Chapter'),
      'base' => 'node_content',
      'description' => t('A chapter in a <em>PageFlip Book</em>.'),
      'has_title' => '1',
      'title_label' => t('Administrative Title'),
      'help' => '',
    ),
    'pageflip_page' => array(
      'name' => t('PageFlip Page'),
      'base' => 'node_content',
      'description' => t('A page in a <em>PageFlip Chapter</em> or a <em>PageFlip Book\'s</em> Opening Pages.'),
      'has_title' => '1',
      'title_label' => t('Administrative Title'),
      'help' => '',
    ),
  );
  return $items;
}
