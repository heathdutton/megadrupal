<?php
/**
 * @file
 * Plugin to provide an relationship handler for a entity collection.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Entity collection'),
  'description' => t('Creates an entity collection context from available contexts.'),
  'context' => 'entity_collection_entity_collection_context',
  'get child' => 'entity_collection_entity_collection_get_child',
  'get children' => 'entity_collection_entity_collection_get_children',
);

/**
 * Get a particular child of this plugin type.
 */
function entity_collection_entity_collection_get_child($plugin, $parent, $child) {
  $entity_collection = entity_collection_load($child);
  if (!$entity_collection) {
    return;
  }
  return _entity_collection_context_info($entity_collection);
}

/**
 * Get an ctools info aray for a particular entity collection.
 */
function _entity_collection_context_info($entity_collection) {
  $info = array(
    'name' => 'entity_collection:' . $entity_collection->name,
    'title' => t('entity collection: !title', array('!title' => $entity_collection->title)),
    'description' => t('Use parts from the entity collection form.'),
    'keyword' => 'entity_collection',
    'context' => 'entity_collection_entity_collection_context',
    'context name' => 'entity:entity_collection',
    'entity collection' => $entity_collection->name,
  );
  if (!empty($entity_collection->requiredcontexts)) {
    $info['required context'] = array();
    foreach ($entity_collection->requiredcontexts as $context) {
      $context_info = ctools_get_context($context['name']);
      $info['required context'][] = new ctools_context_required($context['identifier'], $context_info['context name']);
    }
  }
  return $info;
}

/**
 * Get all children for this plugin.
 */
function entity_collection_entity_collection_get_children($plugin, $parent) {
  $types = array();
  $collections = entity_collection_load_all();
  foreach ($collections as $entity_collection) {
    $info = _entity_collection_context_info($entity_collection);
    $types[$info['name']] = $info;
  }
  return $types;
}

/**
 * Return a new context based on an existing context.
 */
function entity_collection_entity_collection_context($contexts, $conf) {
  list(, $name) = explode(':', $conf['name']);
  $entity_collection = entity_collection_load($name);
  if (count($entity_collection->requiredcontexts)) {
    $entity_collection->contexts = ctools_context_match_required_contexts($entity_collection->requiredcontexts, $contexts);
  }
  return ctools_context_create('entity:entity_collection', $entity_collection);
}
