<?php
/**
 * @file
 * Domain Integration Views Bulk Operations Node Actions.
 */

module_load_include('inc', 'domain_integration_rules', 'domain_integration_rules.rules');

/**
 * domain_integration_vbo_modify_node_domains: Configuration form.
 */
function domain_integration_vbo_modify_node_domains_form($settings, &$form_state) {
  $form = array();

  $form['domain_node_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('New domain settings'),
    '#options' => domain_integration_rules_rules_action_domains_options_list('', 'domain_node_settings'),
    '#description' => t("New domain settings that will be applied on all selected nodes."),
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['domain_node_settings']) ? $settings['settings']['domain_node_settings'] : array(),
  );
  $form['action'] = array(
    '#type' => 'radios',
    '#title' => t('Action to perform'),
    '#options' => domain_integration_rules_rules_action_action_options_list(),
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['action']) ? $settings['settings']['action'] : 'replace',
  );

  return $form;
}

/**
 * domain_integration_vbo_modify_node_domains: Configuration form submit.
 */
function domain_integration_vbo_modify_node_domains_submit($form, $form_state) {
  $return['domain_node_settings'] = $form_state['values']['domain_node_settings'];
  $return['action'] = $form_state['values']['action'];

  // Remove non-selected values.
  foreach ($return['domain_node_settings'] as $domain => $selected) {
    if (!$selected) {
      unset($return['domain_node_settings'][$domain]);
    }
  }

  return $return;
}

/**
 * domain_integration_vbo_modify_node_domains: Action callback.
 */
function domain_integration_vbo_modify_node_domains(&$node, $context) {

  // Direct call to Rules Action callback-function.
  domain_integration_rules_rules_action_domain_node($node, $context['domain_node_settings'], $context['action']);
}
