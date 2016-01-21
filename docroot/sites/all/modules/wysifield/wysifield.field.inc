<?php
/**
 * @file
 * Defines the wysifield_field, which is used by the module to
 * determine that a specific entity bundle is a wysifield bundle.
 */

/**
 * Implements hook_field_info().
 */
function wysifield_field_info() {
  $fields = array();
  $fields['wysifield_field'] = array(
    'label'             => t('Wysifield Settings'),
    'description'       => t('Tags content type as a Wysifield and collects metadata'),
    'settings'          => array(),
    'instance_settings' => array(
      'title'       => t('Wysifield'),
      'description' => t('This field allows creation of a custom Wysifield by assigning it to a content type and enabling it in the WYSIWYG.'),
      'weight'      => 1,
      'rte_icon'    => '[feature_path]/icons/rte_icon.png',
      'button_icon' => '[feature_path]/icons/button_icon.png',
    ),
    'default_widget'    => 'wysifield_bundle_widget',
    'default_formatter' => 'wysifield_bundle_formatter',
  );

  return $fields;
}

/**
 * Implements hook_field_insert().
 */
function wysifield_field_insert($entity_type, $entity, $field, $instance, $items) {

}

/**
 * Implements hook_field_update().
 */
function wysifield_field_update($entity_type, $entity, $field, $instance, $items) {

}

/**
 * Implements hook_field_delete().
 */
function wysifield_field_delete($entity_type, $entity, $field, $instance, $items) {

}

/**
 * Implements hook_field_validate().
 *
 * Possible error codes:
 * - 'wysifield_invalid': The value is invalid.
 */
function wysifield_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if (!empty($item['value'])) {
      if ($item['host_entity_id'] == -1) {
        $errors[$field['field_name']][$langcode][$delta][] = array(
          'error'   => 'wysifield_invalid',
          'message' => t('%name does not accept the value -1.', array('%name' => $instance['label'])),
        );
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function wysifield_field_is_empty($item, $field) {
  if (empty($item) && (string) $item !== '0') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_settings_form().
 *
 * This is the form that manages GLOBAL settings for the field.
 */
function wysifield_field_settings_form($field, $instance, $has_data) {
  // No global settings for this field.
  $form = array();
  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 *
 * This is the form that manages settings for the field on the bundle.
 */
function wysifield_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form = array();
  $form['title'] = array(
    '#type'          => 'textfield',
    '#default_value' => $settings['title'] ? $settings['title'] : '',
    '#title'         => t('Name of Wysifield entity.'),
    '#description'    => t('This is the name that appears in your WYSIWYG configuration.'),
    '#weight'        => 1,
  );
  $form['description'] = array(
    '#type'          => 'textfield',
    '#default_value' => $settings['description'] ? $settings['description'] : '',
    '#title'         => t('Description of Wysifield entity.'),
    '#weight'        => 2,
  );
  $form['rte_icon'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Path to icon for in-context entity (in WYSIWYG body)'),
    '#description'    => t('Use a path relative to docroot'),
    '#default_value'  => $settings['rte_icon'] ? $settings['rte_icon'] : '',
    '#size'           => 70,
    '#weight'         => 3,
  );
  $form['button_icon'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Choose an icon for WYSIWYG button bar'),
    '#description'    => t('Use a path relative to docroot'),
    '#default_value'  => $settings['button_icon'] ? $settings['button_icon'] : '',
    '#size'           => 70,
    '#weight'         => 4,
  );
  $form['weight'] = array(
    '#type'          => 'textfield',
    '#default_value' => $settings['weight'] ? $settings['weight'] : '',
    '#title'         => t('Weight (load order)'),
    '#description'   => t('Enter an integer value. Controls the load order of the WYSIWYG button for this entity bundle.'),
    '#weight'        => 5,
  );

  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function wysifield_field_widget_info() {
  $info = wysifield_field_info();
  $settings = $info['wysifield_field']['settings'];
  return array(
    'wysifield_bundle_widget' => array(
      'label'       => t('Wysifield field'),
      'field types' => array('wysifield_field'),
      'settings'    => $settings,
      'behaviors'   => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value'   => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 *
 * This form represents settings specific to a widget visible on content type
 * edit page.
 */
function wysifield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  if ($instance['widget']['type'] == 'wysifield_bundle_widget') {

    // Used for tracking association between entitys being created eid,
    // and the entity it's being embedded in.
    $element['value']['host_entity_id'] = array(
      '#type'           => 'hidden',
      '#default_value'  => isset($items[$delta]['host_entity_id']) ? $items[$delta]['host_entity_id'] : '',
    );
    $element['value']['uuid'] = array(
      '#type'           => 'hidden',
      '#default_value'  => isset($items[$delta]['uuid']) ? $items[$delta]['uuid'] : '',
    );

    // @BOB add view mode selection to ctools form
    // Pull all available display modes for current entity bundle.
    $bundle = $instance['bundle'];
    $entity_type = $instance['entity_type'];
    $view_modes = field_view_mode_settings($entity_type, $bundle);
    $options = array();
    foreach ($view_modes as $k => $v) {
      if ($v['custom_settings'] == TRUE) {

        // Turn view_mode machine name into readable label.
        $label = ucwords(str_replace('_', ' ', $k));
        $options[$k] = $label;
      }
    }

    // Check items for existing content, or use form_state for new content.
    if (!empty($items)) {
      $selected = isset($items[$delta]['view_mode']) ? $items[$delta]['view_mode'] : reset($options);
    }
    else {
      // Determine defaults for form and pass to ajax callback field.
      $selected = isset($form_state['values']['wysifield_field'][$langcode]['value']['view_mode']) ? $form_state['values']['wysifield_field'][$langcode]['value']['view_mode'] : reset($options);
    }

    return $element;
  }

  return array();
}

/**
 * Form element validation handler for 'test_field_widget_multiple' widget.
 */
function wysifield_widget_multiple_validate($element, &$form_state) {
  $values = array_map('trim', explode(',', $element['#value']));
  $items  = array();
  foreach ($values as $value) {
    $items[] = array('value' => $value);
  }
  form_set_value($element, $items, $form_state);
}

/**
 * Implements hook_field_widget_error().
 */
function wysifield_field_widget_error($element, $error, $form, &$form_state) {
  if (isset($element['value'])) {

    // Widget is test_field_widget.
    $error_element = $element['value'];
  }
  else {

    // Widget is test_field_widget_multiple.
    $error_element = $element;
  }

  form_error($error_element, $error['message']);
}

/**
 * Implements hook_field_widget_settings_form().
 */
function wysifield_field_widget_settings_form($field, $instance) {

  // There are no widget settings.
  $form = array();
  return $form;
}

/**
 * Sample 'default value' callback.
 */
function wysifield_default_value($entity_type, $entity, $field, $instance) {
  return array();
}

/**
 * Implements hook_field_access().
 */
function wysifield_field_access($op, $field, $entity_type, $entity, $account) {
  if ($field['field_name'] == "field_no_{$op}_access") {
    return FALSE;
  }

  return TRUE;
}
