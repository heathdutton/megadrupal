<?php

/**
 * @file
 * Hooks provided by Storage Audit.
 */

/**
 * @addtogroup hooks
 */

/**
 * Determine if extra files in this container should be imported.
 *
 * @param \StorageContainer $container
 *   The container being audited.
 *
 * @return string
 *   ID of selector extra files should become owned by.
 */
function hook_storage_audit_import_container(StorageContainer $container) {
}

/**
 * Alert module to a file having been imported.
 *
 * @param \Storage $storage
 *   Storage of the imported file.
 */
function hook_storage_audit_import(Storage $storage) {
}
