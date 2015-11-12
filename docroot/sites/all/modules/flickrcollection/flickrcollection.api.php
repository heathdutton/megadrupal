<?php

/**
 * @file
 * Hooks provided by the FlickrCollection module.
 */


/**
 * @addtogroup hooks
 * @{
 */


/**
 * Inform about the sets we got from the collection.
 *
 * @param array $sets
 *   An array of set ID's.
 */
function hook_flickrcollection_sets($sets) {

}


/**
 * Alter the collections return from FlickrAPI.
 *
 * @param object $collection
 *   The collection object returned from FlickrAPI.
 */
function hook_flickrcollection_collection_alter($collection) {

}


/**
 * @} End of "addtogroup hooks".
 */
