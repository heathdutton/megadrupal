<?php

/**
 * @file
 * Waywire Theme functions.
 */

/**
 * Theme function for viewing iframe video.
 *
 * @return string $output
 *   The iframe.
 */
function theme_waywire_video_iframe($variables) {

  $size = $variables['size'];
  $width = array_key_exists('width', $variables) && $variables['width'] ? $variables['width'] : NULL;
  $height = array_key_exists('height', $variables) && $variables['height'] ? $variables['height'] : NULL;
  $video_url = $variables['video_url'];
  $player = $variables['player'];
  // Remove these keys so they aren't added as parameters later.
  unset($variables['size']);
  unset($variables['width']);
  unset($variables['height']);
  unset($variables['video_url']);
  unset($variables['player']);

  // Get dimenstions.
  $waywire = new Waywire();
  $dimensions = $waywire->get_dimensions($size, $width, $height);

  // Set up paramaters.
  foreach ($variables as $key => $value) {
    $query[$key] = $value;
  }

  // Remove unwanted keys from query.
  if (isset($query['theme_hook_original'])) {
    unset($query['theme_hook_original']);
  }

  // Set up iframe url.
  $path = $video_url . '/player/' . $player;
  $src = url($path, array('query' => $query));

  $output = '<iframe src="' . $src . '" width="' . $dimensions['width'] . '" height="' . $dimensions['height'] . '" frameborder="0" scrolling="no" allowfullscreen></iframe>';

  if ($size == 'responsive') {
    drupal_add_css(drupal_get_path('module', 'waywire') . '/css/waywire.css');
    $output = '<div class="waywire-container--responsive">' . $output . '</div>';
  }

  return $output;
}
