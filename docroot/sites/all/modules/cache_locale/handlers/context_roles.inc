<?php
/**
 * @file
 * Cache handler based on context and roles.
 */

/**
 * Extends CacheArray to allow for dynamic building of the locale cache.
 */
class CacheLocaleContextRoles extends DrupalTimeBasedCacheArray {
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
   * Constructs a CacheLocaleContextRoles object.
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

    // Add the current user's role IDs to the cache key, this ensures that, for
    // example, strings for admin menu items and settings forms are not cached
    // for anonymous users.
    $rids = implode(':', array_keys($GLOBALS['user']->roles));
    parent::__construct("locale:$this->langcode:$this->context:$rids", $bin, $expire);
  }

  /**
   * Implements DrupalCacheArray::resolveCacheMiss().
   */
  protected function resolveCacheMiss($offset) {
    $translation = db_query("SELECT s.lid, t.translation, s.version FROM {locales_source} s LEFT JOIN {locales_target} t ON s.lid = t.lid AND t.language = :language WHERE s.source = :source AND s.context = :context AND s.textgroup = :textgroup", array(
      ':language' => $this->langcode,
      ':source' => $offset,
      ':context' => $this->context,
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
    // Disabling the usage of string caching allows a module to watch for
    // the exact list of strings used on a page. From a performance
    // perspective that is a really bad idea, so we have no user
    // interface for this. Be careful when turning this option off!
    if (variable_get('locale_cache_strings', 1)) {
      $this->persist($offset);
    }
    return $value;
  }

}
