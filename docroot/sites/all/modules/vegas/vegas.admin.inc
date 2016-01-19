<?php

/**
 * @file
 * Administration forms for the Vegas module.
 */

/**
 * Menu callback; Displays the administration settings for Vegas.
 */
function vegas_admin_settings() {
  // Set up the vertical tabs.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );

  // Set up the tabs.
  $form['configuration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#description'=> t('Provide general configuration for how the images are displayed.'),
    '#group' => 'settings',
  );
  $form['images'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images'),
    '#description'=> t('Configure which images should be presented as background images.'),
    '#group' => 'settings',
  );

  // Images
  $count = 10;
  for ($i = 0; $i < $count; $i++) {
    $image = variable_get('vegas_images_' . $i, '');
    $form['images']['vegas_images_' . $i] = array(
      '#type' => 'managed_file',
      '#default_value' => $image,
      '#upload_location' => 'public://vegas/',
      '#upload_validators' => array(
        'file_validate_extensions' => array(
          0 => 'png jpg gif jpeg',
        ),
      ),
    );
  }

  // Overlay
  $form['configuration']['vegas_overlay'] = array(
    '#type' => 'managed_file',
    '#title' => t('Overlay'),
    '#description' => t('The overlay will be placed on top of the image to give it a neat effect.'),
    '#default_value' => variable_get('vegas_overlay', ''),
    '#upload_location' => 'public://vegas/',
    '#upload_validators' => array(
      'file_validate_extensions' => array(
        0 => 'png jpg gif jpeg',
      ),
    ),
  );

  // Fade
  $form['configuration']['vegas_fade'] = array(
    '#title' => t('Fade'),
    '#type' => 'select',
    '#description' => t('Transition time between slides.'),
    '#default_value' => variable_get('vegas_fade', 0),
    '#options' => array(
      0 => t('None'),
      500 => t('Half a second'),
      1000 => t('One second'),
      2000 => t('Two seconds'),
      3000 => t('Three seconds'),
      4000 => t('Four seconds'),
      5000 => t('Five seconds'),
    ),
  );

  // Delay
  $form['configuration']['vegas_delay'] = array(
    '#title' => t('Delay'),
    '#type' => 'select',
    '#description' => t('The time taken between two slides.'),
    '#default_value' => variable_get('vegas_delay', 5000),
    '#options' => array(
      500 => t('Half a second'),
      1000 => t('One second'),
      2000 => t('Two seconds'),
      3000 => t('Three seconds'),
      4000 => t('Four seconds'),
      5000 => t('Five seconds'),
      6000 => t('Six seconds'),
      7000 => t('Seven seconds'),
      8000 => t('Eight seconds'),
      9000 => t('Nine seconds'),
      10000 => t('Ten seconds'),
      11000 => t('Eleven seconds'),
      12000 => t('Twelve seconds'),
      13000 => t('Thirteen seconds'),
      14000 => t('Fourteen seconds'),
      15000 => t('Fifteen seconds'),
      16000 => t('Sixteen seconds'),
      17000 => t('Seventeen seconds'),
      18000 => t('Eighteen seconds'),
      19000 => t('Nineteen seconds'),
      20000 => t('Twenty seconds'),
    ),
  );

  // Shuffle
  $form['configuration']['vegas_shuffle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Shuffle'),
    '#description' => t('Randomize the order of the images.'),
    '#default_value' => variable_get('vegas_shuffle', TRUE),
  );

  $form['#submit'][] = 'vegas_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form callback; Save the images permanently.
 */
function vegas_admin_settings_submit($form, $form_state) {
  global $user;

  // Make sure to save the files permanently.
  $count = 10;
  for ($i = 0; $i < $count; $i++) {
    // Load the file via file.fid.
    if (isset($form_state['values']['vegas_images_' . $i])) {
      $file = file_load($form_state['values']['vegas_images_' . $i]);
      if ($file) {
        // Change status to permanent.
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        file_usage_add($file, 'user', 'user', $user->uid);
      }
    }
  }

  // Also be sure to save the overlay.
  $file = file_load($form_state['values']['vegas_overlay']);
  if ($file) {
    // Change status to permanent.
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'user', 'user', $user->uid);
  }
}
