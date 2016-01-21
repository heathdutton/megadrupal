<?php

/**
 * @file
 * PageFlip module administrative interface.
 */


/**
 * Build our admin settings form.
 */
function pageflip_settings_form($form_state) {
  $form = array();

  $presets = array(0 => t('Do not use an ImageCache preset.'));
  if (module_exists('imagecache')) {
    foreach (imagecache_presets() as $preset) {
      $presets[$preset['presetid']] = $preset['presetname'];
    }
  }
  $form['pageflip_low_resolution_preset'] = array(
    '#type' => 'select',
    '#title' => t('Low resolution image preset'),
    '#options' => $presets,
    '#default_value' => variable_get('pageflip_low_resolution_preset', 0),
  );
  $form['pageflip_high_resolution_preset'] = array(
    '#type' => 'select',
    '#title' => t('High resolution image preset'),
    '#options' => $presets,
    '#default_value' => variable_get('pageflip_high_resolution_preset', 0),
  );

  $form['pageflip_start_chapters_on_right'] = array(
    '#type' => 'checkbox',
    '#title' => t('Start chapters only on right-side pages'),
    '#default_value' => variable_get('pageflip_start_chapters_on_right', FALSE),
  );

  $form['high'] = array(
    '#type' => 'fieldset',
    '#title' => t('High resolution'),
  );
  $form['high']['pageflip_page_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Page width (pixels)'),
    '#default_value' => variable_get('pageflip_page_width', 960),
    '#description' => t('Width of images used for PageFlip pages, in pixels.'),
  );
  $form['high']['pageflip_page_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Page height (pixels)'),
    '#default_value' => variable_get('pageflip_page_height', 1080),
    '#description' => t('Height of images used for PageFlip pages, in pixels.'),
  );
  $form['low'] = array(
    '#type' => 'fieldset',
    '#title' => t('Low resolution'),
  );
  $form['low']['pageflip_page_width_low'] = array(
    '#type' => 'textfield',
    '#title' => t('Page width (pixels)'),
    '#default_value' => variable_get('pageflip_page_width_low', 640),
    '#description' => t('Width of images used for PageFlip pages, in pixels.'),
  );
  $form['low']['pageflip_page_height_low'] = array(
    '#type' => 'textfield',
    '#title' => t('Page height (pixels)'),
    '#default_value' => variable_get('pageflip_page_height_low', 720),
    '#description' => t('Height of images used for PageFlip pages, in pixels.'),
  );

  // @todo: Maximum ad pages inside front cover?
  // @todo: Maximum ad pages inside back cover?
  return system_settings_form($form);
}
