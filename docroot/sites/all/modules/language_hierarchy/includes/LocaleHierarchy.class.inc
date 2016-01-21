<?php
/**
 * @file
 * Definition of LocaleHierarchy.
 *
 * Inspired by Language fallback module.
 */

/**
 * Class LocaleHierarchy.
 */
class LocaleHierarchy implements ArrayAccess {
  private $langcode;
  private $cache = array();
  private $candidates = NULL;

  public function __construct($langcode) {
    $this->langcode = $langcode;
    $this->cache = array();
  }

  public function offsetExists($offset) {
    return isset($this->cache[$offset]);
  }

  public function offsetGet($offset) {
    /**
     * WARNING!
     * On some rare occasions when drupal is not fully bootstrapped
     * the function language_hierarchy_get_ancestors might not be available.
     * This may happen on some administration pages so it should be
     * safe to just skip all defined fallbacks.
     */
    if ($this->candidates === NULL && function_exists('language_hierarchy_get_ancestors')) {
      drupal_static_reset('language_list');
      $this->candidates = array_keys(language_hierarchy_get_ancestors($this->langcode));
    }

    if (!isset($this->cache[$offset])) {
      $this->cache[$offset] = new LocaleContextHierarchy($this->langcode, $this->candidates, $offset);
    }

    return $this->cache[$offset];
  }

  public function offsetSet($offset, $value) {return;}
  public function offsetUnset($offset) {return;}
}
