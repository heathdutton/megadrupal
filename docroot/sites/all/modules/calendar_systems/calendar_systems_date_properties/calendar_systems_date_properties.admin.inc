<?php

/**
 * @file
 * Contains administration form callbacks.
 */

/**
 * Page callback for calendar_systems_date_properties.
 */
function calendar_systems_date_properties_settings_form($form, $form_state) {
  $form = array();
  $entity_bundles = variable_get('calendar_systems_date_properties_entity_bundles_settings');

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    /*
    '#attached' => array(
        'js' => array(
            'vertical-tabs' => drupal_get_path('module', 'calendar_systems_date_properties') . '/vertical_tabs.admin.js',
        ),
    ),
    */
  );

  $form['info'] = array(
    '#type' => 'item',
    '#weight' => -2,
    '#description' => t('You must enable at least one non-gregorian calendar system , return back to this page and save it'),
  );

  $form['entitites'] = array(
    '#type' => 'item',
    '#title' => t('Entities'),
    //'#collapsible' => FALSE,
    //'#group' => 'additional_settings',
    //'#tree' => TRUE,
    '#weight' => -1,
    '#description' => t('Config the entities'),
  );

  //$var = field_info_instances();
  //var_export($var['reply']['blog_comment']);exit;
  $entity_types = _calendar_systems_date_properties_entity_types();
  //    dsm($entity_types);
  foreach ($entity_types as $entity_type => $entity_type_info) {
    if ($entity_type != 'comment')
      foreach ($entity_type_info['bundles'] as $bundle => $bundle_info) {
        $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle] = array(
          '#type' => 'fieldset',
          '#title' => t($entity_type_info['label'].' -> '.$bundle_info['label']),
          '#collapsible' => FALSE,
          '#group' => 'additional_settings',
          '#tree' => TRUE,
          /*
          '#states' => array(
              'invisible' => array(
                  // If the checkbox is not enabled, show the container.
                  'input[name="vertical_tabs_example[enabled]"]' => array('checked' => FALSE),
              ),
          ),
          */
        );
        $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle]['enabled'] = array(
          '#type' => 'checkbox',
          '#title' => t("Enable"),
          '#tree' => TRUE,
          '#default_value' => @$entity_bundles[$entity_type.'.'.$bundle]['enabled'],
        );
        $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle]['properties'] = array(
          '#type' => 'fieldset',
          '#title' => t("Properties"),
          '#tree' => TRUE,
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
          //'#default_value' => @$entity_bundles[$entity_type.'.'.$bundle]['enabled'],
        );
        $properties = entity_get_property_info($entity_type);
        foreach ($properties['properties'] as $property_name => $property) if (@$property['type'] == 'date') {
          $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle]['properties'][$property_name] = array(
            '#type' => 'checkbox',
            '#title' => $property['label'],
            '#default_value' => @$entity_bundles[$entity_type.'.'.$bundle]['properties'][$property_name],
          );
        }
        $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle]['fields'] = array(
          '#type' => 'fieldset',
          '#title' => t("Fields"),
          '#tree' => TRUE,
          '#collapsible' => FALSE,
          '#collapsed' => FALSE,
          //'#default_value' => @$entity_bundles[$entity_type.'.'.$bundle]['enabled'],
        );

        $fields = field_info_instances($entity_type, $bundle);
        foreach ($fields as $field_name => $field) if (@$field['widget']['module'] == 'OOOOOdate') {
          $form['entity_bundles']['entity_bundles_'.$entity_type.'|'.$bundle]['fields'][$field_name] = array(
            '#type' => 'checkbox',
            '#title' => $field['label'],
            '#default_value' => @$entity_bundles[$entity_type.'.'.$bundle]['fields'][$field_name],
          );
        }
      }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 50,
  );

  //$form['#validate'] = 'calendar_systems_date_properties_node_type_form_validate';
  $form['#submit'][] = 'calendar_systems_date_properties_settings_form_submit';

  return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function calendar_systems_date_properties_settings_form_submit($form, &$form_state) {
  $entity_bundles = array();
  //print_r($form_state['values']);exit;
  foreach ($form_state['values'] as $item_name => $item) {
    if (strpos($item_name, 'entity_bundles_')!==FALSE) {
      $entity_bundle_name = str_replace('entity_bundles_', '', $item_name);
      list($entity_type, $bundle_name) = explode('|', $entity_bundle_name);
      $entity_bundle_full_name = "{$entity_type}_$bundle_name";
      $entity_bundle_name = str_replace('|', '.', $entity_bundle_name);
      $entity_bundles[$entity_bundle_name] = array(
        'enabled' => 0,
        'properties' => array()
      );
      $entity_bundles[$entity_bundle_name] = array_merge($entity_bundles[$entity_bundle_name], $item);
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
  variable_set('calendar_systems_date_properties_entity_bundles_settings', $entity_bundles);
  _calendar_systems_date_properties_schema_apply_change();
  cache_clear_all('schema', 'cache');
  entity_info_cache_clear();
}
