<?php

/**
 * @file
 * Implementation of 'Weight' property behavior plugin for ECK Entities.
 */

// Plugin definition.
$plugin = array(
  'label' => t('Weight'),
  'entity_save' => 'eck_weight_property_entity_save',
  'default_formatter' => 'eck_weight_property_formatter',
);

/**
 * Implements 'default_formatter' plugin.
 */
function eck_weight_property_formatter($property, $vars) {
  $value = NULL;
  if (isset($vars['entity']->{$property})) {
    $value = $vars['entity']->{$property};
  }
  return array(
    '#prefix' => '<div class="property-label field-label">$property',
    '#suffix' => '</div>',
    'content' => array(
      '#prefix' => '<div class="property-content">',
      '#markup' => check_plain($value),
      '#suffix' => '</div>',
    ),
  );
}

/**
 * Set default value when the entity is first saved.
 */
function eck_weight_property_entity_save($property, $vars) {
  $entity = $vars['entity'];
  if (isset($entity->is_new) && $entity->is_new && empty($entity->{$property})) {
    $entity->{$property} = 0;
  }
}
