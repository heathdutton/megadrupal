<?php

/**
 * @file
 * Settings form for Views Sexy Throbber.
 */

/**
 * Create the settings page form.
 */
function views_sexy_throbber_settings($form, &$form_state) {

  // Load the admin css to make the form prettier.
  drupal_add_css(drupal_get_path('module', 'views_sexy_throbber') . '/css/admin-form.css');

  // || PREPARE THE FORM  || //
  // Get base site URL to display option images correctly.
  global $base_url;

  // Set default image.
  $default_image = drupal_get_path('module', 'views_sexy_throbber') . '/images/default.gif';

  // Set additional throbber images.
  $throbber_images = file_scan_directory(drupal_get_path('module', 'views_sexy_throbber') . '/images', '/^.*\.(jpg|gif|png|png)$/');

  // Set custom directory so the user can upload custom throbber images.
  $custom_throbbers_directory = 'public://views_sexy_throbber';

  // Format the custom directory path to display it later to the user.
  $custom_throbbers_directory_path = str_replace($base_url . '/', "", file_create_url($custom_throbbers_directory));

  // Check if the custom throbber directory exist and if not create it.
  if (!file_prepare_directory($custom_throbbers_directory, $options = FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('Notice: There was a problem creating the custom images directory. Please make sure your public folder is writable by Drupal and reinstall the module. You can also ignore this notice and use only the images provided by the module, or you can create the custom image folder manually. The folder must be named “views_sexy_throbber” and must be placed in your public directory. If you need more help please use the module issue page or read the documentation.'), 'warning', TRUE);
  }

  // Scan for custom throbber images (if any).
  $custom_throbber_images = file_scan_directory($custom_throbbers_directory_path, '/^.*\.(jpg|gif|png|png)$/');

  // Set default options for "$form['image']['views_sexy_throbber_image']".
  $image_options = array(
    $default_image => theme('image', array('path' => $default_image)),
  );

  // Add all images to the radio form.
  foreach (array_merge($throbber_images, $custom_throbber_images) as $image_file) {
    $image_file_url = file_create_url($image_file->uri);
    $image_options[$image_file->uri] = theme('image', array('path' => $image_file_url));
  }

  // || GENERATE FORM ELEMENTS || //
  $form['overlay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Overlay Settings'),
  );

  $form['overlay']['views_sexy_throbber_background'] = array(
    '#type' => 'select',
    '#title' => t('Overlay Background'),
    '#default_value' => variable_get('views_sexy_throbber_background', '0,0,0'),
    '#options' => array(
      '0,0,0' => t('Dark'),
      '255,255,255' => t('Light'),
    ),
    '#description' => t('Select overlay background color. <strong>Default value: Dark</strong>'),
    '#required' => TRUE,
  );

  $form['overlay']['views_sexy_throbber_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Overlay Opacity'),
    '#default_value' => variable_get('views_sexy_throbber_opacity', '0.8'),
    '#options' => array(
      '0' => t('0%'),
      '0.1' => t('10%'),
      '0.2' => t('20%'),
      '0.3' => t('30%'),
      '0.4' => t('40%'),
      '0.5' => t('50%'),
      '0.6' => t('60%'),
      '0.7' => t('70%'),
      '0.8' => t('80%'),
      '0.9' => t('90%'),
      '1' => t('100%'),
    ),
    '#description' => t('Select the overlay opacity percentage. <strong>Default value: 80%</strong>'),
    '#required' => TRUE,
  );

  $form['image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Throbber Image'),
  );

  $form['image']['views_sexy_throbber_image'] = array(
    '#type' => 'radios',
    '#title' => t('Select throbber image'),
    '#default_value' => variable_get('views_sexy_throbber_image', $default_image),
    '#options' => $image_options,
    '#description' => t('Do you want to add your own images? Upload them to <span class="path">@custom_throbbers_directory</span> and refresh this page.', array(
      '@custom_throbbers_directory' => $custom_throbbers_directory_path,
      )
    ),
    '#required' => TRUE,
  );

  $form['image']['views_sexy_throbber_image_background'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add solid color background to the image'),
    '#description' => t("This will add a solid background color to the throbber image (pure black for the dark overlay, and pure white for the light). This is useful if you are using gif images that don't handle transparency very well. <strong>Default value: checked</strong>"),
    '#default_value' => variable_get('views_sexy_throbber_image_background', TRUE),
  );

  $form['image']['views_sexy_throbber_image_background_use_border_radius'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply "Rounded corners" effect to the image background'),
    '#description' => t('You can make round corners by adding border radius to the background box. <strong>Default value: checked</strong>'),
    '#default_value' => variable_get('views_sexy_throbber_image_background_use_border_radius', TRUE),
    '#states' => array(
      'visible' => array(
        ':input[name="views_sexy_throbber_image_background"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['image']['views_sexy_throbber_image_background_border_radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Rounded corners border radius'),
    '#description' => t('Enter a number from "0" to "999" representing the border radius in pixels. Greater number, means more rounded corners. <strong>Default value: 16</strong>'),
    '#default_value' => variable_get('views_sexy_throbber_image_background_border_radius', '16'),
    '#empty_value' => '0',
    '#size' => 3,
    '#maxlength' => 3,
    '#states' => array(
      'visible' => array(
        ':input[name="views_sexy_throbber_image_background_use_border_radius"]' => array('checked' => TRUE),
      ),
    ),
    '#field_suffix' => t('pixels'),
    '#element_validate' => array('views_sexy_throbber_valdate_border_radius'),
    '#required' => TRUE,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
  );

  $form['advanced']['views_sexy_throbber_use_in_other_modules'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Views Sexy Throbber in other modules'),
    '#description' => t('This will attempt to replace the default Drupal throbber and apply it to other modules too. (Removes the ".view" class from the css selector) <strong>Default value: unchecked</strong>'),
    '#default_value' => variable_get('views_sexy_throbber_use_in_other_modules', FALSE),
  );

  $form['advanced']['views_sexy_throbber_append_important'] = array(
    '#type' => 'checkbox',
    '#title' => t('Append css !important rules'),
    '#description' => t('Check this box if sexy throbber conflicts with other styles or you are having display issues. This will force the css styling to be overridden by appending !important rules. <strong>Default value: unchecked</strong>'),
    '#default_value' => variable_get('views_sexy_throbber_append_important', FALSE),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Validate border radius.
 */
function views_sexy_throbber_valdate_border_radius($element, &$form_state, $form) {

  // Set a short named varialbe for the element value.
  $value = $element['#value'];

  // Check if the entered value is integer.
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value)) {
    form_error($element, t('%name must be an integer.', array('%name' => $element['#title'])));
  }

  // Check if the integer is positive.
  if ($value < 0) {
    form_error($element, t('%name cannot be negative number.', array('%name' => $element['#title'])));
  }
}

/**
 * Submit handler for views_sexy_throbber_settings().
 */
function views_sexy_throbber_settings_submit($form, &$form_state) {

  // Define $base_url.
  global $base_url;

  // Get image info.
  $image_info = image_get_info($form_state['values']['views_sexy_throbber_image']);

  // Shuld we add the "div.view" css prefix?
  $css_views_prefix = ($form_state['values']['views_sexy_throbber_use_in_other_modules'] == TRUE ? '' : 'div.view ');

  // Should we perpend !important?
  $css_important = ($form_state['values']['views_sexy_throbber_append_important'] == TRUE ? ' !important' : NULL);

  // Generate the CSS file.
  $css = '' . $css_views_prefix . 'div.ajax-progress {
  background: rgba(' . $form_state['values']['views_sexy_throbber_background'] . ',' . $form_state['values']['views_sexy_throbber_opacity'] . ')' . $css_important . ';
  height: 100%' . $css_important . ';
  left: 0' . $css_important . ';
  position: fixed' . $css_important . ';
  top: 0' . $css_important . ';
  width: 100%' . $css_important . ';
  z-index: 9998' . $css_important . ';
}

' . $css_views_prefix . 'div.throbber {
  background: url("' . $base_url . '/' . $form_state['values']['views_sexy_throbber_image'] . '") no-repeat scroll 50% 50%' . $css_important . ';
  background-color: ' . ($form_state['values']['views_sexy_throbber_image_background'] ? 'rgb(' . $form_state['values']['views_sexy_throbber_background'] . ')' : 'transparent') . '' . $css_important . ';
  border-radius: ' . ($form_state['values']['views_sexy_throbber_image_background_use_border_radius'] ? $form_state['values']['views_sexy_throbber_image_background_border_radius'] : '0') . 'px' . $css_important . ';
  height: ' . $image_info['height'] . 'px' . $css_important . ';
  left: 50%' . $css_important . ';
  margin-left: -' . $image_info['width'] * 0.5 . 'px' . $css_important . ';
  margin-top: -' . $image_info['height'] * 0.5 . 'px' . $css_important . ';
  padding: 10px' . $css_important . ';
  position: fixed' . $css_important . ';
  top: 50%' . $css_important . ';
  width: ' . $image_info['width'] . 'px' . $css_important . ';
  z-index: 9999' . $css_important . ';
}
';

  // Format the css code output and remove some unnecessary spaces.
  $css = str_replace("	", " ", $css);

  // Write the final formated css file.
  file_unmanaged_save_data($css, drupal_get_path('module', 'views_sexy_throbber') . '/css/views_sexy_throbber.css', FILE_EXISTS_REPLACE);

  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    variable_set($key, $value);
  }

  drupal_set_message(t('The configuration options have been saved.'));
}
