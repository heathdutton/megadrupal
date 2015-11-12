<?php

/**
 * @file
 * Plugin to provide access control based upon party dashboard context.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Party Dashboard Context"),
  'description' => t('Control access by party dashboard context.'),
  'callback' => 'party_dashboard_party_dashboard_context_ctools_access_check',
  'settings form' => 'party_dashboard_party_dashboard_context_ctools_access_settings',
  'settings form validation' => 'ctools_term_ctools_access_settings_validate',
  'settings form submit' => 'ctools_term_ctools_access_settings_submit',
  'summary' => 'party_dashboard_party_dashboard_context_ctools_access_summary',
  'required context' => new ctools_context_required(t('Party Dashboard Context'), 'party_dashboard_context'),
);

/**
 * Settings form for the 'by term' access plugin
 */
function party_dashboard_party_dashboard_context_ctools_access_settings($form, &$form_state, $conf) {
  $form['settings']['#tree'] = TRUE;
  $form['settings']['party_dashboard_context'] = array(
    '#title' => t('Party Dashboard Context'),
    '#description' => t('Select a party dashboard context or contexts.'),
    '#default_value' => !empty($conf['party_dashboard_context']) ? $conf['party_dashboard_context'] : '',
    '#multiple' => TRUE,
    '#type' => 'checkboxes',
    '#options' => array(),
  );
  // @todo: replace with party_het_get_tree when drupal.org/node/1613400 goes in
  foreach (ctools_export_crud_load_all('party_dashboard_context') as $name => $context) {
    $form['settings']['party_dashboard_context']['#options'][$name] = $context->title;
  }

  return $form;
}

/**
 * Check for access.
 */
function party_dashboard_party_dashboard_context_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data)) {
    return FALSE;
  }

  // Get the allowed contexts.
  $party_dashboard_context = isset($conf['party_dashboard_context']) ? array_filter($conf['party_dashboard_context']): array();

  return !empty($party_dashboard_context[$context->data]);
}

/**
 * Provide a summary description based upon the checked terms.
 */
function party_dashboard_party_dashboard_context_ctools_access_summary($conf, $context) {
  $context_names = array();
  $party_dashboard_contexts = ctools_export_crud_load_all('party_dashboard_context');
  foreach (array_filter($conf['party_dashboard_context']) as $name) {
    $context_names[$name] = $party_dashboard_contexts[$name]->title;
  }

  return format_plural(count($context_names),
    '@context can be "@contexts"',
    '@context can be one of: @contexts',
    array('@contexts' => implode(', ', $context_names),
      '@context' => $context->identifier));
}
