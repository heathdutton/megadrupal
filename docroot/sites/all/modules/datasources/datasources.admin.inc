<?php

/**
 * @file
 * Admin pages for Datasource.
 */


/**
 * Menu callback: displays the a datasource inputs overview.
 *
 */
function datasources_inputs_overview() {
  return theme('datasources_inputs_overview', array('inputs' => datasources_inputs_info()));
}

/**
 * Returns HTML for the datasource inputs overview.
 * 
 * @param array $variables
 * 
 * @ingroup themeable
 */
function theme_datasources_inputs_overview($variables) {
  $inputs=$variables['inputs'];
  
  $rows = array();
  foreach ($inputs as $input_id => $input_info) {
    $row = array();
    $row[] = theme('image', array(
    	'path' => drupal_get_path('module', 'datasource') . '/' . ($input_info['enabled']?'enabled.png':'disabled.png'),
        'alt' => $input_info['enabled']?t('Enabled'):t('Disabled'),
    ));
    $row[] = $input_info['name'];
    $row[] = array('data' => l(t('edit'), "admin/config/content/datasource/$input_id"), 'class' => array('nowrap'));
    if ($input_info['data']) {
      $row[] = array('data' => l(t('reset'), "admin/config/content/datasource/$input_id/reset"), 'class' => array('nowrap'));
    }
    else {
      $row[] = array();
    }
    if ($input_info['configure']) {
      $row[] = array('data' => l(t('configure'), $input_info['configure']), 'class' => array('nowrap'));
    }
    else {
      $row[] = array();
    }
    $rows[]=$row;
  }
  
  $header = array(
    t('Status'),
    t('Name'),
    array('data' => t('Operations'), 'colspan' => 3));
  
  return theme('table', array('header' => $header, 'rows' => $rows, 'empty' => t('No datasource inputs available.')));
}

/**
 * Form builder for the datasource input edit form.
 *
 * @param $input_id
 * 
 * @see datasources_form_edit_input_submit()
 * @ingroup forms
 */
function datasources_form_edit_input($form, $form_state, $input_id) {
  $input=datasources_inputs_info($input_id);
  
  drupal_set_title(t('Edit datasource input !name', array('!name' => $input['name'])));
  
  $form=array(
    '#_input_id' => $input_id,
  );
  
  $form['info'] = array(
    '#markup' => t('Enable or disable datasource input. Choose base entity type and bundle.'),
  );
  
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'item',
    '#markup' => $input['name'],
  );
  
  $form['enabled'] = array(
    '#title' => t('Enable'),
    '#description' => t('Enabling datasource input will allow import process.'),
    '#type' => 'checkbox',
    '#default_value' => $input['enabled'],
  );
  
  $form['entity'] = array(
      '#title' => t('Entity'),
      '#description' => t('Choose the main entity type that this import will create.'),
    );
  
  if ($input['entity']) {
    $entity = $input['entity'];
    $entity_info=entity_get_info($entity);
    $form['entity'] += array (
      '#type' => 'item',
      '#markup' => $entity_info['label'],
    );
    $form['entity']['#description'] = t('To modify this settings you have to reset the datasource input.');
  }
  else {
    $entity = isset($form_state['values']['entity'])?$form_state['values']['entity']:'';
    $entity_options=array('' => t('Choose'));
    foreach (entity_get_info() as $entity_type => $entity_info) {
      $entity_options[$entity_type] = strip_tags($entity_info['label']);
    }
    
    $form['entity'] += array(
      '#type' => 'select',
      '#options' => $entity_options,
      '#default_value' => $entity,
      '#required' => TRUE,
      '#ajax' => array(
          'callback' => 'datasources_inputs_overview_entity_ajax_callback',
          'wrapper' => 'datasource-entity-dependent',
       ),
    );
  }

  $form['entity_dependent']=array(
    '#prefix' => '<div id="datasource-entity-dependent">',
    '#suffix' => '</div>',
  );
  if ($entity) {
    $form['entity_dependent']['bundle'] = array(
      '#title' => t('Bundle'),
      '#description' => t('Choose the bundle that this import will create.'),
    );
    
    if ($input['bundle'])
    {
      $bundle=$input['bundle'];
      $entity_info=entity_get_info($entity);
      $bundle_info=$entity_info['bundles'][$bundle];
      $form['entity_dependent']['bundle'] += array(
        '#type' => 'item',
        '#markup' => $bundle_info['label'],
      );
      $form['entity_dependent']['bundle']['#description'] = t('To modify this settings you have to reset the datasource input.');
    }
    else {
      $bundle = isset($form_state['values']['bundle'])?$form_state['values']['bundle']:'';
      $bundle_options=array('' => t('Choose'));
      $entity_info=entity_get_info($entity);
      foreach ($entity_info['bundles'] as $bundle_type => $bundle_info) {
        $bundle_options[$bundle_type] = strip_tags($bundle_info['label']);
      }
      $form['entity_dependent']['bundle'] += array(
        '#type' => 'select',
        '#options' => $bundle_options,
        '#default_value' => $bundle,
        '#required' => TRUE,
      );
    }
  }

  $form['processing'] = array(
      '#title' => t('Processing options'),
      '#type' => 'fieldset',
  );
  
  $form['processing']['cron'] = array(
    '#title' => t('Launch in cron'),
    '#description' => t('Launch import process in Drupal cron.'),
    '#type' => 'checkbox',
    '#default_value' => $input['cron'],
  );

  $form['processing']['max'] = array(
    '#title' => t('Max records to import'),
    '#description' => t('Maximum number of records to import (in cron). -1 for no max.'),
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => $input['max'],
  );
  
  $form['processing']['count_mode'] = array(
    '#title' => t('Count mode'),
    '#description' => t('Count mode for max.'),
    '#type' => 'select',
    '#default_value' => $input['count_mode'],
    '#options' => array(
        DATASOURCES_COUNT_IMPORTED => t('Imported'),
        DATASOURCES_COUNT_PROCESSED => t('Processed'),
        DATASOURCES_COUNT_ALL => t('All'),
        ),
  );
  
  if (module_exists('process_api')) {
    $form['processing']['process_api_live'] = array(
        '#title' => t('Process API live'),
        '#description' => t('Process API integration : enable live processing at import time.'),
        '#type' => 'checkbox',
        '#default_value' => $input['process_api_live'],
    );
  }
  
  $default = '';
  foreach ($input['custom_fields'] as $k => $v) {
    $default .= "$k: " . json_encode($v) . "\n";
  }
  
  $form['custom_fields'] = array(
      '#title' => t('Custom fields'),
      '#description' => t('You can define some custom fields with fixed values. Put one field/value by line. Format is : [field:]value. value is read with json_decode().'),
      '#type' => 'textarea',
      '#default_value' => $default,
  );  
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * AJAX callback that just returns the entity dependent array.
 */
function datasources_inputs_overview_entity_ajax_callback(array $form, array &$form_state) {
  return $form['entity_dependent'];
}

/**
 * Submit callback.
 *
 * @see datasources_form_edit_input();
 */
function datasources_form_edit_input_submit($form, &$form_state) {
  $input_id=$form['#_input_id'];
  $data = variable_get('datasources_input_' . $input_id, array());
  $data['enabled'] = $form_state['values']['enabled'];
  if (isset($form_state['values']['entity'])) $data['entity'] = $form_state['values']['entity'];
  if (isset($form_state['values']['bundle'])) $data['bundle'] = $form_state['values']['bundle'];
  if (isset($form_state['values']['cron'])) $data['cron'] = $form_state['values']['cron'];
  if (isset($form_state['values']['max'])) $data['max'] = $form_state['values']['max'];
  if (isset($form_state['values']['count_mode'])) $data['count_mode'] = $form_state['values']['count_mode'];
  if (isset($form_state['values']['process_api_live'])) $data['process_api_live'] = $form_state['values']['process_api_live'];
  if (isset($form_state['values']['custom_fields'])) {
    $data['custom_fields'] = array();
    $i=1;
    foreach (preg_split("/\R/", $form_state['values']['custom_fields']) as $field) {
      $field = trim($field);
      if (!$field) continue;
      if (preg_match("/([a-z][a-z0-9_]*)[\s]*[:](.+)/i", $field, $matches)) {
        $data['custom_fields'][$matches[1]] = json_decode(trim($matches[2]));
      }
      else {
        $data['custom_fields']["f$i"] = json_decode($field);
        $i++;
      }
    }
  }
  variable_set('datasources_input_' . $input_id, $data);
  drupal_set_message(t('Settings saved.'));
}

/**
 * Form builder for the datasource input policy form.
 *
 * @param $input_id
 * 
 * @see datasources_form_map_input_submit()
 * @ingroup forms
 */
function datasources_form_policy_input($form, $form_state, $input_id) {
  $input=datasources_inputs_info($input_id);
  
  drupal_set_title(t('Datasource input !name policies', array('!name' => $input['name'])));

  $form=array(
    '#_input_id' => $input_id,
  );
  
  $form['info'] = array(
    '#markup' => t('Datasource input entity matching and update policy. ' . 
              'Udid field are potential functional keys'),
  );
  
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'item',
    '#markup' => $input['name'],
  );

  if (!$input['entity']) {
    drupal_set_message(t('You have to choose an entity type.'), 'error');
    return $form;
  }

  $form['keys'] = array(
    '#tree' => TRUE,
    '#theme' => 'datasources_form_policy_keys_weight',
    '#suffix' => '<div class="description">' . t('You can change the matching order. Smart keys policy allows you to always update, protect user modifications or update only new versions (see udid_compare()).') . '</div>',
  );
  
  foreach ($input['keys'] as $field_id => $key_info) {
    $label = $input['fields'][$field_id]['name'];
    if ($key_info['udid_preprocessor']) {
      $label = t('!path with !preprocessor() to !field', array('!path' => $label, '!preprocessor' => $key_info['udid_preprocessor'], '!field' => $key_info['udid_target_field']));
    }
    elseif ($key_info['udid_target_field']) {
      $label = t('!path to !field', array('!path' => $label, '!field' => $key_info['udid_target_field']));
    }
    $form['keys'][$field_id]['name'] = array(
      '#markup' => $label,
    );
    $form['keys'][$field_id]['use'] = array(
      '#type' => 'checkbox',
      '#default_value' => $key_info['use'],
    );
    $form['keys'][$field_id]['smart'] = array(
      '#type' => 'select',
      '#options' => array(
          DATASOURCES_SMART_DUMB => t('Always update'),
          DATASOURCES_SMART_PROTECT => t('Protect modifications'),
          DATASOURCES_SMART_NEW => t('New versions'),
          ),
      '#default_value' => $key_info['smart'],
    );
    $form['keys'][$field_id]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $key_info['weight'],
    );
  }  
  
  $form['policies'] = array(
    '#type' => 'item',
    '#title' => t('Import mode'),
    '#tree' => TRUE,
  );

  $form['policies']['insert'] = array(
    '#title' => t('Allow insert'),
    '#type' => 'checkbox',
    '#default_value' => $input['policies']['insert'],
  );
  
  $form['policies']['update'] = array(
    '#title' => t('Allow update'),
    '#type' => 'checkbox',
    '#default_value' => $input['policies']['update'],
  );  
  
  $form['policies']['duplicate'] = array(
    '#title' => t('Multiple matching entities'),
    '#type' => 'radios',
    '#options' => array(
    	'abort' => t('Abort'),
    	'first' => t('Update first'),
        'all' => t('Update all'),
     ),
    '#description' => t('You can choose what to do when import matches multiple existing entities for the same input record.'),
    '#default_value' => $input['policies']['duplicate'],
  );

  $form['policies']['bad_bundle'] = array(
    '#title' => t('Bad bundle'),
    '#type' => 'radios',
    '#options' => array(
    	'abort' => t('Abort'),
    	'update' => t('Update anyway'),
     ),
    '#description' => t('You can choose what to do when import matches an entity of the wrong bundle.'),
    '#default_value' => $input['policies']['bad_bundle'],
  );  

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  
  return $form;
}

/**
 * Theme for the keys weights table form fragment.
 * 
 * @param $variables
 *   An associative array containing:
 *   - form: the form to theme
 *   
 * @ingroup themeable
 */
function theme_datasources_form_policy_keys_weight($variables) {
  $form=&$variables['form'];
  
  drupal_add_tabledrag('datasource-keys-weight', 'order', 'sibling', 'key-weight');
  
  $rows=array();
  foreach (element_children($form) as $field) {
    $form[$field]['weight']['#attributes']['class'][] = 'key-weight';
    $row=array();
    
    $row[]=drupal_render($form[$field]['name']);
    $row[]=drupal_render($form[$field]['use']);
    $row[]=drupal_render($form[$field]['smart']);
    $row[]=drupal_render($form[$field]['weight']);
    
    $row['data']=$row;
    $row['class']=array('draggable');
    $rows[$field]=$row;
  }
  $header = array(t('Key'), t('Use'), t('Smart'), t('Weight'));
  
  $output=theme('table', array(
      'header' => $header, 
      'rows' => $rows, 
      'attributes' => array('id' => 'datasource-keys-weight')));

  return $output;
}

/**
 * Submit callback.
 *
 * @see datasources_form_policy_input();
 */
function datasources_form_policy_input_submit($form, &$form_state) {
  $input_id=$form['#_input_id'];
  $data = variable_get('datasources_input_' . $input_id, array());

  if (isset($form_state['values']['keys'])) {
    foreach ($form_state['values']['keys'] as $field => $key_info) {
      $data['keys'][$field]['use'] = $key_info['use'];
      $data['keys'][$field]['smart'] = $key_info['smart'];
      $data['keys'][$field]['weight'] = $key_info['weight'];
    }
  }
  
  foreach ($form_state['values']['policies'] as $policy => $value) {
    $data['policies'][$policy] = $value;
  }
  
  variable_set('datasources_input_' . $input_id, $data);
  drupal_set_message(t('Settings saved.'));
}

/**
 * Form builder for the datasource input map form.
 *
 * @param $input_id
 * 
 * @see datasources_form_map_input_submit()
 * @ingroup forms
 */
function datasources_form_map_input($form, $form_state, $input_id) {
  $input=datasources_inputs_info($input_id);
  
  drupal_set_title(t('Map datasource input !name fields', array('!name' => $input['name'])));

  $form=array(
    '#_input_id' => $input_id,
  );
  
  $form['info'] = array(
    '#markup' => t('Map datasource input fields to chosen entity fields.'),
  );
  
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'item',
    '#markup' => $input['name'],
  );

  if (!$input['entity']) {
    drupal_set_message(t('You have to choose an entity type.'), 'error');
    return $form;
  }
   
  $form['fields'] = array(
    '#tree' => TRUE,
    '#theme' => 'datasources_form_map_input_fields',
  );

  $fields_info=datasources_entity_fields($input_id);
  
  $entity_info=entity_get_info($input['entity']);  
  
  $map_options = array('' => t('Choose'));
  foreach($input['fields'] as $field_id => $field_info) {
    $map_options[$field_id] = strip_tags($field_info['name']). ' ('.$field_info['type'].')';
  }
 
  foreach ($fields_info['fields'] as $field_id => $field_info) {
    
    $preprocessor_options = array('' => t('Choose'));
    foreach (_datasources_preprocessor_filter_callbacks($input, $field_id, $field_info) as $callback_id => $callback) {
      $preprocessor_options[$callback_id] = $callback['name'];
    }
    
    $form['fields'][$field_id]['name'] = array(
      '#markup' => $field_info['name'] . ' (' . $field_id . ')',
    );
    $form['fields'][$field_id]['type'] = array(
      '#markup' => check_plain($field_info['type']),
    );
    $form['fields'][$field_id]['map'] = array(
      '#type' => 'select',
      '#options' => $map_options,
      '#default_value' => isset($field_info['map']['field']) ? $field_info['map']['field'] : NULL,
    );
    $form['fields'][$field_id]['list'] = array(
      '#type' => 'select',
      '#options' => array(
            '' => t('Default'),
            'replace' => t('Managed replace'),
            'append' => t('Managed append'),
            'first' => t('Managed first'),
            'last' => t('Managed last'),
          ),
      '#default_value' => isset($field_info['map']['list']) ? $field_info['map']['list'] : '',
    );
        
    $form['fields'][$field_id]['preprocessor'] = array(
      '#type' => 'select',
      '#options' => $preprocessor_options,
      '#default_value' => isset($field_info['map']['preprocessor']) ? $field_info['map']['preprocessor'] : '',
    );
    
  }
  
  $options=array('' => t('Choose')) + $fields_info['additional_structures'];
  $form['add_structure'] = array(
    '#title' => t('Add structures for !entity (!bundle)', array('!entity' => $entity_info['label'], '!bundle' => $input['bundle'])),
    '#type' => 'select',
    '#options' => $options,
    '#description' => t('You can add fields from the selected structure. Those fields will be added to the mapping seletors.'),
    '#default_value' => '',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  if ($input['structures']) {
    $form['structures'] = array(
      '#type' => 'fieldset',
      '#title' => t('Structures for !entity (!bundle)', array('!entity' => $entity_info['label'], '!bundle' => $input['bundle'])),
      '#tree' => TRUE,
    );
    foreach ($input['structures'] as $prefix => $name) {
      if (!$prefix) continue;
      $form['structures']['list'][$prefix]['delete'] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE,
        '#title' => t('Remove @struct', array('@struct' => check_plain($name))),
      );
    }
    
    $form['structures']['remove'] = array(
        '#type' => 'submit',
        '#value' => t('Remove fields'),
    );
  }
  
  return $form;
}

/**
 * Submit callback.
 *
 * @see datasources_form_map_input();
 */
function datasources_form_map_input_submit($form, &$form_state) {
  $input_id=$form['#_input_id'];
  $data = variable_get('datasources_input_' . $input_id, array());
  if (isset($form_state['values']['add_structure'])&&$form_state['values']['add_structure']) {
    if (!isset($data['structures'])) {
      $data['structures']=array();
    }
    $data['structures'][$form_state['values']['add_structure']] = $form['add_structure']['#options'][$form_state['values']['add_structure']];
  }
  
// user wants some structure to be removed
  if (isset($form_state['values']['structures']['list'])) {
    foreach($form_state['values']['structures']['list'] as $prefix => $info) {
      if ($info['delete']) {
        unset($data['structures'][$prefix]);
      }
    }
  }
  
  if (isset($form_state['values']['fields'])&&is_array($form_state['values']['fields'])) {
    $data['fields_map'] = array();
    foreach ($form_state['values']['fields'] as $field_id => $field_info) {
      $data['fields_map'][$field_id] = array('field' => $field_info['map'], 'list' => $field_info['list'], 'preprocessor' => $field_info['preprocessor']);
    }
  }
  variable_set('datasources_input_' . $input_id, $data);
  drupal_set_message(t('Settings saved.'));
}

/**
 * Theme for the map fields table.
 * 
 * @param $variables
 *   An associative array containing:
 *   - form: the form to theme
 *   
 * @ingroup themeable
 */
function theme_datasources_form_map_input_fields($variables) {
  $form=&$variables['form'];
  
  $rows=array();
  foreach (element_children($form) as $field) {
    $row=array();
    
    $row[]=drupal_render($form[$field]['name']);
    $row[]=drupal_render($form[$field]['type']);
    $row[]=drupal_render($form[$field]['map']);
    $row[]=drupal_render($form[$field]['list']);
    $row[]=drupal_render($form[$field]['preprocessor']);
    
    $rows[$field]=$row;
  }
  
  $header=array(
    t('Target'),
    t('Type'),
    t('Source'),
    t('Multi valued field'),
    t('Preprocessor'),
  );
  
  $output=theme('table', array(
      'header' => $header, 
      'rows' => $rows));

  return $output;
}

/**
 * Form builder for the datasource input reset form.
 *
 * @param $input_id
 * 
 * @see datasources_form_reset_input_submit()
 * @ingroup forms
 */
function datasources_form_reset_input($form, $form_state, $input_id) {
  $input=datasources_inputs_info($input_id);
  return confirm_form(
    array(
      'source' => array(
        '#type' => 'value',
        '#value' => $input_id,
      ),
    ),
    t('Are you sure you want to reset and disable datasource %name?', array('%name' =>$input['name'])),
    'admin/config/content/datasource/' . $input_id,
    t('This action cannot be undone.'),
    t('Reset datasource input'),
    t('Cancel')
  );
}

/**
 * Submit callback.
 *
 * @see datasources_form_reset_input();
 */
function datasources_form_reset_input_submit($form, &$form_state) {
  datasources_reset_input($form_state['values']['source']);
  $form_state['redirect'] = 'admin/config/content/datasource/' . $form_state['values']['source'];
}