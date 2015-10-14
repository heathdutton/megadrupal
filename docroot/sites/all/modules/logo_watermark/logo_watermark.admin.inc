<?php

/**
 * Menu callback for the settings page.
 * @return mixed
 */
function logo_watermark_admin_settings() {
  $form = array();

  $form['logo_watermark_enabled'] = array(
    '#title' => t('Enable Logo Watermark Display'),
    '#description' => t('Enable the display of the logo watermark.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('logo_watermark_enabled', 1),
  );

  $form['logo_watermark_url'] = array(
    '#title' => t('Logo URL Link'),
    '#description' => t('Enter the URL the logo should link to.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('logo_watermark_url', 'http://www.example.com'),
  );

  $form['logo_watermark_anchor_title'] = array(
    '#title' => t('Logo Anchor Title Attribute'),
    '#description' => t('Enter the value for the title attribute that will display on the logo link anchor element.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('logo_watermark_anchor_title', variable_get('site_name', '')),
  );

  $form['logo_watermark_fade_speed'] = array(
    '#title' => t('Logo Fade Speed'),
    '#description' => t('Enter in how fast or slow you want the logo watermark to fade in/out on the screen, in milliseconds (ex 2000 = 2 seconds).'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('logo_watermark_fade_speed', 200),
  );

  $form['logo_watermark_image_fid'] = array(
    '#title' => t('Logo Watermark Image'),
    '#description' => t('Upload the image to be used as the logo watermark. Allowed extensions: gif png jpg jpeg.'),
    '#type' => 'managed_file',
    '#default_value' => variable_get('logo_watermark_image_fid', ''),
    '#upload_location' => 'public://watermark',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
  );

  $form['#validate'][] = 'logo_watermark_validate';
  $form['#submit'][] = 'logo_watermark_submit';

  return system_settings_form($form);
}

/**
 * Form validation.
 * @param $form
 * @param $form_state
 */
function logo_watermark_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['logo_watermark_fade_speed'])) {
    form_set_error('logo_watermark_fade_speed', 'The fade speed must be an integer.');
  }

  if ($form_state['values']['logo_watermark_image_fid'] != 0) {
    $file = file_load($form_state['values']['logo_watermark_image_fid']);
    $validate_size = file_validate_size($file, 102400, 0);
    $validate_dimension = file_validate_image_resolution($file, '200x200', 0);

    if (count($validate_size)) {
      form_set_error('logo_watermark_image_fid', 'The file you are uploading is too large. Please upload an image smaller than 100kb.');
    }

    if (count($validate_dimension)) {
      form_set_error('logo_watermark_image_fid', 'The file you are uploading is larger than 200x200. Please upload a smaller image.');
    }
  }
}

/**
 * Form submit handler.
 * @param $form
 * @param $form_state
 */
function logo_watermark_submit($form, $form_state) {
  if (is_numeric($form_state['values']['logo_watermark_image_fid']) && $form_state['values']['logo_watermark_image_fid'] != 0) {
    global $user;
    $file = file_load($form_state['values']['logo_watermark_image_fid']);

    if ($file->status != FILE_STATUS_PERMANENT) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'logo_watermark', 'image', $user->uid);
      variable_set('logo_watermark_image_fid', $file->fid);
      variable_set('logo_watermark_filepath', file_create_url($file->uri));
    }
  } else {
    variable_del('logo_watermark_filepath');
    variable_del('logo_watermark_image_fid');
  }
}