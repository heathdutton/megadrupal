<?php
/**
 * @file
 * Page callbacks.
 */

/**
 * Displays an updates feed.
 */
function ph_updates_view_feed($name = NULL, $core_api = NULL) {
  // Check path slugs.
  if (!isset($name) || !preg_match('/^' . DRUPAL_PHP_FUNCTION_PATTERN . '$/', $name)) {
    return MENU_NOT_FOUND;
  }
  if (!isset($core_api) || !ph_version_valid_format($core_api, 'core_api')) {
    return MENU_NOT_FOUND;
  }

  // Build and format feed.
  $render = array();
  $format = variable_get('ph_updates_feed_format');
  if ($format_info = ph_updates_get_format_info($format)) {
    // Build info.
    try {
      $tree = ph_updates_build_project_tree($name, $core_api);
    }
    catch (PHUpdatesInfoError $e) {
      $error = $e;
    }

    // Render result.
    $headers = array(
      'Content-Type' => $format_info['mimetype'],
    );
    $formatter = ph_updates_format_create($format);
    if (isset($tree)) {
      $result = $formatter->format($tree);
    }
    elseif (isset($error)) {
      if ($error_status = $error->getStatus()) {
        $headers['Status'] = $error_status;
      }
      $result = $formatter->formatError($error);
    }
    else {
      $result = array();
    }

    // Normalize output to array.
    if (is_array($result)) {
      $render = $result;
    }
    else {
      $render['#markup'] = $result;
    }

    // Prepend headers (so that they can be overridden by the result).
    $render += array('#attached' => array());
    $render['#attached'] += array('drupal_add_http_header' => array());
    foreach ($headers as $header_key => $header_value) {
      $header_args = array($header_key, $header_value);
      array_unshift($render['#attached']['drupal_add_http_header'], $header_args);
    }
  }

  return $render;
}
