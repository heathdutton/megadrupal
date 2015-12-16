<?php
/**
 * @file
 * Google Analytics Vimeo Video Tracking inc.
 */

/**
 * Pass admin config values to Drupal js.
 */
function googleanalytics_vimeo_admin_config_js() {
  $opts = array_filter(variable_get('googleanalytics_vimeo_tracking_options', array()));
  drupal_add_js(array(
    'googleanalytics_vimeo' => array(
      'progress25' => isset($opts['googleanalytics_vimeo_progress25']),
      'progress50' => isset($opts['googleanalytics_vimeo_progress50']),
      'progress75' => isset($opts['googleanalytics_vimeo_progress75']),
      'videoPlayed' => isset($opts['googleanalytics_vimeo_videoPlayed']),
      'videoPaused' => isset($opts['googleanalytics_vimeo_videoPaused']),
      'videoResumed' => isset($opts['googleanalytics_vimeo_videoResumed']),
      'videoSeeking' => isset($opts['googleanalytics_vimeo_videoSeeking']),
      'videoCompleted' => isset($opts['googleanalytics_vimeo_videoCompleted']),
    ),
  ),
  'setting'
  );
}
/**
 * Based on visibility setting this function returns TRUE/FALSE.
 */
function googleanalytics_vimeo_visibility_pages() {
  static $page_match;
  $php_access = user_access('use PHP for vimeo tracking visibility');

  // Cache visibility result if function is called more than once.
  if (!isset($page_match)) {

    $visibility = variable_get('googleanalytics_vimeo_visibility_options', 0);
    $setting_pages = variable_get('googleanalytics_vimeo_visibility_pages', GOOGLEANALYTICS_PAGES);

    // Match path if necessary.
    if (!empty($setting_pages)) {
      // Convert path to lowercase. This allows comparison of the same path
      // with different case. Ex: /Page, /page, /PAGE.
      $pages = drupal_strtolower($setting_pages);
      if ($visibility < 2) {
        // Convert the Drupal path to lowercase.
        $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
        // Compare the lowercase internal and lowercase path alias (if any).
        $page_match = drupal_match_path($path, $pages);
        if ($path != current_path()) {
          $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
        }
        // When $visibility has a value of 0, the tracking code is displayed on
        // all pages except those listed in $pages. When set to 1, it
        // is displayed only on those pages listed in $pages.
        $page_match = !($visibility xor $page_match);
      }
      elseif (module_exists('php') && $php_access) {
        $page_match = php_eval($setting_pages);
      }
      else {
        $page_match = FALSE;
      }
    }
    else {
      $page_match = TRUE;
    }

  }
  return $page_match;
}
