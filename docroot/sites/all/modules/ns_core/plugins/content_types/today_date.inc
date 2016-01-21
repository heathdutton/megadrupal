<?php

/**
 * @file
 * Plugin that shows todays date.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Today date'),
  'icon' => 'icon_node.png',
  'description' => t('Todays date'),
  'category' => t('Date'),
  'defaults' => array(
    'format' => '',
    'label' => 'normal',
  ),
);

/**
 * Render the custom content type.
 */
function ns_core_today_date_content_type_render($subtype, &$conf, $panel_args) {
  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'today_date';
  $variables = array('date' => format_date(time(), 'custom', $conf['format']), 'title' => isset($block->title) ? $block->title : '', 'conf' => $conf);
  // Use a theme function for todays date so that themes can define their own style.
  $block->content = theme('ns_core_today_date', $variables);
  // If the label is not "normal", then we use that instead.
  if ($conf['label'] != 'normal') {
    $conf['override_title'] = 0;
  }
  $block->delta = 0;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function ns_core_today_date_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $form['label'] = array(
    '#type' => 'select',
    '#title' => t('Field label'),
    '#default_value' => isset($conf['label']) ? $conf['label'] : '',
    '#options' => array(
      'normal' => t('Block title'),
      'inline' => t('Inline'),
    ),
    '#description' => t('Configure how the label is going to be displayed. This option takes no effect when "Override title" option is enabled, the specified block title is displayed instead.'),
  );

  $form['format'] = array(
    '#title' => t('Date format'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $conf['format'],
  );

  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function ns_core_today_date_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function ns_core_today_date_content_type_admin_title($subtype, $conf) {
  return t('Todays date');
}
