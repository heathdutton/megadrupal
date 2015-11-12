<?php
/**
 * @file
 * Error result.
 */

/**
 * Error result.
 */
class TmgmtXConnectImportResultError extends TmgmtXConnectImportResultAbstract {
  /**
   * Construct a result.
   *
   * @param string $file_name
   *   The file name.
   * @param string $message
   *   The result message.
   * @param array $message_arguments
   *   The arguments for the message.
   */
  public function __construct(
    $file_name,
    $message = NULL,
    $message_arguments = array()
  ) {
    $this->setFileName($file_name);
    $this->setMessage($message);
    $this->setMessageArguments($message_arguments);
  }

}
