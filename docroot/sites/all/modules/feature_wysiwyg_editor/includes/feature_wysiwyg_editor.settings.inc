<?php

/**
 * @param $settings
 * @param $context
 */
function feature_wysiwyg_editor_wysiwyg_editor_settings_alter(&$settings, $context) {
  global $base_url;

  if ($context['profile']->editor == 'ckeditor') {

    // Example of setting customConfig option. Just note it gets overridden by values in $settings.
    $settings['customConfig'] = '/' . drupal_get_path('module','feature_wysiwyg_editor') . '/ckeditor.js';

    // only do this is the theme is not default
    if (isset($context['profile']->settings['skin']) && $context['profile']->settings['skin'] != 'default') {

      $skins_path = wysiwyg_get_path('ckeditor_skins');

      $active_skin = $context['profile']->settings['skin'];
      // Set custom skin.
      $settings['skin'] = sprintf('%s,' . '%s/%s/%s/', $active_skin, $base_url, $skins_path, $active_skin);

    }

  }

}
