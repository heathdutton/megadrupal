<?php
/**
 * @file
 * ns_blog.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_blog_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ns_blog_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_blog_node_info() {
  $items = array(
    'ns_blog' => array(
      'name' => t('Blog'),
      'base' => 'node_content',
      'description' => t('Your site can contain as many blogs as you want. Add an image, a description and a contributor (blogger) and then let him/her add blog posts to the blog.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
