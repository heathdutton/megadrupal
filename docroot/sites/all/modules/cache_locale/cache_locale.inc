<?php
/**
 * @file
 * Interface and class for custom locale handling.
 */

/**
 * Locale interface.
 */
interface LocaleInterface {
  /**
   * Translate a string.
   *
   * @see locale()
   */
  static public function locale($string, $context = NULL, $langcode = NULL, $textgroup = 'default');

}

/**
 * Custom locale cache function.
 */
class CacheLocale implements LocaleInterface {
  /**
   * Is this Locale handler valid.
   *
   * @var boolean
   */
  static public $isValid = TRUE;

  /**
   * Implements LocaleInterface::locale().
   */
  static public function locale($string, $context = NULL, $langcode = NULL, $textgroup = 'default') {
    global $language;

    // Don't use the advanced drupal_static() pattern, since this is called very
    // often.
    static $drupal_static_fast, $bin, $class;
    if (!isset($drupal_static_fast)) {
      $drupal_static_fast['locale'] = &drupal_static(__FUNCTION__);
      $bin = variable_get('cache_locale_bin', 'cache_locale');
      $class = variable_get('cache_locale_class', 'CacheLocaleCore');
      if (!class_exists($class)) {
        watchdog('locale', 'Class %class does not exist', array('%class' => $class), WATCHDOG_ERROR);
        self::$isValid = FALSE;
        return $string;
      }
      elseif (!class_implements($class, 'ArrayAccess')) {
        watchdog('locale', 'Class %class does not implement ArrayAccess', array('%class' => $class), WATCHDOG_ERROR);
        self::$isValid = FALSE;
        return $string;
      }
    }

    $locale_t = &$drupal_static_fast['locale'];

    if (!isset($string)) {
      // Return all cached strings if no string was specified.
      return $locale_t;
    }

    $langcode = isset($langcode) ? $langcode : $language->language;

    // Strings are cached by langcode and user roles, using instances of the
    // DrupalCacheArray-ish class to handle string lookup and caching.
    if (!isset($locale_t[$langcode][$context]) && isset($language)) {
      $locale_t[$langcode][$context] = new $class($langcode, $context, $bin, $textgroup);
    }
    return ($locale_t[$langcode][$context][$string] === TRUE ? $string : $locale_t[$langcode][$context][$string]);
  }

}
