<?php
/**
 * @file
 * cms_contact_content.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function cms_contact_content_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu_contact:uuid/node/52ec77c2-0281-4140-b891-1aff2cb69709
  $menu_links['main-menu_contact:uuid/node/52ec77c2-0281-4140-b891-1aff2cb69709'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'uuid/node/52ec77c2-0281-4140-b891-1aff2cb69709',
    'router_path' => 'uuid',
    'link_title' => 'Contact',
    'options' => array(
      'identifier' => 'main-menu_contact:uuid/node/52ec77c2-0281-4140-b891-1aff2cb69709',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 10,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Contact');

  return $menu_links;
}
