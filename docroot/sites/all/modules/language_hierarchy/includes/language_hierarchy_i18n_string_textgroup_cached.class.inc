<?php

/**
 * This is a temporary solution for the i18n_string_textgroup_cached.
 *
 * The i18n_string_textgroup_cached class extends i18n_string_textgroup_default
 * class, but we want it to extend the
 * language_hierarchy_i18n_string_textgroup_default class. And it is not
 * possible to change this at runtime. So, this class is just a copy of the
 * i18n_string_textgroup_cached with the parent class replaced to ours.
 *
 * @todo: find a better solution, for example, getting the delegator pattern
 * into the i18n_string project
 */
class language_hierarchy_i18n_string_textgroup_cached extends language_hierarchy_i18n_string_textgroup_default {

  /**
   * Defines the timeout for the persistent caching.
   * @var int
   */
  public $caching_time = CACHE_TEMPORARY;

  /**
   * Extends the existing constructor with a cache handling.
   *
   * @param string $textgroup
   *   The name of this textgroup.
   */
  public function __construct($textgroup) {
    parent::__construct($textgroup);
    // Fetch persistent caches, the persistent caches contain only metadata.
    // Those metadata are processed by the related cache_get() methods.
    foreach (array('cache_multiple', 'strings') as $caches_type) {
      if (($cache = cache_get('i18n:string:tgroup:' . $this->textgroup . ':' . $caches_type)) && !empty($cache->data)) {
        $this->{$caches_type} = $cache->data;
      }
    }
  }

  /**
   * Class destructor.
   *
   * Updates the persistent caches for the next usage.
   * This function not only stores the data of the textgroup objects but also
   * of the string objects. That way we ensure that only cacheable string object
   * go into the persistent cache.
   */
  public function __destruct() {
    // Reduce size to cache by removing NULL values.
    $this->strings = array_filter($this->strings);


    $strings_to_cache = array();
    // Store the persistent caches. We just store the metadata the translations
    // are stored by the string object itself. However storing the metadata
    // reduces the number of DB queries executed during runtime.
    $cache_data = array();
    foreach ($this->strings as $context => $i18n_string_object) {
      $cache_data[$context] = $context;
      $strings_to_cache[$context] = $i18n_string_object;
    }
    cache_set('i18n:string:tgroup:' . $this->textgroup . ':strings', $cache_data, 'cache', $this->caching_time);

    $cache_data = array();
    foreach ($this->cache_multiple as $pattern => $strings) {
      foreach ($strings as $context => $i18n_string_object) {
        $cache_data[$pattern][$context] = $context;
        $strings_to_cache[$context] = $i18n_string_object;
      }
    }
    cache_set('i18n:string:tgroup:' . $this->textgroup . ':cache_multiple', $cache_data, 'cache', $this->caching_time);

    // Cache the string objects related to this textgroup.
    // Store only the public visible data into the persistent cache.
    foreach ($strings_to_cache as $i18n_string_object) {
      // If this isn't an object it's an unprocessed cache item and doesn't need
      // to be stored again.
      if (is_object($i18n_string_object)) {
        cache_set($i18n_string_object->get_cid(), get_object_vars($i18n_string_object), 'cache', $this->caching_time);
      }
    }
  }

  /**
   * Reset cache, needed for tests.
   *
   * Takes care of the persistent caches.
   */
  public function cache_reset() {
    // Reset the persistent caches.
    cache_clear_all('i18n:string:tgroup:' . $this->textgroup , 'cache', TRUE);
    // Reset the complete string object cache too. This will affect string
    // objects of other textgroups as well.
    cache_clear_all('i18n:string:obj:', 'cache', TRUE);

    return parent::cache_reset();
  }

  /**
   * Get translation from cache.
   *
   * Extends the original handler with persistent caching.
   *
   * @param string $context
   *   The context to look out for.
   * @return i18n_string_object|NULL
   *   The string object if available or NULL otherwise.
   */
  protected function cache_get($context) {
    if (isset($this->strings[$context])) {
      // If the cache contains a string re-build i18n_string_object.
      if (is_string($this->strings[$context])) {
        $i8n_string_object = new i18n_string_object(array('textgroup' => $this->textgroup));
        $i8n_string_object->set_context($context);
        $this->strings[$context] = $i8n_string_object;
      }
      // Now run the original handling.
      return parent::cache_get($context);
    }
    return NULL;
  }

  /**
   * Get strings from multiple cache.
   *
   * @param $context array
   *   String context as array with language property at the end.
   *
   * @return mixed
   *   Array of strings (may be empty) if we've got a cache hit.
   *   Null otherwise.
   */
  protected function multiple_cache_get($context) {
    // Ensure the values from the persistent cache are properly re-build.
    $cache_key = implode(':', $context);
    if (isset($this->cache_multiple[$cache_key])) {
      foreach ($this->cache_multiple[$cache_key] as $cached_context) {
        if (is_string($cached_context)) {
          $i8n_string_object = new i18n_string_object(array('textgroup' => $this->textgroup));
          $i8n_string_object->set_context($cached_context);
          $this->cache_multiple[$cache_key][$cached_context] = $i8n_string_object;
        }
      }
    }
    else {
      // Now we try more generic keys. For instance, if we are searching 'term:1:*'
      // we may try too 'term:*:*' and filter out the results.
      foreach ($context as $key => $value) {
        if ($value != '*') {
          $try = array_merge($context, array($key => '*'));
          return $this->multiple_cache_get($try);
        }
      }
    }
    return parent::multiple_cache_get($context);
  }

  public function string_update($i18nstring, $options = array()) {
    // Flush persistent cache.
    cache_clear_all($i18nstring->get_cid(), 'cache', TRUE);
    return parent::string_update($i18nstring, $options);
  }

  public function string_remove($i18nstring, $options = array()) {
    // Flush persistent cache.
    cache_clear_all($i18nstring->get_cid(), 'cache', TRUE);
    return parent::string_remove($i18nstring, $options);
  }
}
