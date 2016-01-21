<?php

namespace Drupal\aws_glacier;

/**
 * Class Command
 * @package Drupal\aws_glacier
 */
abstract class Command {

  /**
   * @var string
   */
  private $command;

  /**
   * @param string $command
   * @return $this
   */
  public function setCommand($command) {
    // Reset Args.
    $this->args = array();
    $this->command = $command;
    return $this;
  }

  /**
   * @param string $command
   * @return $this
   */
  function __construct($command) {
    $this->command = $command;
    return $this;
  }

  /**
   * @var array
   */
  private $args = array();

  /**
   * @param array $args
   * @return $this
   */
  protected function setArgs(array $args) {
    $this->args = array_merge($this->args, $args);
    return $this;
  }

  /**
   * @var array
   */
  private $data;

  /**
   * @param array $data
   * @return $this
   */
  public function setData(array $data) {
    $this->data = $data;
    return $this;
  }

  /**
   * @return array
   */
  public function getData() {
    return $this->data;
  }

  /**
   * @var string
   */
  protected $accountId;

  /**
   * @return string
   */
  public function getAccountId() {
    return $this->accountId ? $this->accountId : '-';
  }

  /**
   * @param string $accountId
   * @return $this
   */
  public function setAccountId($accountId = '-') {
    $this->accountId = $accountId;
    return $this;
  }

  /**
   * Wrapper function to execute a Aws command.
   * @return $this
   */
  public function run(){
    $this->args['accountId'] = $this->getAccountId();
    $this->data = aws_glacier_execute_command($this->command, $this->args);
    return $this;
  }
}
