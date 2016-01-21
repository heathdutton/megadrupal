<?php

/**
 * Contains CToolsAccessBase.
 */

abstract class CToolsAccessBase implements CToolsAccessInterface {

  /**
   * Holds the CTools context.
   *
   * @var \stdClass
   */
  protected $context;

  /**
   * Default constructor.
   *
   * @param array $conf
   *   Plugin configuration.
   * @param array $context
   *   Context object.
   */
  public function __construct($conf, $context) {
    $this->setContext($context);
  }

  /**
   * @param \stdClass $context
   */
  public function setContext($context) {
    $this->context = $context;
  }

  /**
   * @return \stdClass
   */
  public function getContext() {
    return $this->context;
  }

}