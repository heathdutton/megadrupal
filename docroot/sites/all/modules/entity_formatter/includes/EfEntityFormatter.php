<?php
/**
 * @file
 * Class EfEntityFormatter
 */

/**
 * Wraps an entity using an EntityMetadataWrapper.
 *
 * Bundle specific subclasses are generated using EfEntityFormatter as a base class.
 */
class EfEntityFormatter {

  /**
   * Wrapped entity
   *
   * @var EntityMetadataWrapper
   */
  protected $entity;

  /**
   * Wrapped entity type
   *
   * @var string
   */
  protected $entityType;

  /**
   * Stores already created field formatter.
   *
   * @var array
   */
  protected $field_formatter;

  /**
   * Wrap an entity.
   *
   * @param $entity_type
   *   Entity type
   *
   * @param stdClass|int $entity
   *   Entity to wrap. Will load entity if ID is passed.
   */
  public function __construct($entity_type, $entity) {
    global $language_content;

    if (is_numeric($entity)) {
      $entity = entity_load_single($entity_type, $entity);
    }
    $this->entity = entity_metadata_wrapper($entity_type, $entity);
    $this->entityType = $entity_type;

    // Set the language to the current content language.
    $this->entity->language($language_content->language);
  }

  /**
   * Set the language to be used. Please note that EntityMetadataWrapper always
   * falls back to the language of the entity, if a field is not available in a
   * given language.
   *
   * See https://www.drupal.org/node/2453287 and https://www.drupal.org/node/2335357
   *
   * @param $language_prefix
   */
  public function setLanguage($language_prefix) {
    $this->entity->language($language_prefix);
  }

  /**
   * Retrieve the EntityMetadataWrapper object.
   *
   * @return EntityMetadataWrapper
   */
  public function entityMetadataWrapper() {
    return $this->entity;
  }

  /**
   * Retrieve the wrapped entity.
   *
   * @return mixed
   */
  public function entity() {
    return $this->entity->value();
  }

  /**
   * Retrieve the entity ID
   *
   * @return int
   */
  public function id() {
    return $this->entity->getIdentifier();
  }

  /**
   * Returns an URL to the detail page of this entity.
   *
   * @param array $options
   * @return string
   */
  public function url($options = array()) {
    $uri = entity_uri($this->entityType, $this->entity());
    return url($uri['path'], $options);
  }

  /**
   * Retrieve the entity label
   *
   * @return string
   */
  public function label() {
    return check_plain($this->entity->label());
  }

}
