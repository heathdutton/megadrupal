<?php

/**
 * Plugin process callback.
 */
function ilink_info_process(&$plugin, $info) {
  $function_base = $plugin['module'] . '_' . $plugin['name'] . '_ilink_';

  // Menu
  if (!isset($plugin['hook menu']) && function_exists($function_base . 'menu')) {
    $plugin['hook menu'] = $function_base . 'menu';
  }

  // Theme
  if (!isset($plugin['hook theme']) && function_exists($function_base . 'theme')) {
    $plugin['hook theme'] = $function_base . 'theme';
  }

  // Label
  if (!isset($plugin['label']) && function_exists($function_base . 'label')) {
    $plugin['label'] = $function_base . 'label';
  }

  // Attached assets
  if (!isset($plugin['attached']) && function_exists($function_base . 'attached')) {
    $plugin['attached'] = $function_base . 'attached';
  }

  // Ajax
  if (!isset($plugin['ajax']) && function_exists($function_base . 'ajax')) {
    $plugin['ajax'] = $function_base . 'ajax';
  }

  // Options
  if (!isset($plugin['options']) && function_exists($function_base . 'options')) {
    $plugin['options'] = $function_base . 'options';
  }

  // Attributes
  if (!isset($plugin['attributes']) && function_exists($function_base . 'attributes')) {
    $plugin['attributes'] = $function_base . 'attributes';
  }

  // Disable
  if (!isset($plugin['disable']) && function_exists($function_base . 'disable')) {
    $plugin['disable'] = $function_base . 'disable';
  }

  // Provide sane defaults to avoid having to maintain additional checks.
  $plugin += array(
    'label' => '',
    'href' => '',
    'parameters' => array(),
  );
}


/**
 * Retrieves the array value of a plugin property, as this can sometimes be
 * either an array literal, or derived from a callback.
 *
 * @param $key
 *   The name of the definition key requested.
 *
 * @param $plugin
 *   The plugin definition.
 *
 * @param $arguments
 *   An array of arguments provided to the ilink.
 *
 * @return
 *   The resulting array definitions.
 */
function ilink_extract_array_info($key, $plugin, $arguments = array()) {
  if (isset($plugin[$key])) {
    switch (TRUE) {
      case is_string($plugin[$key]) && $function = ctools_plugin_get_function($plugin, $key):
        return $function($plugin, $arguments);

      case is_array($plugin[$key]):
        return $plugin[$key];
    }
  }

  // Fallback return value.
  return array();
}


/**
 * Retrieves the string value of a plugin property, as this can sometimes be
 * either an string literal, or derived from a callback.
 *
 * @param $key
 *   The name of the definition key requested.
 *
 * @param $plugin
 *   The plugin definition.
 *
 * @param $arguments
 *   An array of arguments provided to the ilink.
 *
 * @return
 *   The resulting string value.
 */
function ilink_extract_string_info($key, $plugin, $arguments = array()) {
  if (isset($plugin[$key])) {
    switch (TRUE) {
      case (is_string($plugin[$key]) && $function = ctools_plugin_get_function($plugin, $key)):
        return $function($plugin, $arguments);

      case is_string($plugin[$key]):
        return $plugin[$key];
    }
  }

  // Fallback return value.
  return '';
}


/**
 * Retrieves the boolean value of a plugin property, as this can sometimes be
 * either an boolean literal, or derived from a callback.
 *
 * @param $key
 *   The name of the definition key requested.
 *
 * @param $plugin
 *   The plugin definition.
 *
 * @param $arguments
 *   An array of arguments provided to the ilink.
 *
 * @return
 *   The resulting string value.
 */
function ilink_extract_boolean_info($key, $plugin, $arguments = array()) {
  if (isset($plugin[$key])) {
    switch (TRUE) {
      case (is_string($plugin[$key]) && $function = ctools_plugin_get_function($plugin, $key)):
        return $function($plugin, $arguments);

      case is_bool($plugin[$key]):
        return $plugin[$key];
    }
  }

  // Fallback return value.
  return FALSE;
}


/**
 * Private helper to transpose ctools modal definition into a javascript
 * settings array.
 *
 * @param $arguments
 *   An array of arguments provided to the ilink.
 *
 * @param $plugin
 *   The plugin definition.
 */
function _ilink_attach_modal(&$element, $plugin, $arguments = array()) {
  $settings = ilink_extract_array_info('modal', $plugin, $arguments);

  if (!empty($settings)) {
    $element['#attached']['js'][] = array(
      'type' => 'setting',
      'data' => $settings,
    );
  }
}

/**
 * Compare arguments against parameters to build an ordered, and complete list
 * of arguments. Otherwise, the link will be disabled if we are missing any
 * required arguments.
 *
 * @todo iLink Context should chime in here.
 */
function ilink_build_path(&$plugin, $arguments) {
  $params = ilink_build_parameters($plugin['href']);

  // Track required parameters so we may disable the plugin if not all required
  // arguments are fulfilled.
  $missing = array();

  // Iterate over each component of the path and "rebuild" the path using real
  // values.
  foreach (explode('/', $plugin['href']) as $position => $bit) {
    // Process bits from the path that are parameters...
    if (isset($params[$position])) {
      // Place the provided argument in its right place...
      if (isset($arguments[$params[$position]['name']])) {
        $args[] = $arguments[$params[$position]['name']];
      }
      // ... complain if a required parameter is missing from the arguments.
      elseif ($params[$position]['type'] == 'required') {
        $missing[] = $params[$position]['name'];
      }
    }
    // ... however, string literals as passed "as-is".
    else {
      $args[] = $bit;
    }
  }

  // If all the named parameters have been populated
  if (empty($missing)) {
    return implode('/', $args);
  }

  // Otherwise, making it this far means we have missing required arguments,
  // and therefore, should render this link disabled.
  $plugin['disable'] = TRUE;
  return '';
}

/**
 * Get an array of parameter details, such as the parameter name and
 * requirement type, keyed by their position in the provided path.
 *
 * @param $path
 *   A normal Drupal path.
 */
function ilink_build_parameters($path) {
  $arguments = array();
  foreach (explode('/', $path) as $position => $bit) {
    if ($bit && in_array($bit[0], array('%', '!'))) {
      $arguments[$position] = array(
        'name' => substr($bit, 1),
        'type' => ($bit[0] == '%') ? 'required' : 'optional',
      );
    }
  }

  return $arguments;
}


/**
 * Exception handling for iLink.
 */
class IlinkMissingPluginInfoException extends Exception {}
