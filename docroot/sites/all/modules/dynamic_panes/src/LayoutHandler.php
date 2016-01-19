<?php

/**
 * @file
 * Contains abstract class for layout handlers.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for layout handler classes.
 */
abstract class LayoutHandler {

  /**
   * @var array
   */
  protected $contexts;

  /**
   * @var string
   */
  protected $layoutClass;

  /**
   * @var string
   */
  protected $regionClass;

  /**
   * @var string
   */
  protected $blockClass;

  /**
   * @var Layout[]
   */
  protected $layouts;

  /**
   * @var ContextHandler[]
   */
  protected $contextHandlers;

  /**
   * Constructor for this layout handler.
   *
   * @param array $contexts
   *   An array contains ctools context objects.
   * @param string $layout_class
   *   The class name of layout.
   * @param string $region_class
   *   The class name of region.
   * @param string $block_class
   *   The class name of block.
   */
  public function __construct($contexts, $layout_class, $region_class, $block_class) {
    $this->contexts = $contexts;
    $this->layoutClass = $layout_class;
    $this->regionClass = $region_class;
    $this->blockClass = $block_class;
    $this->initContextHandlers();
  }

  /**
   * Init context handlers for this provider.
   */
  public function initContextHandlers() {
    if (!isset($this->contextHandlers)) {
      $this->contextHandlers = array();

      ctools_include('plugins');
      $plugins = ctools_get_plugins('dynamic_panes', 'dynamic_panes_context_handlers');

      foreach ($plugins as $plugin) {
        foreach ($this->contexts as $context) {
          if (!empty($plugin['dp_layout_handler']) && $plugin['dp_layout_handler'] != get_called_class()) {
            continue;
          }

          if (!empty($plugin['dp_context_type']) && $plugin['dp_context_type'] != $context->plugin) {
            continue;
          }

          if ($class = ctools_plugin_get_class($plugin, 'handler')) {
            $reflection = new \ReflectionClass($class);
            if ($reflection->isSubclassOf('\Drupal\dynamic_panes\ContextHandler')) {
              $this->contextHandlers[$plugin['name']] = new $class($context, $this);
            }
          }
        }
      }
    }
  }

  /**
   * Get layouts for this handler.
   *
   * @return array|Layout[]
   *   An array contains Layout objects or empty array.
   */
  public function getLayouts() {
    if (!isset($this->layouts)) {
      $this->layouts = array();
      foreach ($this->contextHandlers as $context_handler) {
        $this->layouts += $context_handler->getLayouts();
      }
    }

    return $this->layouts;
  }

  /**
   * Get layout class.
   *
   * @return string
   *   The layout class name.
   */
  public function getLayoutClass() {
    return $this->layoutClass;
  }

  /**
   * Get region class.
   *
   * @return string
   *   The region class name.
   */
  public function getRegionClass() {
    return $this->regionClass;
  }

  /**
   * Get block class.
   *
   * @return string
   *   The block class name.
   */
  public function getBlockClass() {
    return $this->blockClass;
  }
}
