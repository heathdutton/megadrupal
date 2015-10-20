<?php

/**
 * @file
 * Defines a factory for instantiating EntityTranslatableInterface instances.
 */

namespace EntityXliff\Drupal\Factories;

use EntityXliff\Drupal\Utils\DrupalHandler;

class EntityTranslatableFactory {

  /**
   * Describes when a given entity uses Entity Field Translation as its primary
   * translation mechanism (e.g. Entity Translation module).
   */
  CONST ENTITYFIELD = 'entity_translation';

  /**
   * Describes when a given entity uses Content Translation as its translation
   * mechanism (e.g. core Translation module).
   */
  CONST CONTENT = 'content_translation';

  /**
   * Describes when a given entity MAY be translatable, but its translatability
   * status is unknown to us.
   */
  CONST UNKNOWN = 'custom';

  /**
   * A singleton instance of the factory.
   *
   * @var EntityTranslatableFactory
   */
  protected static $instance;

  /**
   * @var DrupalHandler
   */
  protected $drupal;

  /**
   * @var array
   */
  protected $classMap = array();

  /**
   * Static cache of pre-built translatables. Used as a way to keep translatable
   * instances as singletons themselves.
   *
   * @var \EntityXliff\Drupal\Interfaces\EntityTranslatableInterface[]
   */
  protected $translatables = array();

  /**
   * Returns the singleton instance of the entity translatable factory.
   *
   * @param Drupalhandler $handler
   *   (optional) Inject a Drupal handler instance.
   *
   * @return EntityTranslatableFactory
   */
  public static function getInstance(DrupalHandler $handler = NULL) {
    if (!isset(self::$instance)) {
      // If no handler was provided, instantiate one ourselves.
      $handler = $handler ?: new DrupalHandler();
      self::$instance = new static($handler);
    }
    return self::$instance;
  }

  /**
   * Protected status prevents the factory from being publicly instantiated.
   */
  protected function __construct(DrupalHandler $handler) {
    $this->drupal = $handler;
    $this->getClassMap();
  }

  /**
   * Builds and returns an internal map of entity types to translatable classes.
   */
  public function getClassMap() {
    if ($this->classMap === array()) {
      foreach ($this->drupal->entityGetInfo() as $type => $definition) {
        if (isset($definition['entity xliff translatable classes'])) {
          $this->classMap[$type] = $definition['entity xliff translatable classes'];
        }
      }
    }
    return $this->classMap;
  }

  /**
   * Given an Entity wrapper, returns its corresponding Translatable.
   *
   * @param \EntityDrupalWrapper $wrapper
   *   The Entity wrapper for which a translatable is desired.
   *
   * @return \EntityXliff\Drupal\Interfaces\EntityTranslatableInterface|null
   *   Returns an instance of the entity's translatable. If no translatable
   *   class is known, NULL is returned.
   */
  public function getTranslatable(\EntityDrupalWrapper $wrapper) {
    $type = $wrapper->type();
    $key = $type . ':' . $wrapper->getIdentifier();
    if (!array_key_exists($key, $this->translatables)) {
      $this->translatables[$key] = NULL;

      $paradigm = $this->getTranslationParadigm($wrapper);
      if (isset($this->classMap[$type][$paradigm])) {
        $translatable = $this->classMap[$type][$paradigm];
        $this->translatables[$key] = new $translatable($wrapper);
      }
    }
    return $this->translatables[$key];
  }

  /**
   * @param \EntityDrupalWrapper $wrapper
   */
  public function getTranslationParadigm(\EntityDrupalWrapper $wrapper) {
    $type = $wrapper->type();
    $bundle = $wrapper->getBundle();
    $entityTranslationExists = $this->drupal->moduleExists('entity_translation');
    $contentTranslationExists = $this->drupal->moduleExists('translation');

    if ($entityTranslationExists && $this->drupal->entityTranslationEnabled($type, $bundle)) {
      return self::ENTITYFIELD;
    }
    // @todo How best to support field collection? Other similar entities?
    elseif ($contentTranslationExists && (($type === 'node' && $this->drupal->translationSupportedType($bundle)) || $type === 'field_collection_item')) {
      return self::CONTENT;
    }
    else {
      return self::UNKNOWN;
    }
  }

}
