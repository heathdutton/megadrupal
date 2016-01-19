<?php
/**
 * @file
 * sharedcontent_template_ui.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function sharedcontent_template_ui_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: management:admin/content/sharedcontent/dashboard
  $menu_links['management:admin/content/sharedcontent/dashboard'] = array(
    'menu_name' => 'management',
    'link_path' => 'admin/content/sharedcontent/dashboard',
    'router_path' => 'admin/content/sharedcontent/dashboard',
    'link_title' => 'ShareBoard',
    'options' => array(),
    'module' => 'system',
    'hidden' => '-1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'admin/content/sharedcontent',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('ShareBoard');


  return $menu_links;
}
