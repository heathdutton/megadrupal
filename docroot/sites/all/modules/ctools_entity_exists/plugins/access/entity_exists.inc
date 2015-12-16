<?php

/**
 * @file
 * Plugin to provide access control based upon entity existence.
 */

$plugin = array(
  'title' => t("Entity: exists"),
  'description' => t('Control access by the entity existence. Entity exists if it has been already created.'),
  'callback' => 'ctools_entity_exists_entity_exists_access_check',
  'summary' => 'ctools_entity_exists_entity_exists_access_summary',
  'get child' => 'ctools_entity_exists_entity_exists_access_get_child',
  'get children' => 'ctools_entity_exists_entity_exists_access_get_children',
);

function ctools_entity_exists_entity_exists_access_get_child($plugin, $parent, $child) {
  $plugins = ctools_entity_exists_entity_exists_access_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function ctools_entity_exists_entity_exists_access_get_children($plugin, $parent) {
  $entities = entity_get_info();
  $plugins = array();
  foreach ($entities as $entity_type => $entity) {
    $child_plugin = $plugin;
    $child_plugin['title'] = $entity['label'] .  ': ' . t('exists');
    $child_plugin['keyword'] = $entity_type;
    $child_plugin['name'] = $parent . ':' . $entity_type;
    $child_plugin['description'] = t('Control access by the entity existence. Entity exists if it has been already created.');
    $child_plugin['required context'] = new ctools_context_required(t('Entity'), $entity_type);
    $plugins[$parent . ':' . $entity_type] = $child_plugin;
  }
  return $plugins;
}

/**
 * Check for entity existence (entity id is presented).
 */
function ctools_entity_exists_entity_exists_access_check($conf, $context, $plugin) {
  list(, $entity_type) = explode(':', $plugin['name']);
  if ((!$entity_type) || (empty($context->data))) {
    return FALSE;
  }

  $info = entity_get_info($entity_type);
  if (empty($info)) {
    return FALSE;
  }

  $entity = $context->data;

  $entity_id = isset($entity->{$info['entity keys']['id']}) ? $entity->{$info['entity keys']['id']} : NULL;

  return (!is_null($entity_id));
}

function ctools_entity_exists_entity_exists_access_summary($conf, $context) {
  return t('Returns true if entity exists.');
}
