<?php
// $Id$

/**
 * @file
 * OM Subthemer Settings Import
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Subthemer import settings form
 *
 */
function om_settings_import($form, &$form_state, $type, $name) {

  drupal_set_title(t('Import %type settings', array('%type' => check_plain($name))), PASS_THROUGH);
  
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['import']['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );    
  $form['import']['settings'] = array(
    '#type' => 'textarea',
    '#title' => t(''),
    '#rows' => 30,    
    '#default_value' => '',
  );  
  $form['submit'] = array( '#type' => 'submit', '#value' => t('Import'), );  
  
  return $form;
}

/**
 * OM Layout Import Submit
 * 
 */
function om_settings_import_submit($form, $form_state) {
  
  $form_values = $form_state['values'];

  // Delete an existing layout
  db_delete('om_subthemer_vars')
    ->condition('type', $form_values['import']['type'], '=')
    ->execute();  
      
  ob_start();
  eval($form_values['import']['settings']);
  ob_end_clean();

  if (is_array($settings)) {
    om_subthemer_settings_upload($settings, $form_values['import']['type']);
  }

  // new settings file has to be written for this new imported layout 
    
  drupal_set_message(t('New settings has been uploaded'));
  
  // redirect of view node
  drupal_goto('admin/settings/om-subthemer/' . $form_values['import']['type']);    
}


