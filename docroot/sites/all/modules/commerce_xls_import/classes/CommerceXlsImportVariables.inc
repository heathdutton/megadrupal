<?php

/**
 * @file
 * CommerceXlsImportVariables class definition.
 */

/**
 * Class containing variables used by Commerce XLS Import.
 */
class CommerceXlsImportVariables {
  private static $validate;
  private static $import;
  private static $status;
  private static $rowTitleStatus = array();

  /**
   * Set $status.
   */
  public static function setValidate($value) {
    CommerceXlsImportVariables::$validate = $value;
    CommerceXlsImportVariables::$import = !$value;
    CommerceXlsImportVariables::setStatus();
  }

  /**
   * Set $import.
   */
  public static function setImport($value) {
    CommerceXlsImportVariables::$import = $value;
    CommerceXlsImportVariables::$validate = !value;
    CommerceXlsImportVariables::setStatus();
  }

  /**
   * Set the status message.
   */
  public static function setStatus() {
    if (CommerceXlsImportVariables::$validate) {
      CommerceXlsImportVariables::$status = "Validate";
    }
    elseif (CommerceXlsImportVariables::$import) {
      CommerceXlsImportVariables::$status = "Import";
    }
  }

  /**
   * Sets the column's title status.
   */
  public static function setRowTitleErrorStatus($row, $status) {
    CommerceXlsImportVariables::$rowTitleStatus[$row] = $status;
  }

  /**
   * Gets the column's title status.
   */
  public static function rowTitleHasError($row) {
    if (isset(CommerceXlsImportVariables::$rowTitleStatus[$row])) {
      return CommerceXlsImportVariables::$rowTitleStatus[$row];
    }
    return FALSE;
  }

  /**
   * Get validate.
   */
  public static function isValidate() {
    return CommerceXlsImportVariables::$validate;
  }

  /**
   * Get import.
   */
  public static function isImport() {
    return CommerceXlsImportVariables::$import;
  }

  /**
   * Get the status message.
   */
  public static function getStatus() {
    return CommerceXlsImportVariables::$status;
  }

}
