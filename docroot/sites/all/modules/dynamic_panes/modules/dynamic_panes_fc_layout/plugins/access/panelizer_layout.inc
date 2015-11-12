<?php

/**
 * @file
 * Plugin to provide access control based upon node type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Panelizer"),
  'description' => t('Control access by Panelizer.'),
  'callback' => 'dynamic_panes_fc_layout_panelizer_access_check',
  'default' => array('layout' => array()),
  'settings form' => 'dynamic_panes_fc_layout_panelizer_access_settings',
  'settings form submit' => 'dynamic_panes_fc_layout_panelizer_access_settings_submit',
  'summary' => 'dynamic_panes_fc_layout_panelizer_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Settings form for the 'Panelizer' access plugin.
 */
function dynamic_panes_fc_layout_panelizer_access_settings($form, &$form_state, $conf) {
  $options = array();
  $panelizers = dynamic_panes_fc_layout_get_panelizers();
  foreach ($panelizers as $key => $panelizer) {
    $options[$key] = check_plain("$panelizer->panelizer_key: $panelizer->title");
  }

  $form['settings']['panelizer'] = array(
    '#title' => t('Panelizer'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => t('Only the checked panelizers will be valid.'),
    '#default_value' => $conf['panelizer'],
  );
  return $form;
}

/**
 * Compress the node_types allowed to the minimum.
 */
function dynamic_panes_fc_layout_panelizer_access_settings_submit($form, &$form_state) {
  $form_state['values']['settings']['panelizer'] = array_filter($form_state['values']['settings']['panelizer']);
}

/**
 * Check for access.
 */
function dynamic_panes_fc_layout_panelizer_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->type)) {
    return FALSE;
  }

  if (empty($context->data->panelizer)) {
    return FALSE;
  }

  return in_array($context->data->panelizer['page_manager']->name, $conf['panelizer']);
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function dynamic_panes_fc_layout_panelizer_access_summary($conf, $context) {
  if (!isset($conf['panelizer'])) {
    $conf['panelizer'] = array();
  }
  $panelizers = dynamic_panes_fc_layout_get_panelizers();

  $names = array();
  // If a panelizer doesn't exist, let the user know, but prevent a notice.
  $missing_types = array();

  foreach (array_filter($conf['panelizer']) as $panelizer) {
    if (!empty($panelizers[$panelizer])) {
      $names[] = check_plain($panelizers[$panelizer]->name);
    }
    else {
      $missing_types[] = check_plain($panelizer);
    }
  }

  if (empty($names) && empty($missing_types)) {
    return t('@identifier is any panelizer', array('@identifier' => $context->identifier));
  }

  if (!empty($missing_types)) {
    $output = array();
    if (!empty($names)) {
      $output[] = format_plural(count($names), '@identifier is "@types"', '@identifier type is one of "@types"', array('@types' => implode(', ', $names), '@identifier' => $context->identifier));
    }
    $output[] = format_plural(count($missing_types), 'Missing/ deleted panelizer "@types"', 'Missing/ deleted panelizer is one of "@types"', array('@types' => implode(', ', $missing_types)));
    return implode(' | ', $output);
  }

  return format_plural(count($names), '@identifier is "@types"', '@identifier is one of "@types"', array('@types' => implode(', ', $names), '@identifier' => $context->identifier));
}
