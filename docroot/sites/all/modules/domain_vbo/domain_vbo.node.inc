<?php
/**
 * @file
 * Contains functions for Domain VBO User action.
 */

/**
 * domain_vbo_modify_node_domains: Configuration form.
 */
function domain_vbo_modify_node_domains_form($settings, &$form_state) {
  $form = array();

  $form['action'] = array(
    '#type' => 'radios',
    '#title' => t('Action'),
    '#options' => array(
      'replace' => t("Replace with"),
      'add' => t("Add"),
      'remove' => t("Remove"),
    ),
    '#description' => t("Choose the action to perform on each node"),
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['action']) ? $settings['settings']['action'] : 'replace',
  );
  $form['domains'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modify domains for node'),
    '#options' => domain_vbo_domains(),
    '#description' => t("Choose domains to process"),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * domain_vbo_modify_node_domains: Configuration form submit.
 */
function domain_vbo_modify_node_domains_submit($form, $form_state) {
  $return = array();
  foreach ($form_state['values']['domains'] as $k => $v) {
    if ($v) {
      $return['domains'][$k] = $v;
    }
  }

  $return['action'] = $form_state['values']['action'];
  return $return;
}

/**
 * domain_vbo_modify_node_domains: Action callback.
 */
function domain_vbo_modify_node_domains(&$node, $context) {
  $action = $context['action'];
  $submitted_domains = $context['domains'];

  // This function is called for each entity (node).
  switch ($action) {
    case 'replace':
      $node->domains = $submitted_domains;
      break;

    case 'add':
      // Get current node's domains and merge with submitted.
      $current_domains = $node->domains;

      // Just add the arrays with domains together.
      $combined_domains = $current_domains + $submitted_domains;

      $node->domains = array_unique($combined_domains);
      break;

    case 'remove':
      // Get current node's domains and substract submitted.
      $current_domains = $node->domains;

      // Remove submitted domains from the current domains array.
      $subtracted_domains = array_diff_assoc($current_domains, $submitted_domains);
      $node->domains = array_unique($subtracted_domains);
      break;
  }

  // Save new settings.
  domain_node_access_records($node);
  // Explicit save to force all node access hooks to be called.
  node_save($node);
}
