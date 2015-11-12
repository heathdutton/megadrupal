<?php

/**
 * @file
 * The include file which controls the Image field.
 */

/**
 * Implements hook_field_widget_info().
 */
function nice_imagefield_widget_field_widget_info() {
  return array(
    'image_nice' => array(
      'label' => t('Nice ImageField'),
      'field types' => array('image'),
      'settings' => array(
        'progress_indicator' => 'throbber',
        'preview_image_style' => 'nice_image',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function nice_imagefield_widget_field_widget_settings_form($field, $instance) {
  $settings = $instance['widget']['settings'];

  // Use the mfw file widget settings form.
  $form = multiupload_filefield_widget_field_widget_settings_form($field, $instance);

  $form['preview_image_style'] = array(
    '#title' => t('Preview image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['preview_image_style'],
    '#description' => t('The preview image will be shown while editing the content.'),
    '#required' => TRUE,
    '#weight' => 15,
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function nice_imagefield_widget_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Add display_field setting to field because file_field_widget_form() assumes it is set.
  $field['settings']['display_field'] = 0;

  // Load the items for form rebuilds from the field state as they might not be
  // in $form_state['values'] because of validation limitations. Also, they are
  // only passed in as $items when editing existing entities.
  $field_state = field_form_get_state($element['#field_parents'], $field['field_name'], $langcode, $form_state);
  if (isset($field_state['items'])) {
    $items = $field_state['items'];
  }

  $elements = multiupload_filefield_widget_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  $settings = $instance['settings'];

  foreach (element_children($elements) as $delta) {
    // Add upload resolution validation.
    if ($settings['max_resolution'] || $settings['min_resolution']) {
      $elements[$delta]['#upload_validators']['file_validate_image_resolution'] = array($settings['max_resolution'], $settings['min_resolution']);
    }

    // If not using custom extension validation, ensure this is an image.
    $supported_extensions = array('png', 'gif', 'jpg', 'jpeg');
    $extensions = isset($elements[$delta]['#upload_validators']['file_validate_extensions'][0]) ? $elements[$delta]['#upload_validators']['file_validate_extensions'][0] : implode(' ', $supported_extensions);
    $extensions = array_intersect(explode(' ', $extensions), $supported_extensions);
    $elements[$delta]['#upload_validators']['file_validate_extensions'][0] = implode(' ', $extensions);

    // Add all extra functionality provided by the image widget.
    $elements[$delta]['#process'][] = 'image_field_widget_process';
  }

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => $instance['description'], 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));

    // Get settings of the field.
    $instance = field_widget_instance($element, $form_state);
    $widget_settings = $instance['widget']['settings'];
    $style = image_style_load($widget_settings['preview_image_style']);

    // Prepare dimensions of the preview image.
    while (!isset($size) && !empty($style['effects'])) {
      $effect = array_pop($style['effects']);
      $effect_data = $effect['data'];
      if (!empty($effect_data['height']) && !empty($effect_data['width'])) {
        $size = array(
          'height' => $effect_data['height'],
          'width' => $effect_data['width'],
        );
      }
    }

    $elements['#theme'] = 'nice_imagefield_widget_multiple';
    $elements['#nice_imagefield_size'] = $size;

    // Include necessary libraries.
    $elements['#attached']['library'] = array(
      array('system', 'ui.sortable'),
      array('nice_imagefield_widget', 'flip'),
    );
  }

  return $elements;
}

/**
 * Implements hook_field_widget_form_alter().
 *
 * @param $element
 * @param $form_state
 * @param $context
 */
function nice_imagefield_widget_field_widget_form_alter(&$element, &$form_state, $context) {
  // If this is an image field type of instance 'field_image_top'
  if (isset($element['#theme']) && $element['#theme'] == 'nice_imagefield_widget_multiple') {
    // Loop through the element children (there will always be at least one).
    foreach (element_children($element) as $key => $child) {
      // Add the new process function to the element
      $element[$key]['#process'][] = 'nice_imagefield_widget_image_field_widget_process';
      $element[$key]['#nice_imagefield_size'] = $element['#nice_imagefield_size'];
    }
  }
}

/**
 * Change the image element.
 *
 * @param $element
 * @param $form_state
 * @param $form
 * @return mixed
 */
function nice_imagefield_widget_image_field_widget_process($element, &$form_state, $form) {
  // Change type of the "_weight" field to hidden.
  if (isset($element['_weight'])) {
    $weight = &$element['_weight'];
    $weight['#type'] = 'hidden';
    $weight += element_info('hidden');
  }

  $element['#theme'] = 'nice_imagefield_widget_image';
  return $element;
}
