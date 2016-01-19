<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity -rules.
 */

/**
 * Implements hook_rules_condition_info().
 */
function fc_rules_condition_info() {
  return array(
    'fc_state' => array(
      'label' => t('Entity has %age Field Completeness'),
      'base' => 'rules_condition_fc_state',
      'parameter' => array(
        'entity' => array('type' => 'entity', 'label' => t('Entity')),
        'percentage' => array(
          'type' => 'integer',
          'label' => t('At least'),
          'description' => t('Example: 100, so this condition will pass if the entity is 100% field complete.'),
          'field_suffix' => '%',
        ),
      ),
      'group' => t('Entities'),
    ),
    'fc_complete' => array(
      'label' => t('Entity is Field Complete'),
      'base' => 'rules_condition_fc_complete',
      'parameter' => array(
        'entity' => array('type' => 'entity', 'label' => t('Entity')),
      ),
      'group' => t('Entities'),
    ),
  );
}

function rules_condition_fc_state(EntityDrupalWrapper $wrapper, $percentage) {
  // Fetch the raw entity
  $entity = $wrapper->value();

  // Fetch/re-calculate the entity's completeness if necessary
  if (empty($entity->fc)) {
    $entity->fc = fcComplete::load($wrapper->type(), $entity);
  }

  return $entity->fc->percentage >= $percentage;
}

function rules_condition_fc_complete(EntityDrupalWrapper $wrapper) {
  // Fetch the raw entity
  $entity = $wrapper->value();

  // Fetch/re-calculate the entity's completeness if necessary
  if (empty($entity->fc)) {
    $entity->fc = fcComplete::load($wrapper->type(), $entity);
  }

  return (bool) $entity->fc->complete;
}
