<?php
/**
 * @file
 * ns_contributor.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_contributor_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_contributor_node_info() {
  $items = array(
    'ns_contributor' => array(
      'name' => t('Contributor'),
      'base' => 'node_content',
      'description' => t('A contributor is someone who is associated with an article or a blog post. Usually the author behind the post. Create new contributors and assign already created users to them.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
