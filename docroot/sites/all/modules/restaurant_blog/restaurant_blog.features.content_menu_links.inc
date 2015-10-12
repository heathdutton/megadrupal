<?php
/**
 * @file
 * restaurant_blog.features.content_menu_links.inc
 */

/**
 * Implements hook_content_menu_links_defaults().
 */
function restaurant_blog_content_menu_links_defaults() {
  $menu_links = array();

  // Exported menu link: main-menu:blog
  $menu_links['main-menu:blog'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'blog',
    'router_path' => 'blog',
    'link_title' => 'Blog',
    'options' => array(),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -48,
    'customized' => 1,
  );
  // Exported menu link: menu-footer-menu:blog
  $menu_links['menu-footer-menu:blog'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'blog',
    'router_path' => 'blog',
    'link_title' => 'Blog',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => 2,
    'customized' => 1,
    'parent_path' => 'node/10',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Blog');


  return $menu_links;
}
