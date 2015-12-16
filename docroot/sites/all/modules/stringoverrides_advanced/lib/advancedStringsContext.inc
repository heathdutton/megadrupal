<?php

class advancedStringOverridesContext implements ArrayAccess {
  protected $langcode;
  protected $context;
  protected $cachedStrings;

  function __construct(advancedStringOverridesLangcode $langcode, $context) {
    $this->langcode = $langcode;
    $this->context = $context;
  }

  public function getContext() {
    return $this->context;
  }

  /*
   * Our array access methods.
   */

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Whether a offset exists
   * @link http://php.net/manual/en/arrayaccess.offsetexists.php
   * @param mixed $offset <p>
   * An offset to check for.
   * </p>
   * @return boolean true on success or false on failure.
   * </p>
   * <p>
   * The return value will be casted to boolean if non-boolean was returned.
   */
  public function offsetExists($offset) {
    // Ensure that we've loaded the strings from the cache.
    $this->ensureCacheLoaded();
    // Skip the DB lookup if we can.
    if (!isset($this->cachedStrings[$offset])) {
      // We do not have this translation cached, so get it from the DB.
      $translation = db_query("SELECT s.lid, t.translation, s.version FROM {stringoverrides_advanced_source} s LEFT JOIN {stringoverrides_advanced_target} t ON s.lid = t.lid AND t.language = :language WHERE s.source = :source AND s.context = :context AND s.textgroup = 'default'", array(
        ':language' => $this->getLangcode(),
        ':source' => $offset,
        ':context' => (string) $this->getContext(),
      ))->fetchObject();
      if ($translation) {
        // We have the source string at least.
        // Cache translation string or TRUE if no translation exists.
        $this->cachedStrings[$offset] = (empty($translation->translation) ? FALSE : $translation->translation);

        if ($translation->version != VERSION) {
          // This is the first use of this string under current Drupal version. Save version
          // and clear cache, to include the string into caching next time. Saved version is
          // also a string-history information for later pruning of the tables.
          db_update('stringoverrides_advanced_source')
            ->fields(array('version' => VERSION))
            ->condition('lid', $translation->lid)
            ->execute();
          cache_clear_all($this->getCacheKey(), $this->getCacheBinName());
        }
      }
      else {
        // We don't have the source string, cache this as untranslated.
        db_merge('stringoverrides_advanced_source')
          ->insertFields(array(
            'location' => request_uri(),
            'version' => VERSION,
          ))
          ->key(array(
            'source' => $offset,
            'context' => (string) $this->getContext(),
            'textgroup' => 'default',
          ))
          ->execute();
        $this->cachedStrings[$offset] = FALSE;
        // Clear locale cache so this string can be added in a later request.
        cache_clear_all($this->getCacheKey(), $this->getCacheBinName());
      }
    }

    return $this->cachedStrings[$offset] !== FALSE;
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Offset to retrieve
   * @link http://php.net/manual/en/arrayaccess.offsetget.php
   * @param mixed $offset <p>
   * The offset to retrieve.
   * </p>
   * @return mixed Can return all value types.
   */
  public function offsetGet($offset) {
    // Ensure that we've loaded the strings from the cache.
    $this->ensureCacheLoaded();
    // Either return the translated string, or the original offset.
    return $this->cachedStrings[$offset] !== FALSE ? $this->cachedStrings[$offset] : $offset;
  }

  protected function getCacheKey() {
    return 'advancedStringOverrides:' . $this->getLangcode() . ':' . $this->getContext();
  }

  protected function getCacheBinName() {
    return 'cache';
  }

  public function getLangcode() {
    return $this->langcode->getLangcode();
  }

  protected function ensureCacheLoaded() {
    if (!isset($this->cachedStrings)) {
      if ($cache = cache_get($this->getCacheKey(), $this->getCacheBinName())) {
        $this->cachedStrings = $cache->data;
      }
      elseif (lock_acquire($this->getCacheKey())) {
        // Refresh database stored cache of translations for given language.
        // We only store short strings used in current version, to improve
        // performance and consume less memory.
        $result = db_query("SELECT s.source, t.translation, t.language FROM {stringoverrides_advanced_source} s LEFT JOIN {stringoverrides_advanced_target} t ON s.lid = t.lid AND t.language = :language WHERE s.textgroup = 'default' AND s.version = :version AND LENGTH(s.source) < :length AND s.context = :context", array(':language' => $this->getLangcode(), ':version' => VERSION, ':length' => variable_get('stringoverrides_advanced_cache_length', 75), ':context' => $this->getContext()));
        foreach ($result as $data) {
          $this->cachedStrings[$data->source] = (empty($data->translation) ? FALSE : $data->translation);
        }
        cache_set($this->getCacheKey(), $this->cachedStrings, $this->getCacheBinName());
        lock_release($this->getCacheKey());
      }
    }
  }

  /**
   * Return an array of all overrides.
   */
  public function getAllOverrides() {
    $strings = array();
    $result = db_query("SELECT s.source, t.translation, t.language FROM {stringoverrides_advanced_source} s INNER JOIN {stringoverrides_advanced_target} t ON s.lid = t.lid AND t.language = :language WHERE s.textgroup = 'default' AND s.version = :version AND s.context = :context", array(':language' => $this->getLangcode(), ':version' => VERSION, ':context' => $this->getContext()));
    foreach ($result as $data) {
      $strings[$data->source] = $data->translation;
    }

    return $strings;
  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Offset to set
   * @link http://php.net/manual/en/arrayaccess.offsetset.php
   * @param mixed $offset <p>
   * The offset to assign the value to.
   * </p>
   * @param mixed $value <p>
   * The value to set.
   * </p>
   * @return void
   */
  public function offsetSet($offset, $value) {

  }

  /**
   * (PHP 5 &gt;= 5.0.0)<br/>
   * Offset to unset
   * @link http://php.net/manual/en/arrayaccess.offsetunset.php
   * @param mixed $offset <p>
   * The offset to unset.
   * </p>
   * @return void
   */
  public function offsetUnset($offset) {

  }


}