<?php
/**
 * @file
 * views_media_browser.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function views_media_browser_views_default_views() {
  // See which version of the Media module we're running.
  $media_version = views_media_browser_get_media_version();

  $view = NULL;
  // Load the correct version of the view based on the version of the media module.
  if ($media_version >= 2) {
    include_once(drupal_get_path('module', 'views_media_browser') . '/includes/views/views_media_browser_2x.inc');
  }
  else {
    include_once(drupal_get_path('module', 'views_media_browser') . '/includes/views/views_media_browser_1x.inc');
  }

  $export = array();
  if (!empty($view)) {
    $export['views_media_browser'] = $view;
  }

  return $export;
}

