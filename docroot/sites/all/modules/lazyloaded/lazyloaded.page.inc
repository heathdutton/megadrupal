<?php

/**
 * Page callback.
 */
function lazyloaded_page($node, $plugin) {
  $plugins = lazyloaded_get_plugins();
  if (!isset($plugins[$plugin])) {
    return MENU_NOT_FOUND;
  }
  $plugin = $plugins[$plugin];

  $data = db_select('lazyloaded_data', 'd')
    ->fields('d', array('data'))
    ->condition('d.nid', $node->nid)
    ->condition('d.plugin', $plugin->name)
    ->execute()
    ->fetchField();
  $data = $data ? unserialize($data) : NULL;

  print theme("lazyloaded_{$plugin->name}_loaded", array('node' => $node, 'data' => $data));

  return NULL;
}
