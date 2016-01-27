<?php

/**
 * @file
 * Views handler definitions for html_title module.
 *
 * @ingroup html_title
 */

/**
* Implementation of hook_views_data_alter()
*/
function html_title_views_data_alter(&$data) {
  // Replace standard node title handler with our own
  $data['node']['title']['field']['handler'] = 'html_title_handler_field_node_title';
}

/**
 * Implementation of hook_views_pre_render()
 */
function html_title_views_pre_render(&$view) {
  // Strip tags from titles in RSS feed
  if ($view->plugin_name == 'rss') {
    // Handle "content" style plugin
    if (property_exists($view->style_plugin->row_plugin, 'nodes') && count($view->style_plugin->row_plugin->nodes)) { //$view->style_plugin->uses_row_plugin()) {
      foreach ($view->style_plugin->row_plugin->nodes as $nid => $node) {
        $view->style_plugin->row_plugin->nodes[$nid]->title = strip_tags($node->title);
      }
    }
    // Handle "field" style plugin
    elseif (property_exists($view, 'result') && count($view->result)) {
      foreach ($view->result as $key => $result) {
        if (property_exists($result, 'node_title')) {
          $view->result[$key]->node_title = strip_tags($result->node_title);
        }
      }
    }
  }
}