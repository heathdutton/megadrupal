<?php

/**
 * Add custom PHPTemplate variables into the node template.
 */
function ninesixtyrobots_omega_alpha_preprocess_node(&$vars) {
  // Grab the node object.
  $node = $vars['node'];
  // Make individual variables for the parts of the date.
  $vars['date_day'] = format_date($node->created, 'custom', 'j');
  $vars['date_month'] = format_date($node->created, 'custom', 'M');
  $vars['date_year'] = format_date($node->created, 'custom', 'Y');

  // Add the .post class to all nodes.
  $vars['classes_array'][] = 'post';

  // Change the theme function used for rendering the list of tags.
  $vars['content']['field_tags']['#theme'] = 'links';
}
