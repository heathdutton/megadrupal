<?php

/**
 * @file
 * Interface for mapping classes.
 */

namespace Drupal\maps_import\Mapping;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\Entity as MapsSystemEntity;

/**
 * The Mapping interface.
 */
interface MappingInterface {

  /**
   * The correspondence table.
   */
  const DB_ENTITIES_TABLE = 'maps_import_entities';

  /**
   * The class constructor.
   *
   * @param $converter
   *   The related converter.
   */
  public function __construct(ConverterInterface $converter);

  /**
   * Return the mapping type.
   *
   * @return string
   */
  public function getType();

  /**
   * Get the converter instance.
   *
   * @return ConverterInterface
   */
  public function getConverter();

  /**
   * Perform the mapping operation on a MaPS System® entity.
   *
   * @param $entity
   *   The MaPS System® entity.
   *
   * @return array
   *   An array containing the MaPS System® entity at index 0,
   *   and the Drupal Entity at index 1.
   *
   */
  public function process(MapsSystemEntity $entity);

  /**
   * Return available properties for a MaPS System® entity.
   *
   * @return array
   */
  public function getSourceProperties();

  /**
   * Return a MaPS System® entity specific property.
   *
   * @param $id
   *   The property name.
   *
   * @return Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper
   */
  public function getSourceProperty($id);

  /**
   * Get all the MaPS System® available attributes.
   *
   * @return array
   */
  public function getSourceAttributes();

  /**
   * Get a specific attribute.
   *
   * @param $id
   *   The attribute ID.
   *
   * @return Drupal\maps_import\Mapping\Source\MapsSystem\Attribute\AttributeInterface
   */
  public function getSourceAttribute($id);

  /**
   * Get applicable Drupal entity fields and perperties.
   */
  public function getTargetFields();

  /**
   * Get a specific Drupal entity field from its name.
   *
   * @return Drupal\maps_import\Mapping\Target\Drupal\Field\
   *   The requested field if applicable to current Drupal entity type or a
   *   broken field instance.
   */
  public function getTargetField($name);

  /**
   * Return all the converter related mapping items.
   *
   * @param string $type
   *   A string that may be:
   *   - all: Get all the related mapping items.
   *   - delayed: Retrieve only the items that relates to delayed entities.
   *   - default: (or any other value) The root mapping items.
   *
   * @return array
   */
  public function getItems($type = 'default');

  /**
   * Return a specific mapping item.
   *
   * @param $id
   *   The mapping item ID.
   *
   * @return Drupal\maps_import\Mapping\Item\ItemInterface
   *   The mapping item or FALSE if not exists.
   */
  public function getItem($id);

  /**
   * Get a list of mapped entities that match the given conditions.
   *
   * @param $uid
   *   The unique identifier.
   *
   * @return array
   *   An array containing the entity information, keyed by MaPS System® language ID.
   */
  public function getMappedEntities($uid);

  /**
   * Get the Drupal Entity id from the MaPS id.
   *
   * @param $type
   *   The type of entity (object or media).
   * @param $mapsId
   *   The MaPS id.
   *
   * @return int
   *   The Drupal Entity id.
   */
  public static function getEntityIdFromMapsId($type, $mapsId, $uid_scope = 1);

  /**
   * Get the current entity.
   *
   * @check Cleaner way to manage files.
   * @return MS\Entity
   */
  public static function getCurrentEntity();

}
