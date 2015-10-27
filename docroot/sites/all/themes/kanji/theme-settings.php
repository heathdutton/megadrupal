<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 */
function kanji_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL) {
  if (isset($form_id)) {
    return;
  }

  // Layout settings
  $form['kanji'] = array(

    '#default_tab' => 'defaults',
  );

  // Breadcrumbs
  $form['kanji']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#weight' => '5',
    '#title' => t('Breadcrumbs'),
  );
  $form['kanji']['breadcrumb']['bd']['breadcrumb_display'] = array(
    '#type' => 'select',
    '#title' => t('Show breadcrumbs'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
  );

	$form['background'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background settings'),
  );
  
  $form['background']['default_background'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default background-image'),
    '#default_value' => (theme_get_setting('default_background') == 0 ? theme_get_setting('default_background') : 1),
  );
  
  $form['background']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="default_background"]' => array('checked' => TRUE),
      ),
    ),
  );
  
  $form['background']['settings']['path_background'] = array(
    '#type' => 'textfield',
    '#title' => t('Current used background-image'),
    '#default_value' => variable_get('kanji:path_background', ''),
  );

  $form['background']['settings']['upload_background'] = array(
    '#type' => 'file',
    '#title' => t('Upload a background image'),
  );

  $form['background']['settings']['background_color'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use background color'),
    '#description' => t('This will remove the background-image'),
    '#default_value' => (theme_get_setting('background_color') == 0 ? theme_get_setting('background_color') : 1),
  );
  
  $form['background']['settings']['background_color_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Provide the backgroundcolor'),
    '#description' => t('Give the value for the background-color, example: FFF, 000'),
    '#default_value' => variable_get('kanji:background_color_value', ''),
    '#field_prefix' => '#',
    '#states' => array(
      'invisible' => array(
        'input[name="background_color"]' => array('checked' => FALSE),
      ),
    ),
  );
  
  $form['#validate'][] = 'kanji_theme_settings_validate';
  $form['#submit'][] = 'kanji_theme_settings_submit';
}

function kanji_theme_settings_validate($form, &$form_state) {
// Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded logo.
  $file = file_save_upload('upload_background', $validators, FALSE, FILE_EXISTS_REPLACE);

  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['upload_background'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('upload_background', t('The header could not be uploaded.'));
    }
  }

  // If the user provided a path for a logo or favicon file, make sure a file
  // exists at that path.
  if ($form_state['values']['path_background']) {
    $path = _system_theme_settings_validate_path($form_state['values']['path_background']);
    if (!$path) {
      form_set_error('path_background', t('The custom header path is invalid.'));
    }
  }
  
  if ($form_state['values']['background_color'] == true && !empty($form_state['values']['background_color_value'])) {
    if (!preg_match("/^[A-Za-z0-9]+$/", $form_state['values']['background_color_value'])) {
      form_set_error('background_color_value', t('The color must be a HEX value, example ff0000'));
    }
  }
}

function kanji_theme_settings_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  // If the user uploaded a new header image, save it to a permanent location
  if ($file = $values['upload_background']) {
    unset($values['upload_background']);
    $filename = file_unmanaged_copy($file->uri, NULL, FILE_EXISTS_REPLACE);
    $values['path_background'] = $filename;
  }

  // If the user entered a path relative to the system files directory for
  // a header image, store a public:// URI so the theme system can handle it.
  if (!empty($values['path_background'])) {
    $values['path_background'] = _system_theme_settings_validate_path($values['path_background']);
    variable_set('kanji:path_background', $values['path_background']);
  }
  
  if (!empty($values['background_color_value'])) {
    variable_set('kanji:background_color_value', $values['background_color_value']);
  }
  
  //Delete background settings when the default background is used
  if ($values['default_background'] == 1) {
  	variable_del('kanji:path_background');
    variable_del('kanji:background_color_value');
  }
}

