<?php
/**
 * @file
 * Base classes.
 */

/**
 * Drupal cache array with expiration.
 */
abstract class DrupalTimeBasedCacheArray extends DrupalCacheArray {
  protected $expire = CACHE_PERMANENT;

  /**
   * Constructs a DrupalTimeBasedCacheArray object.
   *
   * @param string $cid
   *   The cid for the array being cached.
   * @param string $bin
   *   The bin to cache the array.
   * @param int $expire
   *   The expiration time.
   */
  public function __construct($cid, $bin, $expire = NULL) {
    parent::__construct($cid, $bin);
    $this->expire = isset($expire) ? $expire : variable_get('cache_locale_expire', CACHE_PERMANENT);
  }

  /**
   * Translate string.
   *
   * @param string $string
   *   The string to translate.
   *
   * @return string
   *   The translated string if availabe, otherwise the original string.
   */
  public function translate($string) {
    return $this[$string] === TRUE ? $string : $this[$string];
  }

  /**
   * Writes a value to the persistent cache immediately.
   *
   * @param mixed $data
   *   The data to write to the persistent cache.
   * @param bool $lock
   *   Whether to acquire a lock before writing to cache.
   */
  protected function set($data, $lock = TRUE) {
    // Lock cache writes to help avoid stampedes.
    // To implement locking for cache misses, override __construct().
    $lock_name = $this->cid . ':' . $this->bin;
    if (!$lock || lock_acquire($lock_name)) {
      if ($cached = cache_get($this->cid, $this->bin)) {
        $data = $cached->data + $data;
      }
      cache_set($this->cid, $data, $this->bin, $this->expire);
      if ($lock) {
        lock_release($lock_name);
      }
    }
  }

}
