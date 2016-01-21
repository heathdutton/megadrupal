<?php
/**
 * @file
 * Node module integration.
 */

/**
 * Implements hook_setup_info().
 */
function node_setup_info() {
  return array(
    'node_edit' => array(
      'form callback' => 'node_setup_node_edit_form_callback',
      'finish callback' => 'node_setup_node_edit_finish_callback',
    ),
    'node_view' => array(
      'form callback' => 'node_setup_node_view_form_callback',
      'finish callback' => 'node_setup_node_view_finish_callback',
    ),
  );
}

/**
 * Form callback for 'Node edit' method.
 */
function node_setup_node_edit_form_callback($form, &$form_state) {
  if (isset($form_state['setup_step']['nid'])) {
    $node = node_load($form_state['setup_step']['nid']);

    // Get node form.
    module_load_include('inc', 'node', 'node.pages');
    $node_form = drupal_get_form($node->type . '_node_form', $node);
    unset($node_form['actions']);

    // @TODO - Do a recursive merge so we don't blow away anything important.
    $form = array_merge($node_form, $form);
  }

  return $form;
}

/**
 * Finish callback for 'Node edit' method.
 */
function node_setup_node_edit_finish_callback(&$form_state) {
  $node = (object) $form_state['values'];
  node_save($node);
}

/**
 * Form callback for 'Node view' method.
 */
function node_setup_node_view_form_callback($form, &$form_state) {
  if (isset($form_state['setup_step']['nid'])) {
    $node = node_load($form_state['setup_step']['nid']);
    $view_mode = isset($form_state['setup_step']['view_mode']) ? $form_state['setup_step']['view_mode'] : 'full';
    $node_view = node_view($node, $view_mode);

    $form['node'] = array(
      '#markup' => render($node_view),
    );
  }

  return $form;
}

/**
 * Finish callback for 'Node view' method.
 */
function node_setup_node_view_finish_callback(&$form_state) {

}
