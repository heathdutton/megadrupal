<?php
/**
 * @file
 * API documentation for Cacheable CRSF.
 */

/**
 * Returns an array of form IDs to replace CRSF protection for.
 */
function hook_cacheable_csrf_form_ids() {
  return array('example_form_id');
}

/**
 * Returns an array keyed by hook_menu() key where the values are the $data parameter to pass to drupal_get_token().
*/
function hook_cacheable_csrf_paths() {
  // The path as defined in the hook_menu() implementation.
  $paths['example/%example/router/path'] = array(
     // The $data argument that is passed to drupal_get_token() when
     // constructing the token used when linking to and access checking this
     // path.
     'data' => 'example',
  );
  return $paths;
}
