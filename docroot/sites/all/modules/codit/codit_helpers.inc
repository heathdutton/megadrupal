<?php
/**
 * @file
 * Supporting functions for the codit module and environment.
 */

/**
 * Get the full path to the codit_local storage submodule.
 *
 * @param string $submodule_name
 *   The name of the sub-stash (blocks, crons, responsive...).
 *
 * @return string
 *   The full path to the submodule local storage, false if there is no such
 *   local storage or the base local storage. With trailing slash.
 */
function codit_path_to_local($submodule_name = '') {
  $path_to_local = drupal_get_path('module', 'codit_local');
  if (empty($submodule_name)) {
    // Build just the base path to return.
    $path = $path_to_local . '/';
  }
  else {
    // Build the path to that sub-module's local storage to return.
    $path = $path_to_local . '/' . $submodule_name . '/';
  }
  return $path;
}


/**
 * Get the full path to a specific codit submodule within codit.
 *
 * @param string $submodule_name
 *   The name of the framework  or submodule (blocks, crons, responsive...).
 *
 * @return string
 *   The full path to the specific submodule, false if there is no such module
 *   or it is not enabled. Includes trailing slash.
 */
/* *** function slated for removal   UNUSED.
function codit_path($submodule_name = '') {
  $submodule_name = (!empty($submodule_name)) ? '_' . $submodule_name : '';
  if (module_exists('codit_' . $submodule_name)) {
    $path_to_submodule = drupal_get_path('module', 'codit_' . $submodule_name);
    // Build just the base path to codit.
    $path = $path_to_submodule . '/';
  }
  else {
    // Submodule is unavailable.
    $path = FALSE;
  }
  return $path;
}
*/

/**
 * Set the header to force IE to edge mode (the highest level of IE installed).
 */
function codit_set_ie_to_edge() {
  $debug_slug = variable_get('codit_debug_request_slug', 'debug');
  // Force ie to not use compatibility mode.
  if (!empty($_GET) && !empty($_GET[$debug_slug]) && ($_GET[$debug_slug] == 'ie')) {
    // There is a debug so do not force to use edge, do nothing.
  }
  else {
    // There is no debugging going on, so force ie to use edge.
    // Send X-UA-Compatible HTTP header to force IE to use the most recent
    // rendering engine or use Chrome's frame rendering engine if available.
    drupal_add_http_header('X-UA-Compatible', 'IE=edge');
    // Meta is often ineffective, but serves as good indicator of the override.
    $element = array(
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'X-UA-Compatible',
        'content' => 'IE=edge',
      ),
      // IE won't obey it if it not the first meta.
      '#weight' => -99999,
    );
    drupal_add_html_head($element, 'x_ua_compatible');
  }
}
