<?php
/**
 * @file
 * Showoff Feeds theme implementations.
 */

/**
 * Generates the output for the feed.
 */
function theme_showoff_feed($variables) {
  if (isset($variables['feed']) && isset($variables['region'])) {
    $view = views_get_view($variables['feed']['view']);
    $url = url($view->display[$variables['feed']['display']]->display_options['path'], array('absolute' => TRUE));
    $js = '(function ($) { $(document).ready(function() { $.fn.rotateFeed("' . $variables['region'] . '", "' . $url . '"); }); })(jQuery);';
    drupal_add_js($js, 'inline');
  }
  return "";
}

/**
 *
 */
function theme_showoff_layout($variables) {
  global $user;
  if (isset($variables['layout'])) {
      $layout = showoff_feeds_get_layout($variables['layout']);
      $file = file_get_contents($layout['path']);
      $file = check_markup($file, 'full_html');
      drupal_add_js(drupal_get_path('module', 'showoff_feeds') . '/js/showoff_feeds.feed_rotator.js', 'file');
      foreach ($variables['regions'] as $region => $view_data) {
        $file .= theme('showoff_feed', array('feed' => $view_data, 'region' => $region));
      }
      return $file;
  }
  return "";
}
