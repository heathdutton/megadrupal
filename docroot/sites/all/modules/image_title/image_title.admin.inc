<?php
/**
 * form containing available admin settings
 *
 * @return
 * array containing the form
 */
function image_title_admin_form($form, &$form_state) {
  //define our admin form
  $form = array();
  $form['image_title_directory'] = array(
    '#title' => t('Location within default files directory to save title images'),
    '#type' => 'textfield',
    '#required' => 'true',
    '#description' => t('Important: changing the directory where title images are saved will cause all currently uploaded title images to cease to be available.'),
    '#default_value' => variable_get('image_title_directory', 'img-title'),
  );
  $form['submit'] = array(
    '#value' => 'Save settings',
    '#type' => 'submit',
  );
  return $form;
}

/**
 * validate function to check user-entered data from the admin form
 */
function image_title_admin_form_validate($form_id, $form_state) {

  $fullpath = variable_get('file_public_path', conf_path() . '/files') . '/' . $form_state['values']['image_title_directory'];

  if (!file_exists($fullpath)) {
    if (@mkdir($fullpath, intval("00", 8))) {
      $mod = fileperms($fullpath) & 0777;
      // update permission 
      @chmod($fullpath, $mod);
    }
    if (!file_exists($fullpath)) {
      //file_check_directory() creates a form error automatically 
      file_prepare_directory($fullpath, 0, 'image_title_upload');
    }

  }

  // check if not writable .. 
  if ( !is_writable($fullpath) ) {
    $mod = fileperms($fullpath) & 0777;
    // attempt to update permission 
    @chmod($fullpath, $mod);
    if ( !is_writable($fullpath) ) {
      form_set_error('image_title_upload', t("Directory is not writable"));
    }
  }

}

/**
 * submit function sets variables for this module:
 */
function image_title_admin_form_submit($form_id, $form_state) {
  //save the variables
  variable_set('image_title_directory', $form_state['values']['image_title_directory']);
  drupal_set_message(t('Settings saved.'));
}
