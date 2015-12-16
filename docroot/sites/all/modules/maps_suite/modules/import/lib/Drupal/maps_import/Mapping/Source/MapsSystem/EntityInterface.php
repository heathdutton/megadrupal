<?php

/**
 * @file
 * Provide an interface to wrap MaPS System® entities (object, media, etc.).
 */
namespace Drupal\maps_import\Mapping\Source\MapsSystem;

use Drupal\maps_import\Converter\ConverterInterface;

/**
* The MaPS System® Entity interface.
*/
interface EntityInterface {

  /**
   * Class constructor.
   *
   * @param $entity
   *   The raw entity array, as stored in the database.
   */
  public function __construct(array $entity);

  /**
   * Get the entity related profile.
   *
   * @return Drupal\maps_import\Profile\Profile
   *   The profile instance.
   */
  public function getProfile();

  /**
   * Associate an other entity with the current instance.
   *
   * This allows to take care of MaPS System® aliases, that are identical entities
   * with optionally different values for some base properties.
   *
   * @todo accept an entity object and not an array.
   */
  public function addRelatedEntity(array $entity);

  /**
   * Whether the entity is published or not.
   */
  public function isPublished();

  /**
   * Set values for the specified property in the raw values of the entity.
   *
   * @param PropertyWrapperInterface $property
   *   The related property.
   * @param array $options
   *   Some specific options that are defined in the mapping UI.
   * @param ConverterInterface $currentConverter
   *   The current converter (some knid of context).
   * @param bool $log
   */
  public function setRawValues(PropertyWrapperInterface $property, $options = array(), ConverterInterface $currentConverter, $log = TRUE);

  /**
   * Whether the entity has values for the specified property.
   *
   * @param $property
   *   The property to search values.
   * @param $language_id
   *   The language in which the values must be.
   *
   * @return Boolean
   *   The boolean indicating if the property has values for the specified property.
   */
  public function hasValues(PropertyWrapperInterface $property, $language_id = 0);

  /**
   * Return the entity's attributes.
   *
   * @return array
   *   The attributes.
   */
  public function getAttributes();

  /**
   * Return the related entities.
   *
   * @return array
   *   The related entities.
   */
  public function getRelatedEntities();

  /**
   * Return the raw values.
   *
   * @return array
   *   The raw values.
   */
  public function getRawValues();

  /**
   * Return the updated property.
   *
   * @return int
   *   The updated property.
   */
  public function getUpdated();

  /**
   * Set the updated property.
   *
   * @param $updated
   *   The updated property.
   */
  public function setUpdated($updated);

}
