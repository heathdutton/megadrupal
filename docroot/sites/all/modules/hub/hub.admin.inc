<?php

/**
 * Export hub settings via this form.
 */
function hub_export_settings($form, &$form_state) {
  ctools_include('export');
  $name = (isset($_GET['name'])) ? $_GET['name'] : '';
  
  $object = new stdClass;
  $object->name = $name;
  $object->data = hub_load($name);
  
  $code = ctools_export_object('hub', $object, '');
  $lines = substr_count($code, "\n");
  
  $form['export'] = array(
    '#title' => t('Export hub settings'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines + 1,
    '#description' => t('Copypasta this code into your export module.'),
  );
  return $form;
}


/**
 * Menu callback to confirm delete/revert of a hub setting.
 */
function hub_delete_confirm($form, &$form_state) {
  $name = isset($_GET['name']) ? $_GET['name'] : '';
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $name,
  );
  $destination = isset($_GET['destination']) ? $_GET['destination'] : 'admin';
  return confirm_form($form, t('Are you sure you want to delete @name?', array('@name' => $name)), $destination, '', t('Delete'), t('Cancel'));
}


function hub_delete_confirm_submit($form, &$form_state) {
  hub_save('delete', $form_state['values']['name'], TRUE);
}