<?php
/**
 * @file
 * Abstract implementation of the result interface.
 */

/**
 * Abstract implementation of the ImportResult.
 */
abstract class TmgmtXConnectImportResultAbstract implements TmgmtXConnectImportResultInterface {
  /**
   * The imported file name.
   *
   * @var string
   */
  private $fileName;

  /**
   * The result message.
   *
   * @var string
   */
  private $message;

  /**
   * The message arguments.
   *
   * @var array
   */
  private $messageArguments;


  /**
   * {@inheritdoc}
   */
  public function getFileName() {
    return $this->fileName;
  }

  /**
   * Set the file name.
   *
   * @param string $file_name
   *   The file name.
   */
  protected function setFileName($file_name) {
    $this->fileName = $file_name;
  }

  /**
   * {@inheritdoc}
   */
  public function getMessage() {
    return $this->message;
  }

  /**
   * Set the result message.
   *
   * @param string $message
   *   The result message.
   */
  protected function setMessage($message) {
    $this->message = $message;
  }

  /**
   * {@inheritdoc}
   */
  public function getMessageArguments() {
    return $this->messageArguments;
  }

  /**
   * Set the message arguments.
   *
   * @param array $arguments
   *   The message arguments.
   */
  protected function setMessageArguments($arguments) {
    $this->messageArguments = $arguments;
  }

}
