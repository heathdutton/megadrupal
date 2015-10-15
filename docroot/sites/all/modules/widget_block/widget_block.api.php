<?php

/**
 * @file
 * Defines the available hooks for the Widget Block module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Purge all related resource which use the Widget Block with given delta.
 *
 * The id of a Widget is the same as the delta of the Widget Block and is unique
 * for all Widget Blocks.
 *
 * @param array $ids
 *   A list of widget identifiers.
 * @param array $languages
 *   An array of language codes for which purging needs to be performed.
 */
function hook_widget_block_purge(array $ids, array $languages) {
  // Logic to purge resource here.
}

/**
 * @} End of "addtogroup hooks"
 */
