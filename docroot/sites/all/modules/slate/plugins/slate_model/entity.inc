<?php
/**
 * @file
 */


$plugin = array(
  'get child' => 'slate_model_entity_get_child',
  'get children' => 'slate_model_entity_entity_get_children',
  'handler' => array(
    'class' => 'SlateModelPluginEntity',
    'file' => 'entity.class.php',
  ),
);


/**
 * Get a single Slate model entity plugin.
 */
function slate_model_entity_get_child($plugin, $parent, $child) {
  $plugins = slate_model_entity_get_children($plugin, $parent);

  return $plugins[$parent . ':' . $child];
}

/**
 * Get all child Slate model entity plugins.
 */
function slate_model_entity_get_children($plugin, $parent) {
  static $plugins = array();

  if (empty($plugins)) {
    foreach (entity_get_info() as $entity_type => $entity) {
      $child_plugin = $plugin;

      $child_plugin['name'] = $parent . ':' . $entity_type;
      $child_plugin['entity_type'] = $entity_type;

      $plugins[$child_plugin['name']] = $child_plugin;
    }
  }

  return $plugins;
}
