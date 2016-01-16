<?php

/**
 * Page callback for settings.
 */
function yplog_settings() {
  $form['yplog_font'] = array(
    '#type' => 'textfield',
    '#title' => t('Graph font'),
    '#description' => t('Enter the path to a font file on the server file system.'),
    '#default_value' => variable_get('yplog_font', '/usr/local/lib/X11/fonts/webfonts/verdana.ttf'),
  );
  return system_settings_form($form);
}
