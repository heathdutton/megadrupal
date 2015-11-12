<?php

/**
 * @file
 * Administration page callbacks for the regbar module.
 */

/**
 * Form builder: configure the regbar system.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function regbar_admin_settings($form) {
  // Barcode encoding type generation settings.
  $form = array();
  $form['barcodetype'] = array(
    '#type' => 'fieldset',
    '#title' => t('Barcode encoding type'),
  );

  $form['barcodetype']['regbar_type'] = array(
    '#type' => 'radios',
    '#title' => t('Encoding types'),
    '#description' => t('Select the barcode encoding type which will be used for barcode generation.'),
    '#options' => array(
      t('EAN 8'),
      t('EAN 13'),
      t('standard 2 of 5 (industrial)'),
      t('interleaved 2 of 5'),
      t('code 11'),
      t('code 39'),
      t('code 93'),
      t('code 128'),
      t('codabar'),
      t('MSI'),
      t('Datamatrix (ASCII + extended)'),
    ),
    '#default_value' => variable_get('regbar_type', 5),
  );

  // Miscellaneous settings.
  $form['misc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Miscellaneous'),
  );

  $form['misc']['regbar_background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#size' => 8,
    '#maxlength' => 7,
    '#default_value' => variable_get('regbar_background', '#FFFFFF'),
    '#description' => t('Set the background color of barcode. Value of color should be in hexadecimal form e.g #ffffff'),
  );

  $form['misc']['regbar_barcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Barcolor'),
    '#size' => 8,
    '#maxlength' => 7,
    '#default_value' => variable_get('regbar_barcolor', '#000000'),
    '#description' => t('Set the barcolor of barcode. Value of color should be in hexadecimal form e.g #ffffff'),
  );

  $form['misc']['regbar_textcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Barcode text color'),
    '#size' => 8,
    '#maxlength' => 7,
    '#default_value' => variable_get('regbar_textcolor', '#FA0000'),
    '#description' => t('Set the barcode text color. Value of color should be in hexadecimal form e.g. #ffffff'),
  );

  $form['misc']['regbar_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Bar Width [In pixel]'),
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => variable_get('regbar_width', '1'),
    '#description' => t('Set the barcode width. Value of width should be in pixels e.g  2, 3 (do not specify px)'),
  );

  $form['misc']['regbar_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Bar Height [In pixel]'),
    '#size' => 4,
    '#maxlength' => 4,
    '#default_value' => variable_get('regbar_height', '60'),
    '#description' => t('Set the barcode height. Value of height should be in pixel e.g  2, 3 (do not specify px)'),
  );

  // Barcode image generation settings.
  $form['generate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Barcode image generation'),
    '#tree' => FALSE,
  );

  $form['generate']['regbar_barcodevalue'] = array(
    '#type' => 'radios',
    '#title' => t('Select the barcode value'),
    '#description' => t('Select the barcode value which will be used for barcode generation.'),
    '#options' => array(
      t('User name [only allowed for case 39,case 93,case 128 & datamatrix encoding]'),
      t('Random string'),
    ),
    '#default_value' => variable_get('regbar_barcodevalue', 1),
  );

  $form['generate']['regbar_showvalue'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show value below the barcode'),
    '#default_value' => variable_get('regbar_showvalue', 1),
    '#tree' => FALSE,
    '#description' => t('Check here if you want to show code below the barcode.'),
  );

  $form['generate']['position'] = array(
    '#type' => 'fieldset',
    '#title' => t('Barcode position [These option will only work if custom barimage is specified]'),
  );

  $form['generate']['position']['regbar_barcode_center_x'] = array(
    '#type' => 'textfield',
    '#title' => t('X-cordinate of barcode center [In pixel]'),
    '#size' => 6,
    '#maxlength' => 5,
    '#default_value' => variable_get('regbar_barcode_center_x', 100),
    '#description' => t('Set the X-cordinate of barcode center. Value should be in pixel e.g  100,200 [do not specify px]'),
  );

  $form['generate']['position']['regbar_barcode_center_y'] = array(
    '#type' => 'textfield',
    '#title' => t('Y-cordinate of barcode center [In pixel]'),
    '#size' => 6,
    '#maxlength' => 5,
    '#default_value' => variable_get('regbar_barcode_center_y', 100),
    '#description' => t('Set the Y-cordinate of barcode center. Value should be in pixel e.g  100,200 [do not specify px]'),
  );

   // Image upload form settings.
  $form['barimage'] = array(
    '#type' => 'fieldset',
    '#title' => t('Barcode image settings'),
    '#description' => t('If toggled on, the following image will be displayed.'),
    '#suffix' => t('<div><b>Note: </b> If "Use the default barimage" option is not selected and custom barimage path is not specified then only barcode will be generated. No background image will be used.</div>'),
  );

  $form['barimage']['regbar_default_barimage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default barimage'),
    '#default_value' => variable_get('regbar_default_barimage', 0),
    '#tree' => FALSE,
    '#description' => t('Check here if you want to use the image supplied with it.'),
  );
  $barimage_path = variable_get('regbar_barimage_path', '');
  if ($barimage_path == '' && !file_exists($barimage_path)) {
    $barimage_path = NULL;
  }

  $form['barimage']['regbar_barimage_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to custom bar image'),
    '#default_value' => $barimage_path,
    '#description' => t('The path to the file you would like to use as your bar image file instead of the default bar image.'),
  );
  $form['barimage']['regbar_barimage_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload bar image [only .jpeg extension]'),
    '#maxlength' => 40,
    '#description' => t("If you don't have direct file access to the server, use this field to upload your image. Maximum dimensions are %dimensions and the maximum size is %size kB.", array('%dimensions' => '640x480', '%size' => '30')),
  );

  return system_settings_form($form);
}

/**
 * Validate the configuration form.
 */
function regbar_admin_settings_validate($form, &$form_state) {
  $background = $form_state['values']['regbar_background'];
  $barcolor = $form_state['values']['regbar_barcolor'];
  $width  = $form_state['values']['regbar_width'];
  $height = $form_state['values']['regbar_height'];
  $barcode_center_x = $form_state['values']['regbar_barcode_center_x'];
  $barcode_center_y = $form_state['values']['regbar_barcode_center_y'];

  // Define limits for the submission here.
  $validators = array(
    'regbar_validate_image_type' => array(),
    'file_validate_extensions' => array('jpeg'),
    'file_validate_image_resolution' => array(variable_get('user_picture_dimensions', '640x480')),
    'file_validate_size' => array(variable_get('user_picture_file_size', '30') * 1024),
  );

  // Check for a new uploaded barimage, and use that instead.
  if ($file = file_save_upload('regbar_barimage_upload', $validators, FALSE, FILE_EXISTS_REPLACE)) {
    $parts = pathinfo($file->filename);
    $filename = file_default_scheme() . '://regbar/barimage.' . $parts['extension'];

    // The image was saved using file_save_upload() and was added to the
    // files table as a temporary file. We'll make a copy and let the garbage
    // collector delete the original upload.
    if (file_copy($file, $filename, FILE_EXISTS_REPLACE)) {
      $form_state['values']['regbar_default_barimage'] = 0;
      $form_state['values']['regbar_barimage_path'] = $filename;
    }
  }

  if (!preg_match("^\#([a-fA-F0-9]{6}|[a-fA-F0-9]{3})$^", $background)) {
    form_set_error('background', t('Set background color in valid hexadecimal RGB form.'));
  }

  if (!preg_match("^\#([a-fA-F0-9]{6}|[a-fA-F0-9]{3})$^", $barcolor)) {
    form_set_error('barcolor', t('Set bar color in valid hexadecimal RGB form.'));
  }

  if (!is_numeric($width)) {
    form_set_error('width', t('Please enter a number for width.'));
  }

  if (!is_numeric($height)) {
    form_set_error('height', t('Please enter a number for height.'));
  }

  if (!is_numeric($barcode_center_x)) {
    form_set_error('barcode_center_x', t('Please enter a number for X-cordinate fo barcode center.'));
  }

  if (!is_numeric($barcode_center_y)) {
    form_set_error('barcode_center_y', t('Please enter a number for Y-cordinate fo barcode center.'));
  }
}

/**
 * Validate uploaded image extension.
 */
function regbar_validate_image_type(stdClass $file) {
  $errors = array();
  $info = image_get_info($file->uri);
  if ($info['mime_type'] != 'image/jpeg') {
    $errors[] = t('Only JPEG images are allowed currently.');
  }
  return $errors;
}
