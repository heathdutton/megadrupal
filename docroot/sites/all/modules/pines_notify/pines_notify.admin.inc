<?php 
/**
 * @file
 * Admin settings form for the Pines Notify plugin.
 */

/**
 * Custom settings for the Pines Notify plugin.
 */
function pines_notify_settings_form() {

  $form['pines_notify_title_success'] = array(
    '#title' => t('Success title'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pines_notify_title_success', 'Success'),
    '#description' => t('Leave empty to hide success titles.'),
  );

  $form['pines_notify_title_error'] = array(
    '#title' => t('Error title'),
    '#type' => 'textfield',
    '#default_value' => variable_get('pines_notify_title_error', 'Error'),
    '#description' => t('Leave empty to hide error titles.'),
  );

  $form['pines_notify_animation'] = array(
    '#title' => t('Animation'),
    '#type' => 'select',
    '#options' => array(
      'show' => t('Show'),
      'fade' => t('Fade'),
      'slide' => t('Slide'),
    ),
    '#default_value' => variable_get('pines_notify_animation', 'fade'),
    '#description' => t('The animation style used when displaying and hiding notifications.'),
  );

  $form['pines_notify_delay'] = array(
    '#title' => t('Fadeout delay'),
    '#type' => 'select',
    '#options' => array(
      2000 => t('2 seconds'),
      3000 => t('3 seconds'),
      5000 => t('5 seconds'),
      10000 => t('10 seconds'),
      30000 => t('30 seconds'),
    ),
    '#default_value' => variable_get('pines_notify_delay', 10000),
    '#description' => t('The time it takes to hide user notifications.'),
  );

  $form['pines_notify_opacity'] = array(
    '#title' => t('Opacity'),
    '#type' => 'select',
    '#options' => array(
      '0.25' => t('25%'),
      '0.5' => t('50%'),
      '0.75' => t('75%'),
      '1' => t('100%'),
    ),
    '#default_value' => variable_get('pines_notify_opacity', '0.75'),
    '#description' => t('The transparency level of the notifications.'),
  );

  $form['pines_notify_shadow'] = array(
    '#title' => t('Show dropshadows'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('pines_notify_shadow', 1),
    '#description' => t('Display dropshadows around the notifications.'),
  );

  $form['pines_notify_hide'] = array(
    '#title' => t('Automatically hide notices'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('pines_notify_hide', 1),
    '#description' => t('Uncheck this if you do not want remove notices until the page is reloaded or the user closes them.'),
  );

  $form['pines_notify_nonblock'] = array(
    '#title' => t('Non-blocking notices'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('pines_notify_nonblock', 1),
    '#description' => t('Check this if you want to be able to click behind notices.'),
  );

  $form['pines_notify_desktop'] = array(
    '#title' => t('Desktop web notifications'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('pines_notify_desktop', 1),
    '#description' => t('Check this if you want to be enable desktop web notifications.'),
  );

  $form['pines_notify_compression_type'] = array(
    '#type' => 'radios',
    '#title' => t('Javascript compression level'),
    '#options' => array(
      'minified' => t('Production (Minified)'),
      'source' => t('Development (Uncompressed Code)'),
    ),
    '#default_value' => variable_get('pines_notify_compression_type', 'minified'),
  );

  return system_settings_form($form);
}
