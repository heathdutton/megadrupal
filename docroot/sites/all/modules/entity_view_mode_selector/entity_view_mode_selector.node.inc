<?php
/**
 * @file
 * Node support for Entity View Mode Selector.
 */

/**
 * Implements hook_form_FORM_ID_alter() for node_type_form.
 */
function entity_view_mode_selector_form_node_type_form_alter(&$form, &$form_state, $form_id) {
  $bundle = $form['#node_type']->type;

  $form['entity_view_mode_selector'] = array(
    '#type' => 'fieldset',
    '#title' => t('View mode selector'),
    '#description' => t('The available view modes that can be selected per node.'),
    '#collapsible' => TRUE,
    '#group' => 'additional_settings',
  );
  $form['entity_view_mode_selector']['entity_view_mode_selector_node'] = array(
    '#type' => 'checkboxes',
    '#title' => t('View modes'),
    '#description' => t("If no view modes are checked, the view mode selector won't be enabled for this content type."),
    '#options' => entity_view_mode_selector_get_view_modes('node', $bundle),
    '#default_value' => variable_get('entity_view_mode_selector_node_' . $bundle, array()),
  );
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for node_form.
 */
function entity_view_mode_selector_form_node_form_alter(&$form, &$form_state, $form_id) {
  $node = $form['#node'];
  $bundle = $form['#bundle'];
  $view_modes = variable_get('entity_view_mode_selector_node_' . $bundle, array());

  if (!empty($view_modes)) {
    $enabled_view_modes = entity_view_mode_selector_get_view_modes('node', $bundle);
    $options = array();
    foreach ($view_modes as $view_mode) {
      $options[$view_mode] = $enabled_view_modes[$view_mode];
    }

    $form['entity_view_mode_selector'] = array(
      '#type' => 'fieldset',
      '#title' => t('View mode'),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#access' => user_access('select entity view modes'),
    );
    $form['entity_view_mode_selector']['view_mode'] = array(
      '#type' => 'radios',
      '#description' => t('The view mode that this node will use.'),
      '#options' => $options,
      '#default_value' => isset($node->view_mode) ? $node->view_mode : 'full',
    );
  }
}

