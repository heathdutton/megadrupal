<?php

/**
 * @file
 * Content for Node Subpages: Views
 */

// Plugin details
$plugin = array(
  'title' => t('View'),
  'description' => t('Use a View as the content for the subpage'),
  'dependencies' => array('views'),
  'config form' => 'node_subpages_content_views_config',
  'config save prep' => 'node_subpages_content_views_config_save_prep',
  'config summary' => 'node_subpages_content_views_config_summary',
  'content' => 'node_subpages_content_views_content',
  'has content' => 'node_subpages_content_views_has_content',
);


/**
 * Config form for subpages using View content
 */
function node_subpages_content_views_config($type, $plugin_config) {
  $views = views_get_enabled_views();
  $views_list = array('' => t('-- Select One --'));
  foreach ($views as $view_name => $view) {
    foreach ($view->display as $display_name => $display) {
      $display_key = $view_name . ':' . $display_name;
      $friendly_name = $view->human_name . ': ' . $display->display_title;
      $views_list[$display_key] = $friendly_name;
    }
  }

  $default_value = (empty($plugin_config['view'])) ? '' : $plugin_config['view'] . ':' . $plugin_config['display'];

  $form['view_display'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#description' => t('View to display as content of the subpage.'),
    '#multiple' => FALSE,
    '#options' => $views_list,
    '#default_value' => $default_value,
  );
  return $form;
}


/**
 * Prep the config for save to the DB, by placing into an array
 */
function node_subpages_content_views_config_save_prep($values) {
  list($view, $display) = explode(':', $values['view_display']);
  return array('view' => $view, 'display' => $display);
}



/**
 * Return the content for a subpage for the given node and plugin config
 */
function node_subpages_content_views_content($node, $plugin_config) {
  if ($view = views_get_view($plugin_config['view'])) {
    $output = $view->execute_display($plugin_config['display'], array($node->nid));

    // If executing a display with a specified display, it will return an
    // array with content and subject keys. Otherwise, it returns a
    // straight array.
    if (is_array($output) && $output['content']) {
      return $output['content'];
    }

    // Make sure output isn't null, because that causes problems. Make it
    // an empty string instead.
    if (is_null($output)) {
      $output = '';
    }
    return $output;
  }
}

/**
 * Check if the view has content, to determine if the tab should be displayed
 */
function node_subpages_content_views_has_content($node, $plugin_config) {
  if ($view = views_get_view($plugin_config['view'])) {
    $view->execute_display($plugin_config['display'], array($node->nid));
    return !empty($view->result);
  }
  return FALSE;
}


/**
 * Return a summary for the configuration
 *
 * @param $plugin_config
 * Array of config options for the plugin
 *
 * @param $details
 * Array of full details, including subpath, plugin, etc
 */
function node_subpages_content_views_config_summary($plugin_config, $details) {
  // Get the view and display from the plugin config
  $view_name = $plugin_config['view'];
  $view_display = $plugin_config['display'];

  // Get the view object to get the human-friendly view name, and name of the
  // display
  $view = views_get_view($view_name);
  if (is_object($view)) {
    $display = $view->display[$view_display];
    $content = t('View: %human_name, display %display_name', array(
      '%human_name' =>  $view->human_name,
      '%display_name' =>  $display->display_title,
    ));
  }
  else {
    $content = t('View: ERROR');
  }
  return $content;
}
