<?php

/**
 * @file
 * Rules integration for Image Replace module.
 */

/**
 * Implements hook_rules_action_info().
 */
function image_replace_rules_action_info() {
  return array(
    'image_replace_rebuild_mapping_action' => array(
      'label' => t('Rebuild image replace mapping'),
      'parameter' => array(
        'entity' => array(
          'type' => 'entity',
          'label' => t('Entity'),
        ),
      ),
      'group' => t('Image Replace'),
    ),
  );
}

/**
 * Rules action callback: Rebuild image replacement mapping for a given entity.
 *
 * @param EntityMetadataWrapper $wrapper
 *   The entity for which the mapping should be rebuilt.
 */
function image_replace_rebuild_mapping_action(EntityMetadataWrapper $wrapper) {
  image_replace_field_attach_presave($wrapper->type(), $wrapper->value());
}
