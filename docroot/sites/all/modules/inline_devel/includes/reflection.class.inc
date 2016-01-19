<?php

/**
 * @file
 * Implementing a custom use of reflection class.
 */

class inlineDevelReflection {

  /**
   * @var $reflection
   *  The reflection object.
   */
  var $reflection;

  /**
   * @var $name
   *  The name of the function / method.
   */
  var $name;

  /**
   * @var $class
   *  The class name.
   */
  var $class;

  /**
   * @param $cleanArguments
   *  The arguments in a clean state, without the $ sign before them.
   */
  var $cleanArguments = array();

  /**
   * @param $dollarArguments
   *  The arguments with the $ sign before them.
   */
  var $dollarArguments = array();

  /**
   * @param $startLine
   *  The start line of the function.
   */
  var $startLine = 0;

  /**
   * @param $endLine
   *  The end line of the function.
   */
  var $endLine;

  /**
   * @param $fileName;
   *  The file name of the function.
   */
  var $file;

  /**
   * @param $messages;
   *  Variable with the messages that the reflection calss return.
   */
  var $messages;

  /**
   * In the construct function i'll save important things.
   */
  public function __construct($function_name, $class_name = '') {
    $this->name = $function_name;
    $this->class = $class_name;

    if (empty($class_name)) {
      $this->_functionLoad();
    }
    else {
      $this->_methodLoad();
    }

    // When the class failed to load, show the messages.
    if (!empty($this->messages)) {
      return;
    }

    // Set of function to do when loading a function.
    $this->getArguments();
    $this->lines();
    $this->fileName();
  }

  function showMessages() {
    if (!empty($this->messages)) {
      foreach ($this->messages as $message) {
        drupal_set_message($message, 'error');
      }
    }
  }

  /**
   * Get the arguments.
   */
  function getArguments($state = 'empty') {
    $this->_getArguments($this->cleanArguments, FALSE);
    $this->_getArguments($this->dollarArguments, TRUE);
  }

  /**
   * Store the start and the end line of the function.
   */
  function lines() {
    $this->startLine = $this->reflection->getStartLine();
    $this->endLine = $this->reflection->getEndLine();
  }

  /**
   * Get the file name where the function is defined.
   */
  function fileName() {
    $this->file = $this->reflection->getFileName();
  }

  /**
  * Try to load the function.
  */
  private function _functionLoad() {
    try {
      $this->reflection = new ReflectionFunction($this->name);
    }
    catch (Exception $e) {
      $this->messages[] = $e->getMessage();
    }
  }

  /**
   * Try to load the class method.
   */
  private function _methodLoad() {
    try {
      $this->reflection = new ReflectionMethod($this->class, $this->name);
    } catch (exception $e) {
      $this->messages[] = $e->getMessage();
    }
  }

  /**
   * Get the arguments of the function.
   */
  function _getArguments(&$argumentsVariable, $dollar = FALSE) {
    $argumentsVariable = array();

    if (!empty($this->messages)) {
      return drupal_set_message();
    }

    foreach ($this->reflection->getParameters() as $arguments) {
      $argumentsVariable[] = $dollar ? "$" . $arguments->name : $arguments->name;
    }
  }
}
