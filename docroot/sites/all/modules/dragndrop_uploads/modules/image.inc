<?php
/**
 * @file
 * Image module integration.
 */

/**
 * Implements hook_dragndrop_uploads_widgets().
 */
function image_dragndrop_uploads_widgets($type, &$widgets) {
  // TODO: Replace with field_info_fields() when http://drupal.org/node/613754 is fixed.
  $instances = field_info_instances('node', $type);
  $fields = array();
  foreach ($instances as $field_name => $instance) {
    $fields[$field_name] = field_info_field($field_name);
    $fields[$field_name]['label'] = $instance['label'];
  }
  foreach ($fields as $field) {
    if ($field['module'] == 'image') {
      $data = serialize(array(
        'field' => $field['field_name'],
        'module' => $field['module'],
      ));
      $widgets[$data] = $field['label'];
    }
  }
}

/**
 * Implements hook_dragndrop_uploads_js_data().
 */
function image_dragndrop_uploads_js_data($data) {
  $field = field_info_field($data['field']);
  $data = array(
    'id' => $field['field_name'],
    'result' => '.output:last',
    'submit' => 'Upload',
    'wrapper' => '.field-name-' . str_replace('_', '-', $field['field_name']),
  );
  return array($field['field_name'] => $data);
}

/**
 * Implements hook_dragndrop_uploads_elements().
 */
function image_dragndrop_uploads_elements(&$type) {
  $type['managed_file']['#after_build'][] = 'image_dragndrop_uploads_process';
}

/**
 *
 */
function image_dragndrop_uploads_process($element, &$form_state) {
  if ($element['#theme'] == 'image_widget' && $element['#file']) {
    // Build entity and field related objects.
    $entity_type = $element['#entity_type'];
    $entity = isset($form_state['values']) ? $form_state['values'] : array();
    $field = field_info_field($element['#field_name']);
    $instance = field_info_instance($entity_type, $field['field_name'], $element['#bundle']);
    $langcode = $element['#language'];

    // Prepare item.
    $item = $form_state['values'][$field['field_name']][$langcode][$element['#delta']];
    if (function_exists($function = "{$field['module']}_field_load")) {
      $items = array(array($item));
      $function($entity_type, array($entity), $field, array($instance), $langcode, $items, FIELD_LOAD_CURRENT);
      $item = $items[0][0];
    }

    // Load formatter.
    $formatter_type = variable_get('dragndrop_uploads_formatter_' . $element['#bundle'], 'image');
    $formatter = field_info_formatter_types($formatter_type);

    // Prepare display object for formatter.
    $display = array(
      'label' => 'hidden',
      'type' => $formatter_type,
      'module' => $formatter['module'],
      // @TODO - Add support for formatter settings.
      'settings' => $formatter['settings'],
      // @TODO - Move this to includes/custom_formatters.inc and determine a way
      // to allow contextual links without exposing to anonymous user.
      'cf_options' => array(
        '#contextual_links' => FALSE,
      ),
    );

    // Render item.
    if (function_exists($function = "{$formatter['module']}_field_formatter_view")) {
      $element['dragndrop_uploads'] = array(
        '#type' => 'hidden',
        '#value' => render($function($entity_type, $entity, $field, $instance, $langcode, array($item), $display)),
        '#name' => "{$element['#name']}[output]",
        '#id' => "{$element['#id']}-output",
        '#attributes' => array(
          'class' => 'output',
        ),
      );
    }
  }

  return $element;
}
