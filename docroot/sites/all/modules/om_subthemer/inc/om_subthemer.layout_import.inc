<?php
// $Id$

/**
 * @file
 * OM Subthemer Layout Import
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
 
/**
 * OM Subthemer import layout form
 *
 */
function om_layout_import($form, &$form_state) {

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import Layout'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['import']['nid'] = array(
    '#type' => 'hidden',
    '#value' => arg(1),
  );    
  $form['import']['layout'] = array(
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
function om_layout_import_submit($form, $form_state) {
  
  $form_values = $form_state['values'];

  // Delete an existing layout
  db_delete('om_subthemer_layout')
    ->condition('nid', $form_values['import']['nid'], '=')
    ->execute(); 
      
  ob_start();
  eval($form_values['import']['layout']);
  ob_end_clean();
  
  if (is_array($layout)) {
    om_subthemer_layout_upload($layout, $form_values['import']['nid']);
  }
  
  // new layout file has to be written for this new imported layout
  
  // need this to generate the layout
  include_once OM_SUBTHEMER_PATH . '/inc/om_subthemer.write.inc';    

  // need to update the files
  $node = node_load($form_values['import']['nid']);

  // set user folder 
  om_subthemer_user_folder($node);

  // 1) get layout for variables
  om_process_layout($node);
    
  // 2) write om subtheme
  om_file_write($node);   
    
  drupal_set_message(t('New layout has been generated'));
  
  // redirect of view node
  drupal_goto('node/' . $form_values['import']['nid']);    
}


