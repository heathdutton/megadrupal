<?php

/**
 * Global settings form for Zoetrope Viewer.
 */
function zoetrope_viewer_settings_form($form, &$form_state) {

  $form['zoetrope_viewer_cdn_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use custom CDN'),
    '#description' => t('Normally your images will be hosted by Zoetrope. Leave blank unless instructed.'),
    '#default_value' => variable_get('zoetrope_viewer_cdn_enable', FALSE),
  );

  $form['zoetrope_viewer_cdn'] = array(
    '#type' => 'textfield',
    '#title' => t('base URL'),
    '#description' => t('The CDN url with no http:// or https://'),
    '#default_value' => variable_get('zoetrope_viewer_cdn', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="zoetrope_viewer_cdn_enable"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}
