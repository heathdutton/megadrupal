<?php

/**
 * @file template.php
 * A file containing helper functions for the Whiteboard theme for Drupal.
 */
 
/*
 * Initialize theme settings
 */
if (is_null(theme_get_setting('header_image'))) {
  global $theme_key;

  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the theme-settings.php file.
   */
  $defaults = array(
    'header_image' => 1,
  );

  // Get default theme settings.
  //$settings = theme_get_settings($theme_key);
  // Don't save the toggle_node_info_ variables.
  if (module_exists('node')) {
    // NOTE: node_get_types() is renamed to node_type_get_types() in Drupal 7
    foreach (node_type_get_types() as $type => $name) {    
      unset($settings['toggle_node_info_' . $type]);
    }
  }
  // Save default theme settings.
  variable_set(
    str_replace('/', '_', 'theme_'. $theme_key .'_settings'),
    array_merge($defaults, $settings)
  );
  // Force refresh of Drupal internals.
  theme_get_setting('', TRUE);
}

function phptemplate_preprocess_comment(&$variables) {

  // Comment number with link
  if (!isset($post_number)) {
    static $post_number = 0;
  }
  _whiteboard_topic_nid($variables['node']->nid);

  $post_per_page = _comment_get_display_setting('comments_per_page', $variables['node']);
  $page_number = (!empty($_GET['page'])) ? "page=" . $_GET['page'] : 0;

  $post_number++;
  $fragment = 'comment-' . $variables['comment']->cid;
  $query = ($page_number) ? 'page=' . $page_number : NULL;
  $linktext = (($page_number * $post_per_page) + $post_number) . '.';
  $linkpath = 'node/' . _whiteboard_topic_nid();
  $variables['comment_link'] = l($linktext, $linkpath, array('query' => $query, 'fragment' => $fragment));
  $variables['post_number'] = $post_number;
  return $variables;
}

/**
 * Holds the node ID of the node we are on.
 *
 * Used for linking the comment numbers. Copied from Advanced Forum module.
 *
 * @param $nid
 *   Node ID
 */
function _whiteboard_topic_nid($nodeid = 0) {
  static $nid;

  if (!empty($nodeid)) {
    $nid = $nodeid;
  }

  return $nid;
}
