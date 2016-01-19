<?php
/**
 * @file
 * Contains theme implementations for pathinfo.module.
 */

/**
 * Returns a div with a simple string describing the current path.
 *
 * @param array $variables
 *   An associative array containing:
 *   - path: System path to url.
 *   - function: The function that serves the path.
 *   - arguments: Array of arguments passed to the function serving the path.
 *   - complex_arguments: If TRUE, then at least one of the arguments is an
 *                        object or array.
 *   - module: The module that contains the function.
 *   - filepath: Relative ath to the file that implements the function.
 *   - filepath_full: The full path to the file that contains the function.
 *   - line: The line the function is implemented on.
 *   - is_form: Whether or not the function is a form constructor.
 *   - is_core: Whether or not this module is in Drupal core.
 *
 * @ingroup themeable
 */
function theme_pathinfo_served_by($variables) {
  // Link functions to their API docs if required.
  $function = $variables['function'];
  if (variable_get('pathinfo_link_functions', TRUE)) {
    $args = array(
      'function' => $variables['function'],
      'filepath' => $variables['filepath'],
      'is_core' => $variables['is_core'],
    );
    $function = theme('pathinfo_api_link', $args);
  }
  // Convert variable names to t() arguments.
  $t_arguments = array(
    '@path' => $variables['path'],
    '!function' => $function,
    '@module' => $variables['module'],
    '@implemented_in' => $variables['filepath_full'],
    '@line' => $variables['line'],
  );
  $message = '@path is served by !function';
  if ($variables['is_form']) {
    $message .= ' (form definition)';
  }

  $message .= ', implemented by @module';
  // If Reflection fails, we don't know the file or line.
  if (isset($variables['line'])) {
    $message .= ' in @implemented_in at line @line';
  }
  if ($variables['complex_arguments']) {
    $message = t_safe($message, $t_arguments);
    $message .=  t(' with ') . theme('pathinfo_complex_arguments', $variables);
    //$message .= ' with complex arguments.';
  }
  else {
    if ($variables['arguments']) {
      $t_arguments['@arguments'] = "'" . implode("', '", $variables['arguments']) . "'";
      $message .= ' with arguments: @arguments';
    }
    $message = t_safe($message, $t_arguments);
  }

  
  $output = '<div class="pathinfo pathinfo-served-by">';
  $output .= l(t('PathInfo: '), 'admin/config/pathinfo/config');
  $output .= $message;
  $output .= '</div>';

  return $output;
}

/**
 * Generates a link to Drupal API documentation.
 *
 * @param array $variables
 *   An associative array containing:
 *   - function: The name of the function to link to.
 *   - filepath: The full relative path to the file the function is in.
 *   - is_core: Boolean, TRUE when function is in Drupal core.
 *
 * @ingroup themeable
 */
function theme_pathinfo_api_link($variables) {
  if ($variables['is_core']) {
    // Drupal API functions have the file path in the url.
    $url = 'http://api.drupal.org/api/drupal/%s/function/%s/7';
    // Urls in API module are encoded in a funny way.
    $url_filepath = str_replace('/', '%21', $variables['filepath']);
    $url = sprintf($url, $url_filepath, $variables['function']);
  }
  else {
    $url = 'http://api.drupalize.me/api/drupal/function/%s/7';
    $url = sprintf($url, $variables['function']);
  }

  // No t() here because we don't want to change function names!
  return l(
    check_plain($variables['function']),
    filter_xss($url),
    array(
      'options' => array(
        'external' => TRUE,
      )
    )
  );
}

/**
 * Formats complex arguments.
 * 
 * @param array $variables
 *   An associative array containing:
 *   - link_text: Text to show as the link to view the arguments.
 *   - arguments: Array of function arguments.
 *
 * @ingroup themeable
 */
function theme_pathinfo_complex_arguments($variables) {
  // kprint_r requires a user access check which seems to fail for us, even
  // as user 1. Use krumo_ob directly.
  $arg_output = krumo_ob($variables['arguments']);
  // @see pathinfo_init() for accompanying Javascript.
  $args_css_id = 'pathinfo-args-output';
  $link_css_id = 'pathinfo-args-showhide';
  $arg_output = <<<OUTPUT
<div id="$args_css_id">
  {$arg_output}
</div>
OUTPUT;

  $link = l(t($variables['link_text']), $variables['path'], array(
    'fragment' => $args_css_id,
    'attributes' => array(
      'id' => $link_css_id,
      'title' => t('Show arguments'),
    ),
  ));

  
  $output = $link . $arg_output;
  return $output;
}