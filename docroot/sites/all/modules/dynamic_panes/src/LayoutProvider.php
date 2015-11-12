<?php

/**
 * @file
 * Contains abstract class for layout providers.
 */

namespace Drupal\dynamic_panes;

/**
 * Abstract class for layout provider classes.
 */
abstract class LayoutProvider {

  /**
   * @var array
   */
  protected $contexts;

  /**
   * @var Layout[]
   */
  protected $layouts;

  /**
   * @var layoutHandler[]
   */
  protected $layoutHandlers;

  /**
   * Constructor for this layout provider.
   *
   * @param array $contexts
   *   An array contains ctools context objects.
   */
  public function __construct($contexts) {
    $this->contexts = $contexts;
    $this->initLayoutHandlers();
  }

  /**
   * Init layout handlers for this provider.
   */
  public function initLayoutHandlers() {
    if (!isset($this->layoutHandlers)) {
      $this->layoutHandlers = array();

      ctools_include('plugins');
      $plugins = ctools_get_plugins('dynamic_panes', 'dynamic_panes_layout_handlers');

      foreach ($plugins as $plugin) {
        if (isset($plugin['dp_provider_class']) && $plugin['dp_provider_class'] == get_called_class()) {
          $is_valid = !empty($plugin['dp_layout_class']) && !empty($plugin['dp_region_class']) && !empty($plugin['dp_block_class']);

          if ($is_valid && $class = ctools_plugin_get_class($plugin, 'handler')) {
            $reflection = new \ReflectionClass($class);
            if ($reflection->isSubclassOf('\Drupal\dynamic_panes\LayoutHandler')) {
              $this->layoutHandlers[$plugin['name']] = new $class($this->contexts, $plugin['dp_layout_class'], $plugin['dp_region_class'], $plugin['dp_block_class']);
            }
          }
        }
      }
    }
  }

  /**
   * Get layouts for this provider.
   *
   * @return array|Layout[]
   *   An array contains Layout objects or empty array.
   */
  public function getLayouts() {
    if (!isset($this->layouts)) {
      $this->layouts = array();
      foreach ($this->layoutHandlers as $layout_handler) {
        $this->layouts += $layout_handler->getLayouts();
      }
    }

    return $this->layouts;
  }
}
