<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Create true slideshows using any image over internet with many other features.
 */

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _slideshow_creator_admin() {
  // Get the default settings for all slideshows
  $settings = variable_get('slideshow_creator_settings', array());

  // Build the form
  $form['width'] = array(
    '#default_value'  => empty($settings['#width']) ? 400 : $settings['#width'],
    '#description'    => t('The slideshow width, in pixels.'),
    '#title'          => t('Width'),
    '#type'           => 'textfield',
  );
  $form['height'] = array(
    '#default_value'  => empty($settings['#height']) ? 400 : $settings['#height'],
    '#description'    => t('The slideshow height, in pixels.'),
    '#title'          => t('Height'),
    '#type'           => 'textfield',
  );
  $form['layout'] = array(
    '#default_value'  => empty($settings['#layout']) ? 'default' : $settings['#layout'],
    '#description'    => t('The slideshow layout. "Top" puts the slideshow above the control (next-previous) line, and "bottom" does the opposite. "None" removes the controls.'),
    '#options'        => array(
      'default'       => t('Default'),
      'top'           => t('Top'),
      'bottom'        => t('Bottom'),
      'none'          => t('None'),
    ),
    '#title'          => t('Layout'),
    '#type'           => 'select',
  );
  $form['order'] = array(
    '#default_value'  => empty($settings['#order']) ? 'default' : $settings['#order'],
    '#description'    => t('The order of the slideshow components. "Top" puts the image above the title and caption lines, "middle" puts it in between them, and so on. The "Reverse" options invert the title and caption (putting the caption above the title).'),
    '#options'        => array(
      'default'       => t('Default'),
      'top'           => t('Top'),
      'middle'        => t('Middle'),
      'bottom'        => t('Bottom'),
      'reversetop'    => t('Reverse Top'),
      'reversemiddle' => t('Reverse Middle'),
      'reversebottom' => t('Reverse Bottom'),
    ),
    '#title'          => t('Order'),
    '#type'           => 'select',
  );
  $form['current_slide_string'] = array(
    '#default_value'  => !isset($settings['#current_slide_string']) ? 'Slide' : $settings['#current_slide_string'],
    '#description'    => t('The text to be displayed on the Current Slide part.'),
    '#title'          => t('Current Slide String'),
    '#type'           => 'textfield',
  );
  $form['default_tags'] = array(
    '#default_value'  => !isset($settings['#default_tags']) ? '' : $settings['#default_tags'],
    '#description'    => t('A comma-separated list of any other options you want to supply by default to all slideshows; see README.txt for details. Don\'t set the special options Width, Height, Layout, or Order here - use the fields earlier on this page.'),
    '#title'          => t('Default Tags'),
    '#type'           => 'textfield',
  );

  // Submit button
  $form['submit'] = array(
    '#value'          => t('Save these settings'),
    '#type'           => 'submit',
  );

  return $form;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _slideshow_creator_admin_submit(&$from, &$form_state) {
  $settings = $form_state['values'];
  unset($settings['submit']);
  unset($settings['form_id']);
  unset($settings['form_build_id']);
  unset($settings['form_token']);
  unset($settings['op']);

  // Add a '#' in all field names
  foreach ($settings as $index => $value) {
    $settings["#$index"] = $value;
    unset($settings[$index]);
  }

  // Save the data into database
  variable_set('slideshow_creator_settings', $settings);

  // Print a 'OK' message
  drupal_set_message(t('Settings saved'));
}
