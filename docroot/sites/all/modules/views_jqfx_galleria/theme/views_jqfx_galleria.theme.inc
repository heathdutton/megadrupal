<?php

/**
 * @file
 * Theme & preprocess functions for the Views jQFX: Galleria module.
 */

/**
 * @TODO.
 */
function template_preprocess_views_jqfx_galleria(&$vars) {
  // Initialize our $images array.
  $vars['images'] = array();

  // Strip the images from the $rows created by the original view query.
  foreach ($vars['rows'] as $item) {
    // For our images we'll look for them enclosed in anchor tags first.
    if (preg_match('@(<a.*?img.*?</a>)@i', $item, $matches)) {
      $image = $matches[1];
    }
    // If no anchor tags we'll just look for images.
    elseif (preg_match('@(<\s*img\s+[^>]*>)@i', $item, $matches)) {
      $image = $matches[1];
    }
    // If we have no images then we have no Galleria.
    else $image = NULL;
    $vars['images'][] = $image;
  }

  $options = $vars['options']['views_jqfx_galleria'];

  _views_jqfx_galleria_add_js($options, 'views-jqfx-galleria-images-' . $vars['id']);
}

/**
 * @TODO.
 */
function _views_jqfx_galleria_add_js($options, $id) {
  // Path to the plugin.
  $library_path = 'sites/all/libraries';

  // Add the plugin JS.
  if ($options['minify_js']) {
    drupal_add_js($library_path . '/galleria/galleria.min.js');
  }
  else {
    drupal_add_js($library_path . '/galleria/galleria.js');
  }

  // If the history option is chosen add its javascript
  if ($options['history']) {
    drupal_add_js($library_path . '/galleria/plugins/galleria.history.js');
  }

  // Add the module css.
  $drupal_path = drupal_get_path('module', 'views_jqfx_galleria');
  drupal_add_css($drupal_path . '/css/views_jqfx_galleria.css');

  // If the custom theme path is set then use it.
  if ($options['custom_theme'] && !$options['theme']) {
    $js_theme_url = $options['custom_theme'];
    $css_theme_url = str_replace('.js', '.css', $js_theme_url);
  }
  else {
    $js_theme_url = $library_path . '/galleria/themes/' . $options['theme'] . '/galleria.' . $options['theme'] . '.js';
    $css_theme_url = str_replace('.js', '.css', $js_theme_url);
  }

  // Add the theme css and js files to the head if manual load option or both is chosen.
  if (($options['galleria_loadtheme'] == 'manual') || ($options['galleria_loadtheme'] == 'both')) {
    drupal_add_css($css_theme_url);
    drupal_add_js($js_theme_url);
  }

  // Create the javascript loadTheme function to be added inline if either the galleria load or both option is chosen.
  if (($options['galleria_loadtheme'] == 'galleria') || ($options['galleria_loadtheme'] == 'both')) {
    $javascript = '(function ($) {Galleria.loadTheme("' . base_path() . $js_theme_url . '");})(jQuery);';
    drupal_add_js($javascript, array('type' => 'inline', 'scope' => 'footer', 'weight' => 4));
  }

  // Get an array of settings that we will pass to the Galleria
  $settings = array(
    'autoplay'                => (int) $options['autoplay'],
    'carousel'                => (bool) $options['carousel'],
    'carouselFollow'          => (bool) $options['carousel_follow'],
    'carouselSpeed'           => (int) $options['carousel_speed'],
    'carouselSteps'           => ($options['carousel_steps'] == 'auto') ? 'auto' : (int) $options['carousel_steps'],
    'clicknext'               => (bool) $options['clicknext'],
    'debug'                   => (bool) $options['debug'],
    'height'                  => ($options['height'] == 'auto') ? 'auto' : (int) $options['height'],
    'width'                   => ($options['width'] == 'auto') ? 'auto' : (int) $options['width'],
    'idleSpeed'               => (int) $options['idle_speed'],
    'idleTime'                => (int) $options['idle_time'],
    'imageMargin'             => (int) $options['image_margin'],
    'imagePan'                => (bool) $options['image_pan'],
    'imagePanSmoothness'      => (int) $options['image_pan_smoothness'],
    'imagePosition'           => check_plain($options['image_position']),
    'keepSource'              => (bool) $options['keep_source'],
    'lightbox'                => (bool) $options['lightbox'],
    'lightboxFadeSpeed'       => (int) $options['lightbox_fade_speed'],
    'lightboxTransitionSpeed' => (int) $options['lightbox_transition_speed'],
    'minScaleRatio'           => (int) $options['min_scale_ratio'],
    'maxScaleRatio'           => (int) $options['max_scale_ratio'],
    'overlayBackground'       => check_plain($options['overlay_background']),
    'overlayOpacity'          => (float) $options['overlay_opacity'],
    'pauseOnInteraction'      => (bool) $options['pause_on_interaction'],
    'popupLinks'              => (bool) $options['popup_links'],
    'preload'                 => ($options['preload'] == 'auto') ? 'auto' : (int) $options['preload'],
    'queue'                   => (bool) $options['queue'],
    'show'                    => (int) $options['show'],
    'showCounter'             => (bool) $options['show_counter'],
    'showImagenav'            => (bool) $options['show_imagenav'],
    'showInfo'                => (bool) $options['show_info'],
    'thumbFit'                => (bool) $options['thumb_fit'],
    'thumbMargin'             => (int) $options['thumb_margin'],
    'transition'              => $options['transition'],
    'transitionInitial'       => $options['transition_initial'],
    'transitionSpeed'         => (int) $options['transition_speed'],
  );

  // Image crop is true, false or by height or width.
  if ($options['image_crop'] == 'width') {
    $settings['imageCrop'] = 'width';
  }
  elseif ($options['image_crop'] == 'height') {
    $settings['imageCrop'] = 'height';
  }
  else {
    $settings['imageCrop'] = (bool) $options['image_crop'];
  }

  // Thumbnails is empty, numbers or boolean.
  if ($options['thumbnails'] == 'empty') {
    $settings['thumbnails'] = 'empty';
  }
  elseif ($options['thumbnails'] == 'numbers') {
    $settings['thumbnails'] = 'numbers';
  }
  else {
    $settings['thumbnails'] = (bool) $options['thumbnails'];
  }

  // Thumb crop is true, false or by height or width.
  if ($options['thumb_crop'] == 'width') {
    $settings['thumbCrop'] = 'width';
  }
  elseif ($options['thumb_crop'] == 'height') {
    $settings['thumbCrop'] = 'height';
  }
  else {
    $settings['thumbCrop'] = (bool) $options['thumb_crop'];
  }

  // These will break the Galleria if they are passed empty. Make sure they are set first.
  if ($options['data_selector']) {
    $settings['dataSelector'] = $options['data_selector'];
  }
  if ($options['data_source']) {
    $settings['dataSource'] = $options['data_source'];
  }
  if ($options['data_config']) {
    $settings['dataConfig'] = $options['data_config'];
  }
  if ($options['extend']) {
    $settings['extend'] = $options['extend'];
  }

  // Pass the javascript settings.
  drupal_add_js(array('viewsJqfxGalleria' => array($id => $settings)), 'setting');

  // Add the module javascript with the init function in the footer.
  drupal_add_js($drupal_path . '/js/views_jqfx_galleria.js', array('scope' => 'footer', 'weight' => 5));
}
