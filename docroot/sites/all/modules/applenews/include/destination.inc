<?php

/**
 * @file
 * Destination classes.
 */

use \ChapterThree\AppleNewsAPI\Document\Components;
use \ChapterThree\AppleNewsAPI\Document\Styles;

/**
 * Base class for a configurable Apple News destination.
 *
 * These destinations may be instantiated once per export.
 */
abstract class ApplenewsDestinationSingleton {

  // @see __construct().
  protected $module;
  protected $machineName;
  protected $arguments;

  /**
   * Settings for this destination.
   *
   * @var array
   *   Associative array keyed on setting key.
   */
  public $settings = [];

  /**
   * Implements __sleep().
   */
  public function __sleep() {
    return [
      'module',
      'machineName',
      'arguments',
      'settings',
    ];
  }

  /**
   * Reference to parent export object.
   *
   * @var AppleNewsExportConfigurable
   */
  public $export;

  /**
   * Implements __construct().
   *
   * @param string $module
   *   The defining module.
   * @param string $machine_name
   *   The machine name of this object.
   * @param array $arguments
   *   Defined in hook_applenews_api(), altered by hook_applenews_api_alter().
   */
  public function __construct($module, $machine_name, array $arguments) {
    $this->module = $module;
    $this->machineName = $machine_name;
    $this->arguments = $arguments;
  }

  /**
   * Administrative label.
   */
  public function label() {
    return $this->name();
  }

  /**
   * Gets the implementation name.
   *
   * @see hook_applenews_api()
   */
  public function name() {
    module_load_include('inc', 'applenews');
    $id = _applenews_export_id($this->module, $this->machineName);
    $implementation = applenews_get_module_apis('destination')[$id];
    return $implementation['name'];
  }

  /**
   * Exports to an Apple News Document object.
   *
   * @return \ChapterThree\AppleNewsAPI\Document\Base
   *   The object.
   */
  abstract public function export();

  /**
   * Validate a source key.
   *
   * @param string $key
   *   Key to search for.
   * @param array|null $options
   *   Internal use.
   *
   * @return bool
   *   Key is valid.
   */
  public function sourceOptionValid($key, $options = NULL) {
    if (!isset($options)) {
      list($options,) = $this->getSourceOptions();
    }
    if (isset($options[$key]) && !is_array($options[$key])) {
      return TRUE;
    }
    foreach (array_filter($options, 'is_array') as $options) {
      if ($this->sourceOptionValid($key, $options)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Validates source type settings.
   */
  protected function validateSettingsSources($keys) {
    foreach ($keys as $key) {
      $value = @$this->getSetting($key);
      if (empty($value['source']) ||
        !$this->sourceOptionValid($this->getSetting($key)['source'])
      ) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Generates a configurable source setting element.
   *
   * @param string $key
   *   Settings key.
   * @param bool $url
   *   Source should be a url/uri.
   * @param bool $bundle
   *   Add resource bundle checkbox.
   */
  protected function getConfigFormSourceElement($key, $url = FALSE, $bundle = FALSE) {
    list($options, $metadata) = $this->getSourceOptions();
    $settings = $this->getSetting($key);
    $element = [
      '#type'  => 'applenews_source',
      '#tree'  => TRUE,
      'source' => [
        '#title'         => t('Source'),
        '#type'          => 'select',
        '#options'       => $options,
        '#default_value' => @$settings['source'],
      ],
    ];
    // Textarea for literal value sources.
    if (!empty($metadata['types']['value'])) {
      $element['value'] = [
        '#title'             => t('Value'),
        '#type'              => 'textarea',
        '#default_value'     => @$settings['value'],
        '#applenews_visible' => $metadata['types']['value'],
      ];
    }
    if ($url) {
      $element['source']['#description'] = t('A Drupal URI or an accessible URL.');
    }
    if ($bundle) {
      $element['bundle'] = [
        '#title'         => t('Bundle'),
        '#description'   => t('If checked, the resource will be bundled into the export, otherwise the value will be passed to Apple News. If the source is a Drupal URI the needs to be checked. If the value is a URL, if checked the export will attempt to download the resource, and if unchecked Apple News will attempt to download the resource.'),
        '#type'          => 'checkbox',
        '#default_value'     => isset($settings['bundle']) ? $settings['bundle'] : TRUE,
      ];
    }
    return $element;
  }

  /**
   * List available source options.
   *
   * @param string $ancestors
   *   Internal use.
   * @param string $entity_type
   *   Internal use.
   */
  public function getSourceOptions($ancestors = '', $entity_type = NULL) {
    module_load_include('inc', 'applenews');

    // Source implementations.
    $sources = applenews_get_module_apis('source');
    // Filters for sources.
    $value_sources = function() use(&$sources) {
      return array_filter($sources, function($info) {
        return $info['type'] == 'value';
      });
    };
    $entity_sources = function($entity_type) use(&$sources) {
      return array_filter($sources, function($info) use($entity_type) {
        /** @var ApplenewsSourceEntity $class */
        $class = $info['class'];
        return $info['type'] == 'entity' &&
          in_array($entity_type, $class::types());
      });
    };
    $field_sources = function($field_type) use(&$sources) {
      return array_filter($sources, function($info) use($field_type) {
        /** @var ApplenewsSourceField $class */
        $class = $info['class'];
        return $info['type'] == 'field' &&
          in_array($field_type, $class::types());
      });
    };

    // Array of field type keyed on field name.
    $field_types = function($entity_type, $bundles = FALSE) {
      $field_names = [];
      foreach (field_info_field_map() as $field_name => $field_info) {
        if (empty($field_info['bundles'][$entity_type])) {
          continue;
        }
        if (is_array($bundles)) {
          foreach (array_keys($field_info['bundles'][$entity_type]) as $bundle) {
            if (in_array($bundle, $bundles)) {
              $field_names[$field_name] = $field_info['type'];
            }
          }
        }
        else {
          $field_names[$field_name] = $field_info['type'];
        }
      }
      return $field_names;
    };

    $options = [];
    $metadata = [];
    // Top-level-only.
    if (!$ancestors) {
      $options += [
        '' => t('- none -'),
      ];
      // Each value source.
      foreach ($value_sources() as $source_info) {
        /** @var ApplenewsSourceValue $class */
        $class = $source_info['class'];
        foreach ($class::attributes() as $key => $attr_info) {
          $id = implode('::', [
            $ancestors . '::',
            $source_info['module'],
            $source_info['machine_name'],
            $key,
          ]);
          $label = implode(' -- ', [
            $source_info['name'],
            $attr_info,
          ]);
          $options[$id] = $label;
          $metadata['types']['value'][$id] = $id;
        }
      }
    }
    // Can be called recursively.
    if (!isset($entity_type)) {
      $entity_type = $this->export->entityType();
    }
    // Classify so we can order options.
    $opts_atts = $opts_fields = [];
    // Each entity source that applies to $entity_type.
    foreach ($entity_sources($entity_type) as $source_info) {
      // Entity attributes.
      /** @var ApplenewsSourceEntity $class */
      $class = $source_info['class'];
      foreach ($class::attributes() as $key => $attr_info) {
        $id = implode('::', [
          $ancestors . '::',
          $source_info['module'],
          $source_info['machine_name'],
          $key,
        ]);
        // Nested entity source.
        if (is_array($attr_info)) {
          $label = $attr_info['label'];
          list($opts_atts[$label], $m)
            = $this->getSourceOptions($id, $attr_info['type']);
          $metadata += $m;
        }
        // Value source.
        else {
          $label = implode(' -- ', [
            $source_info['name'],
            $attr_info,
          ]);
          $opts_atts[$id] = $label;
          $metadata['types']['entity'][$id] = $id;
        }
      }
    }
    // Each field source that applies to $entity_type.
    $bundles = empty($ancestors) ? $this->export->bundles() : FALSE;
    foreach ($field_types($entity_type, $bundles) as
             $field_name => $field_type
    ) {
      // Each field source that applies to $field_type.
      foreach ($field_sources($field_type) as $source_info) {
        // Field attributes.
        $class = $source_info['class'];
        foreach ($class::attributes() as $key => $attr_info) {
          $id = implode('::', [
            $ancestors . '::',
            $source_info['module'],
            $source_info['machine_name'],
            $entity_type,
            $field_name,
            $key,
          ]);
          // Nested entity source.
          if (is_array($attr_info)) {
            // Don't dive deeper than 1 level, we don't have a mechanism to
            // detect cycles in entity references.
            if (!empty($ancestors)) {
              continue;
            }
            $label = implode(' -- ', [
              $source_info['name'],
              $field_name,
              $attr_info['label']
            ]);
            list($opts_atts[$label], $m)
              = $this->getSourceOptions($id, $attr_info['type']);
            $metadata += $m;
          }
          // Value source.
          else {
            $label = implode(' -- ', [
              $source_info['name'],
              $field_name,
              $attr_info,
            ]);
            $opts_fields[$id] = $label;
            $metadata['types']['field'][$id] = $id;
          }
        }
      }
    }
    $options += $opts_atts + $opts_fields;
    return [$options, $metadata];
  }

  /**
   * Load a source object.
   *
   * @return ApplenewsSourceInterface
   *   A source object.
   */
  public function getSource(stdClass $value, $key_settings) {
    $implementations = applenews_get_module_apis('source');

    $key = @$key_settings['source'];
    if (empty($key)) {
      return NULL;
    }

    $source = NULL;

    // Parse key.
    // @see self::getSourceOptions()
    // Source classes may have a parent -- delineated by '::::'.
    $hierarchy = explode('::::', ltrim($key, '::::'));
    // Data needed to instantiate each object -- delineated by '::'.
    while ($hierarchy) {
      $args = explode('::', array_shift($hierarchy));
      // Verify implementation still defined.
      $module = $args[0];
      $machine_name = $args[1];
      $id = _applenews_export_id($module, $machine_name);
      if (empty($implementations[$id])) {
        $msg = t('Could not load source %machine_name defined by %module.');
        $var = [
          '%machine_name' => $machine_name,
          '%module' => $module,
        ];
        watchdog('applenews', $msg, $var, WATCHDOG_ERROR);
        return NULL;
      }
      $key = array_pop($args);
      // Referenced entity.
      if ($hierarchy) {
        $args[] = $value;
        $source = call_user_func_array('applenews_new_source', $args);
        $value = @$source->export()[$key];
      }
      else {
        // Literal value.
        $reflection = new ReflectionClass($implementations[$id]['class']);
        if ($implementations[$id]['class'] == 'ApplenewsSourceValue' ||
          $reflection->isSubclassOf('ApplenewsSourceValue')
        ) {
          // Add literal.
          $args[] = $key_settings[$key];
        }
        // Add entity.
        else {
          $args[] = $value;
        }
        // Instantiate.
        $source = call_user_func_array('applenews_new_source', $args);
      }
    }

    return $source;
  }

  /**
   * Helper to get a list of values and the configured source key.
   */
  public function getSourceValues(stdClass $value, $source_key) {
    $source = $this->getSource($value, $source_key);
    if (empty($source_key['source'])) {
      return [NULL, []];
    }
    $key = explode('::', $source_key['source']);
    $key = array_pop($key);
    if (empty($source)) {
      return [$key, []];
    }
    $values = $source->export();
    // Handle single/multiple values similarly, as a list of values.
    if (!isset($values[0])) {
      $values = [$values];
    }
    return [$key, $values];
  }

  /**
   * Declare settins and defaults.
   *
   * @return array
   *   Hash of valid keys for getSetting() and setSetting() to their default
   *   value.
   */
  public function settings() {
    return [];
  }

  /**
   * Get a user-configured setting, or default value.
   *
   * @param string $key
   *   Setting key.
   *
   * @return mixed
   *   The set value, a default, or NULL if undefined.
   */
  public function getSetting($key) {
    $defaults = $this->settings();
    if (array_key_exists($key, $defaults)) {
      if (array_key_exists($key, $this->settings)) {
        return $this->settings[$key];
      }
      return $defaults[$key];
    }
    return NULL;
  }

  /**
   * Set a user-configured setting.
   *
   * @param string $key
   *   Setting key.
   * @param mixed $value
   *   Setting value.
   *
   * @return ApplenewsDestination|bool
   *   $this for chaining, or FALSE if $key undefined.
   */
  public function setSetting($key, $value) {
    $defaults = $this->settings();
    if (array_key_exists($key, $defaults)) {
      $this->settings[$key] = $value;
    }
    else {
      return FALSE;
    }
    return $this;
  }

  /**
   * Determines if the configuration is complete.
   *
   * @return bool
   *   If FALSE, then export() will return an empty array.
   */
  public function validateSettings() {
    // No required configuration.
    return TRUE;
  }

  /**
   * Provides a configuration form for this destination.
   */
  public function getConfigForm(array $values = []) {
    // Elements under 'settings' gets saved to $this->settings.
    $form['settings'] = ['#tree' => TRUE, '#weight' => 10];
    return $form;
  }

  /**
   * Validation handler for the configuration form for this export.
   */
  public function validateConfigForm($values) {
  }

  /**
   * Submit handler for the configuration form for this export.
   */
  public function submitConfigForm($values) {
    foreach (array_keys($this->settings()) as $key) {
      $this->setSetting($key, $values['settings'][$key]);
    }
  }

}

/**
 * Base class for a configurable Apple News destination.
 *
 * These destinations can be instantiated more than once per export.
 */
abstract class ApplenewsDestination extends ApplenewsDestinationSingleton {

  /**
   * Administrative title.
   */
  public $title;

  /**
   * Weight among a set of destinations.
   *
   * @var int
   */
  public $weight = 0;

  /**
   * ID of parent destination among a set of destinations.
   *
   * @var int
   */
  public $pid;

  /**
   * Unique id among a set of destinations.
   *
   * @var int|string
   */
  public $id;

  /**
   * Implements __sleep().
   */
  public function __sleep() {
    return array_merge([
      'title',
      'weight',
      'id',
      'pid',
    ], parent::__sleep());
  }

  /**
   * Depth among a set of destinations.
   *
   * @var int
   */
  public $depth;

  /**
   * {@inheritdoc}
   */
  public function label() {
    if (empty($this->title)) {
      return t('!name (!id)', ['!name' => $this->name(), '!id' => $this->id]);
    }
    return $this->title;
  }

  /**
   * Root-only -- can not be a child.
   *
   * @return bool
   *   Is a root.
   */
  public function isRoot() {
    return FALSE;
  }

  /**
   * Leaf-only -- can not have children.
   *
   * @return bool
   *   Is a leaf.
   */
  public function isLeaf() {
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {

    $form = parent::getConfigForm($values);

    $form['title'] = [
      '#type' => 'textfield',
      '#title' => t('Administrative Title'),
      '#default_value' => $this->title,
      '#weight' => -10,
    ];

    $form['weight'] = [
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $this->weight,
      '#delta' => 50,
      '#weight' => 0,
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigForm($values) {
    $this->title = @$values['title'] ? $values['title'] : '';
    $this->weight = @$values['weight'] ? $values['weight'] : 0;
    parent::submitConfigForm($values);
  }

  /**
   * Exports to an array of Apple News Document objects.
   *
   * @return array
   *   A list of \ChapterThree\AppleNewsAPI\Document\Base objects.
   */
  public function export() {
    return [];
  }

}
