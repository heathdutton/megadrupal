<?php

/**
 * @file
 * Contains GoogleDfp.
 */

namespace Drupal\google_dfp;

/**
 * A class for defining an ad.
 */
class GoogleDfp implements GoogleDfpInterface {

  /**
   * The ad block delta.
   *
   * @var string
   */
  public $placement;

  /**
   * The ad block width.
   *
   * @var int
   */
  public $width;

  /**
   * The ad block height.
   *
   * @var int
   */
  public $height;

  /**
   * The ad block plugin tiers.
   *
   * @var string
   */
  public $tiers = array();

  /**
   * The ad block keyword plugins.
   *
   * @var string
   */
  public $keywords = array();

  /**
   * The ad block position.
   *
   * @var int
   */
  public $position;

  /**
   * The ad block screen targets.
   *
   * @var string
   */
  public $screen;

  /**
   * Constructs an ad object.
   *
   * @param object|NULL $config
   *   Configuration.
   */
  public function __construct($config = NULL) {
    // Init object values.
    $unserialize = array('tiers', 'keywords', 'screen');
    if ($config) {
      foreach (get_object_vars($config) as $key => $value) {
        if (in_array($key, $unserialize) && !is_array($value)) {
          $value = unserialize($value);
        }
        $this->{$key} = $value;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getTierInstances($enabled_only = FALSE) {
    return $this->getPluginInstances('tiers', 'GoogleDfp\Tier', $enabled_only);

  }

  /**
   * {@inheritdoc}
   */
  public function getKeywordInstances($enabled_only = FALSE) {
    return $this->getPluginInstances('keywords', 'GoogleDfp\Keyword', $enabled_only);
  }

  /**
   * Gets plugins instances.
   *
   * @param string $property
   *   Property to load the plugins from.
   * @param string $type
   *   Plugin type.
   * @param bool $enabled_only
   *   (optional) TRUE to return only enabled plugins. Defaults to FALSE.
   *
   * @return array[\Drupal\google_dfp\PluginInterface]
   *   An array of \Drupal\google_dfp\PluginInterface objects.
   */
  protected function getPluginInstances($property, $type, $enabled_only = FALSE) {
    $instances = array();
    foreach ($this->getPlugins($type) as $plugin_id => $plugin) {
      $class = $plugin['class'];
      if (!empty($this->{$property}[$plugin_id])) {
        $config = $this->{$property}[$plugin_id];
        $instance = $class::createInstance($plugin_id, $config);
      }
      elseif (!$enabled_only) {
        $instance = $class::createInstance($plugin_id, array('weight' => 100));
      }
      else {
        continue;
      }
      $instances[] = $instance;
    }
    uasort($instances, array('\Drupal\google_dfp\GoogleDfp', 'weightSort'));
    return $instances;
  }

  /**
   * Load the ctools plugins.
   *
   * @param string $type
   *   The plugin type.
   *
   * @return array
   *   An array of ctools plugin items.
   */
  protected function getPlugins($type) {
    ctools_include('plugins');
    return ctools_get_plugins('google_dfp', $type);
  }

  /**
   * Utility to build the data for the theme callback.
   */
  protected function getData() {
    return array(
      'placement' => check_plain($this->placement),
      'tier' => $this->getTiers(),
      'width' => check_plain($this->width),
      'height' => check_plain($this->height),
      'screen' => $this->screen,
      'keywords' => $this->getKeywords(),
      'pos' => $this->position,
    );
  }

  /**
   * Generates keywords.
   *
   * @return array
   *   Array of keywords.
   */
  protected function getKeywords() {
    $return = array();
    foreach ($this->getKeywordInstances(TRUE) as $instance) {
      if ($keywords = $instance->getKeywords()) {
        $return = array_merge($return, $keywords);
      }
    }
    return $return;
  }

  /**
   * Generates tiers.
   *
   * @return array
   *   Array of tiers.
   */
  protected function getTiers() {
    $tiers = array();
    foreach ($this->getTierInstances(TRUE) as $instance) {
      if ($tier = $instance->getTier()) {
        if (is_array($tier)) {
          foreach ($tier as $subtier) {
            $tiers[] = $subtier;
          }
        }
        else {
          $tiers[] = $tier;
        }
      }
    }
    if ($this->premium) {
      return implode('/', $tiers);
    }
    $first = array_shift($tiers);
    return $first . '/' . implode('//', $tiers);
  }

  /**
   * {@inheritdoc}
   */
  public function buildOutput() {
    $element = array(
      '#theme' => 'google_dfp_block',
      '#data' => $this->getData(),
    );
    // We only need to attach the js to the first block.
    if ($js = google_dfpd_js()) {
      $element['#attached'] = array(
        'js' => $js,
      );
    }
    return array(
      $element,
    );
  }

  /**
   * Utility method to weight sort plugin instances.
   */
  protected static function weightSort($a, $b) {
    if ($a->getConfiguration('weight') == $b->getConfiguration('weight')) {
      return 0;
    }
    return ($a->getConfiguration('weight') < $b->getConfiguration('weight')) ? -1 : 1;
  }
}
