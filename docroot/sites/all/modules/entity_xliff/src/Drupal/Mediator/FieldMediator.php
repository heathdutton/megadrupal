<?php

/**
 * @file
 * Defines a mediator between the Drupal field subsystem and the Entity XLIFF
 * translatable system.
 */

namespace EntityXliff\Drupal\Mediator;

use EntityXliff\Drupal\Interfaces\FieldHandlerInterface;
use EntityXliff\Drupal\Utils\DrupalHandler;

class FieldMediator {

  /**
   * @var DrupalHandler
   */
  protected $drupal;

  /**
   * @var array
   */
  protected $classMap = array();

  /**
   * @param DrupalHandler $handler
   *   (optional) An instance of the DrupalHandler class.
   */
  public function __construct(DrupalHandler $handler = NULL) {
    // If no handler was provided, instantiate one ourselves.
    if ($handler === NULL) {
      $handler = new DrupalHandler();
    }

    $this->drupal = $handler;
    $this->buildMap();
  }

  /**
   * Builds an internal map of entity types to translatable classes.
   */
  public function buildMap() {
    foreach ($this->drupal->entityXliffGetFieldHandlers() as $type => $info) {
      $this->classMap[$type] = $info['class'];
    }
  }

  /**
   * Given an Entity metadata wrapper, returns its corresponding field's
   * handler.
   *
   * @param \EntityMetadataWrapper $wrapper
   *   The metadata wrapper for which a field handler is desired.
   *
   * @return FieldHandlerInterface|null
   *   Returns an instance of the entity's translatable. If no translatable
   *   class is known, NULL is returned.
   */
  public function getInstance(\EntityMetadataWrapper $wrapper) {
    if ($translatable = $this->getClass($wrapper)) {
      return new $translatable();
    }
    else {
      return NULL;
    }
  }

  /**
   * Returns whether or not a given field can be translated (more specifically,
   * whether or not an FieldHandlerInterface compatible class is known for the
   * given field type.
   *
   * @param \EntityMetadataWrapper $wrapper
   *   The metadata wrapper for which translatability status is desired.
   *
   * @return bool
   *   TRUE if the given field can be translated
   */
  public function canBeTranslated(\EntityMetadataWrapper $wrapper) {
    $info = $wrapper->info();
    return isset($info['type']) && isset($this->classMap[$info['type']]);
  }

  /**
   * Returns the class for a given metadata wrapper's corresponding handler.
   *
   * @param \EntityMetadataWrapper $wrapper
   *   The metadata wrapper for which a handler class is desired.
   *
   * @return string|bool
   *   The name of the class (suitable for dynamic class instantiation) or FALSE
   *   if the field type is not known to be translatable.
   */
  public function getClass(\EntityMetadataWrapper $wrapper) {
    if ($this->canBeTranslated($wrapper)) {
      $info = $wrapper->info();
      return $this->classMap[$info['type']];
    }
    else {
      return FALSE;
    }
  }

}
