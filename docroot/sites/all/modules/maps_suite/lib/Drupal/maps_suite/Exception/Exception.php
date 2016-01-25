<?php

/**
 * @file
 * Define the MaPS Suite Exception class, that exposes some specific helpers
 * for submodules.
 */

namespace Drupal\maps_suite\Exception;

/**
 * Exception class for MaPS Suite.
 */
class Exception extends \Exception {

  /**
   * The unprocessed message.
   *
   * @var string
   */
  protected $_unprocessed;

  /**
   * Arguments that will be used within watchdog().
   *
   * @var array
   */
  protected $_args = array();

  /**
   * Contain context variables that may be used by watchdog() method.
   *
   * @var array
   */
  protected $_context = array();

  /**
   * Overrides parent constructor.
   */
  public function __construct($message, $code = 0, array $args = array(), array $context = array()) {
    parent::__construct($message, $code);

    $this->_args = _drupal_decode_exception($this);
    $this->_args['!context'] = '';
    $this->_context = $context;
    $this->_unprocessed = '%type: !message in %function (line %line of %file).<br />Context:<hr />!context';

    if ($args) {
      maps_suite_format_string($this->message, $args);
      $this->_args['!message'] = format_string($this->message, $args);
    }

    $this->updateMessage();
  }

  /**
   * Add a context variable.
   *
   * @param $name
   *   The variable name.
   * @param $variable
   *   The variable to add to the context.
   *
   * @return Drupal\maps_suite\Exception\Exception
   *   The current Exception object.
   */
  public function addContext($name, $variable) {
    $this->_context[$name] = $variable;
    return $this;
  }

  /**
   * Add current context to the message and reset
   * the &_context array.
   *
   * @return Drupal\maps_suite\Exception\Exception
   *   The current Exception object.
   */
  public function updateMessage() {
    $this->renderContext();
    $this->_context = array();
    $this->message = strtr($this->_unprocessed, $this->_args);
    return $this;
  }

  /**
   * Render the context as HTML and add it to the
   * $_args array.
   */
  protected function renderContext() {
    if ($this->_context) {
      $string = array();

      if (drupal_strlen($this->_args['!context'])) {
        $string[] = $this->_args['!context'];
      }

      foreach (array_keys($this->_context) as $name) {
        $string[] = 'Variable "' . drupal_substr($name, 1) . '": ' . $name;
      }

      $string = implode('<hr />', $string);
      maps_suite_format_string($string, $this->_context);
      $this->_args['!context'] = format_string($string, $this->_context);
    }
  }

  /**
   * Get the Exception context.
   *
   * @return array
   */
  public function getContext() {
    return $this->_context;
  }

  /**
   * Logs a message using Drupal log system.
   *
   * We do not use watchdog_exception because the exception message is
   * processed by check_plain() that transform HTML tags to plain text.
   *
   * @see watchdog()
   */
  public function watchdog($severity = WATCHDOG_ERROR, $link = NULL) {
    $this->updateMessage();
    watchdog('maps_suite', $this->_unprocessed, $this->_args, $severity, $link);
  }

}
