<?php

/**
 * @file
 * Theme & preprocess functions for the Views jQFX: Supersized module.
 */

/**
 * Preprocess mainly to extract image and title fields from views output.
 */
function template_preprocess_views_jqfx_supersized(&$vars) {
  // Initialize our $images array.
  $vars['images'] = array();
  // Strip all images from the $rows created by the original view query.
  foreach ($vars['rows'] as $item) {

    $doc = new DOMDocument();
    $doc->loadHTML('<?xml encoding="UTF-8">' . $item);
    // Just to make xpath more simple.
    $xml = simplexml_import_dom($doc);
    $images = $xml->xpath('//img');
    $title = '';
    foreach ($images as $img) {
      $title = check_plain($img['title']);
    }


    if (preg_match('@(<\s*img\s+[^>]*>)@i', $item, $matches)) {
      $image = $matches[1];
      // Get the image src.
      if (preg_match("/\< *[img][^\>]*[src] *= *[\"\']{0,1}([^\"\'\ >]*)/i", $image, $match)) {
        $imgsrc = $match[1];
        $thumbsrc = str_replace('/background_image/', '/background_image_thumb/', $imgsrc);
      }
      // If our image is in an anchor tag, use its URL.
      if (preg_match('@<\s*a\s+href\s*=\s*"\s*([^"]+)\s*"[^>]*>[^<]*' . preg_quote($image) . '[^<]*<\s*/a\s*>@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Or look to see if we have any other links in our views items.
      elseif (preg_match('@href\s*=\s*"([^"]+)"@i', $item, $urls)) {
        $url = $urls[1];
      }
      // Otherwise we have no $url.
      else {
        $url = NULL;
      }

      // Our image gets wrapped with the $url anchor if it is present.
      if ($url) {
        $image = "{image : '" . $imgsrc . "', title : '" . $title . "', url : '" . $url . "', thumb : '" . $thumbsrc . "' }, ";
      }
      else {
        $image = "{image : '" . $imgsrc . "', title : '" . $title . "', thumb : '" . $thumbsrc . "', url : '' }, ";
      }

      // Add the image to our image array to display.
      $vars['images'][] = $image;
    }
  }
  // Build our slides 'array'.
  $options = $vars['options']['views_jqfx_supersized'];
  $options['slides'] = '[ ';
  foreach ($vars['images'] as $image) {
    $options['slides'] .= $image;
  }
  $options['slides'] = trim($options['slides'], ', ');
  $options['slides'] .= ' ]';

  _views_jqfx_supersized_add_js($options, 'views-jqfx-supersized-images-' . $vars['id']);
}

/**
 *  Builds the javascript to add to the page. Using settings from views.
 */
function _views_jqfx_supersized_add_js($options, $id) {

  // Get the path to our plugin.
  $path = libraries_get_path('supersized') . '/slideshow';

  // Load the plugin javascript and css.
  drupal_add_js($path . '/js/jquery.easing.min.js');

  if ($options['pack_js']) {
    drupal_add_js($path . '/js/supersized.3.2.7.min.js');
  }
  else {
    drupal_add_js($path . '/js/supersized.3.2.7.js');
  }

  if ($options['ss_theme'] == 'shutter') {
    if ($options['pack_js']) {
      drupal_add_js($path . '/theme/supersized.shutter.min.js');
    }
    else {
      drupal_add_js($path . '/theme/supersized.shutter.js');
    }
  }

  drupal_add_css($path . '/css/supersized.css');

  // Load the theme css.
  drupal_add_css($path . "/theme/supersized.shutter.css");

  // Add the module js.
  $drupal_path = drupal_get_path('module', 'views_jqfx_supersized');
  drupal_add_js($drupal_path . '/js/views_jqfx_supersized.js');

  // Get the javascript settings.
  $settings = array(
    // Display stuff.
    'start_slide' => (int) $options['start_slide'],
    'slideshow' => (bool) $options['slideshow'],
      // Theme Stuff.
    'progress_bar' => (bool) $options['theme']['progress_bar'],
      // Size & Position Stuff.
    'min_width' => (int) $options['size']['min_width'],
    'min_height' => (int) $options['size']['min_height'],
    'vertical_center' => (bool) $options['size']['vertical_center'],
    'horizontal_center' => (bool) $options['size']['horizontal_center'],
    'fit_always' => (bool) $options['size']['fit_always'],
    'fit_portrait' => (bool) $options['size']['fit_portrait'],
    'fit_landscape' => (bool) $options['size']['fit_landscape'],
    // Transition stuff.
    'autoplay' => (bool) $options['transition']['autoplay'],
    'transition' => (int) $options['transition']['transition'],
    'transition_speed' => (int) $options['transition']['transition_speed'],
    'slide_interval' => (int) $options['transition']['slide_interval'],
    'random' => (bool) $options['transition']['random'],
    'stop_loop' => (bool) $options['transition']['stop_loop'],
    // Navigate stuff.
    'keyboard_nav' => (bool) $options['navigation']['keyboard_nav'],
    'slide_links' => $options['navigation']['slide_links'],
    'thumb_links' => (bool) $options['navigation']['thumb_links'],
    'thumbnail_navigation' => (bool) $options['navigation']['thumbnail_navigation'],
    'pause_hover' => (bool) $options['navigation']['pause_hover'],
  );

  if (isset($_GET['gallery_page'])) {
    $settings['start_slide'] = (int) $_GET['gallery_page'];
  }

  // Setup settings in format expected by plugin.
  // Tried to add using drupal_add_js 'settings' but the
  // sanitization was messing with supersized.
  $customsettings = '';
  foreach ($settings as $key => $value) {
    switch ($value) {
      case $value === 'FALSE':
        $customsettings .= $key . " : 0 , \n";
        break;

      case $value === TRUE:
        $customsettings .= $key . " : 1 , \n";
        break;

      case is_int($value):
        $customsettings .= $key . " : " . $value . ", \n";
        break;

      case is_string($value):
        $customsettings .= $key . " : '" . $value . "', \n";
        break;

    }
  }
  $customsettings = trim($customsettings, ', ');

  // Add our Settings & Slides, initialize the slideshow.
  drupal_add_js('jQuery(function($) {
  $.supersized({ ' . $customsettings . ' slides : '
    . $options['slides'] . ' }); });',
    array('type' => 'inline', 'scope' => 'header', 'group' => 'JS_THEME'));
}
