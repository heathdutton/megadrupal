<?php

/**
 * @file
 * Fraction Field API functions
 */

/**
 * Implements of hook_field_info().
 */
function fraction_field_info() {
  return array(
    'fraction' => array(
      'label' => t('Fraction'),
      'description' => t('This field stores a decimal in fraction form (with a numerator and denominator) for maximum precision.'),
      'settings' => array(),
      'instance_settings' => array(),
      'default_widget' => 'fraction_default',
      'default_formatter' => 'fraction_default',
      // Entity API integration.
      'property_type' => 'field_item_fraction',
      'property_callbacks' => array('fraction_property_info_callback'),
    ),
  );
}

/**
 * Callback to alter the property info of fraction fields.
 *
 * @see fraction_field_info().
 */
function fraction_property_info_callback(&$info, $entity_type, $field, $instance, $field_type) {
  $name = $field['field_name'];
  $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$name];
  $property['type'] = ($field['cardinality'] != 1) ? 'list<field_item_fraction>' : 'field_item_fraction';
  $property['getter callback'] = 'entity_metadata_field_verbatim_get';
  $property['setter callback'] = 'entity_metadata_field_verbatim_set';
  $property['auto creation'] = 'fraction_default_values';
  $property['property info'] = fraction_data_property_info();
  unset($property['query callback']);
}

function fraction_default_values() {
  return array(
    'numerator' => 0,
    'denominator' => 100,
  );
}

/**
 * Defines info for the properties of the fraction field data structure.
 */
function fraction_data_property_info($name = NULL) {
  // Build an array of basic property information for the fraction field.
  $properties = array(
    'numerator' => array(
      'label' => t('Numerator'),
    ),
    'denominator' => array(
      'label' => t('Denominator'),
    ),
  );
  foreach ($properties as &$property) {
    $property += array(
      'type' => 'integer',
      'description' => !empty($name) ? t('!label of field %name', array('!label' => $property['label'], '%name' => $name)) : '',
      'getter callback' => 'entity_property_verbatim_get',
      'setter callback' => 'entity_property_verbatim_set',
    );
  }
  return $properties;
}

/**
 * Implements hook_field_is_empty().
 */
function fraction_field_is_empty($item, $field) {
  return empty($item['numerator']) || empty($item['denominator']);
}

/**
 * Implements of hook_field_info().
 */
function fraction_field_widget_info() {
  return array(
    'fraction_default' => array(
      'label' => t('Numerator and denominator'),
      'field types' => array('fraction'),
      'settings' => array(),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
    'fraction_decimal' => array(
      'label' => t('Decimal'),
      'field types' => array('fraction'),
      'settings' => array(
        'precision' => FRACTION_PRECISION_DEFAULT,
        'auto_precision' => TRUE,
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function fraction_field_widget_settings_form($field, $instance) {
  $form = array();
  $widget = $instance['widget'];
  $defaults = field_info_widget_settings($widget['type']);
  $settings = array_merge($defaults, $widget['settings']);

  if ($widget['type'] == 'fraction_decimal') {
    $form['precision'] = array(
      '#type' => 'textfield',
      '#title' => t('Precision'),
      '#description' => t('Specify the number of digits after the decimal place to display when converting the fraction to a decimal. When "Auto precision" is enabled, this value essentially becomes a minimum fallback precision.'),
      '#default_value' => $settings['precision'],
    );
    $form['auto_precision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto precision'),
      '#description' => t('Automatically determine the maximum precision if the fraction has a base-10 denominator. For example, 1/100 would have a precision of 2, 1/1000 would have a precision of 3, etc.'),
      '#default_value' => $settings['auto_precision'],
    );
  }

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function fraction_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $numerator = isset($items[$delta]['numerator']) ? $items[$delta]['numerator'] : '';
  $denominator = isset($items[$delta]['denominator']) ? $items[$delta]['denominator'] : '';

  $settings = $instance['widget']['settings'];

  switch ($instance['widget']['type']) {

    // Default fraction widget: two textfields (numerator and denominator).
    case 'fraction_default':
      $element['#type'] = 'fieldset';
      $element['numerator'] = array(
        '#type' => 'textfield',
        '#title' => t('Numerator'),
        '#default_value' => $numerator,
        '#element_validate' => array('element_validate_integer'),
        '#size' => 15,
      );
      $element['denominator'] = array(
        '#type' => 'textfield',
        '#title' => t('Denominator'),
        '#default_value' => $denominator,
        '#element_validate' => array('element_validate_integer'),
        '#size' => 15,
      );
      break;

    // Decimal widget: a single textfield that accepts a decimal number.
    // The default value is converted to a decimal with the specified precision.
    case 'fraction_decimal':
      $element += array(
        '#type' => 'fieldset',
        '#element_validate' => array('_fraction_decimal_widget_validate'),
      );
      $auto_precision = !empty($settings['auto_precision']) ? TRUE : FALSE;
      $element['value'] = array(
        '#type' => 'textfield',
        '#default_value' => fraction($numerator, $denominator)->toDecimal($settings['precision'], $auto_precision),
        '#element_validate' => array('element_validate_number'),
        '#size' => 15,
      );
      break;
  }

  return $element;
}

/**
 * Validation function for the fraction_decimal widget to convert the decimal to a fraction.
 */
function _fraction_decimal_widget_validate($element, &$form_state) {

  // Search through the form values to find the current field value.
  $parents = $element['#parents'];
  array_pop($parents);
  $values = drupal_array_get_nested_value($form_state['values'], $parents);

  // Convert the value to a fraction.
  $fraction = fraction_from_decimal($values[0]['value']);
  $values['numerator'] = $fraction->getNumerator();
  $values['denominator'] = $fraction->getDenominator();

  // Set the numerator and denominator values for the form.
  form_set_value($element, $values, $form_state);
}

/**
 * Implements hook_field_widget_error().
 */
function fraction_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'fraction':
      form_error($element, $error['message']);
      break;
  }
}

/**
 * Implements of hook_field_info().
 */
function fraction_field_formatter_info() {
  return array(
    'fraction_default' => array(
      'label' => t('Fraction'),
      'field types' => array('fraction'),
      'settings' => array('separator' => '/'),
    ),
    'fraction_decimal' => array(
      'label' => t('Decimal'),
      'field types' => array('fraction'),
      'settings' => array(
        'precision' => FRACTION_PRECISION_DEFAULT,
        'auto_precision' => TRUE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function fraction_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = '';

  if ($display['type'] == 'fraction_default') {
    $summary = t('Separator: @separator', array('@separator' => $settings['separator']));
  }

  elseif ($display['type'] == 'fraction_decimal') {
    $auto_precision = !empty($settings['auto_precision']) ? 'On' : 'Off';
    $summary = t('Precision: @precision, Auto-precision: @auto_precision', array('@precision' => $settings['precision'], '@auto_precision' => $auto_precision));
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function fraction_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $element = array();

  if ($display['type'] == 'fraction_default') {

    // Numerator and denominator separator.
    $element['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#description' => t('Specify the separator to display between the numerator and denominator.'),
      '#default_value' => $settings['separator'],
      '#required' => TRUE,
    );
  }

  elseif ($display['type'] == 'fraction_decimal') {

    // Decimal precision.
    $element['precision'] = array(
      '#type' => 'textfield',
      '#title' => t('Decimal precision'),
      '#description' => t('Specify the number of digits after the decimal place to display. When "Auto precision" is enabled, this value essentially becomes a minimum fallback precision.'),
      '#default_value' => $settings['precision'],
      '#required' => TRUE,
      '#element_validate' => array('element_validate_integer'),
    );

    // Auto precision.
    $element['auto_precision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto precision'),
      '#description' => t('Automatically determine the maximum precision if the fraction has a base-10 denominator. For example, 1/100 would have a precision of 2, 1/1000 would have a precision of 3, etc.'),
      '#default_value' => $settings['auto_precision'],
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function fraction_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $settings = $display['settings'];
  $element = array();

  // Iterate through the items.
  foreach ($items as $delta => $item) {

    // Merge the item with default values, in case it's empty.
    $defaults = array(
      'numerator' => 0,
      'denominator' => 1,
    );
    $item = array_merge($defaults, $item);

    // Build markup based on display type.
    switch ($display['type']) {

      // This formatter simply outputs the field as a fraction ([numerator]/[denominator]).
      case 'fraction_default':
        $element[$delta] = array(
          '#type' => 'markup',
          '#markup' => fraction($item['numerator'], $item['denominator'])->toString($settings['separator']),
        );
        break;

      // This formatter outputs the field as a decimal value with a fixed or automatic precision.
      case 'fraction_decimal':
        $auto_precision = !empty($settings['auto_precision']) ? TRUE : FALSE;
        $element[$delta] = array(
          '#type' => 'markup',
          '#markup' => fraction($item['numerator'], $item['denominator'])->toDecimal($settings['precision'], $auto_precision),
        );
        break;
    }
  }

  return $element;
}

/**
 * Implements hook_field_load().
 */
function fraction_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {

  // Iterate through entities and their items.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {

      // Load the numerator and denominator values.
      $numerator = $items[$id][$delta]['numerator'];
      $denominator = $items[$id][$delta]['denominator'];

      // Create a new Fraction object and add it to the field.
      $items[$id][$delta]['fraction'] = fraction($numerator, $denominator);
    }
  }
}

