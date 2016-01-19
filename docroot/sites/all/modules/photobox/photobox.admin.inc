<?php
/**
 * @file
 * Administration pages for the Photobox module.
 */

/**
 * Admin settings form.
 */
function photobox_settings_form() {
  $form = array();
  $settings = _photobox_settings();
  $form['photobox_history'] = array(
    '#type' => 'checkbox',
    '#title' => t('Browser history'),
    '#description' => t('Enable/disable HTML5 history using hash urls.'),
    '#default_value' => $settings['history'],
  );
  $form['photobox_loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop'),
    '#description' => t('Loop back to last image before the first one and to the first image after last one.'),
    '#default_value' => $settings['loop'],
  );
  $form['photobox_thumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Thumbs'),
    '#description' => t('Show thumbs of all the images in the gallery at the bottom.'),
    '#default_value' => $settings['thumbs'],
  );
  $form['photobox_zoomable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Zoom'),
    '#description' => t('Enable/Disable mousewheel zooming over images.'),
    '#default_value' => $settings['zoomable'],
  );

  return system_settings_form($form);
}
