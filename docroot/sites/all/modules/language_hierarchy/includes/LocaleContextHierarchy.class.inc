<?php
/**
 * @file
 * Definition of LocaleContextHierarchy.
 *
 * Inspired by Language fallback module.
 */

/**
 * Class LocaleContextHierarchy
 */
class LocaleContextHierarchy implements ArrayAccess {
  private $langcode = '';
  private $context = '';

  public function __construct($langcode, $candidates, $context) {
    $this->langcode = $langcode;
    $this->candidates = $candidates;
    $this->context = $context;
  }

  public function offsetGet($offset) {
    if (function_exists('locale')) {
      $locale_t = &drupal_static('locale');
      $translation = locale($offset, $this->context, $this->langcode);
      // Check whether string returned was a translation or the untranslated original.
      // Have to check locale()'s static variable, because just comparing the strings
      // might be misleading. (Stupid example: (en) village == (fr) village.)
      if (isset($this->candidates) && $locale_t[$this->langcode][$this->context][$offset] === TRUE) {
        foreach ($this->candidates as $candidate) {
          $translation = locale($offset, $this->context, $candidate);
          if ($locale_t[$candidate][$this->context][$offset] !== TRUE) {
            break;
          }
        }
      }
      return $translation;
    }
    return $offset;
  }

  public function offsetExists($offset) {
    return TRUE;
  }

  public function offsetSet($offset, $value) {return;}
  public function offsetUnset($offset) {return;}
}
