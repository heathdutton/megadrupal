<?php

/**
 * @file
 * Provides the administration page for Commerce Cart Favicon.
 */

/**
 * Administration settings page.
 */
function commerce_cart_favicon_admin($form, $form_state) {
  $form = array();

  $settings = array(
    'animation' => 'none',
    'position' => 'down',
    'type' => 'circle',
    'bgColor' => '#d00',
    'textColor' => '#fff',
    'fontFamily' => 'sans-serif',
    'fontStyle' => 'bold',
  );

  $settings = variable_get('commerce_cart_favicon_settings', $settings);

  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['settings']['animation'] = array(
    '#type' => 'select',
    '#title' => t('Animation'),
    '#description' => t('Set the counter appearance animation.'),
    '#default_value' => $settings['animation'],
    '#options' => array(
      'none' => t('None'),
      'slide' => t('Slide'),
      'fade' => t('Fade'),
      'pop' => t('Pop'),
      'popFade' => t('Pop & Fade'),
    ),
  );

  $form['settings']['position'] = array(
    '#type' => 'select',
    '#title' => t('Badge position'),
    '#description' => t('Set the counter position.'),
    '#default_value' => $settings['position'],
    '#options' => array(
      'down' => t('Down'),
      'up' => t('Up'),
      'left' => t('Down & Left'),
      'upleft' => t('Up & Left'),
    ),
  );

  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Badge shape'),
    '#description' => t('Set the badge shape.'),
    '#default_value' => $settings['type'],
    '#options' => array(
      'circle' => t('Circle'),
      'rectangle' => t('Rectangle'),
    ),
  );

  $form['settings']['bgColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Badge background color'),
    '#description' => t('Set the badge background color.'),
    '#default_value' => $settings['bgColor'],
  );

  $form['settings']['textColor'] = array(
    '#type' => 'textfield',
    '#title' => t('Badge text color'),
    '#description' => t('Set the badge text color.'),
    '#default_value' => $settings['textColor'],
  );

  $form['settings']['fontFamily'] = array(
    '#type' => 'textfield',
    '#title' => t('Text font family'),
    '#description' => t('Set text font family (Arial, Verdana, Times New Roman, serif, sans-serif,...).'),
    '#default_value' => $settings['fontFamily'],
  );

  $form['settings']['fontStyle'] = array(
    '#type' => 'select',
    '#title' => t('Font style'),
    '#description' => t('Set font style.'),
    '#default_value' => $settings['fontStyle'],
    '#options' => array(
      'normal' => t('Normal'),
      'italic' => t('Italic'),
      'oblique' => t('Oblique'),
      'bold' => t('Bold'),
      'bolder' => t('Bolder'),
      'lighter' => t('Lighter'),
    ),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Form validation.
 */
function commerce_cart_favicon_admin_validate($form, &$form_state) {
  $color = $form_state['values']['settings']['textColor'];
  if(!preg_match('/^#[a-f0-9]{6}$/i', $color) && !preg_match('/^#[a-f0-9]{3}$/i', $color)) {
    form_set_error('', t('Text color must be a valid hexadecimal value (Ex. #fff or #ffffff).'));
  }

  $color = $form_state['values']['settings']['bgColor'];
  if(!preg_match('/^#[a-f0-9]{6}$/i', $color) && !preg_match('/^#[a-f0-9]{3}$/i', $color)) {
    form_set_error('', t('Background color must be a valid hexadecimal value (Ex. #fff or #ffffff).'));
  }
}

/**
 * Form submit.
 */
function commerce_cart_favicon_admin_submit($form, &$form_state) {
  variable_set('commerce_cart_favicon_settings', $form_state['values']['settings']);
  drupal_set_message(t('Settings have been set.'));
}