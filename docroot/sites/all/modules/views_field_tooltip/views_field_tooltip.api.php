<?php

/**
 * @file
 * Describe hooks provided by Views Field Tooltip module.
 */

/**
 * Provides information about supported tooltip libraries.
 *
 * @see views_field_tooltip_views_field_tooltip_library_info()
 * @return
 *   keyed array of entries describing each tooltip library:
 *   - key is the machine name of the library
 *   - 'name' is the human name of the library
 *   - 'help callback' is a function that returns a string describing typical settings for the given tooltip library
 *     @see views_field_tooltip__qtip_get_help()
 *   - 'needs local file' is a flag whether the admin settings should prompt for the path of a local JavaScript file for this library
 *   - 'attached' array of JS and CSS with the same format as Form API attribute '#attached'
 *     @see https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#attached
 */
function hook_views_field_tooltip_library_info() {
  $path = drupal_get_path('module', 'views_field_tooltip');
  return array(
    'qtip' => array(
      'name' => t('qTip'),
      'help callback' => 'views_field_tooltip__qtip_get_help',
      'needs local file' => TRUE,
      'attached' => array(
        'js' => array(
          $path . '/js/views_field_tooltip.qtip.js' => 'file',
        ),
      ),
    ),
    'qtip2' => array(
      'name' => t('qTip2'),
      'help callback' => 'views_field_tooltip__qtip2_get_help',
      'needs local file' => FALSE,
      'attached' => array(
        'js' => array(
          'http://cdn.jsdelivr.net/qtip2/2.2.0/jquery.qtip.min.js' => 'external',
          $path . '/js/views_field_tooltip.qtip2.js' => 'file',
        ),
        'css' => array(
          'http://cdn.jsdelivr.net/qtip2/2.2.0/jquery.qtip.min.css' => 'external',
          // CSS fixes for iframe tooltips.
          '.html .qtip { max-width: none; }
           .html .qtip-content { height: 100%; }
          ' => 'inline',
        ),
      ),
    ),
  );
}

/**
 * Alters information about tooltip libraries.
 *
 * @param $info
 *   Reference to library info array as described above.
 */
function hook_views_field_tooltip_library_info_alter(&$info) {
}
