<?php
/**
 * @file
 * Provides filter callback routines.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Filter process callback.
 */
function _portable_path_file_id_process($text, $filter, $format) {
  global $portable_path_absolute;

  if (empty($text)) {
    // This will occur on an empty summary for a "Long text and summary" field.
    return $text;
  }

  $portable_path_absolute = !empty($filter->settings['absolute_path']);

  // Return an absolute or relative URL.
  // This regular expression handles tokens with and without a query string, and
  // eliminates the need for a hook_update_N() to add ':' to existing tokens.
  // Use non-capturing group syntax '?:' on third colon so array sent to process
  // callback is unchanged.
  return preg_replace_callback('@\[portable:(\d+):(\w*)(?::(.*)|)\]@', 'portable_path_file_id_process_callback', $text);
}

/**
 * Filter process callback.
 *
 * Callback for preg_replace_callback() within _portable_path_file_id_process().
 */
function portable_path_file_id_process_callback($matches) {
  global $portable_path_absolute;

  if (portable_path_state_check()) {
    // This handles use case in which local site is first to request content
    // after caches are cleared on local and remote sites.
    return portable_path_remote_file_id_process_callback($matches);
  }

  if (isset($matches[1]) && $file = file_load($matches[1])) {
    $uri = $file->uri;
    if (!empty($matches[2])) {
      // Add image style pathing to uri.
      $uri = image_style_path($matches[2], $file->uri);
    }
    $query = !empty($matches[3]) ? '?' . $matches[3] : '';
    $target = file_uri_target($uri);
    $scheme = file_uri_scheme($uri);
    if ($path = portable_path_scheme_url($scheme, $portable_path_absolute)) {
      // Return absolute or relative URL based on filter setting.
      return $path . $target . $query;
    }
  }
  return $matches[0];
}

/**
 * Filter settings callback.
 */
function _portable_path_file_id_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  global $databases;

  $filter->settings += $defaults;

  $elements['absolute_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use absolute path'),
    '#default_value' => $filter->settings['absolute_path'] ? $filter->settings['absolute_path'] : FALSE,
    '#description' => t('Include the full URL prefix "@base_url" in all links and image tags.', array('@base_url' => $GLOBALS['base_url'])),
    '#weight' => 0,
  );

  return $elements;
}

/**
 * Filter tips callback.
 */
function _portable_path_file_id_tips($filter, $format, $long = FALSE) {
  $settings = portable_path_config();
  $stream_wrappers = implode(', ', $settings['stream_wrappers']);
  $field_types = implode(', ', $settings['field_types']); // Not displayed.
  $field_names = implode(', ', array_filter($settings['field_names']));
  $source = '[portable:fid:style]';
  $replacement = portable_path_scheme_url() . 'styles/style-name/public/path-to-file/filename';
  if (!$long) {
    // This string will be shown in the content add/edit form.
    return t('Tokens containing a file ID and any associated image style, e.g. %source, will be replaced with full relative URLs, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
  else {
    return t('In %field_names fields, embedded tokens containing the file ID of a managed file and any associated image style, e.g. %source, will be replaced with full relative URLs, e.g. %replacement. The file URIs must be stored in <em>%stream_wrappers</em> stream wrapper notation.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
}

/**
 * Filter process callback.
 */
function _portable_path_stream_wrapper_process($text, $filter, $format) {
  if (empty($text)) {
    // This will occur on an empty summary for a "Long text and summary" field.
    return $text;
  }

  if (portable_path_state_check()) {
    // This handles use case in which local site is first to request content
    // after caches are cleared on local and remote sites.
    return portable_path_remote_stream_wrapper_process($text, $filter, $format);
  }

  $absolute_path = !empty($filter->settings['absolute_path']);
  $settings = portable_path_config();
  foreach (array_filter($settings['stream_wrappers']) as $scheme) {
    if ($path = portable_path_scheme_url($scheme, $absolute_path)) {
      // Return absolute or relative URL based on filter setting.
      $text = str_replace($scheme . '://', $path, $text);
    }
  }
  return $text;
}

/**
 * Filter settings callback.
 *
 * @deprecated Never invoked.
 */
function _portable_path_stream_wrapper_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  return portable_path_file_id_settings($form, $form_state, $filter, $format, $defaults, $filters);
}

/**
 * Filter tips callback.
 */
function _portable_path_stream_wrapper_tips($filter, $format, $long = FALSE) {
  $settings = portable_path_config();
  $stream_wrappers = implode(', ', $settings['stream_wrappers']);
  $field_types = implode(', ', $settings['field_types']); // Not displayed.
  $field_names = implode(', ', array_filter($settings['field_names']));
  $source = 'public://path-to-file/filename';
  $replacement = portable_path_scheme_url() . 'path-to-file/filename';
  if (!$long) {
    // This string will be shown in the content add/edit form.
    return t('URLs in stream wrapper notation, e.g. %source, will be replaced with full relative URLs, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%source' => $source, '%replacement' => $replacement));
  }
  else {
    return t('In %field_names fields, embedded URLs in <em>%stream_wrappers</em> stream wrapper notation, e.g. %source, will be replaced with full relative URLs, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
}

/**
 * Filter process callback.
 */
function _portable_path_remote_file_id_process($text, $filter, $format) {
  // Return an absolute URL to remote site.
  // This regular expression handles tokens with and without a query string, and
  // eliminates the need for a hook_update_N() to add ':' to existing tokens.
  // Use non-capturing group syntax '?:' on third colon so array sent to process
  // callback is unchanged.
  return preg_replace_callback('@\[portable:(\d+):(\w*)(?::(.*)|)\]@', 'portable_path_remote_file_id_process_callback', $text);
}

/**
 * Filter process callback.
 *
 * Callback for preg_replace_callback() within
 * _portable_path_remote_file_id_process().
 */
function portable_path_remote_file_id_process_callback($matches) {
  if (!portable_path_state_check()) {
    return $matches[0];
  }

  $key = RemoteContentController::$remoteContent;
  $connections = variable_get('remote_content_connections', array());
  if (!isset($connections[$key])) {
    return $matches[0];
  }

  $url = $connections[$key]['url'];
  $path = $connections[$key]['file_public_path'];

  if (isset($matches[1]) && $file = file_load($matches[1])) {
    // The file_load() query is run against the file_managed table on the remote
    // database to get url in stream wrapper format.
    $uri = $file->uri;
    if (!empty($matches[2])) {
      // Add image style pathing to uri.
      $uri = image_style_path($matches[2], $file->uri);
    }
    $query = !empty($matches[3]) ? '?' . $matches[3] : '';
    $target = file_uri_target($uri);
    $scheme = file_uri_scheme($uri);
    if ($scheme == 'public') {
      // Change to absolute URL to remote site.
      return $url . '/' . $path . '/' . $target . $query;
    }
  }
  return $matches[0];
}

/**
 * Filter settings callback.
 *
 * @todo This setting is not respected in process callbacks.
 */
function _portable_path_remote_file_id_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  global $databases;

  $filter->settings += $defaults;

  $elements['description'] = array(
    '#type' => 'item',
    '#description' => t('Check the box for each remote database connection to recognize.'),
    '#weight' => -5,
   );

  $weight = 0;
  $keys = array_diff(array_keys($databases), array('default'));
  foreach ($keys as $key) {
    $elements[$key] = array(
      '#type' => 'checkbox',
      '#title' => t('Convert file id tokens for "@key" connection', array('@key' => $key)),
      '#default_value' => isset($filter->settings[$key]) ? $filter->settings[$key] : FALSE,
      '#weight' => $weight++,
    );
  }

  return $elements;
}

/**
 * Filter tips callback.
 */
function _portable_path_remote_file_id_tips($filter, $format, $long = FALSE) {
  $settings = portable_path_config();
  $stream_wrappers = implode(', ', $settings['stream_wrappers']);
  $field_types = implode(', ', $settings['field_types']); // Not displayed.
  $field_names = implode(', ', array_filter($settings['field_names']));
  $source = '[portable:fid:style]';
  $replacement = 'http://www.example.com/sites/remote-site/files/styles/style-name/public/path-to-file/filename';
  if (!$long) {
    // This string will be shown in the content add/edit form.
    return t('For content from a remote site, tokens containing a file ID and any associated image style, e.g. %source, will be replaced with absolute URLs, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
  else {
    return t('For content from a remote site, in %field_names fields, embedded tokens containing the file ID of a managed file and any associated image style, e.g. %source, will be replaced with absolute URLs to the remote site, e.g. %replacement. The file URIs must be stored in <em>%stream_wrappers</em> stream wrapper notation.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
}

/**
 * Filter process callback.
 */
function _portable_path_remote_stream_wrapper_process($text, $filter, $format) {
  if (!portable_path_state_check()) {
    return $text;
  }

  $key = RemoteContentController::$remoteContent;
  $connections = variable_get('remote_content_connections', array());
  if (!isset($connections[$key])) {
    return $text;
  }

  $url = $connections[$key]['url'];
  $path = $connections[$key]['file_public_path'];

  $settings = portable_path_config();
  foreach (array_filter($settings['stream_wrappers']) as $scheme) {
    if ($scheme == 'public') {
      // Change to absolute URL to remote site.
      $text = str_replace($scheme . '://', $url . '/' . $path . '/', $text);
    }
  }
  return $text;
}

/**
 * Filter settings callback.
 *
 * @deprecated Never invoked.
 */
function _portable_path_remote_stream_wrapper_settings($form, &$form_state, $filter, $format, $defaults, $filters) {
  return portable_path_remote_file_id_settings($form, $form_state, $filter, $format, $defaults, $filters);
}

/**
 * Filter tips callback.
 *
 * @todo Edit this.
 */
function _portable_path_remote_stream_wrapper_tips($filter, $format, $long = FALSE) {
  $settings = portable_path_config();
  $stream_wrappers = implode(', ', $settings['stream_wrappers']);
  $field_types = implode(', ', $settings['field_types']); // Not displayed.
  $field_names = implode(', ', array_filter($settings['field_names']));
  $source = 'public://path-to-file/filename';
  $replacement = 'http://www.example.com/sites/remote-site/files/path-to-file/filename';
  if (!$long) {
    // This string will be shown in the content add/edit form.
    return t('For content from a remote site, URLs in stream wrapper notation, e.g. %source, will be replaced with absolute URLs, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%source' => $source, '%replacement' => $replacement));
  }
  else {
    return t('For content from a remote site, in %field_names fields, embedded URLs in <em>%stream_wrappers</em> stream wrapper notation, e.g. %source, will be replaced with absolute URLs to the remote site, e.g. %replacement.', array('%stream_wrappers' => $stream_wrappers, '%field_names' => $field_names, '%source' => $source, '%replacement' => $replacement));
  }
}

/**
 * Returns the absolute or relative URL associated with a stream wrapper scheme.
 *
 * @param string $scheme
 *   (optional) A stream wrapper scheme.
 * @param boolean $absolute_path
 *   (optional) Whether to return absolute or relative URL.
 *
 * @return string
 *   The relative form of the external URL associated with the scheme.
 */
function portable_path_scheme_url($scheme = 'public', $absolute_path = FALSE) {
  $path = '';
  if ($wrapper = file_stream_wrapper_get_instance_by_scheme($scheme)) {
    $path = $wrapper->getExternalUrl();
    if (!$absolute_path) {
      $path = str_replace($GLOBALS['base_root'], '', $path);
    }
  }
  return $path;
}

/**
 * Returns TRUE if conditions exist for remote file processing.
 *
 * @return boolean
 *   TRUE if conditions exist for remote file processing; FALSE otherwise.
 */
function portable_path_state_check() {
  if (!class_exists('RemoteContentController')) {
    return FALSE;
  }
  if (!RemoteContentController::$remoteContent) {
    return FALSE;
  }
  if (!module_exists('remote_content')) {
    // @todo If remote_content module is not enabled on remote site, then these
    // filters will not be invoked on local site (due to query run in
    // filter_list_format() which will use the remote connection).
    // If we ignore this condition, then the filters can be enabled on remote.
    return FALSE;
  }
  return TRUE;
}
