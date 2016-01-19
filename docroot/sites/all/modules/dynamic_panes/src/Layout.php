<?php

/**
 * @file
 * Contains abstract class for layout classes.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for layout classes.
 */
abstract class Layout {

  /**
   * @var object
   */
  protected $data;

  /**
   * @var LayoutHandler
   */
  protected $layoutHandler;

  /**
   * @var Region
   */
  protected $region;

  /**
   * Constructor for this layout.
   *
   * @param mixed $data
   *   Contains any data of layout.
   * @param LayoutHandler $layout_handler
   *   The layout handler.
   */
  public function __construct($data, $layout_handler) {
    $this->data = $data;
    $this->layoutHandler = $layout_handler;
    $this->region = NULL;
  }

  /**
   * Init region of this layout.
   *
   * @param string $region_name
   *   The region name to init.
   */
  protected abstract function initRegion($region_name);

  /**
   * Get layout type.
   *
   * @return string
   *   The type of this layout.
   */
  public abstract function getLayoutType();

  /**
   * Get human readable layout name.
   *
   * @return string
   *   The human readable name of this layout.
   */
  public abstract function getLayoutName();

  /**
   * Get layout ID.
   *
   * @return string
   *   The ID of this layout.
   */
  public abstract function getLayoutID();

  /**
   * Get handler of this layout.
   *
   * @return LayoutHandler
   *   The handler of this layout.
   */
  public function getLayoutHandler() {
    return $this->layoutHandler;
  }

  /**
   * Get layout uniquer ID.
   *
   * @return string
   *   The uniquer ID of this layout.
   */
  public function getLayoutUniqueID() {
    return $this->getLayoutType() . '-' . $this->getLayoutID();
  }

  /**
   * Get layout data.
   *
   * @return mixed
   *   Contains any data of layout.
   */
  public function getData() {
    return $this->data;
  }

  /**
   * Get region of this layout.
   *
   * @return Region
   *   Region attached to this layout.
   */
  public function getRegion() {
    return $this->region;
  }

  /**
   * Pre render layout.
   *
   * @param string $region_name
   *   The region name.
   */
  public function preRender($region_name) {
    $this->initRegion($region_name);
  }

  /**
   * Wrap region to region class.
   *
   * @param object $region
   *   The region object.
   *
   * @return Region|bool
   *   Region object if exist, FALSE otherwise.
   */
  protected function wrapRegion($region) {
    $class = $this->layoutHandler->getRegionClass();
    $reflection = new \ReflectionClass($class);
    if ($reflection->isSubclassOf('\Drupal\dynamic_panes\Region')) {
      return new $class($region, $this);
    }

    return FALSE;
  }
}
