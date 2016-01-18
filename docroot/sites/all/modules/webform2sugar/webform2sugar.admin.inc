<?php

/**
 * @file
 * Forms and callbacks for controlling the admin side of webform2sugar.
 * 
 * These forms will appear in webform2sugar local action of webforms.
 */

function webform2sugar_manage($node) {
  //Lookup a webform2sugar_webform entry if it exists
  $select = db_select('webform2sugar_webform', 'sugar_webform');
  $select->fields('sugar_webform');
  $select->condition('nid', $node->nid);
  $sugar_webform = $select->execute()->fetchAll();
  if ($sugar_webform) {
    $sugar_webform = $sugar_webform[0];
  }
  
  $select = db_select('webform2sugar_component', 'sugar_component');
  $select->fields('sugar_component');
  $select->condition('nid', $node->nid);
  $sugar_components = $select->execute()->fetchAll();
  
  $settings = array();
  $mappings = array();
  
  $select = db_select('webform_component', 'component');
  $select->fields('component');
  $select->condition('nid', $node->nid);
  $component_results = $select->execute()->fetchAll();

  $components = array();
  foreach ($component_results as $component) {
    $components[$component->cid] = $component;
  }
  
  foreach ($sugar_components as $sugar_component) {
    $delete_url = url('node/' . $node->nid . '/webform/webform2sugar/delete/' . $sugar_component->sid);
    $component = $components[$sugar_component->cid];
    $mappings[] = array($component->name, $component->form_key, $sugar_component->sugarkey, "<a href='$delete_url'>Delete</a>");
  }
  
  $render_array['mappings'] = array(
    '#theme' => 'table',
    '#header' => array('Form Field', 'Form Key', 'Sugar Field', 'Operations'),
    '#rows' => $mappings,
    '#empty' => 'No Mappings Yet'
  );
  
  $render_array['form'] = drupal_get_form('webform2sugar_settings_form', $node, $sugar_webform, $sugar_components);
  
  if ($sugar_webform) {
    if (!$sugar_webform->active) {
      drupal_set_message(t('This webform is currently set to NOT send to sugarCRM.'), 'warning');
    }
  }
  
  return $render_array;
}

function webform2sugar_component_delete($node, $sid) {
  try {
    db_delete('webform2sugar_component')
      ->condition('sid', $sid)
      ->condition('nid', $node->nid)
      ->execute();
      
    drupal_goto('node/' . $node->nid . '/webform/webform2sugar');
  } catch (Exception $e) {
    drupal_set_message(t('Error trying to store settings in database'), 'error');
  }
  
}

function theme_webform2sugar_manage_overview($variables) {
  $settings = $variables['settings'];
  $mappings = $variables['mappings'];
  
  return 'SugarCRM Settings';
}

function webform2sugar_component_add_validate($form, &$form_state) {
  if ($form_state['values']['sugar_key'] == 'See Below' && !$form_state['values']['sugar_other']) {
    form_set_error('sugar_other', 'Field is required when Other is selected for Sugar Field');
  }
  
  if ($form_state['values']['sugar_other'] && $form_state['values']['sugar_key'] != 'See Below') {
    form_set_error('sugar_key', 'Sugar Field must be Other if using a custom mapping');
  }
}

function webform2sugar_component_add_submit($form, &$form_state) {
  
  $values['nid'] = $form_state['values']['nid'];
  $values['cid'] = $form_state['values']['component_id'];
  
  if ($form_state['values']['sugar_other']) {
    $values['sugarkey'] = $form_state['values']['sugar_other'];
  }
  else {
    $values['sugarkey'] = $form_state['values']['sugar_key'];
  }
  
  try {
    $return_value = db_insert('webform2sugar_component')
              ->fields($values)
              ->execute();
  } catch (Exception $e) {
    drupal_set_message(t('Error trying to store settings in database'), 'error');
  }
  
  drupal_goto('node/' . $form_state['values']['nid'] . '/webform/webform2sugar');
}


function webform2sugar_component_add($form, &$form_state, $node) {
  $select = db_select('webform_component', 'component');
  $select->fields('component');
  $select->condition('nid', $node->nid);
  $component_results = $select->execute()->fetchAll();

  include(drupal_get_path('module', 'webform2sugar') . "/webform2sugar_fields.inc");

  $component_options = array();
  
  foreach ($component_results as $component) {
    $component_options[$component->cid] = $component->name . ' (' . $component->form_key . ')';
  }
  
  $sugar_options = array();
  
  foreach ($webform2sugar_available_fields as $key => $value) {
    $sugar_options[$key] = $value . ' (' . $key . ')';
  }
  
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  
  $form['component_id'] = array(
    '#type' => 'select',
    '#title' => t('Webform Field'),
    '#options' => $component_options,
    '#required' => TRUE,
    '#description' => 'Field in Webform to map from',
  );
  
  $form['sugar_key'] = array(
    '#type' => 'select',
    '#title' => t('Sugar Field'),
    '#options' => $sugar_options,
    '#required' => TRUE,
    '#description' => 'Field to map to in SugarCRM',
  );
  
  $form['sugar_other'] = array(
    '#type' => 'textfield',
    '#title' => t('Other Sugar Field (Custom Mapping)'),
    '#maxlength' => 255,
    '#required' => FALSE,
    '#description' => "If 'Other' is selected above, enter custom sugar mapping",
  );
    
  $form['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}

function webform2sugar_settings_form_submit($form, &$form_state) {

  $nid = $form_state['values']['nid'];
  $select = db_select('webform2sugar_webform', 'sugar_webform');
  $select->fields('sugar_webform');
  $select->condition('nid', $nid);
  $sugar_webform = $select->execute()->fetchAll();
  
  $values['nid'] = $nid;
  $values['url'] = $form_state['values']['url'];
  $values['campaign'] = $form_state['values']['campaign'];
  $values['active'] = $form_state['values']['active'];
  
  try {
    if ($sugar_webform) {
      $count = db_update('webform2sugar_webform')
          ->fields($values)
          ->condition('nid', $values['nid'])
          ->execute();
    }
    else {
      $return_value = db_insert('webform2sugar_webform')
                    ->fields($values)
                    ->execute();
    }
  } catch (Exception $e) {
    drupal_set_message(t('Error trying to store settings in database'), 'error');
  }  
}

function webform2sugar_settings_form_validate($form, &$form_state) {
  //Make sure the URL has a valid schema
  if (strpos($form_state['values']['url'], 'http://') !== 0 && strpos($form_state['values']['url'], 'https://') !== 0) {
    form_set_error('url', 'URL Must begin with either http:// or https://');
  }
  
  //Remove a trailing / if one exists
  if (strrpos($form_state['values']['url'], '/') == strlen($form_state['values']['url']) -1) {
    $form_state['values']['url'] = substr($form_state['values']['url'], 0, strlen($form_state['values']['url']) -1);
  }
  
  //Make sure the campaign id at least looks correct.
  if (substr_count($form_state['values']['campaign'], '-') != 4) {
    form_set_error('campaign', 'Campaign must be UID from Sugar CRM.  Example: b93f96b1-0e52-eac7-cff1-4f0e1fcadc39');
  }
  
  //Check the url to see if we can connect to it.
  $result = drupal_http_request($form_state['values']['url'], array('max_redirects' => 0, 'timeout' => 10));
  if ($result->code <= 0) {
    form_set_error('url', "Unable to connect to url, message: '$result->error'");
  }
}

function webform2sugar_settings_form($form, &$form_state, $node, $sugar_webform, $sugar_components) {
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sugar CRM Settings'),
  );
  
  $form['settings']['active'] = array(
    '#type' => 'select',
    '#title' => t('Send to SugarCRM'),
    '#default_value' => $sugar_webform ? $sugar_webform->active : 0,
    '#options' => array('0' => 'False', '1' => 'True'),
    '#required' => TRUE,
  );
  
  $form['settings']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('SugarCRM URL'),
    '#default_value' => $sugar_webform ? $sugar_webform->url : '',
    '#size' =>  80,
    '#maxlength' => 64,
    '#required' => TRUE,
    '#description' => t('Enter the URL of your SugarCRM System (Ex: http://www.example.com/sugarcrm)'),
  );
  
  $form['settings']['campaign'] = array(
    '#type' => 'textfield',
    '#title' => t('Default SugarCRM Campaign Id'),
    '#default_value' => $sugar_webform ? $sugar_webform->campaign : '',
    '#size' =>  80,
    '#maxlength' => 64,
    '#required' => TRUE,
    '#description' => t('Enter the id of your SugarCRM Campaign (Ex: b93f96b1-0e52-eac7-cff1-4f0e1fcadc39)'),
  );
    
  $form['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  
  return $form;
}