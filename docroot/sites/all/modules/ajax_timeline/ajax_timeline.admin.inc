<?php
/**
 * @file ajax_timeline.admin.inc
 * Ajax Timeline administration pages.
 */

/**
 * Implements hook_admin().
 */
function ajax_timeline_admin_form($form, &$form_state) {
  $form = array();
  
  $form = system_settings_form($form);

  // ALl content types.
  $content_types = node_type_get_types();
  $options = array();
  foreach ($content_types as $type => $data) $options[$type] = $data->name;

  // TODO: Eventually support more than one type and more than one timeline.
  $form['ajax_timeline_type'] = array(
    '#type' => 'select',
    '#title' => t('Content Type Selection'),
    '#options' => $options,
    '#default_value' => variable_get('ajax_timeline_type', 'page'),
    '#description' => t('The content type to build the timeline off of.'),
    '#ajax' => array(
      'event' => 'change',
      'callback' => '_ajax_timeline_load_fields',
      'wrapper' => 'timeline-field-wrap',
      'method' => 'replace',
    ),
  );
  
  $form['ajax_timeline_field_wrap'] = array(
    '#type' => 'container',
    '#id' => 'timeline-field-wrap',
  );
  if (isset($form_state['values']['ajax_timeline_type'])) {
    $type = $form_state['values']['ajax_timeline_type'];
  }
  else {
    $type = variable_get('ajax_timeline_type', 'page');
  }
  
  // Get all fields available for this type
  $fields = field_info_instances('node', $type);
  $date_fields = array('created' => t('Date Created'), 'changed' => t('Date Updated'));
  $image_fields = array('' => t('-- None --'));
  
  // Only 'date' and 'image' fields are allowed
  foreach ($fields as $type => $data) {
    if ($data['widget']['module'] == 'date') {
      $date_fields[$type] = $data['label'];
    }
    elseif ($data['widget']['module'] == 'image') {
      $image_fields[$type] = $data['label'];
    }
  }
  
  $form['ajax_timeline_field_wrap']['ajax_timeline_field'] = array(
    '#title' => t('Date Field Selection'),
    '#type' => 'select',
    '#options' => $date_fields,
    '#default_value' => variable_get('ajax_timeline_field', 'created'),
    '#description' => t('The field to use for the timeline dates'),
  );
  
  $form['ajax_timeline_field_wrap']['ajax_timeline_image'] = array(
    '#title' => t('Image Field Selection (Optional)'),
    '#type' => 'select',
    '#options' => $image_fields,
    '#default_value' => variable_get('ajax_timeline_image', ''),
    '#description' => t('The field to use for the timeline image.'),
  );  
  
  $form['ajax_timeline_infinite_scroll'] = array(
    '#title' => t('Infinite Scroll'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('ajax_timeline_infinite_scroll', FALSE),
    '#description' => t('If checked, timeline will load yearly events as users scroll down.')
  );

  return $form;
}


/**
 * Load the select with available date fields
 */
function _ajax_timeline_load_fields($form, &$form_state) {
  return render($form['ajax_timeline_field_wrap']);
}
