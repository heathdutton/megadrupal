<?php

function entity_hash_get_supported_entity_info() {
  $info = array();
  $info['user'] = array(
    'base table' => 'users',
    'entity keys' => array(
      'hash' => 'hash',
    ),
  );
  $info['node'] = array(
    'base table' => 'node',
    'revision table' => 'node_revision',
    'entity keys' => array(
      'hash' => 'hash',
    ),
  );
  if (module_exists('comment')) {
    $info['comment'] = array(
      'base table' => 'comment',
      'entity keys' => array(
        'hash' => 'hash',
      ),
    );
  }
  if (module_exists('file')) {
    $info['file'] = array(
      'base table' => 'file_managed',
      'entity keys' => array(
        'hash' => 'hash',
      ),
    );
  }
  if (module_exists('taxonomy')) {
    $info['taxonomy_term'] = array(
      'base table' => 'taxonomy_term_data',
      'entity keys' => array(
        'hash' => 'hash',
      ),
    );
  }
  if (module_exists('field_collection')) {
    $info['field_collection_item'] = array(
      'base table' => 'field_collection_item',
      'entity keys' => array(
        'hash' => 'hash',
      ),
    );
  }

  //hook to add supported entities

  drupal_alter("entity_hash_supported_entity", $info);

  return $info;
}

function entity_hash_sync_all() {
  module_invoke_all('entity_hash_sync');
}

/**
 * Implements of hook_entity_info_alter().
 *
 * @see entity_hash_get_supported_entity_info().
 */
function entity_hash_entity_info_alter(&$info) {
  foreach (entity_hash_get_supported_entity_info() as $entity_type => $supported_info) {
    $info[$entity_type]['hash'] = TRUE;
    $info[$entity_type]['entity keys']['hash'] = $supported_info['entity keys']['hash'];
    if (!empty($supported_info['entity keys']['revision hash'])) {
      $info[$entity_type]['entity keys']['revision hash'] = $supported_info['entity keys']['revision hash'];
    }
  }
}

/**
 * Implements of hook_entity_property_info_alter().
 *
 * This adds the Hash as an entity property for all Entity Hash enabled entities
 * which automatically gives us token and Rules integration.
 */
function entity_hash_entity_property_info_alter(&$info) {
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (isset($entity_info['hash']) && $entity_info['hash'] == TRUE && !empty($entity_info['entity keys']['hash'])) {
      $info[$entity_type]['properties'][$entity_info['entity keys']['hash']] = array(
        'label' => t('Hash'),
        'type' => 'text',
        'description' => t("The entity's hash."),
        'schema field' => $entity_info['entity keys']['hash'],
      );
      if (!empty($entity_info['entity keys']['revision hash'])) {
        $info[$entity_type]['properties'][$entity_info['entity keys']['revision hash']] = array(
          'label' => t('Revision Hash'),
          'type' => 'text',
          'description' => t("The revision's hash."),
          'schema field' => $entity_info['entity keys']['revision hash'],
        );
      }
    }
  }
}