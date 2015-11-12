<?php

/**
 * @file
 * Theme and preprocess functions for Flexslider Views
 *
 * @author Mathew Winstone (minorOffense) <mwinstone@coldfrontlabs.ca>
 */

/**
 * Swiper Views theme for the main wrapper element
 *
 * @ingroup themeable
 */
function _views_slideshow_swiper_preprocess_views_slideshow_swiper_main_frame(&$vars) {
  // @todo see if we can get this to call theme('swiperjs') instead.
  // Load the settings, row information, view data and views slideshow ID into
  // more conveniently named variables.
  $settings = $vars['settings'];
  $rows = $vars['rows'];
      $view = $vars['view'];
  $vss_id = $vars['vss_id'];

  // Cast the strings into int or bool as necessary
  $new_settings = array();
  foreach ($settings as $key => $value) {
    if (is_string($value)) {

      $value = str_ireplace("\n", ' ', $value);
      $value = trim($value);

      if (is_numeric($value)) {
        $value = (int)$value;
      }
      elseif (drupal_strtolower($value) == 'true') {
        $value = TRUE;
      }
      elseif (drupal_strtolower($value) == 'false') {
        $value = FALSE;
      }
    }

    $new_settings[$key] = $value;
  }

  _views_slideshow_swiper_remove_default_values($new_settings);
  if (isset($new_settings['slidesPerView']) && ($new_settings['slidesPerView'] === 0)) {
    $new_settings['slidesPerView'] = 'auto';
  }
  if (isset($new_settings['autoplay']) && ($new_settings['autoplay'] === 0)) {
    unset($new_settings['autoplay']);
  }
  

  // Merge the existing settings with the generated ones
  $settings = array(
    'num_divs' => sizeof($rows),
    'id_prefix' => '#views_slideshow_swiper_main_',
    //'div_prefix' => '#'
    // @todo figure out what the div prefix is used for
    'vss_id' => $vss_id,
    'options' => $new_settings,
  );

  // We need to go through the current js setting values to make sure the one we
  // want to add is not already there. If it is already there then append -[num]
  // to the id to make it unique.
  $slideshow_count = 1;
  $current_settings = drupal_add_js();
  foreach ($current_settings['settings']['data'] AS $current_setting) {
    if (isset($current_setting['views_slideshow_swiper'])) {
      $current_keys = array_keys($current_setting['views_slideshow_swiper']);
      if (stristr($current_keys[0], '#views_slideshow_swiper_main_' . $vss_id)) {
        $slideshow_count++;
      }   
    }
  }

  // Append the new unique IDs if required
  if ($slideshow_count > 1) {
    $vss_id .= '-' . $slideshow_count;
    $settings['vss_id'] = $vss_id;
  }

  // Load the Swiper library for use.
  libraries_load('Swiper');

  // Create the settings container
  // Due to a limitation inside of views_slideshow, the ID for the settings container
  // must end with _main
  //
  // see lines 162 through 169 in views_slideshow.theme.inc
  drupal_add_js(array('viewsSlideshowSwiper' => array('#views_slideshow_swiper_main_' . $vss_id => $settings)), 'setting');

  // Load the initilization javascript
  drupal_add_js(drupal_get_path('module', 'views_slideshow_swiper') . '/js/views_slideshow_swiper.js');

  // Required container class for Swiper
  $vars['classes_array'][] = 'swiper-wrapper';

  // Render the rows
  $rendered_rows = '';
  $slideshow_count = 0;
  foreach ($rows as $count => $row) {
    $items[] = $row;
    // @todo see if we can group multiple items per slide
    $rendered_rows .= theme('views_slideshow_swiper_main_frame_row', array('vss_id' => $vss_id, 'items' => $items, 'count' => $count, 'view' => $view));

    // Clear the items array
    $items = array();
    // Count the number of slides created
    $slideshow_count++;
  }

  // Save the rendered rows
  $vars['rendered_rows'] = $rendered_rows;
}

/**
 * Swiper Views Slideshow theme for the row element
 *
 * @ingroup themeable
 */
function _views_slideshow_swiper_preprocess_views_slideshow_swiper_main_frame_row(&$vars) {
  $current = $vars['count'] + 1;
  $vars['classes_array'][] = 'swiper-slide views-row-' . $current;
  $vars['data_hash'] = 'slide' . $current;

  // @todo figure out why this if statement is here
  if ($vars['count']) {
    $vars['classes_array'][] =  'views_slideshow_swiper_hidden';
  }
  // @todo add option to toggle views rows counters on/off
  $vars['classes_array'][] = ($vars['count'] % 2) ? 'views-row-even' : 'views-row-odd';

  $vars['rendered_items'] = '';
  foreach ($vars['items'] as $item_count => $item) {
    $vars['rendered_items'] .= theme('views_slideshow_swiper_main_frame_row_item', array('item' => $item, 'item_count' => $item_count, 'view' => $vars['view']));
  }
}

/**
 * Swiper Views Slideshow theme for an item within a row
 *
 * @ingroup themeable
 */
function _views_slideshow_swiper_preprocess_views_slideshow_swiper_main_frame_row_item(&$vars) {
  $vars['classes_array'][] = 'views-row views-row-' . $vars['item_count'];
  if (!$vars['item_count']) {
    $vars['classes_array'][] = 'views-row-first';
  }
  if ($vars['item_count'] % 2) {
    $vars['classes_array'][] = 'views-row-even';
  }
  else {
    $vars['classes_array'][] = 'views-row-odd';
  }

  /**
   * Support custom row classes.
   */
  if ($row_class = $vars['view']->style_plugin->get_row_class($vars['item_count'])) {
    $vars['classes_array'][] = $row_class;
  }
}
