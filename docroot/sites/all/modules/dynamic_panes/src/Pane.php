<?php

/**
 * @file
 * Contains class for build "Dynamic pane" content type plugin.
 */

namespace Drupal\dynamic_panes;

/**
 * Class for build "Dynamic pane" content type plugin.
 */
class Pane {

  /**
   * @var array
   */
  protected $contexts;

  /**
   * @var array
   */
  protected $params;

  /**
   * @var object
   */
  protected $display;

  /**
   * @var LayoutProvider
   */
  protected $layoutProvider;

  /**
   * Constructor for this pane.
   *
   * @param array $contexts
   *   An array contains ctools context objects.
   * @param array $params
   *   An array contains configs of content type plugin.
   * @param object $display
   *   The panels display.
   *
   * @throws \Exception
   */
  public function __construct($contexts, $params, $display) {
    $this->contexts = $contexts;
    $this->params = $params;
    $this->display = $display;

    $this->layoutProvider = FALSE;

    $this->adminLinks = array();

    $this->initLayoutProvider();
    if (!$this->layoutProvider) {
      throw new \Exception('Layout provider not exist');
    };
  }

  /**
   * Init layout provider for this pane.
   */
  protected function initLayoutProvider() {
    if (!empty($this->params['layout_provider'])) {
      $provider_name = $this->params['layout_provider'];
      $providers = dynamic_panes_get_layout_providers($this->contexts);
      if (!empty($providers) && isset($providers[$provider_name])) {
        $this->layoutProvider = $providers[$provider_name];
      }
    }
  }

  /**
   * Pre render pane.
   */
  protected function preRender() {
    foreach ($this->layoutProvider->getLayouts() as $layout) {
      $layout->preRender($this->getFullRegionName());
    }
  }

  /**
   * Get layout of panels display to which attached this pane.
   *
   * @return string
   *   The layout name of display.
   */
  public function getDisplayLayout() {
    return $this->display->layout;
  }

  /**
   * Get region of layout to which attached this pane.
   *
   * @return string
   *   The region name of display.
   */
  public function getRegionName() {
    return dynamic_panes_region_is_static($this->params) ? $this->params['static_region'] : $this->params['region'];
  }

  /**
   * Get region name, which consists of a display name and the region name.
   *
   * @return string
   *   The full region name.
   */
  public function getFullRegionName() {
    $region_name = $this->getRegionName();

    if (!$this->isStatic()) {
      $region_name = $this->getDisplayLayout() . '-' . $region_name;
    }

    return $region_name;
  }

  /**
   * Get levels, which need render.
   *
   * @return array
   *   An array contains levels to render.
   */
  public function getLevels() {
    return !empty($this->params['level']) ? $this->params['level'] : array_replace(array('all' => 'all'), _dynamic_panes_get_config_levels());
  }

  /**
   * Get human readable type name of this pane.
   *
   * @return string
   *   The human readable name.
   */
  public function getTypeName() {
    return $this->isStatic() ? t('Static') : t('Dynamic');
  }

  /**
   * Checks type of this pane.
   *
   * @return bool
   *   TRUE, if this pane is static, FALSE otherwise.
   */
  public function isStatic() {
    return !empty($this->params['static_region']);
  }

  /**
   * Render pane.
   *
   * @return array
   *   An array contains rendered content.
   */
  public function render() {
    $this->preRender();

    $content = array();

    $levels = $this->getLevels();

    foreach ($this->layoutProvider->getLayouts() as $layout) {
      if ($region = $layout->getRegion()) {
        foreach ($levels as $level) {
          foreach ($region->getBlocksByLevel($level) as $block) {
            $content[$level][]['#markup'] = $block->render();
          }
        }
      }
    }

    return $content;
  }
}
