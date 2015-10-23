<?php
/**
 * @file
 * Cache handler based on context and roles.
 */

/**
 * Extends CacheArray to allow for dynamic building of the locale cache.
 */
class CacheLocaleUncached extends DrupalTimeBasedCacheArray {
  /**
   * A language code.
   *
   * @var string
   */
  protected $langcode;

  /**
   * The msgctxt context.
   *
   * @var string
   */
  protected $context;

  /**
   * Constructs a CacheLocalePath object.
   *
   * @param string $langcode
   *   The language code, e.g. 'en'.
   * @param string $context
   *   The context.
   * @param string $bin
   *   The cache bin to use.
   * @param string $textgroup
   *   The text group to use.
   */
  public function __construct($langcode, $context = NULL, $bin = 'cache', $textgroup = 'default') {
    $this->langcode = $langcode;
    $this->context = (string) $context;
    $this->textgroup = (string) $textgroup;
  }

  /**
   * Implements DrupalCacheArray::resolveCacheMiss().
   */
  protected function resolveCacheMiss($offset) {
    $translation = db_query("SELECT s.lid, t.translation, s.version FROM {locales_source} s LEFT JOIN {locales_target} t ON s.lid = t.lid AND t.language = :language WHERE s.source = :source AND s.context = :context AND s.textgroup = :textgroup", array(
      ':language' => $this->langcode,
      ':source' => $offset,
      ':context' => $this->context,
      ':textgroup' => $this->textgroup,
    ))->fetchObject();
    if ($translation) {
      if ($translation->version != VERSION) {
        // This is the first use of this string under current Drupal version.
        // Update the {locales_source} table to indicate the string is current.
        db_update('locales_source')
          ->fields(array('version' => VERSION))
          ->condition('lid', $translation->lid)
          ->execute();
      }
      $value = !empty($translation->translation) ? $translation->translation : TRUE;
    }
    else {
      // We don't have the source string, update the {locales_source} table to
      // indicate the string is not translated.
      db_merge('locales_source')
        ->insertFields(array(
          'location' => request_uri(),
          'version' => VERSION,
        ))
        ->key(array(
          'source' => $offset,
          'context' => $this->context,
          'textgroup' => $this->textgroup,
        ))
        ->execute();
      $value = TRUE;
    }

    $this->storage[$offset] = $value;
    return $value;
  }

  /**
   * Don't write cache blob.
   */
  protected function set($data, $lock = TRUE) {}

}
