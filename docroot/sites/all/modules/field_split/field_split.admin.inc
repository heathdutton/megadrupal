<?php

/**
 * @file
 * Administrative interface for Field Split.
 */

/**
 * Configuration form.
 */
function field_split_config_form($form, $form_state) {
  $form['#tree'] = TRUE;
  $form['#attached']['css'][] = drupal_get_path('module', 'field_split') . '/css/field-split.css';
  $field_info = field_info_fields();
  $entity_info = entity_get_info();

  foreach ($field_info as $field) {
    foreach ($field['bundles'] as $entity_type => $bundles) {
      foreach ($bundles as $bundle) {
        $settings = variable_get('field_split_config_' . $entity_type . '__' . $bundle, array());

        // Field split can be only available for fields w/ more than one value.
        if ($field['cardinality'] == 1) {
          continue;
        }

        $field_info = field_info_instance($entity_type, $field['field_name'], $bundle);
        $label = $field_info['label'];
        $total_fields = $field['cardinality'] != FIELD_CARDINALITY_UNLIMITED ? $field['cardinality'] : 50;

        $options = array();

        // Build a list with available values.
        foreach (range(2, $total_fields) as $number) {
          $options[$number] = $number;
        }

        if (!isset($settings[$entity_type][$bundle][$field['field_name']])) {
          $settings[$entity_type][$bundle][$field['field_name']] = array(
            'enable' => FALSE,
            'values' => 1,
          );
        }

        if (!isset($form['entity_types'][$entity_type])) {
          $form['entity_types'][$entity_type] = array(
            '#type' => 'fieldset',
            '#title' => $entity_info[$entity_type]['label'],
          );
        }

        if (!isset($form['entity_types'][$entity_type][$bundle])) {
          $form['entity_types'][$entity_type][$bundle] = array(
            '#type' => 'fieldset',
            '#title' => $entity_info[$entity_type]['bundles'][$bundle]['label'],
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
          );
        }

        if (isset($settings[$field['field_name']]['enable']) && $settings[$field['field_name']]['enable']) {
          $form['entity_types'][$entity_type][$bundle]['#collapsed'] = FALSE;
        }

        $form['entity_types'][$entity_type][$bundle][$field['field_name']]['enable'] = array(
          '#title' => t('Split %field (@machine-name)', array('%field' => $label, '@machine-name' => $field['field_name'])),
          '#type' => 'checkbox',
          '#default_value' => isset($settings[$field['field_name']]['enable']) ? $settings[$field['field_name']]['enable'] : FALSE,
        );

        $form['entity_types'][$entity_type][$bundle][$field['field_name']]['values'] = array(
          '#title' => t('Amount of total fields'),
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => isset($settings[$field['field_name']]['values']) ? $settings[$field['field_name']]['values'] : NULL,
          '#states' => array(
            'visible' => array(
              'input[name$="entity_types[' . $entity_type . '][' . $bundle . '][' . $field['field_name'] . '][enable]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
    }
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  if (!isset($form['entity_types'])) {
    $form['actions']['submit']['#disabled'] = TRUE;
  }

  return $form;
}

/**
 * Submit function for configuration form.
 */
function field_split_config_form_submit($form, &$form_state) {
  if (!isset($form_state['values']['entity_types'])) {
    return;
  }

  foreach ($form_state['values']['entity_types'] as $entity_type => $bundles) {
    foreach ($bundles as $bundle_name => $bundle) {
      $var = 'field_split_config_' . $entity_type . '__' . $bundle_name;
      $enabled = FALSE;

      // Check whether Field Split is enabled.
      foreach ($bundle as $field) {
        if ($field['enable']) {
          $enabled = TRUE;
          break;
        }
      }

      // Store variable if bundle is enabled for field splitting otherwise
      // delete that variable.
      if ($enabled) {
        variable_set($var, $bundle);
      }
      else {
        variable_del($var, 'field_split_config_' . $entity_type . '__' . $bundle_name);
      }
    }
  }

  // Clear field info cache in order to show new extra fields.
  field_info_cache_clear();

  drupal_set_message(t('The configuration options have been saved.'));
}
