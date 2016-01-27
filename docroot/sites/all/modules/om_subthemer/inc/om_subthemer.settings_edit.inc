<?php
// $Id$

/**
 * @file
 * OM Subthemer Settings Edit
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Menu callback for admin/settings/surveys/[node_type]/settings.
 *
 */
function om_settings_edit($form, &$form_state, $type, $name) {

  drupal_set_title(t('Settings for %type nodes', array('%type' => check_plain($name))), PASS_THROUGH);

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t(''),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  //vid, variable, value, source, type
  $sql = "SELECT vid, variable, value_html, value_php, source, type 
          FROM {om_subthemer_vars} 
          WHERE type = :type 
          ORDER BY variable ASC";

  $result = db_query($sql, array(':type' => $type));
  
  $vars = array();
  while ($record = $result->fetchObject()) {
    $vars[] = _om_settings_edit($record);
  }
  //dsm($vars);
  $settings = array();
  if (empty($vars)) {
    $settings = om_base_theme_file($type, 'settings');
    
    om_subthemer_settings_upload($settings, $type);
            
    $result = db_query($sql, array(':type' => $type));
    
    $vars = array();
    while ($record = $result->fetchObject()) {
      $vars[] = _om_settings_edit((object) $record);
    }    
  }
                   
  $record = new stdClass();
  $record->type = $type;
  _om_new_rows($vars, 5, $record, 'settings_edit');   
 
  $form['settings'] += $vars;  
  //dsm($form);
  $form['submit'] = array( '#type' => 'submit', '#value' => t('Save Settings'), );

  $form['export'] = array( '#type' => 'submit', '#value' => t('Export'), );

  $form['import'] = array( '#type' => 'submit', '#value' => t('Import'), );
    
  return $form;
}

/**
 * Layout Form
 *
 */
function _om_settings_edit($record = NULL) {
  //dsm($record);
  $form['vid'] = array(
    '#type' => 'hidden',
    '#value' => isset($record->vid) ? $record->vid: '',
  );
  
  $form['type'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($record->type) ? $record->type: '',
  );  
  
  $form['variable'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable'),    
    '#default_value' => isset($record->variable) ? $record->variable: '',
    '#size' => 30,
  );  
  
  $form['value_html'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML Value'),
    '#default_value' => isset($record->value_html) ? $record->value_html: '',
  );

  $form['value_php'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP Value'),
    '#default_value' => isset($record->value_php) ? $record->value_php: '',
  );  
  $form['source'] = array(
    '#type' => 'textfield',
    '#title' => t('Source'),
    '#default_value' => isset($record->source) ? $record->source: '',
    '#size' => 30,
  );

  $form['delete'] = array(
    '#type' => ($form['vid']['#value']) ? 'checkbox' : 'hidden',
    '#title' => t('Delete'),
    '#default_value' => 0,
    '#description' => t(''),
  );    
  return $form;
}

/**
 * Implementation of theme_hook()
 * - View in table format
 *
 */
function theme_om_settings_edit($vars) {
  $form = $vars['form'];
  //dsm($form);
  $rows = array();
  $output = '';
  $count = 0;

  foreach (element_children($form['settings']) as $key) {
    $count++;
    $row = array();
    // Strips labels
    $form['settings'][$key]['variable']['#title'] = '';
    $form['settings'][$key]['value_html']['#title'] = '';
    $form['settings'][$key]['value_php']['#title'] = '';
    $form['settings'][$key]['source']['#title'] = '';
    $form['settings'][$key]['delete']['#title'] = '';
    $row[] = array('class' => 'col-number', 'data' => $count . ' ' . drupal_render($form['settings'][$key]['vid']) . drupal_render($form['settings'][$key]['type']));
    $row[] = drupal_render($form['settings'][$key]['variable']);
    $row[] = drupal_render($form['settings'][$key]['value_html']);
    $row[] = drupal_render($form['settings'][$key]['value_php']);
    $row[] = drupal_render($form['settings'][$key]['source']);
    $row[] = drupal_render($form['settings'][$key]['delete']);
    $rows[] = $row;
  }

  $header = array('No.', 'Variable', 'HTML Value', 'PHP Value', 'Source', 'Delete');

  // Header
  $form['settings']['#children'] = theme('table', array('header' => $header, 'rows' => $rows));

  $output .= drupal_render_children($form);
  
  return $output;
}


/**
 * Settings Submit
 * 
 */
function om_settings_edit_submit($form, $form_state) {
  $form_values = $form_state['values'];

  if ($form_state['values']['op'] == t('Save Settings')) {

    foreach ($form_values['settings'] as $vars) {
  
      $variable = ucwords(trim($vars['variable']));
      $source   = ucwords(trim($vars['source']));
    
      if ($vars['delete']) {
        // Delete an existing vars
        db_delete('om_subthemer_vars')
          ->condition('vid', $vars['vid'], '=')
          ->execute(); 
        
        drupal_set_message(t('The variable (' .  $variable . ') has been deleted.'));
      }
      elseif ($vars['vid']) {
        // Update an existing vars  
        db_update('om_subthemer_vars') // Table name no longer needs {}
          ->fields(array(
            'variable'   => $variable,
            'value_html' => $vars['value_html'],
            'value_php'  => $vars['value_php'],
            'source'     => $source,
          ))
          ->condition('vid', $vars['vid'], '=')
          ->execute();
          
        drupal_set_message(t('The variable (' .  $variable . ') has been updated.'));
      }
      elseif (!$vars['vid'] && !empty($variable)) {
        // Create a new vars
        db_insert('om_subthemer_vars')
          ->fields(array('variable', 'value_html', 'value_php', 'source', 'type'))
          ->values(array(
            'variable'   => $variable,
            'value_html' => $vars['value_html'],  
            'value_php'  => $vars['value_php'],                    
            'source'     => $source,
            'type'       => $vars['type'],                                                                          
          ))
          ->execute();
          
        drupal_set_message(t('The variable (' .  $variable . ') has been added.'));
      }
      else {
        // If no condition is set, nothing should happen
      }
    }
  }
  elseif ($form_state['values']['op'] == t('Export')) {
    drupal_goto('admin/settings/om-subthemer/'. $form_values['settings'][0]['type'] . '/export');
  }
  elseif ($form_state['values']['op'] == t('Import')) {
    drupal_goto('admin/settings/om-subthemer/'. $form_values['settings'][0]['type'] . '/import');
  }  
}



