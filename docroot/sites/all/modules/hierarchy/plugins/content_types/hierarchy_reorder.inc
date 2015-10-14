<?php

$plugin = array(
  'title' => t('Hierarchy reorder tree'),
  'description' => t('Hierarchy reorder tree.'),
  'render callback' => 'hierarchy_hierarchy_reorder_content_type_render',
  'edit form' => 'hierarchy_hierarchy_reorder_content_type_edit_form',
  'required context' => new ctools_context_required(t('Hierarchy'), 'entity:hierarchy'),
  'category' => t('Miscellaneous'),
  'defaults' => array(),
  'single' => TRUE,
);

/**
 * Render callback for the content type.
 */
function hierarchy_hierarchy_reorder_content_type_render($subtype, $conf, $args, $contexts) {
  if (empty($contexts) || empty($contexts->data)) {
    return NULL;
  }

  $hierarchy_tree = drupal_get_form('hierarchy_reorder_items_form', $contexts->data, NULL);

  $block = new stdClass();
  $block->title = !empty($conf['override_title']) ? check_plain($conf['override_title_text']) : '';
  $block->content = $hierarchy_tree;

  return $block;
}

/**
 * Edit form callback for the content type.
 */
function hierarchy_hierarchy_reorder_content_type_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Submit callback for the example_text_edit_form().
 */
function hierarchy_hierarchy_reorder_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
