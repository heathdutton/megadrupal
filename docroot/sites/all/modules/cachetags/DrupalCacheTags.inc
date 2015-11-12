<?php

/**
 * Interface for cache tag implementations.
 *
 * Cache tags and their corresponding invalidation counters are stored
 * independently from the cache itself. Cache tag storage is stored on a
 * site-wide level (as opposed to per-bin) so only one cache tag storage engine
 * can be used at one time.
 *
 * To make Drupal use your cache tags implementation, you must set a variable
 * (either with variable_set() or in settings.php via $conf):
 * @code
 *  variable_set('cache_tags_class', 'MyCustomCacheTags');
 * @endcode
 *
 * @see cache_tags()
 * @see DrupalDatabaseCacheTags
 */
interface DrupalCacheTagsInterface {
  /**
   * Invalidate each tag in the $tags array.
   *
   * Cache entries tagged with any one of these will subsequently return
   * FALSE for cache_get().
   *
   * @param $tags
   *   Associative array of tags, in the same format that is passed to cache_set().
   *
   * @see cache_set()
   */
  function invalidate($tags);

  /**
   * Calculate the sum of invalidations of the given tags.
   *
   * @param $tags
   *   Associative array of tags to calculate checksum for.
   *
   * @return
   *   Integer representing the sum of invalidations of the given tags.
   */
  function checksum($tags);

  /**
   * Determine if the checksum is current and valid.
   *
   * @param $checksum
   *   A checksum to test against.
   * @param $tags
   *   Associative array of tags, in the same format that is passed to cache_set().
   *
   * @return
   *   Boolean TRUE if $checksum is current for the given tags, FALSE otherwise.
   */
  function isValid($checksum, $tags);
}

/**
 * Base abstract class for cache tags implementations.
 *
 * Provides default implementations and helper methods.
 */
abstract class DrupalCacheTags implements DrupalCacheTagsInterface {
  function isValid($checksum, $tags) {
    return $checksum == $this->checksum($tags);
  }

  /**
   * Flatten a tags array into a numeric array suitable for string storage.
   *
   * @param $tags
   *   Associative array of tags to flatten.
   *
   * @return
   *   Numeric array of flattened tag identifiers.
   */
  static function flatten($tags) {
    if (isset($tags[0])) return $tags;
    $flat_tags = array();
    foreach ($tags as $namespace => $values) {
      if (is_array($values)) {
        foreach ($values as $value) {
          $flat_tags[] = "$namespace:$value";
        }
      }
      else {
        $flat_tags[] = "$namespace:$values";
      }
    }
    return $flat_tags;
  }
}

/**
 * Factory for instantiating and statically caching the cache tags instance.
 *
 * By default, this returns an instance of the DrupalDatabaseCacheTags class.
 * To override this, you must set a variable (either with variable_set() or
 * in settings.php via $conf):
 * @code
 *  variable_set('cache_tags_class', 'MyCustomCacheTags');
 * @endcode
 *
 * @see DrupalCacheTagsInterface
 *
 * @return DrupalCacheTagsInterface
 *   The cache tags object.
 */
function cache_tags() {
  $object = &drupal_static(__FUNCTION__);
  if (!isset($object)) {
    $class = variable_get('cache_tags_class', 'DrupalDatabaseCacheTags');
    $object = new $class;
  }
  return $object;
}

/**
 * Invalidate each tag in the $tags array.
 *
 * Procedural (convenience) wrapper for the cache_tags()->invalidate() method.
 * Cache entries tagged with any one of these will subsequently return
 * FALSE for cache_get().
 *
 * @param $tags
 *   Associative array of tags, in the same format that is passed to cache_set().
 *
 * @see cache_set()
 */
function cache_invalidate($tags) {
  return cache_tags()->invalidate($tags);
}
