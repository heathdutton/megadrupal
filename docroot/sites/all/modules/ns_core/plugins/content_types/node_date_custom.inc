<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Node custom date'),
  'icon' => 'icon_node.png',
  'description' => t('The date the referenced node was created, with possibility of customization.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
  'defaults' => array(
    'format' => '',
    'node_date' => 'created',
    'label' => 'normal',

  ),
);

/**
 * Render the custom content type.
 */
function ns_core_node_date_custom_content_type_render($subtype, &$conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Get a shortcut to the node.
  $node = $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'node_created';
  switch($conf['node_date']) {
    case 'created':
      $block->title = !$conf['override_title'] ? t('Created date') : check_plain($conf['override_title_text']);
      $date = $node->created;
      break;
    case 'updated':
      $block->title = !$conf['override_title'] ? t('Updated date') : check_plain($conf['override_title_text']);
      $date = $node->changed;
      break;
  }

  $variables = array('conf' => $conf, 'title' => $block->title, 'date' => format_date($date, 'custom', $conf['format']));
  $block->content = theme('ns_core_node_date_custom', $variables);
  if ($conf['label'] != 'normal') {
    unset($block->title);
    $conf['override_title'] = 0;
  }
  $block->delta = $node->nid;

  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function ns_core_node_date_custom_content_type_edit_form($form, &$form_state) {
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

  $form['node_date'] = array(
    '#type' => 'select',
    '#title' => t('Node Date Type'),
    '#options' => array(
      'created' => t('Created Date'),
      'updated' => t('Updated Date'),
    ),
    '#default_value' => $conf['node_date'],
  );

  return $form;
}

/**
 * Submit handler for the custom type settings form.
 */
function ns_core_node_date_custom_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function ns_core_node_date_custom_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" custom node date', array('@s' => $context->identifier));
}
