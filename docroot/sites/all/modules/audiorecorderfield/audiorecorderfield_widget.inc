<?php
// $Id:

/**
 * @file
 * audiorecorderfield widget hooks and callbacks.
 */

 
 /**
 * Implements hook_field_load().
 */
function audiorecorderfield_field_load($entity_type, $entities, $field, $instances, $langcode, &$items, $age) {
  file_field_load($entity_type, $entities, $field, $instances, $langcode, $items, $age);
}

/**
 * Implements hook_field_prepare_view().
 */
function audiorecorderfield_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  //TODO: Check this:
  //  Remove files specified to not be displayed.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!file_field_displayed($item, $field)) {
        unset($items[$id][$delta]);
      }
      // Ensure consecutive deltas.
      $items[$id] = array_values($items[$id]);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function audiorecorderfield_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}

/**
 * Implements hook_field_widget_info().
 */
function audiorecorderfield_field_widget_info() {
  return array(  
    'audiorecorderfield_widget' => array(
      'label' => t('Audio Recorder'),
      'field types' => array('file'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
 
function audiorecorderfield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  //TODO: Add display_field setting to field because file_field_widget_form() assumes it is set.
  $field['settings']['display_field'] = 0;
  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);
  $settings = $instance['settings'];

  foreach (element_children($elements) as $delta) {
    // Add all extra functionality provided by the audiorecorder widget.
      //$elements[$delta]['#process'][] = 'audiorecorderfield_widget_process';
      $elements[$delta]['#process'] = array('audiorecorderfield_widget_process');
  }

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array('description' => $instance['description'], 'upload_validators' => $elements[0]['#upload_validators']));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }

  return $elements;
}

/**
 * An element #process callback for the audiorecorderfield_widget field type.
 *
 * Expands the audiorecorderfield_widget type to additional fields.
 */
function audiorecorderfield_widget_process($element, &$form_state, $form) {

  // If not a recognized field instance, do not process.
  if (!isset($element['#field_name']) || !($instance = field_widget_instance($element, $form_state))) {
    return;
  }

  $field_name=audiorecorderfield_str_replace_once('field_', '', $element['#field_name']);
  //Change "_" into "-"
  $field_name=str_replace('_', '-', $field_name);
  $recorder = audiorecorderfield_get_recorder();
  
  //$element['#delta'] doesn't hold correct delta for the field, therefore using $element['#parents'][2]
  $element['applet'] = array(
    '#type' =>'item',
    '#markup' => "<div id='audiorecorderfield-".$field_name."-".$element['#language']."-".$element['#parents'][2]."-wrapper'>".$recorder['callback']($element)."</div>",
  ); 

  $element['fid'] = array(
    '#type' =>'hidden',
    '#value' => $element['#value']['fid'],
  );  

  return $element;
}

/**
 * Theme the output of the audio recorder field.
 */
 //TODO:????
function theme_audiorecorderfield_widget($variables) {
  $element = $variables['element'];
  
  $output = audiorecorderfield_recorder_nanogong($element);
  return '<div class="filefield-source filefield-source-recorder clear-block">' . $output . '</div>';
}