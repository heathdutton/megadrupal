<?php
// $Id$

/**
 * @file
 * OM Subthemer Admin
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 * For users with admin access
 *  - admin can assign which content type to be used in OM Subtheming
 *
 */

/**
 * Configure Form
 * call back for admin/settings/om-subthemer menu
 */ 
function om_subthemer_admin($form, &$form_state) {
  
  drupal_set_title(t('OM Subthemer Settings'));
  $om_subthemer = variable_get('om_subthemer', array());  
    
  $form = array();
  
  $form['om_subthemer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subtheme Types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  
  // scan and load all available base themes and skins
  $base_themes = om_available_base_themes();
  $skins = om_available_skins();
  
  // get all node types
  $configure = array();
  foreach (node_type_get_types() as $type => $info) {
    $type_settings = isset($om_subthemer[$type]) ? $om_subthemer[$type]: array();
    $configure[$type] = _om_subthemer_admin($info, $type_settings, $base_themes, $skins);
  }

  $form['om_subthemer'] += $configure;  

  $form['submit'] = array( '#type' => 'submit', '#value' => t('Save Settings'), );

  return $form;
}

/**
 * Admin Configure form
 *
 */
function _om_subthemer_admin($info, $type_settings, $base_themes, $skins) {
  $form['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t($info->name),
    '#default_value' => isset($type_settings['enabled']) ? $type_settings['enabled']: 0,
  ); 
  $form['base_theme'] = array(
    '#type' => 'select',
    '#title' => t('Base Theme'),
    '#default_value' => isset($type_settings['base_theme']) ? $type_settings['base_theme']: 'om',
    '#options' => $base_themes,
  );
  $form['default_skin'] = array(
    '#type' => 'select',
    '#title' => t('Default Skin'),
    '#default_value' => isset($type_settings['default_skind']) ? $type_settings['default_skin']: 'starter',
    '#options' => $skins,
  );
  $form['html5'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML5'),
    '#default_value' => isset($type_settings['html5']) ? $type_settings['html5']: 1,
  );
  $form['grid'] = array(
    '#type' => 'select',
    '#title' => t('960gs'),
    '#default_value' => isset($type_settings['grid']) ? $type_settings['grid']: 0,
    '#options' => array(0 => '', 12 => 12, 16 => 16, 24 => 24),
  ); 
  $form['id_wrapper'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrapper ID'),
    '#default_value' => isset($type_settings['id_wrapper']) ? $type_settings['id_wrapper']: 1,
  ); 
  $form['id_wrapper_inner'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inner Wrapper ID'),
    '#default_value' => isset($type_settings['id_wrapper_inner']) ? $type_settings['id_wrapper_inner']: 0,
  );   
  $form['class_wrapper_level'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrapper Level Class'),
    '#default_value' => isset($type_settings['class_wrapper_level']) ? $type_settings['class_wrapper_level']: 0,
  );  
  $form['class_row'] = array(
    '#type' => 'checkbox',
    '#title' => t('Row Class'),
    '#default_value' => isset($type_settings['class_row']) ? $type_settings['class_row']: 0,
  );  
  $form['class_column'] = array(
    '#type' => 'checkbox',
    '#title' => t('Column Class'),
    '#default_value' => isset($type_settings['class_column']) ? $type_settings['class_column']: 0,
  ); 
  $form['class_first_last'] = array(
    '#type' => 'checkbox',
    '#title' => t('First/Last Class'),
    '#default_value' => isset($type_settings['class_first_last']) ? $type_settings['class_first_last']: 0,
  );
  return $form; 
}


/**
 * Submit
 * 
 */
function om_subthemer_admin_submit($form, $form_state) {
  
  $form_values = $form_state['values'];
  $om_subthemer = variable_get('om_subthemer', array());  
    
  //dsm($form_values['om_subthemer']);
  foreach ($form_values['om_subthemer'] as $type => $type_settings) {
    //dsm($admin);
    if ($type_settings['enabled']) {
      $om_subthemer[$type]['enabled']             = $type_settings['enabled'];
      $om_subthemer[$type]['base_theme']          = $type_settings['base_theme'];
      $om_subthemer[$type]['default_skin']        = $type_settings['default_skin'];    
      $om_subthemer[$type]['html5']               = $type_settings['html5'];    
      $om_subthemer[$type]['grid']                = $type_settings['grid'];    
      $om_subthemer[$type]['id_wrapper']          = $type_settings['id_wrapper'];
      $om_subthemer[$type]['id_wrapper_inner']    = $type_settings['id_wrapper_inner'];
      $om_subthemer[$type]['class_wrapper_level'] = $type_settings['class_wrapper_level'];
      $om_subthemer[$type]['class_row']           = $type_settings['class_row'];
      $om_subthemer[$type]['class_column']        = $type_settings['class_column'];     
      $om_subthemer[$type]['class_first_last']    = $type_settings['class_first_last'];    
    }
    else {
      unset($om_subthemer[$type]);
    } 
  } 

  variable_set('om_subthemer', $om_subthemer);  
  
  drupal_set_message(t('The settings have been succesfully saved.'));
  // for the new tab to appear
  cache_clear_all();
  menu_rebuild();  
}


/**
 * Implementation of theme_hook().
 *
 * View in table format
 *
 */
function theme_om_subthemer_admin($vars) {
  $form = $vars['form'];
  $rows = array();
  $output = '';

  foreach (element_children($form['om_subthemer']) as $key) {
    $row = array();
    // Strips labels
    //$form['om_subthemer'][$key]['enabled']['#title'] = '';
    $form['om_subthemer'][$key]['base_theme']['#title'] = '';
    $form['om_subthemer'][$key]['default_skin']['#title'] = '';
    $form['om_subthemer'][$key]['html5']['#title'] = '';
    $form['om_subthemer'][$key]['grid']['#title'] = '';
    $form['om_subthemer'][$key]['id_wrapper']['#title'] = '';
    $form['om_subthemer'][$key]['id_wrapper_inner']['#title'] = '';
    $form['om_subthemer'][$key]['class_wrapper_level']['#title'] = '';
    $form['om_subthemer'][$key]['class_row']['#title'] = '';
    $form['om_subthemer'][$key]['class_column']['#title'] = '';
    $form['om_subthemer'][$key]['class_first_last']['#title'] = '';
    
    $row[] = drupal_render($form['om_subthemer'][$key]['enabled']);
    $row[] = drupal_render($form['om_subthemer'][$key]['base_theme']);
    $row[] = drupal_render($form['om_subthemer'][$key]['default_skin']);
    $row[] = drupal_render($form['om_subthemer'][$key]['html5']);
    $row[] = drupal_render($form['om_subthemer'][$key]['grid']);
    $row[] = drupal_render($form['om_subthemer'][$key]['id_wrapper']);
    $row[] = drupal_render($form['om_subthemer'][$key]['id_wrapper_inner']);
    $row[] = drupal_render($form['om_subthemer'][$key]['class_wrapper_level']);         
    $row[] = drupal_render($form['om_subthemer'][$key]['class_row']);
    $row[] = drupal_render($form['om_subthemer'][$key]['class_column']);
    $row[] = drupal_render($form['om_subthemer'][$key]['class_first_last']);   

    $rows[] = $row;
  }
  
  // Header
  $header = array(t('Enabled'), t('Base Theme'), t('Default Skin'), t('HTML5'), t('960gs'), t('Wrapper ID'), t('Inner Wrapper ID'), t('Wrapper Level Class'), t('Row Class'), t('Column Class'), t('First/Last Class'));

  $form['om_subthemer']['#children'] = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  
  return $output;
}

