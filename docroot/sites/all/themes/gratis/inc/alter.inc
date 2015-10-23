<?php

/**
 * @file
 * gratis custom alter functions.
 *
 * If you need to add or modify functions do it in your sub-theme.
 */

/**
 * Implements hook_html_head_alter().
 */
function gratis_html_head_alter(&$head_elements) {
  global $base_url;
  // Get our current uri.
  $uri = drupal_get_path_alias();

  // We try to match it by forming the right key with the info we have.
  $key = 'drupal_add_html_head_link:canonical:</' . $uri . '>;';

  // Check that it is set, then we re-set it to the correct full url.
  if (isset($head_elements[$key])) {
    // Alter our head_element.
    $head_elements[$key]['#attributes']['href'] = $base_url . '/' . $uri;
  }

  // Simplify the meta charset declaration.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements hook_page_alter().
 */
function gratis_page_alter($page) {
  // Define the module path for use below.
  $theme_path = drupal_get_path('theme', 'gratis');

  if (theme_get_setting('gratis_hacks_settings') == TRUE) {
    drupal_add_css($theme_path . '/css/hacks.css', array(
      'group' => CSS_THEME,
      'preprocess' => TRUE,
    ));
  }

  // Custom scripts file, minfied or regular.
  if (theme_get_setting('gratis_minifiedjs') == TRUE) {
    drupal_add_js($theme_path . '/js/site.min.js', array(
      'weight' => '999',
      'group' => JS_THEME,
    ));
  }
  else {
    drupal_add_js($theme_path . '/source/js/site.js', array(
      'weight' => '9999',
      'group' => JS_THEME,
    ));
  }

  // Look in each visible region for blocks.
  foreach (system_region_list($GLOBALS['theme'], REGIONS_VISIBLE) as $region => $name) {
    if (!empty($page[$region])) {
      // Find the last block in the region.
      $blocks = array_reverse(element_children($page[$region]));
      while ($blocks && !isset($page[$region][$blocks[0]]['#block'])) {
        array_shift($blocks);
      }
      if ($blocks) {
        $page[$region][$blocks[0]]['#block']->last_in_region = TRUE;
      }
    }
  }

  if (theme_get_setting('gratis_viewport') == FALSE) {
    // No pinch and zoom.
    $viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => 'width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no',
      ),
    );
    drupal_add_html_head($viewport, 'viewport');
  }

  else {
    // Pinch and Zoom enabled.
    $viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => 'width=device-width, initial-scale=1.0, maximum-scale=2.0, minimum-scale=0.55, user-scalable=yes',
      ),
    );
    drupal_add_html_head($viewport, 'viewport');
  }

  // Live reload.
  if (theme_get_setting('gratis_livereload') == TRUE) {
    // livereload.js
    $livereload_js = 'http://localhost:35729/livereload.js';
    drupal_add_js($livereload_js, array(
      'scope' => 'footer',
    ));
  }

}
