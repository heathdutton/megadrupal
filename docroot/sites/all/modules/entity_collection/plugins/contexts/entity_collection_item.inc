<?php
/**
 * @file
 * Plugin to provide a context describing the current item in the list.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Entity Collection: item"),
  'description' => t('Information about an item in an entity collection.'),
  'context' => 'entity_collection_item_context_create',
  'keyword' => 'entity_collection_item',
  'context name' => 'entity_collection_item',
  'convert list' => array(
    'position' => t('Position in the collection'),
    'parent' => t('The parent item, if any'),
    'entity_type' => t('The entity type'),
    'entity_id' => t('The entity ID'),
  ),
  'convert' => 'entity_collection_item_context_convert',
);

/**
 * Get the context.
 */
function entity_collection_item_context_create($empty, $data = NULL, $conf = FALSE, $plugin = array()) {
  $context = new ctools_context('entity_collection_item');
  $context->plugin = 'entity_collection_item';
  if ($empty) {
    return $context;
  }
  if (is_object($data)) {
    $context->data = $data;
    $context->title = $data->key;
    $context->argument = $data->key;
    return $context;
  }
}

/**
 * Convert a context into a string.
 */
function entity_collection_item_context_convert($context, $type) {
  switch ($type) {
    case 'position':
      return $context->data->position;
    case 'parent':
      return $context->data->parent->key;
    case 'entity_type':
      return $context->data->entity_type;
    case 'entity_id':
      return $context->data->entity_id;
  }
}
