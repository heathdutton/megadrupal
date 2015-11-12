<?php
/**
 * @file
 * Content add form plugin for entity collections.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Add content form'),
  'description' => t('The author of the referenced node.'),
  'required context' => new ctools_context_required(t('Entity collection'), 'entity:entity_collection'),
  'category' => t('Entity collection'),
  'defaults' => array(),
);

/**
 * Render the content add form.
 */
function entity_collection_content_add_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }
  $entity_collection = $context->data;
  module_load_include('admin.inc', 'entity_collection');
  // Build the content type block.
  $block = new stdClass();
  $block->module  = 'entity_collection_content_add_form';
  $block->content = drupal_get_form('entity_collection_content_add_form', $entity_collection);
  $block->delta   = $entity_collection->name;

  return $block;
}

/**
 * Settings form for content add.
 */
function entity_collection_content_add_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * Submit handler for the entity collection add settings form.
 */
function entity_collection_content_add_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function entity_collection_content_add_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" add form', array('@s' => $context->identifier));
}
