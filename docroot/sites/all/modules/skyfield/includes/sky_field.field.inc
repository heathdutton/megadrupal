<?php

/**
 * Sky field types info.
 */
function sky_field_types_info() {
  $items = &drupal_static(__FUNCTION__);
  if (isset($items)) {
    return $items;
  }
  $items = array();
  $items['text'] = array(
    'type' => 'text',
    'label' => t('Text'),
    'widget' => 'sky_field_text_widget',
    'formatter' => 'sky_field_text_formatter',
    'settings' => 'sky_field_default_field_settings',
  );
  $items['text_long'] = array(
    'type' => 'text_long',
    'label' => t('Long text'),
    'widget' => 'sky_field_text_long_widget',
    'formatter' => 'sky_field_text_long_formatter',
    'settings' => 'sky_field_default_field_settings',
  );
  $items['link'] = array(
    'type' => 'link',
    'label' => t('Link'),
    'widget' => 'sky_field_link_widget',
    'formatter' => 'sky_field_link_formatter',
    'settings' => 'sky_field_link_settings',
  );
  $items['checkbox'] = array(
    'type' => 'checkbox',
    'label' => t('Checkbox'),
    'widget' => 'sky_field_checkbox_widget',
    'formatter' => 'sky_field_checkbox_formatter',
    'settings' => 'sky_field_default_field_settings',
  );
  $items['radio'] = array(
    'type' => 'radio',
    'label' => t('Radio'),
    'widget' => 'sky_field_radio_widget',
    'formatter' => 'sky_field_radio_formatter',
    'settings' => 'sky_field_radio_settings',
  );
  $items['select'] = array(
    'type' => 'select',
    'label' => t('Select'),
    'widget' => 'sky_field_select_widget',
    'formatter' => 'sky_field_select_formatter',
    'settings' => 'sky_field_select_settings',
  );
  if (module_exists('date_popup')) {
    $items['date'] = array(
      'type' => 'date',
      'label' => t('Date'),
      'widget' => 'sky_field_date_widget',
    );
  }
  if (module_exists('taxonomy')) {
    $items['taxonomy'] = array(
      'type' => 'taxonomy',
      'label' => t('Vocabulary'),
      'widget' => 'sky_field_taxonomy_widget',
      'formatter' => 'sky_field_taxonomy_formatter',
      'settings' => 'sky_field_taxonomy_settings',
    );
  }
  drupal_alter('sky_field_info', $items);
  return $items;
}

/**
 * Get sky field widget form.
 *
 * @param $type
 *   Requested field type.
 *
 * @return
 *   Form array.
 */
function sky_field_sky_field_widget_form($sky_field) {
  $form = array();
  $type = $sky_field['type'];
  $sky_field_types = sky_field_types_info();
  $function = isset($sky_field_types[$type]['widget']) ? $sky_field_types[$type]['widget'] : FALSE;
  if ($function && is_callable($function)) {
    $form = $function($sky_field);
    drupal_alter('sky_field_widget', $sky_field, $form);
  }
  return $form;
}

/**
 * Get sky field settings form.
 *
 * @param $type
 *   Requested field type.
 *
 * @return
 *   Form array.
 */
function sky_field_sky_field_settings_form($sky_field) {
  $form = array();
  $type = $sky_field['type'];
  $sky_field_types = sky_field_types_info();
  $function = isset($sky_field_types[$type]['settings']) ? $sky_field_types[$type]['settings'] : FALSE;
  if ($function && is_callable($function)) {
    $form = $function($sky_field);
    drupal_alter('sky_field_settings', $sky_field, $form);
  }
  return $form;
}

function sky_field_default_field_settings($sky_field) {
  $form = array();
  return $form;
}

/**
 * Get sky field formatter.
 *
 * @param $type
 *   Requested field type.
 *
 * @return
 *   Form array.
 */
function sky_field_sky_field_formatter($sky_field) {
  $sky_field_types = sky_field_types_info();
  $function = isset($sky_field_types[$sky_field['type']]['formatter']) ? $sky_field_types[$sky_field['type']]['formatter'] : FALSE;
  if ($function && is_callable($function)) {
    $sky_field['value'] = $function($sky_field);
    drupal_alter('sky_field_formatter', $sky_field, $value);
  }
  return $sky_field;
}

/**
 * Widget for text field.
 */
function sky_field_text_widget($sky_field) {
  $form = array(
    '#type' => 'textfield',
    '#title' => $sky_field['title'],
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Formatter for text field.
 */
function sky_field_text_formatter($sky_field) {
  if ($sky_field['value']) {
    return check_plain($sky_field['value']);
  }
}

/**
 * Widget for long text field.
 */
function sky_field_text_long_widget($sky_field) {
  $form = array(
    '#type' => 'textarea',
    '#title' => $sky_field['title'],
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Formatter for long text field.
 */
function sky_field_text_long_formatter($sky_field) {
  if ($sky_field['value']) {
    return check_plain($sky_field['value']);
  }
}

/**
 * Widget for link field.
 */
function sky_field_link_widget($sky_field) {
  $form = array(
    '#type' => 'textfield',
    '#title' => $sky_field['title'],
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

function sky_field_link_settings($sky_field) {
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url address'),
    '#default_value' => !empty($sky_field['settings']['url']) ? $sky_field['settings']['url'] : NULL,
  );
  return $form;
}

/**
 * Formatter for link field.
 */
function sky_field_link_formatter($sky_field) {
  if (!$sky_field['value']) {
    return;
  }
  if (empty($sky_field['settings']['url'])) {
    return check_plain($sky_field['value']);
  }
  /*
   * Attention! Temporary hack!
   * @todo fix it.
   */
  $sky_field['settings']['url'] = str_replace('[id]', $sky_field['entity_id'], $sky_field['settings']['url']);
  $link = l($sky_field['value'], $sky_field['settings']['url']);
  return $link;
}

/**
 * Widget for checkbox field.
 */
function sky_field_checkbox_widget($sky_field) {
  $form = array(
    '#type' => 'checkbox',
    '#title' => $sky_field['title'],
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Formatter for checkbox field.
 */
function sky_field_checkbox_formatter($sky_field) {
  if (!$sky_field['value']) {
    return;
  }
  $output = check_plain($sky_field['value']);
  return $output;
}

/**
 * Widget for radio field.
 */
function sky_field_radio_widget($sky_field) {
  $options = sky_field_get_options_field_options($sky_field);
  if (empty($options)) {
    return;
  }
  $form = array(
    '#type' => 'radios',
    '#title' => $sky_field['title'],
    '#options' => $options,
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Settings for radio field.
 */
function sky_field_radio_settings($sky_field) {
  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => !empty($sky_field['settings']['options']) ? $sky_field['settings']['options'] : NULL,
    '#description' => t(SKY_FIELD_OPTIONS_FIELD_DESCRIPTION)
  );
  return $form;
}

/**
 * Formatter for radio field.
 */
function sky_field_radio_formatter($sky_field) {
  $options = sky_field_get_options_field_options($sky_field);
  if (!isset($sky_field['value']) || !$options) {
    return;
  }
  if (!isset($options[$sky_field['value']])) {
    return;
  }
  return check_plain($options[$sky_field['value']]);
}

/**
 * Widget for select field.
 */
function sky_field_select_widget($sky_field) {
  $options = sky_field_get_options_field_options($sky_field);
  if (empty($options)) {
    return;
  }
  $form = array(
    '#type' => 'select',
    '#title' => $sky_field['title'],
    '#options' => $options,
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Settings for select field.
 */
function sky_field_select_settings($sky_field) {
  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => !empty($sky_field['settings']['options']) ? $sky_field['settings']['options'] : NULL,
    '#description' => t(SKY_FIELD_OPTIONS_FIELD_DESCRIPTION)
  );
  return $form;
}

/**
 * Formatter for radio field.
 *
 * @return
 *   Form array.
 */
function sky_field_select_formatter($sky_field) {
  $options = sky_field_get_options_field_options($sky_field);
  if (!isset($sky_field['value']) || !$options) {
    return;
  }
  if (!isset($options[$sky_field['value']])) {
    return;
  }
  return check_plain($options[$sky_field['value']]);
}

/**
 * Widget for date field.
 *
 * @return
 *   Form array.
 */
function sky_field_date_widget($sky_field) {
  $form = array(
    '#type' => 'date_popup',
    '#title' => $sky_field['title'],
    '#date_format' => 'm/d/Y',
    '#date_year_range' => '-90:+10',
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
  );
  return $form;
}

/**
 * Widget for taxonomy field.
 */
function sky_field_taxonomy_widget($sky_field) {
  if (empty($sky_field['settings']['vocs'])) {
    return;
  }
  $wrapper_id = 'sky-field-value-wrapper-' . $sky_field['id'];
  $terms_options = array();
  $terms = taxonomy_get_tree($sky_field['settings']['vocs']);
  foreach ($terms as $term) {
    $terms_options[$term->tid] = $term->name;
  }
  $form = array(
    '#type' => 'select',
    '#title' => $sky_field['title'],
    '#options' => $terms_options,
		'#required' => $sky_field['required'],
    '#default_value' => $sky_field['value'],
    '#prefix' => '<div id="' . $wrapper_id . '">',
    '#suffix' => '</div>',
    '#disabled' => empty($sky_field['settings']['vocs']) ? TRUE : FALSE,
  );
  return $form;
}

/**
 * Settings for taxonomy field.
 */
function sky_field_taxonomy_settings($sky_field) {
  $wrapper_id = 'sky-field-value-wrapper-' . $sky_field['id'];
  $vocs_options = array();
  $vocs = taxonomy_vocabulary_get_names();
  foreach ($vocs as $voc) {
    $vocs_options[$voc->vid] = $voc->name;
  }
  $form['vocs'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#options' => $vocs_options,
    '#empty_option' => t('- Select -'),
    '#default_value' => !empty($sky_field['settings']['vocs']) ? $sky_field['settings']['vocs'] : NULL,
  );
  return $form;
}

/**
 * Formatter for taxonomy field.
 */
function sky_field_taxonomy_formatter($sky_field) {
  if ($sky_field['value']) {
    $term = taxonomy_term_load($sky_field['value']);
    $link = l($term->name, 'taxonomy/term/' . $term->tid);
    return $link;
  }
}

function sky_field_get_options_field_options($sky_field) {
  $options = array();
  if (!empty($sky_field['settings']['options'])) {
    $options = list_extract_allowed_values($sky_field['settings']['options'], 'list_text', TRUE);
  }
  return $options;
}