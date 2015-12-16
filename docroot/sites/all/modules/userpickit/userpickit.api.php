<?php

/**
 * @file
 * API documentation for User Pic Kit.
 */

/**
 * Define picture types.
 *
 * @return Array
 *   Array of picture types.
 *
 * @see userpickit_picture_type_info()
 */
function hook_userpickit_info() {
  $picture_types = array();

  // If a picture type is missing any (required) keys, it will be ignored.
  $picture_types['picture_type_1'] = array( // machine name
    'title' => t('My picture type'),
    // (required)
    'description' => t('A description of my picture type.'),
    // (optional) Description show in administration interface.
    'callback' => 'my_userpickit_picture',
    // (required) callback arguments: ($picture_type, $account)
    'default callback' => 'my_picture_type_default',
    // (optional) Path to default image if hook_userpickit_picture() does
    // not return a picture.
    // callback arguments: ($picture_type, $account = NULL)
    'user cache invalidate' => FALSE,
    // (optional) Defaults to FALSE Allow users to invalidate his current
    // cache for a picture type.
    // Set to TRUE if an action outside of the site will change the result
    // of the downloaded picture.
  );

  return $picture_types;
}

/**
 * Get the picture for a user.
 *
 * Hook behaviour:
 *
 * ## Cache
 *
 * If there is no cache, or cache has expired:
 *
 *   If 'fid' is defined, else skip:
 *     And file exists, file will be used as picture.
 *     File does not exist, see 'uri'.
 *   If 'uri' is defined, else skip:
 *     And uri is valid,
 *     And was successfully downloaded,
 *     And is an image, file will be used as picture.
 *
 *   At this point, the above state is saved to a cache.
 *     'fid', and or 'uri' values may be NULL.
 *
 * ## Render
 *
 *   If 'fid' is defined, else skip:
 *     File will be used as picture.
 *   If 'uri' is defined, else skip:
 *     URI will be used as picture.
 *   Else, see Defaults
 *
 * ## Defaults
 *
 * If the above conditions cannot be met.
 *
 *   Check if 'default callback' for $picture_type exists.
 *     Execute callback and check if result is a valid uri.
 *     Use result as 'uri'
 *     This uri is not cached locally.
 *
 * If none of 'fid', 'uri' or the 'default callback' resolve, then the site
 * default picture will be displayed. The picture type will be marked as
 * inactive on the picture configuration UI. It would be useful to provide a
 * 'message' to the user if a picture cannot be generated.
 *
 * @param object $picture_type
 *   Picture type.
 * @param object $account
 *   User account. An anonymous account may be passed. Do not expect a fully
 *   loaded account object.
 * @param array $options
 *   KEEP REMOTE or CACHE LOCALLY?
 *
 * @return array|NULL
 *   If null is returned, the 'default callback' for this picture type will be
 *   called. If undefined, then the system default image will be rendered.
 */
function my_userpickit_picture($picture_type, $account) {
  if (!empty($account->uid)) {
    // Registered user.
  }
  else {
    // Anonymous user.
  }

  // return 'fid' OR 'uri'. If one is not defined, default callback will be
  // called.
  return array(
    'fid' => 0,
    // File ID. Fill this value if the file is already stored locally.
    'uri' => 'http://',
    // Full uri to image, http://, public:// etc.
    'cache lifetime',
    // (optional) It is preferred to omit this and default to
    // admin setting. Allowed values:
    // - (int): Seconds to wait before automatically running this hook again.
    // - USERPICKIT_CACHE_EXPIRE_NEVER: never expire.
    // - USERPICKIT_CACHE_DISABLE: never cache.
    // Forced GC may cause the cache to expire by force.
    'message' => 'Your user ID is @uid',
    // (optional) Personalized message for the user displayed on user
    // picture type ui. If possible, do not translate. Use variable
    // placeholders as in format_string().
    'message variables' => array('@uid' => $account->uid),
    // (optional) Translatable variables for message.
    //   Usage: t('message', 'message variables')
  );

  return FALSE;
}

/**
 * Implement default image callback.
 *
 * @param object $picture_type
 *   Picture type.
 * @param object $account
 *   User object.
 *
 * @see hook_userpickit_info().
 */
function my_picture_type_default($picture_type, $account = NULL) {
  return 'http://www.example.com/test.jpg';
}

/**
 * Executed when a cache is created.
 *
 * @see userpickit_cache_set()
 */
function hook_userpickit_cache_insert($cache) {
  if ($cache->type == 'picture_type_1') {
    if ($cache->fid) {
      // File is local
    }
  }
}

/**
 * Executed when a cache is updated.
 *
 * @see userpickit_cache_set()
 */
function hook_userpickit_cache_update($cache) {
  if ($cache->type == 'picture_type_1') {
    if (!$cache->fid && $cache->uri) {
      // File was not cached locally.
    }
  }
}

/**
 * Modify a cache before it is saved.
 *
 * @param array $cache
 *   Cache to modify.
 */
function hook_userpickit_cache_alter($cache) {
  
}

/**
 * Modify all picture type info before it is cached.
 *
 * @param array $picture_types
 *   Reference to all defined picture types.
 */
function hook_userpickit_picture_type_info_alter(&$picture_types) {
  $i = 0;
  foreach ($picture_types as &$picture_type) {
    // Add number to title of each picture type.
    $picture_type['title'] .= ' ' . $i;
  }
}