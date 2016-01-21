<?php

/**
 * @file
 * Hooks provided by the Nivo Lightbox module.
 *
 * Modules and themes may implement any of the available hooks to interact with
 * the lightbox.
 */

/**
 * Register lightbox themes.
 *
 * This hook can be used to register themes for the lightbox. Themes will be
 * displayed and made selectable when configuring the formatter.
 *
 * Lightbox themes get a unique CSS class to use for styling and can specify an
 * unlimited number of CSS and JS files to include when the lightbox is
 * displayed.
 */
function hook_nivo_lightbox_theme_info() {
  return array(
    'theme_name' => array(
      'name' => t('Theme name'), // Human readable theme name
      'description' => t('Theme description.'), // Description of the theme
      'resources' => array(
        'css' => array(
          drupal_get_path('module', 'module_name') . '/css/example.css', // Full path to a CSS file to include with the theme
          drupal_get_path('module', 'module_name') . '/css/demonstration.css',
        ),
        'js' => array(
          drupal_get_path('module', 'module_name') . '/js/example.css', // Full path to a JS file to include with the theme
          drupal_get_path('module', 'module_name') . '/js/demonstration.css',
        ),
      ),
    )
  );
}

/**
 * Alter lightbox themes.
 *
 * @param $themes
 *   The associative array of theme information from
 *   hook_nivo_lightbox_theme_info().
 *
 * @see hook_nivo_lightbox_theme_info()
 */
function hook_nivo_lightbox_theme_info_alter(&$themes) {
  // Modify the default theme's name and description
  $themes['default']['name'] = t('My theme');
  $themes['default']['description'] = t('An excellent theme to appropriate for your own use!');

  // Replace the default theme styling
  $themes['default']['resources']['css'] = drupal_get_path('module', 'my_module') . '/my_theme.css';
}
