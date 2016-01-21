<?php
/**
 * @file
 * Field API hooks for Paywall Field module.
 */

/**
 * Implements hook_field_info().
 */
function field_paywall_field_info() {
  return array(
    'field_paywall' => array(
      'label' => t('Paywall'),
      'description' => t('Hides fields when an entity is viewed and displays a custom message to the visitor.'),
      'default_widget' => 'field_paywall_widget',
      'default_formatter' => 'field_paywall_default',
      'settings' => array(
        'paywall' => _field_paywall_get_paywall_defaults('instance'),
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function field_paywall_field_widget_info() {
  return array(
    'field_paywall_widget' => array(
      'label' => t('Paywall'),
      'field types' => array('field_paywall'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_NONE,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function field_paywall_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function field_paywall_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  // Add your instance settings fields here.
  $form = array();

  $form['paywall'] = array(
    '#type' => 'fieldset',
    '#title' => t('Paywall per entity overrides'),
    '#description' => t('Disallow entity authors to edit the default values set below.'),
  );

  foreach (_field_paywall_get_paywall_option_types() as $setting_name) {
    $default_value = !empty($settings['paywall']) ? $settings['paywall'][$setting_name] : NULL;
    $form['paywall'] += _field_paywall_get_paywall_options_form($setting_name, 'instance', $default_value, $instance);
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function field_paywall_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'field_paywall_widget':
      $element['#instance'] = $instance;
      $element['#process'][] = 'field_paywall_field_widget_process';
      if (!empty($items[$delta])) {
        $element['#value'] = $items[$delta];
      }
      break;
  }
  return $element;
}

/**
 * Element process function for paywall widget form.
 */
function field_paywall_field_widget_process($element, &$form_state, $form) {
  $item = !empty($element['#value']) ? $element['#value'] : array();
  $instance = $element['#instance'];
  $element['#access'] = FALSE;

  $element['paywall'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => $element['#title'],
    '#collapsible' => TRUE,
  );

  // Show paywall fields only if on the field ui edit form or the disallow
  // setting isn't enabled for the given field instance.
  foreach (_field_paywall_get_paywall_option_types() as $setting_name) {
    $default_value = isset($item[$setting_name]) ? $item[$setting_name] : FALSE;
    $element['paywall'] += _field_paywall_get_paywall_options_form($setting_name, 'entity', $default_value, $instance);
    $element['paywall'][$setting_name]['#access'] = empty($instance['settings']['paywall'][$setting_name]) || $form_state['build_info']['form_id'] == 'field_ui_field_edit_form';

    // Only show the paywall fieldset if at least one option is available.
    if ($element['paywall'][$setting_name]['#access']) {
      $element['#access'] = TRUE;
    }
  }

  return $element;
}

/**
 * Implements hook_field_insert().
 */
function field_paywall_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  field_paywall_field_process_before_save($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Implements hook_field_update().
 */
function field_paywall_field_update($entity_type, $entity, $field, $instance, $langcode, &$items) {
  field_paywall_field_process_before_save($entity_type, $entity, $field, $instance, $langcode, $items);
}

/**
 * Process paywall values before saving them.
 */
function field_paywall_field_process_before_save($entity_type, $entity, $field, $instance, $langcode, &$items) {
  foreach ($items as &$item) {
    // Serialize roles and hidden fields values.
    foreach ($item as &$item_property_value) {
      if (is_array($item_property_value)) {
        $item_property_value = serialize(array_filter($item_property_value));
      }
    }

    // Ensure enabled has an integer boolean value.
    $item['enabled'] = isset($item['enabled']) ? (int) ((bool) $item['enabled']) : 0;
  }
}

/**
 * Implements hook_field_formatter_info().
 */
function field_paywall_field_formatter_info() {
  return array(
    'field_paywall_default' => array(
      'label' => t('Default'),
      'field types' => array('field_paywall'),
    ),
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function field_paywall_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  $paywall_name = $field['field_name'];

  foreach ($items as $entity_id => &$values) {
    if (empty($values)) {
      // Prepopulate with default values if no value is present at all for the
      // node.
      foreach (_field_paywall_get_paywall_option_types() as $paywall_setting_key) {
        if (!empty($instances[$entity_id]['settings']['paywall'][$paywall_setting_key])) {
          $values[0][$paywall_setting_key] = $instances[$entity_id]['default_value'][0][$paywall_setting_key];
        }
      }
    }
    else {
      foreach ($values as &$paywall) {
        foreach ($paywall as $paywall_setting_key => &$paywall_setting) {
          // If the option to prevent entity specific paywall values is checked
          // for this paywall field, populate the value with the default.
          if (!empty($instances[$entity_id]['settings']['paywall'][$paywall_setting_key])) {
            $paywall_setting = $instances[$entity_id]['default_value'][0][$paywall_setting_key];
          }
          // Deserialize any loaded values.
          else {
            if (in_array($paywall_setting_key, array(
                'fields_hidden',
                'roles',
              )) && !is_array($paywall_setting)
            ) {
              $paywall_setting = unserialize($paywall_setting);
            }
          }
        }
      }
    }
    foreach ($values as $paywall) {
      $paywall['active'] = FALSE;

      $entities[$entity_id]->active_paywalls[$paywall_name] = $paywall;
    }
  }
}

/**
 * Implements hook_field_formatter_view().
 */
function field_paywall_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'field_paywall_default':
      foreach ($items as $delta => $item) {
        $message = $item['message'];
        $element[$delta]['#markup'] = $message;
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_field_display_alter().
 */
function field_paywall_field_attach_view_alter(&$output, $context) {
  $entity = $context['entity'];
  $entity_type = $context['entity_type'];
  $entity_ids = entity_extract_ids($entity_type, $entity);
  $entity_id = reset($entity_ids);

  if (!isset($entity->active_paywalls)) {
    return;
  }

  foreach ($entity->active_paywalls as $paywall_name => $paywall_settings) {
    // Add alter hooks for each paywall property.
    foreach (_field_paywall_get_paywall_option_types() as $paywall_setting_key) {
      drupal_alter('field_paywall_' . $paywall_setting_key, $paywall_settings[$paywall_setting_key], $paywall_name, $entity_type, $entity_id);
    }

    // Add alter hook for the paywall as a whole.
    drupal_alter('field_paywall_paywall', $paywall_settings, $paywall_name, $entity_type, $entity_id);

    // Don't activate paywalls that don't appear in the view mode for this node.
    if (!isset($output[$paywall_name])) {
      continue;
    }

    // Only run on active paywalls.
    if ($paywall_settings['active'] != 1 || $paywall_settings['enabled'] == 0) {
      $output[$paywall_name]['#access'] = FALSE;
      continue;
    }

    // Get all hidden fields specified by the paywall.
    $hidden_fields = $paywall_settings['fields_hidden'];

    // Hide any fields specified by the active paywall.
    foreach (element_children($output) as $field_name) {
      if (isset($hidden_fields[$field_name]) && !empty($hidden_fields[$field_name])) {
        $output[$field_name]['#access'] = FALSE;
      }
    }
  }
}

/**
 * Implements hook_field_load().
 */
function field_paywall_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function field_paywall_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  if ($form['#field']['type'] == 'field_paywall') {
    // Prevent multiple cardinality on paywall fields.
    $form['field']['cardinality']['#options'] = array(1 => 1);
  }
}

/**
 * Get the paywall options for a given field or field settings.
 *
 * @param string $setting_field
 *   The setting field to retrieve (use all to retrieve all fields).
 * @param string $context
 *   The context to retrieve within (instance or entity).
 * @param mixed $default_value
 *   The default value to place in the field.
 * @param array $instance
 *   The instance the field being applied to.
 *
 * @return array
 *   The form options for the given settings.
 */
function _field_paywall_get_paywall_options_form($setting_field, $context = 'instance', $default_value = NULL, $instance = NULL) {
  $labels = _field_paywall_get_paywall_labels();

  $form = array();

  // If the context is an instance form, allow admins to give entity authors the
  // ability to override the field value within the entity form.
  if ($context == 'instance') {
    $form[$setting_field] = array(
      '#type' => 'checkbox',
      '#title' => t('Force defaults for %field.', array(
        '%field' => $labels[$setting_field],
      )),
    );
  }
  else {
    $form[$setting_field] = array(
      '#title' => $labels[$setting_field],
    );

    switch ($setting_field) {
      case 'roles':
        $form[$setting_field]['#type'] = 'checkboxes';
        $form[$setting_field]['#description'] = t('User roles which activate the paywall.');
        $form[$setting_field]['#options'] = user_roles();
        $form[$setting_field]['#element_validate'][] = '_field_paywall_clean_checkbox_values';

        if (is_string($default_value)) {
          $default_value = unserialize($default_value);
        }
        break;

      case 'message':
        $form[$setting_field]['#type'] = 'textarea';
        $form[$setting_field]['#description'] = t('Message to display to the end user.');
        break;

      case 'enabled':
        $form[$setting_field]['#type'] = 'checkbox';
        $form[$setting_field]['#description'] = t('Whether or not the paywall is active by default.');
        break;

      case 'fields_hidden':
        if ($instance) {
          // Get the field map.
          $fields = field_info_instances($instance['entity_type'], $instance['bundle']);
        }
        else {
          $fields = field_info_field_map();
        }

        $field_options = array();
        foreach ($fields as $field_name => $field_info) {
          $field_options[$field_name] = $field_info['label'] . ' (' . $field_info['field_name'] . ')';
        }

        if (isset($field_options[$instance['field_name']])) {
          unset($field_options[$instance['field_name']]);
        }

        $form[$setting_field]['#type'] = 'checkboxes';
        $form[$setting_field]['#description'] = t('Fields hidden by the paywall.');
        $form[$setting_field]['#options'] = $field_options;
        $form[$setting_field]['#element_validate'][] = '_field_paywall_clean_checkbox_values';

        if (is_string($default_value)) {
          $default_value = unserialize($default_value);
        }
        break;
    }
  }

  if ($default_value) {
    $form[$setting_field]['#default_value'] = $default_value;
  }

  return $form;
}

/**
 * Element validation function to filter out empty role and hidden field values.
 */
function _field_paywall_clean_checkbox_values($element, &$form_state) {
  $field_value = & $form_state['values'];
  foreach ($element['#parents'] as $parent) {
    $field_value = & $field_value[$parent];
  }
  $field_value = array_filter($field_value);
}

/**
 * Get available settings setting types for paywall configuration.
 *
 * @return array
 *   The setting field types in use by the entity paywall field.
 */
function _field_paywall_get_paywall_option_types() {
  $types = array(
    'roles',
    'message',
    'fields_hidden',
    'enabled',
  );

  return $types;
}

/**
 * Default paywall setting labels.
 *
 * @return array
 *   Paywall setting labels keyed by setting name.
 */
function _field_paywall_get_paywall_labels() {
  return array(
    'roles' => t('User roles'),
    'message' => t('Paywall message'),
    'fields_hidden' => t('Hidden fields'),
    'enabled' => t('Enabled by default'),
  );
}

/**
 * Get default values for the paywall settings.
 *
 * @param string $context
 *   In which context do the defaults exist, for the default checkboxes or the
 *   fields themselves.
 *
 * @return array
 *   Defaults keyed by their setting name.
 */
function _field_paywall_get_paywall_defaults($context = 'instance') {
  $types = array(
    'roles' => $context == 'instance' ? 0 : array(),
    'message' => $context == 'instance' ? 0 : '',
    'fields_hidden' => $context == 'instance' ? 0 : array(),
    'enabled' => $context == 'instance' ? 0 : 1,
  );

  return $types;
}
