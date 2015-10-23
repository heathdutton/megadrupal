<?php
// $Id: mumu.admin.inc 2/9/12 5:25 PM johnvsc@gmail.com

/**
 * @return array
 */
function mumu_admin_form($form, &$form_state) {
  // if there is a file, save it to the form state
  $css_file_fid = variable_get('mumu_css');
  $js_file_fid  = variable_get('mumu_js');
  if (!empty($css_file_fid)) {
    $form_state['css_orig_file_object'] = file_load($css_file_fid);
  }
  if (!empty($js_file_fid)) {
    $form_state['js_orig_file_object'] = file_load($js_file_fid);
  }

  $form             = array();
  $form['mumu_css'] = array(
    '#title'             => t('CSS'),
    '#type'              => 'managed_file',
    '#description'       => t("Upload a CSS file. Allowed extension is: css."),
    '#default_value'     => $css_file_fid,
    '#upload_location'   => 'public://',
    '#upload_validators' => array('file_validate_extensions' => array('css')),
  );

  $form['mumu_js'] = array(
    '#title'             => t('JS'),
    '#type'              => 'managed_file',
    '#description'       => t("Upload a JavaScript file. Allowed extension is: js."),
    '#default_value'     => $js_file_fid,
    '#upload_location'   => 'public://',
    '#upload_validators' => array('file_validate_extensions' => array('js')),
  );

  $form['submit'] = array(
    '#type'     => 'submit',
    '#value'    => t('Update'),
    '#validate' => array('mumu_admin_form_validate'),
    '#submit'   => array('mumu_admin_form_submit'),
  );

  return $form;
}

function mumu_admin_form_validate($form, &$form_state) {

  if (!empty($form_state['values']['mumu_css'])) {
    // Load the file via file.fid.();
    $css_file = file_load($form_state['values']['mumu_css']);
    // Change status to permanent.
    $css_file->status = FILE_STATUS_PERMANENT;
    // Save.
    $form_state['saved_css'] = file_save($css_file);
    // Record that the module (in this example, user module) is using the file.
    file_usage_add($css_file, 'mumu', 'file', $form_state['values']['mumu_css']['fid']);
  }
  // we need to remove the original file
  if (!empty($form_state['css_orig_file_object']->fid) && $form_state['values']['mumu_css'] != $form_state['css_orig_file_object']->fid) {
    file_delete($form_state['css_orig_file_object']);
  }

  if (!empty($form_state['values']['mumu_js'])) {
    // Load the file via file.fid.
    $js_file = file_load($form_state['values']['mumu_js']);
    // Change status to permanent.
    $js_file->status = FILE_STATUS_PERMANENT;
    // Save.
    $form_state['saved_js'] = file_save($js_file);
    // Record that the module (in this example, user module) is using the file.
    file_usage_add($js_file, 'mumu', 'file', $form_state['values']['mumu_js']['fid']);
  }
  // we need to remove the original file
  if (!empty($form_state['js_orig_file_object']->fid) && $form_state['values']['mumu_js'] != $form_state['js_orig_file_object']->fid) {
    file_delete($form_state['js_orig_file_object']);
  }
}

function mumu_admin_form_submit($form, &$form_state) {

  if ($form_state['values']['mumu_css'] != 0) {
    variable_set('mumu_css', $form_state['values']['mumu_css']);
  }
  else {
    variable_set('mumu_css', '');
  }

  if ($form_state['values']['mumu_js'] != 0) {
    variable_set('mumu_js', $form_state['values']['mumu_js']);
  }
  else {
    variable_set('mumu_js', '');
  }
  drupal_set_message(t('MuMu has been updated.'));
  cache_clear_all();
}

