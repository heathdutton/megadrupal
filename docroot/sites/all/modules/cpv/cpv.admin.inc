<?php
/**
 * @file
 * Implements the administrative configuration page for CPV.
 */

/**
 * Settings form.
 */
function cpv_settings($form, &$form_state) {
  $form['cpv_save'] = array(
    '#type' => 'radios',
    '#title' => t('Method of serving code'),
    '#default_value' => variable_get('cpv_save', 'inline'),
    '#options' => array(
      'inline' => t('Inline'),
      'file' => t('File'),
    ),
    '#description' => t('When code is served inline, it is embedded into the HTML; when it is served from a file, the code is cached in a file referenced by the HTML.'),
  );
  $form['cpv_path'] = array(
    '#title' => t('File storage path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('cpv_path', 'public://cpv'),
    '#description' => t('The path where Code per Views Page Display will store files.'),
  );

  $form['#submit'][] = 'cpv_settings_submit';
  return system_settings_form($form);
}

/**
 * Settings form - validation callback.
 */
function cpv_settings_validate($form, &$form_state) {
  // Trim path of leading or trailing slashes and whitespaces.
  $form_state['values']['cpv_path'] = trim(trim($form_state['values']['cpv_path'], '/'));
  
  // Verify the path is a valid and allowed URI.
  $path = $form_state['values']['cpv_path'];
  if (!file_valid_uri($path)) {
    form_set_error('cpv_path', t('%path is not valid or not allowed.', array('%path' => $recipient)));
  }
}

/**
 * Settings form - submit callback.
 */
function cpv_settings_submit($form, &$form_state) {
  $old_path = variable_get('cpv_path', 'public://cpv');
  $new_path = $form_state['values']['cpv_path'];
  
  // Delete cache of code files if code is served inline.
  if ($form_state['values']['cpv_save'] == 'inline') {
    file_unmanaged_delete_recursive($old_path);
  }
  else {
    // If the path changed, notify that the folder must be moved.
    file_prepare_directory($new_path, FILE_CREATE_DIRECTORY);
    
    // If code was previously served inline, create new file cache for code.
    if (variable_get('cpv_save', 'inline') == 'inline') {
      $views = views_get_all_views();
      
      // Tracks if any errors occur while caching code into files.
      $error_count = 0;
      // Counts how many code files have been generated.
      $cache_count = 0;
      
      // Iterate through all displays of all views
      foreach ($views as $view_name => $view) {
        foreach ($view->display as $display_name => $display) {
          $display_options = $display->display_options;
          $codes = array('css', 'js');
          
          // Check if any code exists for each display.
          foreach ($codes as $code) {
            $key = 'cpv_' . $code;
            
            // If code exists, save code into file, [view]-[display].[code].
            if (array_key_exists($key, $display_options)) {
              $data = trim($display_options[$key]);
              if (!empty($data)) {
                $filename = $view_name . '-' . $display_name . '.' . $code;
                if (!cpv_save_file($data, $filename, $new_path)) {
                  drupal_set_message(t('The %code code for the %view_name view, %display_name display could not be saved into %filename', array(
                    '%code'         => strtoupper($code),
                    '%view_name'    => $view_name,
                    '%display_name' => $display_name,
                    '%filename'     => $filename,
                  )), 'error');
                  $error_count++;
                }
                else {
                  $cache_count++;
                }
              }
            }
          }
        }
      }
      
      // Output how many files were cached and how many errors occurred.
      $cache_line = format_plural($cache_count,
        '1 code file has been cached.',
        '@count code files have been cached.'
      );
      $error_line = format_plural($error_count,
        '1 error occurred.',
        '@count errors occurred.'
      );
      drupal_set_message($cache_line . ' ' . $error_line);
    }
    // Else move existing cache to new path
    else {
      if ($new_path != $old_path and file_exists($old_path)) {
        if (cpv_dir_unmanaged_move($old_path, $new_path)) {
          drupal_set_message(t('The file storage path has changed; the contents of %old_path have been moved to %new_path.', array(
            '%old_path' => $old_path,
            '%new_path' => $new_path,
          )), 'status');
        }
        // Alert that the folder could not be automatically moved due to an error.
        else {
          drupal_set_message(t('The file storage path has changed, but the contents of %old_path could not be moved to %new_path. A manual move is needed.', array(
            '%old_path' => $old_path,
            '%new_path' => $new_path,
          )), 'warning');
        }
      }
    }
  }
}
