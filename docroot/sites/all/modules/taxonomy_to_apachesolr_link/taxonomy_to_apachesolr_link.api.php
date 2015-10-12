<?php

/**
 * @file
 * Hooks provided by taxonomy to apachesolr link.
 */

/**
 * Alter a taxonomy link construction.
 *
 * @param string $link
 *   The end result
 *
 * @param assoc array containing:
 *
 * @param stdobject $term
 * The taxonomy term object to generate a link from
 * @param string $field_name
 *   The current field (term reference)
 *
 * @param string $search_path
 *   The search path given by the user
 */
function hook_taxonomy_to_apachesolr_link_alter(&$link, $vars) {
  global $base_url;
  $link = urldecode($base_url . '/' . $search_path . '/' . $term->name . '?f[0]=im_' . $field_name . ':' . $term->tid);
}
