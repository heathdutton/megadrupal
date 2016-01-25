<?php
/**
 * @file
 * Definition of localeWithFallback and localeContextWithFallback.
 */

class localeWithFallback implements ArrayAccess {
  private $langcode;
  private $cache = array();
  private $fallback;

  public function __construct($langcode, $fallback = '') {
    $this->langcode = $langcode;
    $this->cache = array();
    $languages = language_list();
    $this->fallback = isset($languages[$fallback]) ? $fallback : language_default('language');
  }

  public function offsetExists($offset) {
    return (isset($this->cache[$offset]));
  }

  public function &offsetGet($offset) {
    if (!isset($this->cache[$offset])) {
      $this->cache[$offset] = new localeContextWithFallback($this->langcode, $this->fallback, $offset);
    }
    return $this->cache[$offset];
  }

  public function offsetSet($offset, $value)  { return; }
  public function offsetUnset($offset) { return; }
}

class localeContextWithFallback implements ArrayAccess {
  private $first_language = '';
  private $second_language = '';
  private $context = '';

  public function __construct($langcode, $fallback, $context) {
    $this->first_language = $langcode;
    $this->second_language = $fallback;
    $this->context = $context;
  }

  public function offsetGet($offset) {
    $locale_t = &drupal_static('locale');
    $translation = locale($offset, $this->context, $this->first_language);
    // Check whether string returned was a translation or the untranslated original.
    // Have to check locale()'s static variable, because just comparing the strings
    // might be misleading. (Stupid example: (en) village == (fr) village.)
    if ($locale_t[$this->first_language][$this->context][$offset] === TRUE) {
      $translation = locale($offset, $this->context, $this->second_language);
    }
    return $translation;
  }

  public function offsetExists($offset) {
    $locale_t = &drupal_static('locale');
    $translation = locale($offset, $this->context, $this->first_language);
    if ($locale_t[$this->first_language][$this->context][$offset] !== TRUE) {
      return TRUE;
    }
    else {
      $translation = locale($offset, $this->context, $this->second_language);
      return ($locale_t[$this->second_language][$this->context][$offset] !== TRUE);
    }
  }

  public function offsetSet($offset, $value)  { return; }
  public function offsetUnset($offset) { return; }
}