<?php

/**
 * @file
 * Contains Rules integration for Stanbol Enhancer Rules module.
 */

/**
 * Implements hook_rules_event_info().
 */
function stanbol_enhancer_rules_rules_event_info() {
  $info = [];

  $info['stanbol_enhancer_entity_successfully_enhanced'] = [
    'group' => t('Stanbol Enhancer Rules'),
    'label' => t('Entity successfully enhanced'),
    'help' => t('Triggered when the Stanbol Enhancer successfully enhanced an entity.'),
    'variables' => [
      'entity' => [
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The enhanced entity.'),
      ],
    ],
  ];

  $info['stanbol_enhancer_entity_enhancement_failed'] = [
    'group' => t('Stanbol Enhancer Rules'),
    'label' => t('Entity enhancement failed'),
    'help' => t('Triggered when the Stanbol Enhancer failed on the entity enhancement.'),
    'variables' => [
      'entity' => [
        'type' => 'entity',
        'label' => t('Entity'),
        'description' => t('The enhanced entity.'),
      ],
    ],
  ];

  return $info;
}
