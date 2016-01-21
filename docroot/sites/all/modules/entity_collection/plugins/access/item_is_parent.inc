<?php

/**
 * @file
 * Plugin to provide access control based on the item's position in the tree.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Entity collection item: parent"),
  'description' => t('Check if the item is a parent item.'),
  'callback' => 'entity_collection_item_parent_ctools_access_check',
  'default' => array(),
  'summary' => 'entity_collection_item_parent_ctools_access_summary',
  'settings form' => 'entity_collection_item_parent_ctools_access',
  'required context' => new ctools_context_required(t('Entity Collection: Item'), 'entity_collection_item'),
);

/**
 * Settings form.
 */
function entity_collection_item_parent_ctools_access_settings($form, &$form_state, $conf) {
  // No additional configuration necessary.
  return $form;
}


/**
 * Check for access.
 */
function entity_collection_item_parent_ctools_access_check($conf, $context) {
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }
  return !isset($context->data->parent->key);
}

/**
 * Provide a summary description.
 */
function entity_collection_item_parent_ctools_access_summary($conf, $context) {
  return t('Item is parent');
}

