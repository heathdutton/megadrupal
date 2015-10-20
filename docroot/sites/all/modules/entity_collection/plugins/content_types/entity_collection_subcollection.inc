<?php
/**
 * @file
 * Content add form plugin for entity collections.
 */
/**

/**
 * Plugins are described by creating a $plugin array which will be used
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Entity Collection: Sub-collection'),
  'description' => t('A sub-collection of the entity collection in context.'),
  'required context' => new ctools_context_required(t('Entity Collection: item'), 'entity_collection_item'),
  'category' => t('Entity collection'),
  'defaults' => array(
    'style_settings' => array(),
  ),
);

/**
 * Render the content add form.
 */
function entity_collection_entity_collection_subcollection_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }
  $tree = $context->data;
  $entity = entity_collection_load($tree->entity_id);

  // Build the content type block.
  $content = new stdClass();
  $content->module  = 'entity_collection_entity_collection_subcollection';
  // Load the style.
  ctools_include('plugins');
  $plugin = ctools_get_plugins('entity_collection', 'plugins', 'entity_collection_style_limit');
  if (isset($plugin['entity_collection_style']['class'])) {
    $style = new $plugin['entity_collection_style']['class'](NULL, array());
  }
  // Build the content with the style.
  if (isset($style) && method_exists($style, 'build')) {
    $content->content = $style->build($entity, $tree, NULL, $conf['style_settings']);
  }

  return $content;
}

/**
 * Settings form for content add.
 */
function entity_collection_entity_collection_subcollection_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  // Load the style.
  ctools_include('plugins');
  $plugin = ctools_get_plugins('entity_collection', 'plugins', 'entity_collection_style_limit');
  if (isset($plugin['entity_collection_style']['class'])) {
    $style = new $plugin['entity_collection_style']['class'](NULL, array());
  }
  $form['settings']['style_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Style settings'),
    '#tree' => TRUE,
  );
  if (isset($style) && method_exists($style, 'settingsForm')) {
    $style->settingsForm($form['settings']['style_settings'], $conf['style_settings']);
  }

  return $form;
}

/**
 * Submit handler for the entity collection add settings form.
 */
function entity_collection_entity_collection_subcollection_content_type_edit_form_submit($form, &$form_state) {
  // Load the style.
  ctools_include('plugins');
  $plugin = ctools_get_plugins('entity_collection', 'plugins', 'entity_collection_style_limit');
  if (isset($plugin['entity_collection_style']['class'])) {
    $style = new $plugin['entity_collection_style']['class'](NULL, array());
    $style->formSettingsSubmit($form_state['values']['style_settings']);
  }
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Returns the administrative title for a type.
 */
function entity_collection_entity_collection_subcollection_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" sub-collection', array('@s' => $context->identifier));
}
