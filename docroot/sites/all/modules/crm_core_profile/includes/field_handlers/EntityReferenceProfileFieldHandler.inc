<?php

/**
 * @file
 * Entity reference fields handler.
 */

/**
 * Class EntityReferenceProfileFieldHandler.
 */
class EntityReferenceProfileFieldHandler extends DefaultProfileFieldHandler {

  public function renderFieldConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {
    $field_config = array();

    if (isset($form['#profile'])) {
    $profile = $form['#profile'];
    }

    $field_instance = field_info_instance($entity_type, $field_name, $bundle);
    $default_field_config = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'field_name' => $field_name,
      'field_item' => '',
      'field_label' => $field_instance['label'],
      'weight' => _crm_core_profile_default_weight(),
      'handler' => get_class($this),
    );
    $field_separator = _crm_core_profile_field_separator($default_field_config);
    if (isset($profile->fields[$field_separator])) {
      $field_config = $profile->fields[$field_separator];
    }
    $field_config += $default_field_config;

    $form['fields'][$field_separator] = array();
    $row =& $form['fields'][$field_separator];

    $weight = isset($field_config['weight']) ? $field_config['weight'] : _crm_core_profile_default_weight();

    $row['#weight'] = isset($field_config['weight']) ? _crm_core_profile_weight_fix($field_config['weight']) : $weight;

    $row['field_name'] = array(
      '#type' => 'value',
      '#value' => $field_config['field_name'],
    );
    $row['field_item'] = array(
      '#type' => 'value',
      '#value' => $field_config['field_item'],
    );
    $row['handler'] = array(
      '#type' => 'value',
      '#value' => $field_config['handler'],
    );
    $row['entity_type'] = array(
      '#type' => 'value',
      '#value' => $field_config['entity_type'],
    );
    $row['bundle'] = array(
      '#type' => 'value',
      '#value' => $field_config['bundle'],
    );

    $row['status'] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($field_config['status']) ? $field_config['status'] : FALSE,
    );

    $row['title_wrapper'] = array(
      '#type' => 'container',
    );
    $row['title_wrapper']['title'] = array(
      '#markup' => t('@field_label (%field_entity)', array(
          '@field_label' => $field_config['field_label'],
          '%field_entity' => $entity_label,
        )
      ),
    );

    $row['hidden'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hidden'),
      '#title_display' => 'after',
      '#default_value' => isset($field_config['hidden']) ? $field_config['hidden'] : FALSE,
    );

    $field = field_info_field($field_config['field_name']);

    // We must set form parents as well as $form_state to exclude errors while
    // generating field.
    $form['#parents'] = array();
    $form_state = array();

    $items = isset($field_config['default']) ? $field_config['default'] : array();
    $default = field_default_form($entity_type, NULL, $field, $field_instance, LANGUAGE_NONE, $items, $form, $form_state);

    $row['default'] = $default[$field_name][LANGUAGE_NONE];

    $row['default']['#required'] = FALSE;
    $row['default']['#title'] = t('Default Value');

    $row['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $field_config['field_label'])),
      '#title_display' => 'invisible',
      '#default_value' => isset($field_config['weight']) ? $field_config['weight'] : $weight,
      '#delta' => DEFAULT_WEIGHT_DELTA,
    );
  }

  public function renderField($field_name, $fields_config, &$form, &$form_state) {

    // This check is a must to handle custom entity references and references to
    // nodes.
    if (isset($form[$field_name][LANGUAGE_NONE]['#entity_type'])) {
      $entity_type = $form[$field_name][LANGUAGE_NONE]['#entity_type'];
      $bundle = $form[$field_name][LANGUAGE_NONE]['#bundle'];
    }
    else {
      $entity_type = $form[$field_name][LANGUAGE_NONE][0]['target_id']['#entity_type'];
      $bundle = $form[$field_name][LANGUAGE_NONE][0]['target_id']['#bundle'];
    }

    // Store the field prefix.
    $field_prefix = $entity_type . '_' . $bundle . '_';
    $profile_field = $field_prefix . $field_name;

    if (array_key_exists($profile_field, $fields_config)) {
      // Apply default field value.
      if (!empty($fields_config[$profile_field]['default'][0]['target_id'])) {

        $field = field_info_field($field_name);
        $id = $fields_config[$profile_field]['default'][0]['target_id'];
        $entity = entity_load_single($field['settings']['target_type'], $id);
        $label = entity_label($field['settings']['target_type'], $entity);
        $key = "$label ($id)";
        // Labels containing commas or quotes must be wrapped in quotes.
        if (strpos($key, ',') !== FALSE || strpos($key, '"') !== FALSE) {
          $key = '"' . str_replace('"', '""', $key) . '"';
        }
        if (isset($form[$field_name][LANGUAGE_NONE][0]['target_id']['#default_value'])) {
          $default_value =& $form[$field_name][LANGUAGE_NONE][0]['target_id']['#default_value'];
        }
        else {
          $default_value =& $form[$field_name][LANGUAGE_NONE]['#default_value'];
        }
        $default_value = $key;
      }
    }

    // Only default values need help rendering, call parent renderField.
    parent::renderField($field_name, $fields_config, $form, $form_state);
  }
}
