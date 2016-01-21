<?php

/**
 * @file
 * Generate configuration form and save settings.
 *
 * Available transitions.
 * 2D: bars, blinds, blocks, blocks2, concentric, dissolve, slide, warp, zip.
 * 3D: bars3d, blinds3d, cube, tiles3d, turn.
 */

/**
 * Configuration form for slider options.
 */
function flux_slider_option_configuration_form($form, &$form_state) {
  // Get the path to the module.
  $path = drupal_get_path('module', 'flux_slider');
  // Attach the CSS to the form.
  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => $path . '/css/form_theme.css',
    ),
  );

  // Set up fieldset for radio buttons.
  $form['trans_radios'] = array(
    '#type' => 'fieldset',
    '#title' => t('Transitions'),
    '#weight' => 4,
    '#collapsible' => TRUE,
  );

  // Set up radio buttons for 2D transition effect.
  $form['trans_radios']['trans_type'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('trans_type', 'bars'),
    '#options' => array(
      'bars' => t('Vertical bars dropping from the top down.'),
      'blinds' => t('Blinds sweeping from left to right.'),
      'blocks' => t('Blocks changing in a random pattern.'),
      'blocks2' => t('Blocks changing from left to right.'),
      'concentric' => t('Concentric circles twisting as they change.'),
      'dissolve' => t('Dissolve/fade between images.'),
      'slide' => t('Slide from right to left.'),
      'warp' => t('Multiple concentric circles twisting as they change.'),
      'zip' => t('Vertical bars sliding to the top and bottom.'),
      'bars3d' => t('3D vertical bars dropping from the top down.'),
      'blinds3d' => t('3D blinds sweeping from left to right'),
      'cube' => t('Single 3D cube rotating right to left.'),
      'tiles3d' => t('3D blocks changing from left to right.'),
      'turn' => t('Page turn effect from right to left.'),
    ),
    '#title' => t('Select a transition to perform between slides.'),
    '#description' => t('Some of the 3D transitions will not function for older browsers.<br />Both of the concentric circle transitions will result in an image jump if the image is larger than the slider.'),
  );

  // Set up fieldset for optional settings.
  $form['trans_opts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#weight' => 5,
    '#collapsible' => TRUE,
  );

  // Form element for autoplay selection.
  $form['trans_opts']['trans_autoplay'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('trans_autoplay', 1),
    '#options' => array(
      0 => t('Off'),
      1 => t('On'),
    ),
    '#title' => t('Autoplay'),
  );

  // Form element for delay setting.
  $form['trans_opts']['trans_delay'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('trans_delay', 6000),
    '#title' => t('Transition delay'),
    '#size' => 6,
    '#field_suffix' => t('milliseconds'),
  );

  // Form element for pagination display.
  $form['trans_opts']['trans_pagination'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('trans_pagination', 0),
    '#options' => array(
      0 => t('Hide'),
      1 => t('Show'),
    ),
    '#title' => t('Pagination controls'),
  );

  // Form element for display of controls.
  $form['trans_opts']['trans_controls'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('trans_controls', 0),
    '#options' => array(
        0 => t('Hide'),
        1 => t('Show'),
    ),
    '#title' => t('Slider controls'),
  );

  // Form element for display of captions.
  $form['trans_opts']['trans_captions'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('trans_captions', 0),
    '#options' => array(
        0 => t('Hide'),
        1 => t('Show'),
    ),
    '#title' => t('Slider captions'),
  );

  // Form element to set output width.
  $form['trans_opts']['trans_width'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('trans_width', 960),
    '#title' => t('Slider width'),
    '#size' => 6,
    '#field_suffix' => t('pixels'),
  );

  // Form element to set output height.
  $form['trans_opts']['trans_height'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('trans_height', 300),
    '#title' => t('Slider height'),
    '#size' => 6,
    '#field_suffix' => t('pixels'),
  );
  return system_settings_form($form);
}
