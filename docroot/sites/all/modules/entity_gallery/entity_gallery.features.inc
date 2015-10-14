<?php

/**
 * @file
 * Features integration for entity galleries.
 */

/**
 * Implements hook_features_api().
 */
function entity_gallery_features_api() {
  return array(
    'entity_gallery' => array(
      'name' => t('Gallery types'),
      'feature_source' => TRUE,
      'default_hook' => 'entity_gallery_info',
      'alter_type' => FEATURES_ALTER_TYPE_INLINE,
    ),
  );
}
