<?php
/**
 * @file
 * Theme & preprocess functions for the jQuery Carousel module.
 */

/**
 * Preprocessor for jquery carousel view style plugin.
 */
function _jquery_carousel_preprocess_jquery_carousel(&$vars) {
  _jquery_carousel_settings_format($vars['options']);
  _jquery_carousel_include_css_js($vars['options']);
  $vars['selector_class'] = drupal_attributes(
    array(
      'class' => array($vars['options']['selector']),
    )
  );
}
