<?php

/**
 * @file
 * FVL configuration file.
 */

/**
 * Callback for the admin configuration page.
 */
function fvl_admin_settings($form, &$form_state) {
  $settings = variable_get('fvl_fields', array());
  $form = array();
  $instances = field_info_instances();
  $form_state['entity_array'] = array();

  $form['replacement_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Text for the "to show" link'),
    '#description' => t('This text will be set as default.'),
    '#default_value' => variable_get('fvl_show_link_text', 'Show'),
    '#required' => TRUE,
  );
  $form['fields'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h3>' . t('Select a fields for each of entity and bundle that should be handled by the module:') . '</h3>',
  );
  // Creating list of fields for each entity and bundle having a field(s).
  foreach ($instances as $entity_name => $bundles) {
    $entity_info = entity_get_info($entity_name);
    if (!$entity_info['fieldable']) {
      continue;
    }
    $form[$entity_name] = array(
      '#type' => 'fieldset',
      '#title' => $entity_info['label'],
      '#description' => t('Bundles and fields for the %entity_label (as entity type).', array('%entity_label' => $entity_info['label'])),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#group' => 'fields',
    );
    $empty = TRUE;
    foreach ($bundles as $bundle_name => $fields) {
      if ($fields) {
        $empty = FALSE;
        $form[$entity_name][$bundle_name] = array(
          '#type' => 'fieldset',
          '#title' => $entity_info['bundles'][$bundle_name]['label'],
          '#description' => t('Fields for the %bundle_label (as bundle of the entity).', array('%bundle_label' => $entity_info['bundles'][$bundle_name]['label'])),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );
        foreach ($fields as $field) {
          $form[$entity_name][$bundle_name][$field['field_name']] = array(
            '#type' => 'fieldset',
          );
          $form[$entity_name][$bundle_name][$field['field_name']]['name'] = array(
            '#type' => 'checkbox',
            '#title' => '<strong>' . $field['label'] . '</strong> (' . $field['field_name'] . ')',
            '#default_value' => isset($settings[$entity_name][$bundle_name][$field['field_name']]),
          );
          $field_tree = $entity_name . '[' . $bundle_name . '][' . $field['field_name'] . ']';
          $form[$entity_name][$bundle_name][$field['field_name']]['show_custom_text'] = array(
            '#type' => 'checkbox',
            '#title' => t('Custom text for replacement'),
            '#description' => t('Select this to assign a custom text for the replacement link.'),
            '#default_value' => isset($settings[$entity_name][$bundle_name][$field['field_name']]) && !empty($settings[$entity_name][$bundle_name][$field['field_name']]),
            '#states' => array(
              'invisible' => array(
                ':input[name="' . $field_tree . '[name]"]' => array('checked' => FALSE),
              ),
            ),
          );
          $form[$entity_name][$bundle_name][$field['field_name']]['custom_text'] = array(
            '#type' => 'textfield',
            '#title' => t('Text'),
            '#default_value' => isset($settings[$entity_name][$bundle_name][$field['field_name']]) ? $settings[$entity_name][$bundle_name][$field['field_name']] : '',
            '#states' => array(
              'invisible' => array(
                ':input[name="' . $field_tree . '[show_custom_text]"]' => array('checked' => FALSE),
              ),
            ),
          );
        }
      }
    }
    if ($empty) {
      unset($form[$entity_name]);
    }
    else {
      $form_state['entity_array'][] = $entity_name;
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Configuration form submit.
 */
function fvl_admin_settings_submit($form, $form_state) {
  $settings = array();
  $values = $form_state['values'];

  foreach ($form_state['entity_array'] as $entity) {
    foreach ($values[$entity] as $bandle => $fields) {
      foreach ($fields as $field => $value) {
        if ($values[$entity][$bandle][$field]['name']) {
          $text = $values[$entity][$bandle][$field]['show_custom_text'] ? $values[$entity][$bandle][$field]['custom_text'] : '';
          $settings[$entity][$bandle][$field] = $text;
        }
      }
    }
  }
  variable_set('fvl_show_link_text', $values['replacement_default']);
  variable_set('fvl_fields', $settings);
  drupal_set_message(t('The configuration options have been saved.'));
}

