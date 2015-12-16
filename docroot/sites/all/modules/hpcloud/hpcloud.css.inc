<?php
/**
 * @file
 *
 * This was cloned, with minimal alteration, from the CDN module. Its sole
 * purpose is to work around Drupal's relative paths for CSS files.
 *
 * basically, all of this is done for the one modification of adding
 * file_create_path() to the path (the very end of this file).
 *
 * Original author: Wim Leers.
 *
 * Overrides of Drupal's CSS aggregation system. Ensures that files referenced
 * by CSS files are also served from the CDN, according to the CDN module's
 * CSS aggregation rules.
 */


/**
 * Near-identical to @see drupal_aggregate_css().
 *
 * Changes: call _hpcloud_build_css_cache() instead of drupal_build_css_cache().
 */
function _hpcloud_aggregate_css(&$css_groups) {
  $preprocess_css = (variable_get('preprocess_css', FALSE) && (!defined('MAINTENANCE_MODE') || MAINTENANCE_MODE != 'update'));

  // For each group that needs aggregation, aggregate its items.
  foreach ($css_groups as $key => $group) {
    switch ($group['type']) {
      // If a file group can be aggregated into a single file, do so, and set
      // the group's data property to the file path of the aggregate file.
      case 'file':
        if ($group['preprocess'] && $preprocess_css) {
          $css_groups[$key]['data'] = _hpcloud_build_css_cache($group['items']);
        }
        break;
      // Aggregate all inline CSS content into the group's data property.
      case 'inline':
        $css_groups[$key]['data'] = '';
        foreach ($group['items'] as $item) {
          $css_groups[$key]['data'] .= drupal_load_stylesheet_content($item['data'], $item['preprocess']);
        }
        break;
    }
  }
}

/**
 * Near-identical to @see drupal_build_css_cache().
 *
 * Changes:
 * - the inner loop is modified
 * - uses _hpcloud_build_css_path() instead of drupal_build_css_path().
 */
function _hpcloud_build_css_cache($css) {
  $data = '';
  $uri = '';
  $map = variable_get('drupal_css_cache_files', array());
  $key = hash('sha256', serialize($css));
  if (isset($map[$key])) {
    $uri = $map[$key];
  }

  if (empty($uri) || !file_exists($uri)) {
    // Build aggregate CSS file.
    foreach ($css as $stylesheet) {
      // Only 'file' stylesheets can be aggregated.
      if ($stylesheet['type'] == 'file') {
        $contents = drupal_load_stylesheet($stylesheet['data'], TRUE);

        // Get the parent directory of this file, relative to the Drupal root.
        $css_base_url = substr($stylesheet['data'], 0, strrpos($stylesheet['data'], '/'));
        _hpcloud_build_css_path(NULL, $css_base_url . '/');

        // Anchor all paths in the CSS with its base URL, ignoring external and absolute paths.
        $data .= preg_replace_callback('/url\(\s*[\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\s*\)/i', '_hpcloud_build_css_path', $contents);
      }
    }

    // Per the W3C specification at http://www.w3.org/TR/REC-CSS2/cascade.html#at-import,
    // @import rules must proceed any other style, so we move those to the top.
    $regexp = '/@import[^;]+;/i';
    preg_match_all($regexp, $data, $matches);
    $data = preg_replace($regexp, '', $data);
    $data = implode('', $matches[0]) . $data;

    // Prefix filename to prevent blocking by firewalls which reject files
    // starting with "ad*".
    $filename = 'css_' . drupal_hash_base64($data) . '.css';
    // Create the css/ within the files folder.
    $csspath = 'public://css';
    $uri = $csspath . '/' . $filename;
    // Create the CSS file.
    file_prepare_directory($csspath, FILE_CREATE_DIRECTORY);
    if (!file_exists($uri) && !file_unmanaged_save_data($data, $uri, FILE_EXISTS_REPLACE)) {
      return FALSE;
    }
    // If CSS gzip compression is enabled, clean URLs are enabled (which means
    // that rewrite rules are working) and the zlib extension is available then
    // create a gzipped version of this file. This file is served conditionally
    // to browsers that accept gzip using .htaccess rules.
    if (variable_get('css_gzip_compression', TRUE) && variable_get('clean_url', 0) && extension_loaded('zlib')) {
      if (!file_exists($uri . '.gz') && !file_unmanaged_save_data(gzencode($data, 9, FORCE_GZIP), $uri . '.gz', FILE_EXISTS_REPLACE)) {
        return FALSE;
      }
    }
    // Save the updated map.
    $map[$key] = $uri;
    variable_set('drupal_css_cache_files', $map);
  }
  return $uri;
}

/**
 * Near-identical to @see drupal_build_css_path().
 *
 * Changes: apply file_create_url() to every file!
 */
function _hpcloud_build_css_path($matches, $base = NULL) {
  $_base = &drupal_static(__FUNCTION__);
  // Store base path for preg_replace_callback.
  if (isset($base)) {
    $_base = $base;
  }

  // Prefix with base and remove '../' segments where possible.
  $path = $_base . $matches[1];
  $last = '';
  while ($path != $last) {
    $last = $path;
    $path = preg_replace('`(^|/)(?!\.\./)([^/]+)/\.\./`', '$1', $path);
  }

  return 'url(' . _hpcloud_file_create_url($path) . ')';
}

/**
 * Near identical to file_create_url with the exception that handling of Drupal
 * internal paths are not run through drupal_encode_path. If that were to happen
 * query strings on css elements would be encoded causing them not to work.
 */
function _hpcloud_file_create_url($uri) {
  // Allow the URI to be altered, e.g. to serve a file from a CDN or static
  // file server.
  drupal_alter('file_url', $uri);

  $scheme = file_uri_scheme($uri);

  if (!$scheme) {
    // Allow for:
    // - root-relative URIs (e.g. /foo.jpg in http://example.com/foo.jpg)
    // - protocol-relative URIs (e.g. //bar.jpg, which is expanded to
//   http://example.com/bar.jpg by the browser when viewing a page over
//   HTTP and to https://example.com/bar.jpg when viewing a HTTPS page)
    // Both types of relative URIs are characterized by a leading slash, hence
    // we can use a single check.
    if (drupal_substr($uri, 0, 1) == '/') {
      return $uri;
    }
    else {
      // If this is not a properly formatted stream, then it is a shipped file.
      // Therefore, return the urlencoded URI with the base URL prepended.
      return $GLOBALS['base_url'] . '/' . $uri;
    }
  }
  elseif ($scheme == 'http' || $scheme == 'https') {
    // Check for http so that we don't have to implement getExternalUrl() for
    // the http wrapper.
    return $uri;
  }
  else {
    // Attempt to return an external URL using the appropriate wrapper.
    if ($wrapper = file_stream_wrapper_get_instance_by_uri($uri)) {
      return $wrapper->getExternalUrl();
    }
    else {
      return FALSE;
    }
  }
}
