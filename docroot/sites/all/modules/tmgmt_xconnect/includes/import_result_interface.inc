<?php
/**
 * @file
 * Interface definition for translation import results.
 */

/**
 * Interface for import result objects.
 */
interface TmgmtXConnectImportResultInterface {
  /**
   * Get the file name that is imported.
   *
   * @return string
   *   The filename.
   */
  public function getFileName();

  /**
   * Get the result message.
   *
   * @return string
   *   The message (not translated!).
   */
  public function getMessage();

  /**
   * Get the message arguments.
   *
   * @return array
   *   The message variables.
   */
  public function getMessageArguments();

}
