<?php
/**
 * @file
 * Contains DataUriCreatorSettings.
 */

/**
 * Implements a helper class with utility methods involving strings.
 */
final class DataUriCreatorSettings {

  /**
   * Hides the DataUriCreatorSettings constructor.
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
   * Gets the maximum byte-size of data uploads to the Data URI Creator page.
   *
   * @return int|null
   *   The maximum allowed file size, specified as a positive integer;
   *   otherwise NULL to indicate no limit.
   */
  public static function getMaximumFileSize() {
    $max_file_size = (integer) variable_get(
      'data_uri_creator_max_file_size', 262144 /* 256KB by default */);
    if ($max_file_size <= 0) {
      $max_file_size = NULL;
    }

    return $max_file_size;
  }

  /**
   * Sets the maximum byte-size of data uploads to the Data  URI Creator page.
   *
   * @param int|null $max_file_size
   *   The maximum allowed file size, specified as a positive integer;
   *   otherwise an empty value for no limit.
   */
  public static function setMaximumFileSize($max_file_size) {
    $max_file_size = (integer) $max_file_size;
    if ($max_file_size <= 0) {
      $max_file_size = NULL;
    }

    variable_set('data_uri_creator_max_file_size', $max_file_size);
  }

  /**
   * Determines whether the Data URI Creator page is publicly available.
   *
   * @return bool
   *   TRUE when the page is publicly available; otherwise FALSE for restricted
   *   access.
   */
  public static function getPagePublicFlag() {
    $is_public = user_access(
      DATA_URI_CREATOR_PAGE_PERMISSION, drupal_anonymous_user());
    return $is_public;
  }

  /**
   * Specifies whether the Data URI Creator page is publicly available.
   *
   * @param bool $is_public
   *   (optional) TRUE to make the page publicly available; otherwise FALSE for
   *   restricted access.
   */
  public static function setPagePublicFlag($is_public = TRUE) {
    $permissions = array(DATA_URI_CREATOR_PAGE_PERMISSION => (bool) $is_public);
    user_role_change_permissions(DRUPAL_ANONYMOUS_RID, $permissions);
  }
}
