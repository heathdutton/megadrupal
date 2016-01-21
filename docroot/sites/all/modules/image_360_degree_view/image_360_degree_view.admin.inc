<?php

/**
 * @file
 * Admin file for 360 Image view for Block.
 */

/**
 * Admin settings for Image 360 degree view.
 */
function _image_360_degree_view_admin_settings() {

  $filecount = 0;
  $form['image_360_degree_view'] = array(
    '#title' => t('Image 360 degree view - Image uploader'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['image_360_degree_view']['allfiles'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  // First loading of image file.
  if (!$fileslist = variable_get('image_360_degree_view_image_files', FALSE)) {
    $fileslist = array(
      'files' => array(),
      'count' => 0,
    );
  }

  // Load existing list.
  foreach ($fileslist['files'] as $key => $value) {

    // Use the #managed_file FAPI element to upload an image file.
    $form['image_360_degree_view']['allfiles']['file_' . $filecount] = array(
      '#title' => t('Image no :') . ($filecount + 1),
      '#type' => 'managed_file',
      '#upload_location' => 'public://image_360_degree_view/',
      '#default_value' => $value,
      '#upload_validators' => array(
        'file_validate_extensions' => array('gif png jpg jpeg'),
        'file_validate_size' => array(1000000),
      ),
    );

    $image = file_load($fileslist['files'][$key]);
    if (is_object($image)) {
      $image->status = FILE_STATUS_PERMANENT;
      file_save($image);
      file_usage_add($image, 'image_360_degree_view', 'image', $fileslist['files'][$key]);

      $image_path = file_create_url($image->uri);
    }

    $form['image_360_degree_view']['allfiles']['image_' . $filecount] = array(
      '#markup' => '<img src = "' . $image_path . '" width = "100" />',
    );

    $filecount++;
  }
  // Use the #managed_file FAPI element to upload an image file.
  $form['image_360_degree_view']['allfiles']['file_' . $filecount] = array(
    '#title' => t('Image no :') . ($filecount + 1),
    '#type' => 'managed_file',
    '#upload_location' => 'public://image_360_degree_view/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(1000000),
    ),
  );

  $form['image_360_degree_view']['image_360_degree_view_image_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Width (in px)'),
    '#default_value' => variable_get('image_360_degree_view_image_width'),
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['image_360_degree_view']['image_360_degree_view_image_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Image height (in px)'),
    '#default_value' => variable_get('image_360_degree_view_image_height'),
    '#size' => 5,
    '#required' => TRUE,
  );

  // Submit Button.
  $form['image_360_degree_view']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Images'),
  );
  return $form;
}

/**
 * Validate image width and height.
 */
function _image_360_degree_view_admin_settings_validate($form, $form_state) {

  if (isset($form_state['values']['image_360_degree_view_image_width'])) {
    if (!(is_numeric($form_state['values']['image_360_degree_view_image_width']))) {
      form_set_error('image_360_degree_view_image_width', t('Image width should be in numeric.'));
    }
    elseif ($form_state['values']['image_360_degree_view_image_width'] < 10) {
      form_set_error('image_360_degree_view_image_width', t('Image width should be greater than equal to 10.'));
    }
  }

  if (isset($form_state['values']['image_360_degree_view_image_height'])) {
    if (!(is_numeric($form_state['values']['image_360_degree_view_image_height']))) {
      form_set_error('image_360_degree_view_image_height', t('Image height should be in numeric.'));
    }
    elseif ($form_state['values']['image_360_degree_view_image_height'] < 10) {
      form_set_error('image_360_degree_view_image_height', t('Image height should be greater than equal to 10.'));
    }
  }
}

/**
 * Saving image uplaoded and its width and size.
 *
 * @param array $form
 *   Drupal submit form array.
 *
 * @param string $form_state
 *   Drupal form state array.
 */
function _image_360_degree_view_admin_settings_submit($form, &$form_state) {

  variable_set('image_360_degree_view_image_width', $form_state['values']['image_360_degree_view_image_width']);
  variable_set('image_360_degree_view_image_height', $form_state['values']['image_360_degree_view_image_height']);

  // To get the current list.
  if ($fileslist = variable_get('image_360_degree_view_image_files', FALSE)) {

    // Have to find out deleted ones.
    $i = 0;
    foreach ($fileslist['files'] as $fileindex => $fileidvalue) {
      if ($form_state['values']['allfiles']['file_' . $i] == 0) {
        // Deleted.
        $file = file_load($fileidvalue);
        if ($file) {
          // When a module is managing a file, it must manage the usage count.
          // Here we decrement the usage count with file_usage_delete().
          file_usage_delete($file, 'image_360_degree_view', 'sample_image', 1);

          // The file_delete() function takes a file object and checks to see if
          // the file is being used by any other modules. If it is the delete
          // operation is cancelled, otherwise the file is deleted.
          file_delete($file);
        }
      }
      $i++;
    }
  }

  // Now save files.
  $newlist = array('files' => array(), 'count' => 0);
  foreach ($form_state['values']['allfiles'] as $key => $value) {
    if ($value != 0) {
      // File exists.
      $file = file_load($value);
      if ($file->status == 0) {
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);

        // When a module is managing a file, it must manage the usage count.
        // Here we increment the usage count with file_usage_add().
        file_usage_add($file, 'image_360_degree_view', 'sample_image', 1);
      }
      $newlist['files'][$file->fid] = $file->fid;
      $newlist['count'] = $newlist['count'] + 1;
    }
  }

  if ($newlist['count'] > 0) {
    variable_set('image_360_degree_view_image_files', $newlist);
  }
  else {
    variable_set('image_360_degree_view_image_files', FALSE);
  }
  $form_state['redirect'] = 'admin/config/media/image_360_degree_view';
}
