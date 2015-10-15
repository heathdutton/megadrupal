<?php
/**
 * @file
 * Theme and preprocess functions for breadcrumbs, messages, tabs..etc
 */

/**
 * Implements template_preprocess().
 */
function unary_preprocess_page(&$variables, $hook) {
  // Add btn class to action-links.
  foreach ($variables['action_links'] as &$action_link) {
    $action_link['#link']['localized_options']['attributes']['class'] = array('btn');
  }
}
