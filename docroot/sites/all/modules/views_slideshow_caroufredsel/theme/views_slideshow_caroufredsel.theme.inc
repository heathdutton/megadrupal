<?php

/**
 * @file
 * Theme & preprocess functions for the Views Slideshow: caroufredsel module.
 */

/**
 * Views Slideshow: Theme the main frame wrapper.
 *
 * @ingroup themeable
 */
function _views_slideshow_caroufredsel_preprocess_views_slideshow_caroufredsel_main_frame(&$vars) {
  $settings = $vars['settings'];
  $rows = $vars['rows'];
  $view = $vars['view'];
  $vss_id = $vars['vss_id'];

  // Cast the strings into int or bool as necessary.
  $new_settings = _views_slideshow_caroufredsel_prepare_settings($settings);

  $settings = array_merge(
    array(
      'num_divs' => sizeof($rows),
      'vss_id' => $vss_id,
    ),
    $new_settings
  );

  // We need to go through the current js setting values to make sure the one we
  // want to add is not already there. If it is already there then append -[num]
  // to the id to make it unique.
  $slideshow_count = 1;
  $current_settings = drupal_add_js();
  foreach ($current_settings['settings']['data'] AS $current_setting) {
    if (isset($current_setting['viewsSlideshowcaroufredsel'])) {
      $current_keys = array_keys($current_setting['viewsSlideshowcaroufredsel']);
      if (stristr($current_keys[0], $vss_id)) {
        $slideshow_count++;
      }
    }
  }

  if ($slideshow_count > 1) {
    $vss_id .= '-' . $slideshow_count;
    $settings['vss_id'] = $vss_id;
  }

    // Don't load javascript unless libraries module is present.
  if (module_exists('libraries')) {
    // Load jQuery caroufredsel
    if ($caroufredsel_path = _views_slideshow_caroufredsel_library_path()) {
      drupal_add_js($caroufredsel_path);
      // Load our caroufredsel js
      $module_path = drupal_get_path('module', 'views_slideshow_caroufredsel');
      drupal_add_js($module_path . '/js/views_slideshow_caroufredsel.js');
      drupal_add_js(array('viewsSlideshowcaroufredsel' => array($vss_id => $settings)), 'setting');
    }
  }

  $styles = '';
  if (isset($view->display_handler->display->display_options['style_options']['views_slideshow_caroufredsel'])) {
    $styles = $view->display_handler->display->display_options['style_options']['views_slideshow_caroufredsel'];
  }

  $styles_default = '';
  if (isset($view->display['default']->display_options['style_options']['views_slideshow_caroufredsel'])) {
    $styles_default = $view->display['default']->display_options['style_options']['views_slideshow_caroufredsel'];
  }

  // Retrive the number of items per frame
  if (isset($styles['items_per_slide']) && $styles['items_per_slide'] > 0) {
    $items_per_slide = $styles['items_per_slide'];
  }
  elseif (isset($styles_default['items_per_slide']) && $styles_default['items_per_slide'] > 0) {
    $items_per_slide = $styles_default['items_per_slide'];
  }
  else {
    $items_per_slide = 1;
  }

  $vars['items_per_slide'] = $items_per_slide;

  $items = array();
  $slideshow_count = 0;
  $rendered_rows = '';
  foreach ($rows as $count => $item) {
    $items[] = $item;
    if (count($items) == $items_per_slide || $count == (count($rows)-1)) {
      $rendered_rows .= theme('views_slideshow_caroufredsel_main_frame_row', array('vss_id' => $vss_id, 'items' => $items, 'count' => $slideshow_count, 'view' => $vars['view']));
      $items = array();
      $slideshow_count++;
    }
  }

  $vars['rendered_rows'] = $rendered_rows;
}

/**
 * Views Slideshow slideshow rows.
 *
 * @ingroup themeable
 */
function _views_slideshow_caroufredsel_preprocess_views_slideshow_caroufredsel_main_frame_row(&$vars) {

  $vars['rendered_items'] = '';
  foreach ($vars['items'] as $item_count => $item) {
    $vars['rendered_items'] .= theme('views_slideshow_caroufredsel_main_frame_row_item', array('item' => $item, 'item_count' => $item_count, 'view' => $vars['view']));
  }
}

function _views_slideshow_caroufredsel_preprocess_views_slideshow_caroufredsel_main_frame_row_item(&$vars) {
  /**
   * Support custom row classes.
   */
  if ($row_class = $vars['view']->style_plugin->get_row_class($vars['item_count'])) {
    $vars['classes_array'][] = $row_class;
  }
}
