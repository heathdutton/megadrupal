<?php
/**
 * @file
 * Provides a plugin that maps arbitrary values in field collections to the ids of the containing entities.
 *
 * © 2014 RedBottle Design, LLC and Five Star Tool Company. All rights reserved.
 *
 * Based on the "Entity Field Query Finder" plugin from the "Feeds Tamper" module.
 * 
 * @author Guy Paddock (guy.paddock@redbottledesign.com)
 */

$plugin = array(
  'form'      => 'feeds_tamper_efq_fc_finder_form',
  'callback'  => 'feeds_tamper_efq_fc_finder_callback',
  'validate'  => 'feeds_tamper_efq_fc_finder_validate',
  'name'      => 'Field Collection Entity Field Query finder',
  'multi'     => 'loop',
  'category'  => 'Other',
);

/**
 * Configuration form.
 */
function feeds_tamper_efq_fc_finder_form($importer, $element_key, array $settings, array &$form_state) {
  $form = array(
    '#prefix' => '<div id="feeds-tamper-fc-efq-finder-wrapper">',
    '#suffix' => '</div>'
  );
  
  $form['update'] = array(
    '#type'       => 'submit',
    '#submit'     => array('feeds_tamper_ui_add_plugin_form_submit'),
    '#value'      => t('Update'),
    '#attributes' => array('class' => array('no-js')),
  );

  // Get some info on entities and bundles.
  $entity_types = array();
  
  foreach (entity_get_info() as $machine_name => $info) {
    $entity_types[$machine_name] = check_plain($info['label']);
  }

  $selected_entity_type = _feeds_tamper_efq_fc_finder_get_form_value('entity_type', $form_state, $settings);
  $selected_entity_type = isset($entity_types[$selected_entity_type]) ? $selected_entity_type : NULL;

  $form['entity_type'] = array(
    '#type'           => 'select',
    '#title'          => t('Entity type'),
    '#options'        => $entity_types,
    '#default_value'  => $selected_entity_type,
    '#ajax'           => array(
      'callback'  => 'feeds_tamper_efq_fc_finder_ajax',
      'wrapper'   => 'feeds-tamper-fc-efq-finder-wrapper',
    ),
    '#required' => TRUE,
  );

  if (!empty($selected_entity_type)) {
    // Gather bundle information.
    $entity_info = entity_get_info($selected_entity_type);
    $bundles     = array();
    
    foreach ($entity_info['bundles'] as $machine_name => $info) {
      $bundles[$machine_name] = check_plain($info['label']);
    }
  
    $selected_bundle = _feeds_tamper_efq_fc_finder_get_form_value('bundle', $form_state, $settings);
    $selected_bundle = isset($bundles[$selected_bundle]) ? $selected_bundle : NULL;
  
    $form['bundle'] = array(
      '#type'           => 'select',
      '#title'          => t('Bundle'),
      '#options'        => $bundles,
      '#default_value'  => $selected_bundle,
      '#ajax'           => array(
        'callback'  => 'feeds_tamper_efq_fc_finder_ajax',
        'wrapper'   => 'feeds-tamper-fc-efq-finder-wrapper',
      ),
      '#empty_option' => t('-- Select --'),
    );
  
    // Gather field definitions.
    $fields = _feeds_tamper_efq_fc_get_entity_fields($selected_entity_type, 'field_collection', $selected_bundle);
    
    $selected_field_name = _feeds_tamper_efq_fc_finder_get_form_value('field', $form_state, $settings);
    $selected_field_name = isset($fields[$selected_field_name]) ? $selected_field_name : NULL;
  
    $form['field'] = array(
      '#type'           => 'select',
      '#title'          => t('Field collection field'),
      '#options'        => $fields,
      '#default_value'  => $selected_field_name,
      '#ajax'           => array(
        'callback'  => 'feeds_tamper_efq_fc_finder_ajax',
        'wrapper'   => 'feeds-tamper-fc-efq-finder-wrapper',
      ),
      '#required' => TRUE,
    );
  
    if (!empty($selected_field_name)) {
      // For field collections, the field name in the parent entity is the bundle of the field collection item.
      $fc_fields = _feeds_tamper_efq_fc_get_entity_fields('field_collection_item', NULL, $selected_field_name);
      
      $selected_fc_field_name = _feeds_tamper_efq_fc_finder_get_form_value('field_collection_field', $form_state, $settings);
      $selected_fc_field_name = isset($fc_fields[$selected_fc_field_name]) ? $selected_fc_field_name : NULL;
  
      $form['field_collection_field'] = array(
        '#type'           => 'select',
        '#title'          => t('Field in Field Collection'),
        '#options'        => $fc_fields,
        '#default_value'  => $selected_fc_field_name,
        '#ajax'           => array(
          'callback'  => 'feeds_tamper_efq_fc_finder_ajax',
          'wrapper'   => 'feeds-tamper-fc-efq-finder-wrapper',
        ),
        '#required'       => TRUE,
      );
      
      if (!empty($selected_fc_field_name)) {
        $field_info = field_info_field($selected_fc_field_name);
        $columns    = array();
        
        foreach (array_keys($field_info['columns']) as $column) {
          $columns[$column] = $column;
        }
      
        $selected_column_name = _feeds_tamper_efq_fc_finder_get_form_value('column', $form_state, $settings);
        $selected_column_name = isset($columns[$selected_column_name]) ? $selected_column_name : key($columns);
      
        $form['column'] = array(
          '#type'           => 'select',
          '#title'          => t('Column'),
          '#options'        => $columns,
          '#default_value'  => $selected_column_name,
          '#required'       => TRUE,
        );
      }
    }
  }

  return $form;
}

/**
 * Determines what field collection fields exist in the entity described by the
 * provided array of info, and then returns them.
 * 
 * @param string  $entity_type
 *                The machine name of the entity type.
 *              
 * @param string  $field_module
 *                An optional parameter for restricting the fields returned to
 *                only those handled by the specified module. For example,
 *                "field_collection" would only return field collection fields.
 *                If left unset, fields handled by any module are returned.
 *              
 * @param string  $bundle
 *                The bundle name.
 *              
 * @return        An associative array with field machine names as the keys and
 *                the human-readable title as the value.
 */
function _feeds_tamper_efq_fc_get_entity_fields($entity_type, $field_module = NULL, $bundle = NULL) {
  $fields           = array();
  $bundle_instances = array();
  
  if (!empty($bundle)) {
    $bundle_instances = field_info_instances($entity_type, $bundle);
  }
  
  else {
    foreach (field_info_instances($entity_type) as $instances_for_bundle) {
      $bundle_instances = array_merge($bundle_instances, $instances_for_bundle);
    }
  }
  
  foreach ($bundle_instances as $machine_name => $info) {
    if (($field_module == NULL) || ($info['widget']['module'] == $field_module)) {
      $fields[$machine_name] = check_plain($info['label']);
    }
  }
  
  return $fields;
}

/**
 * Searches for the correct form value.
 */
function _feeds_tamper_efq_fc_finder_get_form_value($key, array $form_state, array $settings) {
  if (!empty($form_state['values']['settings'][$key])) {
    return $form_state['values']['settings'][$key];
  }

  if (!empty($settings[$key])) {
    return $settings[$key];
  }
}

/**
 * Validation handler for feeds_tamper_efq_fc_finder_form().
 */
function feeds_tamper_efq_fc_finder_validate(array &$settings) {
  // Validate entity type
  $entity_types = array();
  
  foreach (entity_get_info() as $machine_name => $info) {
    $entity_types[$machine_name] = check_plain($info['label']);
  }

  $selected_entity_type = $settings['entity_type'];
  
  if (!isset($entity_types[$selected_entity_type])) {
    form_set_error('settings][entity_type', t('Invalid entity type.'));
  }
  
  else {
    // Validate bundle type
    $entity_info = entity_get_info($selected_entity_type);
    $bundles     = array();
    
    foreach ($entity_info['bundles'] as $machine_name => $info) {
      $bundles[$machine_name] = check_plain($info['label']);
    }
  
    $selected_bundle = $settings['bundle'];
    
    if (!isset($bundles[$selected_bundle])) {
      form_set_error('settings][bundle', t('Invalid bundle type.'));
    }
    
    else {
      // Validate field collection name
      $fields = _feeds_tamper_efq_fc_get_entity_fields($selected_entity_type, 'field_collection', $selected_bundle);      
      $selected_field_name = $settings['field'];
      
      if (!isset($fields[$selected_field_name])) {
        form_set_error('settings][field', t('Invalid field collection field.'));
      }
      
      else {
        // For field collections, the field name in the parent entity is the bundle of the field collection item.
        $fc_fields = _feeds_tamper_efq_fc_get_entity_fields('field_collection_item', NULL, $selected_field_name);
        $selected_fc_field_name = $settings['field_collection_field'];
        
        if (!isset($fc_fields[$selected_fc_field_name])) {
          form_set_error('settings][field_collection_field', t('Invalid field in field collection type.'));
        }
        
        else {
          $field_info = field_info_field($selected_fc_field_name);
          $columns    = array();
          
          foreach (array_keys($field_info['columns']) as $column) {
            $columns[$column] = $column;
          }
        
          $selected_column_name = $settings['column'];
          
          if (!isset($columns[$selected_column_name])) {
            form_set_error('settings][column', t('Invalid column in field of field collection type.'));
          }
        }
      }
    }
  }
}

/**
 * Ajax callback for form changes.
 */
function feeds_tamper_efq_fc_finder_ajax(array $form, array &$form_state) {
  return isset($form['plugin']['settings']) ? $form['plugin']['settings'] : $form['settings'];
}

/**
 * Sets an entity id based on the value of a field.
 */
function feeds_tamper_efq_fc_finder_callback($result, $item_key, $element_key, &$field, array $settings) {
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'field_collection_item');
  
  // For field collections, the field name in the parent entity is the bundle of the field collection item.
  $query->entityCondition('bundle', $settings['field']);
  $query->fieldCondition($settings['field_collection_field'], $settings['column'], $field);
  $query->range(0, 1);

  $query_result = $query->execute();

  $have_value = FALSE;
  
  // @todo: Implement choice of behavior when multiple entities are found.
  // Currently we only use the first.
  if (!empty($query_result)) {
    $result                     = reset($query_result);
    $field_collection_item_id   = key($result);
    
    if (!empty($field_collection_item_id)) {
      $item   = field_collection_item_load($field_collection_item_id);
    
      if (!empty($item)) {
        $parent = field_collection_item_get_host_entity($item);
    
        // Workaround for obscure issue with field collection items that don't quite match-up.
        if (!empty($parent) && ($parent instanceof EntityDrupalWrapper)) {
          $field      = $parent->getIdentifier(); 
          $have_value = TRUE;
        }
      }
    }
  }
  
  if (!$have_value) {
    // We didn't find a value, remove the item.
    // @todo Fallback.
    unset($result->items[$item_key][$element_key]);
  }
}