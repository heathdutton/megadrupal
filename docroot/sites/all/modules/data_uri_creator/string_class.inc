<?php
/**
 * @file
 * Contains DataUriCreatorString.
 */

/**
 * Implements a helper class with utility methods involving strings.
 */
final class DataUriCreatorString {

  /**
   * Hides the DataUriCreatorString constructor.
   *
   * By combining the "protected" keyword (that is specified on the
   * constructor) with the "final" keyword (that is specified on the class
   * definition), this class is effectively turned into a sealed static class
   * that can never be instantiated or derived from.  To use the class, one
   * may only call existing public static methods from externally.
   */
  protected function __construct() {
    // This is not supposed to execute ever.
  }

  /**
   * Determines whether the start of a string matches the specified substring.
   *
   * @param string $string
   *   The string to be tested.
   * @param string $value
   *   The substring to test for.
   * @param bool $ignore_case
   *   (optional) A boolean that indicates whether character-case should be
   *   ignored.  TRUE to ignore case during the comparison; otherwise, FALSE.
   *
   * @return bool
   *   TRUE if the value parameter matches the beginning of this string;
   *   otherwise FALSE.
   */
  public static function startsWith($string, $value, $ignore_case = FALSE) {
    if (!isset($value)) {
      return FALSE;
    }

    $length = strlen($value);
    if ($length === 0) {
      return TRUE;
    }

    if (!$ignore_case) {
      return (strncmp($string, $value, $length) === 0);
    }

    return (substr_compare($string, $value, 0, $length, $ignore_case) === 0);
  }

  /**
   * Determines whether the end of a string matches the specified substring.
   *
   * @param string $string
   *   The string to be tested.
   * @param string $value
   *   The substring to test for.
   * @param bool $ignore_case
   *   (optional) A boolean that indicates whether character-case should be
   *   ignored.  TRUE to ignore case during the comparison; otherwise, FALSE.
   *
   * @return bool
   *   TRUE if the value parameter matches the end of this string;
   *   otherwise FALSE.
   */
  public static function endsWith($string, $value, $ignore_case = FALSE) {
    if (!isset($value)) {
      return FALSE;
    }

    $length = strlen($value);
    if ($length === 0) {
      return TRUE;
    }

    if ($length > strlen($string)) {
      return FALSE;
    }

    return (substr_compare($string, $value, -$length, $length, $ignore_case) === 0);
  }
}
