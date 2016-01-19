<?php

/**
 * @file
 * Contains abstract class for context handler.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for context handlers classes.
 */
abstract class ContextHandler {

  /**
   * @var object
   */
  protected $context;

  /**
   * @var Layout[]
   */
  protected $layouts;

  /**
   * @var LayoutHandler
   */
  protected $layoutHandler;

  /**
   * Constructor for this context handler.
   *
   * @param object $context
   *   The ctools context object.
   * @param LayoutHandler $layout_handler
   *   The layout handler.
   */
  public function __construct($context, $layout_handler) {
    $this->context = $context;
    $this->layouts = array();
    $this->layoutHandler = $layout_handler;
    $this->initLayouts();
  }

  /**
   * Init layouts for this context.
   */
  protected abstract function initLayouts();

  /**
   * Add layout to $this->layouts.
   *
   * @param object $data
   *   The object contains any info about layout.
   */
  protected function addLayout($data) {
    if ($layout = $this->wrapLayout($data)) {
      $this->layouts[$layout->getLayoutUniqueID()] = $layout;
    }
  }

  /**
   * Wrap data to layout class.
   *
   * @param object $data
   *   The object contains any info about layout.
   *
   * @return Layout|bool
   *   Layout object if exist, FALSE otherwise.
   */
  protected function wrapLayout($data) {
    $class = $this->layoutHandler->getLayoutClass();
    $reflection = new \ReflectionClass($class);
    if ($reflection->isSubclassOf('\Drupal\dynamic_panes\Layout')) {
      return new $class($data, $this->layoutHandler);
    }

    return FALSE;
  }

  /**
   * Get layouts for this handler.
   *
   * @return array|Layout[]
   *   An array contains Layout objects or empty array.
   */
  public function getLayouts() {
    return $this->layouts;
  }
}
