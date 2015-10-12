<?php

/**
 * @file
 * Define formforall field type.
 */


/**
 * Implements hook_field_info().
 *
 * Provides the description of the field.
 */
function formforall_field_info() {
  return array(
    // We name our field as the associative name of the array.
    'formforall_form' => array(
      'label' => t('FormForAll form'),
      'description' => t('This field store FormForAll form ID.'),
      'default_widget' => 'formforall_form_widget_default',
      'default_formatter' => 'formforall_form_formatter_default',
      'instance_settings' => array(
        'text_processing' => 0,
      ),
    ),
  );
}


/**
 * Implements hook_field_validate().
 *
 * This hook gives us a chance to validate content that's in our
 * field. We're really only interested in the $items parameter, since
 * it holds arrays representing content in the field we've defined.
 */
function formforall_field_validate($entity_type, $entity, $field, $instance, $langcode, &$items, &$errors) {
  if ($field['type'] == 'formforall_form') {
    foreach ($items as $delta => $item) {
      if (!empty($item['id'])) {
        if (!formforall_is_valid_id($item['id'])) {
          $errors[$field['field_name']][$langcode][$delta][] = array(
            'error' => 'formforall_invalid_form_id',
            'message' => t("%name must be a valid UUID.", array('%name' => $instance['label'])),
          );
        }
      }
    }
  }
}


/**
 * Implements hook_field_is_empty().
 *
 * hook_field_is_emtpy() is where Drupal asks us if this field is empty.
 * Return TRUE if it does not contain data, FALSE if it does. This lets
 * the form API flag an error when required fields are empty.
 */
function formforall_field_is_empty($item, $field) {
  if ($field['type'] == 'formforall_form') {
    return empty($item['id']);
  }
}


/**
 * Implements hook_field_widget_info().
 */
function formforall_field_widget_info() {
  return array(
    'formforall_form_widget_default' => array(
      'label' => t('Default'),
      'field types' => array('formforall_form'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}


/**
 * Implements hook_field_widget_form().
 */
function formforall_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element['#delta'] = $delta;

  switch ($instance['widget']['type']) {
    case 'formforall_form_widget_default':
      $element['id'] = array(
        '#type' => 'select',
        '#title' => t("FormForAll form"),
        '#options' => formforall_get_forms_options(),
        '#default_value' => isset($items[$delta]['id']) ? $items[$delta]['id'] : NULL,
        '#empty_option' => t("None"),
      );
      break;
  }

  return $element;
}


/**
 * Implements hook_field_formatter_info().
 */
function formforall_field_formatter_info() {
  return array(
    'formforall_form_formatter_default' => array(
      'label' => t('Default'),
      'field types' => array('formforall_form'),
    ),
  );
}


/**
 * Implements hook_field_formatter_view().
 */
function formforall_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if (!empty($items)) {
    switch ($display['type']) {
      case 'formforall_form_formatter_default':
        foreach ($items as $delta => $item) {
          $element[$delta] = array(
            '#theme' => 'formforall_form',
            '#form_id' => $item['id'],
          );
        }
        break;
    }
  }

  return $element;
}
