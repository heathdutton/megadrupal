<?php

/**
 * @file
 * API documentation for the Translation Field Diffuser module.
 */

/**
 * Allows modules to propagate entity field's data.
 *
 * This hook is call on entity update to allow propagation of field's data
 * from an entity source to its others languages.
 *
 * @param object $entity
 *   The entity object.
 * @param string $type
 *   The type of entity.
 *
 * @see hook_entity_update()
 */
function hook_data_propagation($entity, $type) {
}
