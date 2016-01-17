<?php
/**
 * @file
 *  Provides various callbacks of allowed values to be used in the fields/instances controlled by the media_nivo_slider module.
 */

/**
 * Allowed values callback for media_nivo_slider_block list field.
 */
function _media_nivo_slider_get_block_values() {
  return array(
    0 => t('Do not create a nivo slider block for this gallery.'),
    1 => t('Create a Nivo Slider block for this gallery.'),
  );
}

/**
 * Allowed values callback for media_nivo_slider_effect list field.
 */
function _media_nivo_slider_get_effect_values() {
  return array(
    'sliceDown' => t('Slice Down'),
    'sliceDownLeft' => t('Slice Down Left'),
    'sliceUp' => t('Slice Up'),
    'sliceUpLeft' => t('Slice Up Left'),
    'sliceUpDown' => t('Slice Up Down'),
    'sliceUpDownLeft' => t('Slice Up Down Left'),
    'fold' => t('Fold'),
    'fade' => t('Fade'),
    'slideInRight' => t('Slide In Right'),
    'slideInLeft' => t('Slide In Left'),
    'boxRandom' => t('Box Random'),
    'boxRain' => t('Box Rain'),
    'boxRainReverse' => t('Box Rain Reverse'),
    'boxRainGrow' => t('Box Rain Grow'),
    'boxRainGrowReverse' => t('Box Rain Grow Reverse'),
    'random' => t('Random'),
  );
}

/**
 * Allowed values callback for media_nivo_slider_image_style field.
 */
function _media_nivo_slider_get_image_style_values() {
  $allowed = &drupal_static(__FUNCTION__);
  
  if (!isset($allowed)) {
    if ($cache = cache_get('media_nivo_slider_image_styles')) {
      $allowed = $cache->data;
    }
    else {
      // Add option for no style.
      $allowed['_none'] = '- None -';
    
      // Get a list of the available image styles.
      $styles = array_keys(image_styles());
      
      // Format the available styles into a usable array.
      foreach ($styles as $key) {
        $allowed[$key] = $key;
      }
      
      // Cache the results
      cache_set('media_nivo_slider_image_styles', $allowed, 'cache');
    }
  }
  
  return $allowed;
}

/**
 * Allowed values callback for media_nivo_slider_effect list field.
 */
function _media_nivo_slider_get_theme_values() {
  $themes = &drupal_static(__FUNCTION__);

  if (!isset($themes)) {
    if ($cache = cache_get('media_nivo_slider_themes')) {
      $themes = $cache->data;
    }
    else {
      // Scan the libraries theme folder for nivo slider theme style sheets.
      $library_path = libraries_get_path('nivo-slider');
      $theme_files = file_scan_directory($library_path . '/themes', '/.*\.css$/');

      $themes = array();
      foreach ($theme_files as $theme_file) {
        $name = $theme_file->name;
        $themes[$name] = t($name);
      }
      
      // Cache the results
      cache_set('media_nivo_slider_themes', $themes, 'cache');
    }
  }

  return $themes;
}

/**
 * Allowed values callback for media_nivo_slider_config list field.
 */
function _media_nivo_slider_get_configs() {
  $presets = &drupal_static(__FUNCTION__);
  
  if (!isset($presets)) {
    if ($cache = cache_get('media_nivo_slider_presets')) {
      $presets = $cache->data;
    }
    else {
      $presets = array();
      
      $results = db_select('media_nivo_slider', 'ns')
        ->fields('ns', array('cid', 'name'))
        ->execute();
      
      foreach ($results as $preset) {
        $presets[$preset->cid] = $preset->name;
      }
      
      // Cache the results
      cache_set('media_nivo_slider_presets', $presets, 'cache');
    }
  }

  return $presets;
}

