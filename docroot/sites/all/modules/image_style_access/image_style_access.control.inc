<?php
/**
 * @file
 * Image style access control implementation.
 */

/**
 * Controller for access control configurations.
 */
class ImageStyleAccessConfigController {
  /**
   * Control factory instances.
   * @var ImageStyleAccessControlFactoryInterface[]
   */
  protected $factories = array();

  /**
   * Loads a configuration.
   *
   * @param int $cid
   * @return ImageStyleAccessConfig|NULL
   */
  public function load($cid) {
    if ($configs = $this->loadMultiple(array($cid))) {
      return reset($configs);
    }
  }

  /**
   * Loads configurations by a list of identifiers.
   *
   * @param int[] $ids
   * @return ImageStyleAccessConfig[]
   */
  public function loadMultiple(array $ids = array()) {
    $configs = array();
    if ($result = $this->buildQuery(array('cid' => $ids))->execute()) {
      foreach ($result as $data) {
        $configs[$data['cid']] = $this->readConfig($data);
      }
    }
    return $configs;
  }

  /**
   * Matches configurations for download access check.
   */
  public function loadMultipleByDownload($style, $scheme, $enabledOnly = TRUE) {
    // Prepare query.
    $query = $this->buildQuery();
    if (isset($style)) {
      $query->condition(db_or()
        ->condition('style', $style)
        ->condition('style', NULL, 'IS NULL'));
    }
    if (isset($scheme)) {
      $query->condition(db_or()
        ->condition('scheme', $scheme)
        ->condition('scheme', NULL, 'IS NULL'));
    }
    if ($enabledOnly) {
      $query->condition('enabled', TRUE);
    }
    // Load configurations.
    $configs = array();
    if ($result = $query->execute()) {
      foreach ($result as $data) {
        $configs[$data['cid']] = $this->readConfig($data);
      }
    }
    return $configs;
  }

  /**
   * Loads configurations using conditions.
   */
  protected function buildQuery(array $conditions = array()) {
    // Construct query for all fields.
    $query = db_select('image_style_access_config', 'c', array('fetch' => PDO::FETCH_ASSOC))->fields('c');
    // Add conditions.
    foreach ($conditions as $key => $value) {
      if (!isset($value)) {
        $query->condition($key, NULL, 'IS NULL');
      }
      elseif (is_scalar($value)) {
        $query->condition($key, $value);
      }
      elseif (!empty($value)) {
        $query->condition($key, $value, 'IN');
      }
    }
    // Add sort criteria.
    $query->orderBy('priority')
      ->orderBy('scheme')
      ->orderBy('style')
      ->orderBy('target_base')
      ->orderBy('type')
      ->orderBy('action')
      ->orderBy('cid');

    return $query;
  }

  /**
   * Creates a configuration object from data.
   */
  protected function readConfig(array $data) {
    // Unserialize settings.
    if (!empty($data['settings']) && $settings = @unserialize($data['settings'])) {
      $data['settings'] = $settings;
    }
    return new ImageStyleAccessConfig($data);
  }

  /**
   * Returns a form controller.
   */
  public function getFormController($type) {
    $type_info = image_style_access_get_type_info();
    if (isset($type_info[$type])) {
      return new $type_info[$type]['form controller class']($type_info[$type]);
    }
  }

  /**
   * Returns a control factory.
   */
  public function getControlFactory($type) {
    if (!isset($this->factories[$type])) {
      $type_info = image_style_access_get_type_info();
      if (isset($type_info[$type])) {
        $this->factories[$type] = new $type_info[$type]['factory class']($type_info[$type]);
      }
    }
    return $this->factories[$type];
  }

  /**
   * Saves a configuration.
   */
  public function save(&$config) {
    if (!empty($config->cid)) {
      $return = drupal_write_record('image_style_access_config', $config, 'cid');
    }
    else {
      $return = drupal_write_record('image_style_access_config', $config);
    }
    $config->cid = (int) $config->cid;
    image_style_access_clear_cache();
    return $return;
  }

  /**
   * Deletes a configuration.
   */
  public function delete($cid) {
    db_delete('image_style_access_config')->condition('cid', $cid)->execute();
    image_style_access_clear_cache();
  }
}

/**
 * Access control configuration.
 */
class ImageStyleAccessConfig {
  /**
   * Serial identifier.
   */
  public $cid;

  /**
   * Administrative title.
   * @var string
   */
  public $title;

  /**
   * Download scheme.
   * @var string
   */
  public $scheme;

  /**
   * Base URI target under access control.
   * @var string
   */
  public $target_base;

  /**
   * Image style name.
   * @var string
   */
  public $style;

  /**
   * Access control type.
   */
  public $type;

  /**
   * Access control configuration settings.
   * @var array
   */
  public $settings;

  /**
   * Access control action.
   * @var string
   */
  public $action;

  /**
   * Configuration priority.
   * @var int
   */
  public $priority;

  /**
   * Whether this configuration is enabled.
   * @var bool
   */
  public $enabled;

  /**
   * Constructs a new configuration object.
   */
  public function __construct(array $data = array()) {
    // Populate values.
    $data = $this->getNormalizedData($data);
    foreach ($data as $key => $value) {
      if (isset($value)) {
        $this->$key = $value;
      }
    }
  }

  /**
   * Normalizes configuration data.
   */
  protected function getNormalizedData(array $data) {
    // Add default values.
    static $defaults = array(
      'cid' => NULL,
      'title' => '',
      'scheme' => NULL,
      'target_base' => '',
      'style' => NULL,
      'type' => 'default',
      'settings' => array(),
      'action' => NULL,
      'priority' => 0,
      'enabled' => TRUE,
    );
    $data += $defaults;
    // Normalize data types.
    if (!is_array($data['settings'])) {
      $data['settings'] = $defaults['settings'];
    }
    if (!is_int($data['priority'])) {
      $data['priority'] = (int) $data['priority'];
    }
    if (!is_bool($data['enabled'])) {
      $data['enabled'] = (bool) $data['enabled'];
    }

    return $data;
  }
}

/**
 * Access control factory interface.
 */
interface ImageStyleAccessControlFactoryInterface {
  /**
   * Creates an access control.
   *
   * @param string $style
   *   Image style name.
   * @param string $scheme
   *   Image URI scheme.
   * @param string $target
   *   Image URI target.
   * @return ImageStyleAccessControlInterface
   */
  public function createControl($style, $scheme, $target);
}

/**
 * Access control plugin interface.
 */
interface ImageStyleAccessControlInterface {
  /**
   * Matches a control in a configuration.
   *
   * @param ImageStyleAccessConfig $config
   *   Configuration for this control.
   * @return bool
   *   Whether the given configuration matches.
   */
  public function match(ImageStyleAccessConfig $config);
}

/**
 * Default control factory.
 */
class ImageStyleAccessDefaultControlFactory implements ImageStyleAccessControlFactoryInterface {
  /**
   * {@inheritdoc}
   */
  public function createControl($style, $scheme, $target) {
    return new ImageStyleAccessDefaultControl();
  }
}

/**
 * Default access control.
 */
class ImageStyleAccessDefaultControl implements ImageStyleAccessControlInterface {
  /**
   * {@inheritdoc}
   */
  public function match(ImageStyleAccessConfig $config) {
    // Always match.
    return TRUE;
  }
}

/**
 * Access configuration evaluator.
 */
class ImageStyleAccessChecker {
  /**
   * Cached controls.
   * @var ImageStyleAccessControlInterface[]
   */
  protected $controls = array();

  /**
   * Evaluates access to an image.
   *
   * @param ImageStyleAccessConfig[] $configs
   *   An array of configurations to evaluate.
   * @param string|NULL $style
   *   Style name, or NULL if original image.
   * @param string $scheme
   *   URI scheme of image to check.
   * @param string $target
   *   URI target of image to check.
   * @return string|NULL
   *   Access result: IMAGE_STYLE_ACCESS_ALLOW or IMAGE_STYLE_ACCESS_DENY.
   */
  public function checkImage(array $configs, $style, $scheme, $target) {
    $configs = $this->matchTarget($configs, $target);
    $config = $this->matchControlLast($configs, $style, $scheme, $target);
    return $config ? $config->action : NULL;
  }

  /**
   * Matches configurations on target.
   *
   * @param ImageStyleAccessConfig[] $configs
   * @param string $target
   * @return ImageStyleAccessConfig[]
   */
  public function matchTarget(array $configs, $target) {
    $matchedConfigs = array();
    $targetParts = explode('/', $target);
    foreach ($configs as $cid => $config) {
      /** @var $config ImageStyleAccessConfig */
      $targetBase = $config->target_base;

      // Match blank base.
      if (!strlen($targetBase)) {
        $matched = TRUE;
      }
      // Match blank target.
      elseif (!strlen($target)) {
        continue;
      }
      // Match base parts.
      else {
        $targetBaseParts = explode('/', $targetBase);
        $finalIndex = count($targetBaseParts) - 1;
        $matched = TRUE;
        foreach ($targetBaseParts as $index => $targetBasePart) {
          // Fail if target is shorter than base.
          if (!isset($targetParts[$index])) {
            $matched = FALSE;
            break;
          }

          // Match blank part.
          if (!strlen($targetBasePart)) {
            // Only match final part.
            if ($index < $finalIndex) {
              $matched = FALSE;
              break;
            }
          }
          // Match non-wildcard part.
          elseif ($targetBasePart[0] != '%') {
            $targetPart = $targetParts[$index];
            // Match final part as substring.
            if ($index == $finalIndex && 0 !== strpos($targetPart, $targetBasePart)) {
              $matched = FALSE;
              break;
            }
            // Match regular part.
            elseif ($targetPart != $targetBasePart) {
              $matched = FALSE;
              break;
            }
          }
        }
      }

      // Match configuration.
      if ($matched) {
        $matchedConfigs[$cid] = $config;
      }
    }
    return $matchedConfigs;
  }

  /**
   * Matches configurations on control criteria.
   *
   * @param ImageStyleAccessConfig[] $configs
   * @param string $style
   * @param string $scheme
   * @param string $target
   * @return ImageStyleAccessConfig[]
   */
  public function matchControl(array $configs, $style, $scheme, $target) {
    $matchedConfigs = array();
    foreach ($configs as $cid => $config) {
      if ($this->matchControlCriteria($config, $style, $scheme, $target)) {
        $matchedConfigs[$cid] = $config;
      }
    }
    return $matchedConfigs;
  }

  /**
   * Matches the last configuration on control criteria.
   *
   * This method is a derivative of self::matchControl() for more efficient
   * evaluation by simply evaluating in reverse and returning the first matched.
   *
   * @param ImageStyleAccessConfig[] $configs
   * @param string $style
   * @param string $scheme
   * @param string $target
   * @return ImageStyleAccessConfig
   */
  public function matchControlLast(array $configs, $style, $scheme, $target) {
    foreach (array_reverse($configs) as $config) {
      if ($this->matchControlCriteria($config, $style, $scheme, $target)) {
        return $config;
      }
    }
    return NULL;
  }

  /**
   * Matches a single control condition.
   */
  public function matchControlCriteria($config, $style, $scheme, $target) {
    $control = $this->getControl($config->type, $style, $scheme, $target);
    return $control && $control->match($config);
  }

  /**
   * Matches controls.
   */
  protected function getControl($type, $style, $scheme, $target) {
    $key = "$type:$style:$scheme:$target";
    if (!isset($this->controls[$key])) {
      $this->controls[$key] = image_style_access_config_controller()
        ->getControlFactory($type)
        ->createControl($style, $scheme, $target);
    }
    return $this->controls[$key];
  }
}
