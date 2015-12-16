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
  'title' => t('Entity Collection: Row'),
  'description' => t('A referenced node from the entity collection or a sub-collection in context.'),
  'required context' => new ctools_context_required(t('Entity Collection: item'), 'entity_collection_item'),
  'category' => t('Entity collection'),
  'defaults' => array(
    'delta' => 0,
    'row_settings' => 0,
  ),
);

/**
 * Render the content add form.
 */
function entity_collection_entity_collection_item_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }
  $tree = $context->data;
  $entity_collection = entity_collection_load($tree->entity_id);
  $item = array_shift($tree->getFlat($conf['delta'], 1));
  if (isset($item) && isset($conf['row_settings']['style'])) {
    $item->style = $conf['row_settings']['style'];
  }
  // Build the content type block.
  $content = new stdClass();
  $content->module  = 'entity_collection_entity_collection_subcollection';
  // Load the style.
  ctools_include('plugins');
  $plugin = ctools_get_plugins('entity_collection', 'plugins', 'entity_collection_row_panels_mini');
  if (isset($plugin['entity_collection_row']['class'])) {
    $row = new $plugin['entity_collection_row']['class'](NULL, array());
  }
  // Build the content with the style.
  if (isset($row, $item) && method_exists($row, 'build')) {
    $content->content = $row->build($entity_collection, $item, NULL);
  }

  return $content;
}

/**
 * Settings form for content add.
 */
function entity_collection_entity_collection_item_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['delta'] = array(
    '#type' => 'textfield',
    '#title' => t('Offset'),
    '#description' => t('The number of items to skip. For example, if this field is 3, the first 3 items will be skipped and not displayed.'),
    '#default_value' => $conf['delta'],
  );
  // Load the style.
  ctools_include('plugins');
  $plugin = ctools_get_plugins('entity_collection', 'plugins', 'entity_collection_row_panels_mini');
  if (isset($plugin)) {
    $row = new $plugin['entity_collection_row']['class'](entity_collection_load('front'), array());
  }
  $form['settings']['row_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Row settings'),
    '#tree' => TRUE,
  );
  if (isset($row) && method_exists($row, 'getAllOptions')) {
    $form['settings']['row_settings']['style'] = array(
      '#type' => 'radios',
      '#title' => t('Choose panel'),
      '#options' => $row->getAllOptions(),
      '#default_value' => isset($conf['row_settings']['style']) ? $conf['row_settings']['style'] : 1,
    );
  }

  return $form;
}

/**
 * Submit handler for the entity collection add settings form.
 */
function entity_collection_entity_collection_item_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function entity_collection_entity_collection_item_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" row#@d', array('@s' => $context->identifier, '@d' => $conf['delta']));
}
