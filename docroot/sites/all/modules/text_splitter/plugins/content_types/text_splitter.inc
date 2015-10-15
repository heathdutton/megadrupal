<?php

/**
 * @file
 * Plugin definition.
 */

$plugin = array(
  'title' => t('Text Splitter'),
  'description' => t('Displays a filter or button of a text splitter.'),
  'category' => t('Miscellaneous'),
  'defaults' => array(
    'type' => 'filter',
    'selector' => '.field-name-body',
    'tags' => 'H2',
    'button_text' => t('Show a full text'),
  ),
  'single' => TRUE,
);

/**
 * Render callback.
 */
function text_splitter_text_splitter_content_type_render($subtype, $conf, $args, $contexts) {
  $id = REQUEST_TIME . rand();
  $type = $conf['type'];

  $elements = array();

  $elements[$type] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'text-splitter-' . $type,
        'text-splitter-id-' . $id,
      ),
      'style' => 'display: none;',
    ),
  );

  if ($type == 'button') {
    $elements[$type]['#type'] = 'html_tag';
    $elements[$type]['#tag'] = 'button';
    $elements[$type]['#value'] = t($conf['button_text']);
  }

  $settings = array(
    'text_splitter' => array(
      array(
        'id' => $id,
        'type' => $type,
        'selector' => $conf['selector'],
        'tags' => $conf['tags'],
      ),
    ),
  );

  drupal_add_js($settings, 'setting');
  drupal_add_js(drupal_get_path('module', 'text_splitter') . '/js/text_splitter.js');

  $block = new stdClass();
  $block->title = '';
  $block->content = $elements;

  return $block;
}

/**
 * Returns an edit form.
 */
function text_splitter_text_splitter_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#options' => array(
      'filter' => t('Show a filter'),
      'button' => t('Show a reset button'),
    ),
    '#default_value' => !empty($conf['type']) ? $conf['type'] : $form_state['plugin']['defaults']['type'],
    '#description' => t('Select an operating mode.'),
  );

  $form['selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Selector'),
    '#default_value' => !empty($conf['selector']) ? $conf['selector'] : $form_state['plugin']['defaults']['selector'],
    '#description' => t("A jQuery selector that specifies where to work with a text. Example: .field-name-body, .main-content."),
  );

  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#default_value' => !empty($conf['tags']) ? $conf['tags'] : $form_state['plugin']['defaults']['tags'],
    '#description' => t("Specify tags to search desired elements."),
  );

  $form['button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => !empty($conf['button_text']) ? $conf['button_text'] : $form_state['plugin']['defaults']['button_text'],
    '#description' => t("The text that will appear in the button."),
    '#states' => array(
      'visible' => array(
        ':input[name="type"]' => array('value' => 'button'),
      ),
    ),
  );

  return $form;
}

/**
 * Validate handler.
 */
function text_splitter_text_splitter_content_type_edit_form_validate($form, &$form_state) {
  foreach ($form_state['plugin']['defaults'] as $key => $value) {
    if (empty($form_state['values'][$key])) {
      $form_state['values'][$key] = $value;
    }
  }
}

/**
 * Submit handler.
 */
function text_splitter_text_splitter_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Callback to provide administrative title.
 */
function text_splitter_text_splitter_content_type_admin_title($subtype, $conf, $context) {
  return t('Text splitter @type', array('@type' => $conf['type']));
}

/**
 * Callback to provide administrative info.
 */
function text_splitter_text_splitter_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = $conf['selector'];
  $block->content = ($conf['type'] == 'filter') ? $conf['tags'] : $conf['button_text'];
  return $block;
}
