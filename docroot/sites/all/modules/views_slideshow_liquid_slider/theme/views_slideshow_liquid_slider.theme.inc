<?php
/**
 * @file
 * views_slideshow_liquid_slider.theme.inc
 */

/**
 * Implements hook_preprocess_HOOK().
 */
function views_slideshow_liquid_slider_preprocess_views_slideshow_liquid_slider_main_frame(&$variables) {
  if ($library = _views_slideshow_liquid_slider_library_path()) {
    $module_path = drupal_get_path('module', 'views_slideshow_liquid_slider');

    $settings = $variables['settings'];
    $vss_id = $variables['vss_id'];
    $settings['vss_id'] = $vss_id;
    $library_path = libraries_get_path('liquidslider');

    drupal_add_css($library_path . '/css/liquid-slider.css');

    // Using Libraries 2.x $library would just be a boolean value because it
    // has already been loaded, however 1.x we will have to load it ourselves.
    if (is_string($library)) {
      drupal_add_js($library);
    }
    //drupal_add_js($module_path . '/js/jquery-1.8.2.min.js');
    //drupal_add_js($library_path . '/js/jquery.touchSwipe.min.js');
    //drupal_add_js($library_path . '/js/jquery.easing.1.3.js');
    //drupal_add_js($module_path . '/js/liquidslider.js');
    drupal_add_js(array('viewsSlideshowLiquidSlider' => array('#views_slideshow_liquid_slider_main_' . $vss_id => $settings)), 'setting');
  }
  else {
    watchdog('views_slideshow_liquid_slider', 'Unable to load jQuery Liquid Slider plugin.', array(), WATCHDOG_ERROR);
  }
}
