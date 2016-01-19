<?php

/**
 * Node field types info.
 */
function node_field_types_info() {
  $items = & drupal_static(__FUNCTION__);
  if (isset($items)) {
    return $items;
  }
  $items = array();
  $items['text'] = array(
    'type' => 'text',
    'label' => t('Text'),
    'widget' => 'node_field_text_widget',
    'formatter' => 'node_field_text_formatter',
    'settings' => 'node_field_default_field_settings',
  );
  $items['text_long'] = array(
    'type' => 'text_long',
    'label' => t('Long text'),
    'widget' => 'node_field_text_long_widget',
    'formatter' => 'node_field_text_long_formatter',
    'settings' => 'node_field_text_long_settings',
  );
  $items['link'] = array(
    'type' => 'link',
    'label' => t('Link'),
    'widget' => 'node_field_link_widget',
    'formatter' => 'node_field_link_formatter',
    'settings' => 'node_field_link_settings',
  );
  $items['checkbox'] = array(
    'type' => 'checkbox',
    'label' => t('Checkbox'),
    'widget' => 'node_field_checkbox_widget',
    'formatter' => 'node_field_checkbox_formatter',
    'settings' => 'node_field_default_field_settings',
  );
  $items['radio'] = array(
    'type' => 'radio',
    'label' => t('Radio'),
    'widget' => 'node_field_radio_widget',
    'formatter' => 'node_field_radio_formatter',
    'settings' => 'node_field_radio_settings',
  );
  $items['select'] = array(
    'type' => 'select',
    'label' => t('Select'),
    'widget' => 'node_field_select_widget',
    'formatter' => 'node_field_select_formatter',
    'settings' => 'node_field_select_settings',
  );
  if (module_exists('select_or_other')) {
    $items['select_or_other'] = array(
      'type' => 'select_or_other',
      'label' => t('Select or Other'),
      'widget' => 'node_field_select_or_other_widget',
      'formatter' => 'node_field_select_or_other_formatter',
      'settings' => 'node_field_select_or_other_settings',
    );
  }
  if (module_exists('node_reference')) {
    $items['node_reference'] = array(
      'type' => 'node_reference',
      'label' => t('Node reference'),
      'widget' => 'node_field_node_reference_widget',
      'formatter' => 'node_field_node_reference_formatter',
      'settings' => 'node_field_node_reference_settings',
    );
  }
  if (module_exists('date_popup')) {
    $items['date'] = array(
      'type' => 'date',
      'label' => t('Date'),
      'widget' => 'node_field_date_widget',
      'settings' => 'node_field_default_field_settings',
    );
  }
  if (module_exists('taxonomy')) {
    $items['taxonomy'] = array(
      'type' => 'taxonomy',
      'label' => t('Vocabulary'),
      'widget' => 'node_field_taxonomy_widget',
      'formatter' => 'node_field_taxonomy_formatter',
      'settings' => 'node_field_taxonomy_settings',
    );
  }
  drupal_alter('node_field_info', $items);
  return $items;
}

/**
 * Get node field widget form.
 *
 * @param $type
 *   Requested node field type.
 *
 * @return
 *   Form array.
 */
function node_field_node_field_widget_form($node_field, &$form_state) {
  $form = array();
  $type = $node_field['type'];
  $node_field_types = node_field_types_info();
  $function = isset($node_field_types[$type]['widget']) ? $node_field_types[$type]['widget'] : FALSE;
  if ($function && is_callable($function)) {
    $form = $function($node_field, $form_state);
    drupal_alter('node_field_widget', $node_field, $form);
  }
  return $form;
}

/**
 * Get node field settings form.
 *
 * @param $node_field
 *
 * @return
 *   Form array.
 */
function node_field_node_field_settings_form($node_field) {
  $form = array();
  $type = $node_field['type'];
  $node_field_types = node_field_types_info();
  $function = isset($node_field_types[$type]['settings']) ? $node_field_types[$type]['settings'] : FALSE;
  if ($function && is_callable($function)) {
    $form = $function($node_field);
    drupal_alter('node_field_settings', $node_field, $form);
  }
  return $form;
}

function node_field_default_field_settings($node_field) {
  $form = array();
  return $form;
}

/**
 * Setting for text long field.
 */
function node_field_text_long_settings($node_field) {
  $form['use_text_format'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable text formats'),
    '#default_value' => !empty($node_field['settings']['use_text_format']) ? $node_field['settings']['use_text_format'] : 0,
  );
  return $form;
}

/**
 * Get node field formatter.
 *
 * @param $node_field
 *
 * @return
 *   Form array.
 */
function node_field_node_field_formatter($node_field) {
  $node_field_types = node_field_types_info();
  $function = isset($node_field_types[$node_field['type']]['formatter']) ? $node_field_types[$node_field['type']]['formatter'] : FALSE;
  if ($function && is_callable($function)) {
    $value = $function($node_field);
    drupal_alter('node_field_formatter', $node_field, $value);
    $node_field['value'] = $value;
  }
  return $node_field;
}

/**
 * Widget for text field.
 */
function node_field_text_widget($node_field) {
  $form = array(
    '#type' => 'textfield',
    '#title' => $node_field['title'],
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Formatter for text field.
 */
function node_field_text_formatter($node_field) {
  if (isset($node_field['value'])) {
    return check_plain($node_field['value']);
  }
}

/**
 * Widget for long text field.
 */
function node_field_text_long_widget($node_field) {
  $form = array(
    '#type' => 'textarea',
    '#title' => $node_field['title'],
    '#default_value' => $node_field['value'],
  );
  if (!empty($node_field['settings']['use_text_format'])) {
    $form['#type'] = 'text_format';
    if (!empty($node_field['settings']['text_format'])) {
      $form['#format'] = $node_field['settings']['text_format'];
    }
  }
  return $form;
}

/**
 * Formatter for long text field.
 */
function node_field_text_long_formatter($node_field) {
  if (isset($node_field['value'])) {
    if (!empty($node_field['settings']['text_format']) && !empty($node_field['settings']['use_text_format'])) {
      return check_markup($node_field['value'], $node_field['settings']['text_format']);
    }
    return check_plain($node_field['value']);
  }
}

/**
 * Widget for link field.
 */
function node_field_link_widget($node_field) {
  $form = array(
    '#type' => 'textfield',
    '#title' => $node_field['title'],
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Settings for link field.
 */
function node_field_link_settings($node_field) {
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url address'),
    '#default_value' => !empty($node_field['settings']['url']) ? $node_field['settings']['url'] : NULL,
  );
  return $form;
}

/**
 * Formatter for link field.
 */
function node_field_link_formatter($node_field) {
  if (!$node_field['value'] || empty($node_field['settings']['url'])) {
    return;
  }
  $link = l($node_field['value'], $node_field['settings']['url']);
  return $link;
}

/**
 * Widget for checkbox field.
 */
function node_field_checkbox_widget($node_field) {
  $form = array(
    '#type' => 'checkbox',
    '#title' => $node_field['title'],
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Formatter for checkbox field.
 */
function node_field_checkbox_formatter($node_field) {
  return $node_field['value'] ? TRUE : FALSE;
}

/**
 * Widget for radio field.
 */
function node_field_radio_widget($node_field) {
  $options = node_field_get_options_field_options($node_field);
  $form = array(
    '#type' => 'radios',
    '#title' => $node_field['title'],
    '#options' => $options,
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Settings for radio field.
 */
function node_field_radio_settings($node_field) {
  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => !empty($node_field['settings']['options']) ? $node_field['settings']['options'] : NULL,
    '#description' => t(NODE_FIELD_OPTIONS_FIELD_DESCRIPTION)
  );
  return $form;
}

/**
 * Formatter for radio field.
 */
function node_field_radio_formatter($node_field) {
  $options = node_field_get_options_field_options($node_field);
  if ($node_field['value']) {
    if ($options) {
      return check_plain($options[$node_field['value']]);
    }
  }
  else {
    $default = array_shift($options);
    return check_plain($default);
  }
}

/**
 * Widget for select field.
 */
function node_field_select_widget($node_field) {
  $options = node_field_get_options_field_options($node_field);
  $form = array(
    '#type' => 'select',
    '#title' => $node_field['title'],
    '#options' => $options,
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Settings for select field.
 */
function node_field_select_settings($node_field) {
  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => !empty($node_field['settings']['options']) ? $node_field['settings']['options'] : NULL,
    '#description' => t(NODE_FIELD_OPTIONS_FIELD_DESCRIPTION)
  );
  return $form;
}

/**
 * Formatter for select field.
 *
 * @return
 *   Form array.
 */
function node_field_select_formatter($node_field) {
  $options = node_field_get_options_field_options($node_field);
  if (isset($node_field['value'])) {
    if ($options) {
      if (isset($options[$node_field['value']])) {
        return check_plain($options[$node_field['value']]);
      }
    }
  }
}

/**
 * Settings for select_or_other field.
 *
 */
function node_field_select_or_other_settings($node_field) {
  $form = array();
  $form['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => !empty($node_field['settings']['options']) ? $node_field['settings']['options'] : NULL,
    '#description' => t(NODE_FIELD_OPTIONS_FIELD_DESCRIPTION),
  );
  $form['other_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Other Title'),
    '#default_value' => !empty($node_field['settings']['other_title']) ? $node_field['settings']['other_title'] : NULL,
    '#description' => t('Other field title.'),
  );
  return $form;
}

/**
 * Widget for select_or_other field.
 *
 */
function node_field_select_or_other_widget($node_field) {
  $options = node_field_get_options_field_options($node_field);
  $form = array(
    '#type' => 'select_or_other',
    '#title' => $node_field['title'],
    '#options' => $options,
    '#default_value' => $node_field['value'],
    '#other' => isset($node_field['settings']['other_title']) ? $node_field['settings']['other_title'] : t('Other'),
    '#multiple' => FALSE,
    '#other_unknown_defaults' => 'other',
    '#other_delimiter' => FALSE,
    '#select_type' => 'select',
  );
  return $form;
}

/**
 * Formatter for select_or_other field.
 *
 * @return
 *   Formatted output
 */
function node_field_select_or_other_formatter($node_field) {
  $options = node_field_get_options_field_options($node_field);
  if (isset($node_field['value'])) {
    if ($options) {
      if (isset($options[$node_field['value']])) {
        return check_plain($options[$node_field['value']]);
      }
      return check_plain($node_field['value']);
    }
  }
}

/**
 * Widget for node_reference field.
 */
function node_field_node_reference_widget($node_field) {
  $path = drupal_get_path('module', 'node_field');
  require_once $path . '/model/node_field.db.inc';
  $options = array();
  $content_type = $node_field['settings']['options'];
  $nodes = node_field_node_reference_db_get_nodes($content_type);
  foreach ($nodes as $nid => $title) {
    $options[$nid] = $title;
  }
  $form = array(
    '#type' => 'select',
    '#title' => $node_field['title'],
    '#default_value' => $node_field['value'],
    '#options' => $options
  );
  return $form;
}

/**
 * Settings for node_reference field.
 */
function node_field_node_reference_settings($node_field) {
  $content_types = node_type_get_types();
  foreach ($content_types as $type) {
    $options[] = $type->type;
  }
  $options = drupal_map_assoc($options);
  $form['options'] = array(
    '#type' => 'select',
    '#title' => t('Allowed content types'),
    '#options' => $options,
    '#default_value' => !empty($node_field['settings']['options']) ? $node_field['settings']['options'] : NULL,
  );
  return $form;
}

/**
 * Formatter for taxonomy field.
 */
function node_field_node_reference_formatter($node_field) {
  if (!empty($node_field['value'])) {
    $node = node_load($node_field['value']);
    if ($node) {
      return l($node->title, 'node/' . $node->nid);
    }
  }
}

/**
 * Widget for date field.
 *
 * @return
 *   Form array.
 */
function node_field_date_widget($node_field) {
  $form = array(
    '#type' => 'date_popup',
    '#title' => $node_field['title'],
    '#date_format' => 'm/d/Y',
    '#date_year_range' => '-90:+10',
    '#default_value' => $node_field['value'],
  );
  return $form;
}

/**
 * Widget for taxonomy field.
 */
function node_field_taxonomy_widget($node_field) {
  if (empty($node_field['settings']['vocs'])) {
    return;
  }
  $wrapper_id = 'node-field-value-wrapper-' . $node_field['id'];
  $terms_options = array();
  $terms = taxonomy_get_tree($node_field['settings']['vocs']);
  foreach ($terms as $term) {
    $terms_options[$term->tid] = $term->name;
  }
  $form = array(
    '#type' => 'select',
    '#title' => $node_field['title'],
    '#options' => $terms_options,
    '#default_value' => $node_field['value'],
    '#prefix' => '<div id="' . $wrapper_id . '">',
    '#suffix' => '</div>',
    '#disabled' => empty($node_field['settings']['vocs']) ? TRUE : FALSE,
  );
  return $form;
}

/**
 * Settings for taxonomy field.
 */
function node_field_taxonomy_settings($node_field) {
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
    '#default_value' => !empty($node_field['settings']['vocs']) ? $node_field['settings']['vocs'] : NULL,
  );
  return $form;
}

/**
 * Formatter for taxonomy field.
 */
function node_field_taxonomy_formatter($node_field) {
  if ($node_field['value']) {
    $term = taxonomy_term_load($node_field['value']);
    $link = l($term->name, 'taxonomy/term/' . $term->tid);
    return $link;
  }
}

function node_field_get_options_field_options($node_field) {
  $options = array();
  if (isset($node_field['settings']['options'])) {
    $options = list_extract_allowed_values($node_field['settings']['options'], 'list_text', TRUE);
  }
  return $options;
}