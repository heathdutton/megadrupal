<?php

/**
 * @file
 * Provides link meta display filter module admin pages.
 */

/**
 * Menu callback; administer settings and upload images.
 */
function linkmeta_admin_page() {
  $form = array();

  // Append file size to link.
  $form['linkmeta_file_size'] = array(
    '#title' => t('Append file size to link'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('linkmeta_file_size', 0),
  );

  // Append extension to link.
  $form['linkmeta_file_extension'] = array(
    '#title' => t('Append file extension to link'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('linkmeta_file_extension', 0),
  );

  // Help text for icons.
  $form['help'] = array(
    '#type' => 'item',
    '#markup' => t('Uploaded icons will be resized to 16x16px. Upload icon files in correct size for best visual result.'),
  );

  // Icon for internal links.
  $form['linkmeta_internal_img'] = array(
    '#title' => t('Internal link image'),
    '#type' => 'managed_file',
    '#default_value' => variable_get('linkmeta_internal_img', ''),
    '#upload_location' => 'public://linkmeta/',
    '#theme' => 'image_widget',
  );
  if ($file = file_load(variable_get('linkmeta_internal_img', ''))) {
    $form['linkmeta_internal_img'] += array(
      '#attached' => array(
        'css' => array(
          'image-preview' => drupal_get_path('module', 'image') . '/image.css',
        ),
      ),
      'preview' => array(
        '#theme' => 'image',
        '#path' => $file->uri,
      ),
    );
  }

  // Icon for external links.
  $form['linkmeta_external_img'] = array(
    '#title' => t('External link image'),
    '#type' => 'managed_file',
    '#default_value' => variable_get('linkmeta_external_img', ''),
    '#upload_location' => 'public://linkmeta/',
    '#theme' => 'image_widget',
  );
  if ($file = file_load(variable_get('linkmeta_external_img', ''))) {
    $form['linkmeta_external_img'] += array(
      '#attached' => array(
        'css' => array(
          'image-preview' => drupal_get_path('module', 'image') . '/image.css',
        ),
      ),
      'preview' => array(
        '#theme' => 'image',
        '#path' => $file->uri,
      ),
    );
  }

  // Images for automatically detected extensions.
  $types = variable_get('linkmeta_types', '');
  $hidden_types = array('com', 'net', 'info', 'org');
  if (is_array($types)) {
    foreach ($types as $extension) {
      if (!in_array($extension, $hidden_types) && strlen($extension) > 2) {
        $form['linkmeta_' . $extension] = array(
          '#title' => t('Icon for @extension', array('@extension' => $extension)),
          '#type' => 'managed_file',
          '#default_value' => variable_get('linkmeta_' . $extension, ''),
          '#upload_location' => 'public://linkmeta/',
          '#theme' => 'image_widget',
        );
        if ($file = file_load(variable_get('linkmeta_' . $extension, ''))) {
          $form['linkmeta_' . $extension] += array(
            '#attached' => array(
              'css' => array(
                'image-preview' => drupal_get_path('module', 'image') . '/image.css',
              ),
            ),
            'preview' => array(
              '#theme' => 'image',
              '#path' => $file->uri,
            ),
          );
        }
      }
    }
  }

  $form['#validate'] = array('linkmeta_admin_form_validate');

  return system_settings_form($form);
}

/**
 * Validate: Code for uploaded images.
 */
function linkmeta_admin_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $key => $array) {
    if (substr($key, 0, 9) === 'linkmeta_' && $key !== 'linkmeta_file_size' && $key !== 'linkmeta_file_extension') {
      if ((int) $form_state['values'][$key] > 0 && $file = file_load($form_state['values'][$key])) {
        // Change status to permanent.
        $file->status = FILE_STATUS_PERMANENT;
        // Save file.
        file_save($file);

        // Scale and crop it.
        if ($image = image_load($file->uri)) {
          image_scale_and_crop($image, 16, 16);
          image_save($image);
        }

        variable_set($key, $form_state['values'][$key]);

        // Record that the module is using the file.
        file_usage_add($file, 'linkmeta', 'file', $file->fid);
      }
      else {
        $delete_fid = variable_get($key);
        $delete_fid = $delete_fid['fid'];
        // Delete file, if exists.
        if ($file = file_load($delete_fid)) {
          // Delete the file and the usage record.
          file_delete($file, TRUE);
        }
        variable_del($key);
      }

      // Make sure it is unset for system submit.
      unset($form_state['values'][$key]);
    }
  }

  // Regenerate css.
  module_load_include('inc', 'linkmeta', 'linkmeta.generate_css');
  linkmeta_generate_css(TRUE);
}
