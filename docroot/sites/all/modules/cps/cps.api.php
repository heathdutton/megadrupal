<?php

/**
 * @file
 * Hook documentation for CPS.
 */

/**
 * Modify the live 'initial' revision prior to saving.
 */
function hook_cps_live_revision_create($entity_type, $entity) {
}

/**
 * Determine if a particular entity should be tracked by CPS.
 *
 * Entities are tracked by default whenever the entity type supports CPS, as
 * determined by cps_is_supported(). To block CPS from tracking an entity of
 * one of those types that has not yet been tracked, return FALSE from this
 * hook. Any other return value will be ignored.
 *
 * Only use this hook in cases where you want to completely block certain
 * entities from ever being tracked by CPS, or where an entity is initially
 * created in a custom "not yet live" state and you want to make sure it is not
 * tracked by CPS until it is actually live (for example, file entities start
 * off as temporary files on initial upload and don't truly become "live" until
 * the file is saved permanently; therefore, the cps_file_entity module does
 * not allow them to be tracked until that happens). Using this hook to delay
 * CPS tracking for other reasons may be dangerous because when an entity is
 * tracked by CPS for the first time, the live version will be automatically
 * unpublished, and if it was previously visible to site visitors it no longer
 * will be.
 */
function hook_cps_can_entity_be_tracked($entity_type, $entity) {
}
