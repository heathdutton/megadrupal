<?php

/**
 * @file
 * Interface for a Drupal target Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Mapping\Target\Drupal\Translation\TranslationInterface;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Converter\Converter;


interface EntityInterface {

  /**
   * Defines no language for MaPS System®.
   */
  const LANGUAGE_NONE = 0;

  /**
   * Defines a neutral translation mode.
   */
  const TRANSLATION_NONE = 0;

  /**
   * Defines a translation mode based on content (Locale module from core).
   */
  const TRANSLATION_ENABLED = 1;

  /**
   * Defines a translation mode based on content (Translation module from core / i18n module).
   */
  const TRANSLATION_CONTENT = 2;

  /**
   * Defines a translation mode based on fields (Entity Trnaslation module).
   */
  const TRANSLATION_FIELD = 4;

  /**
   * Class constructor.
   *
   * @param $converter
   *   The Converter instance.
   * @param $existingEntities
   *   An array whose keys are MaPS System® language IDs and whose values
   *   are the raw entity array, as stored in the {maps_import_entities}
   *   table.
   */
  public function __construct(ConverterInterface $converter, array $existingEntities = array());

  /**
   * Get the entity related converter.
   *
   * @return Converter
   *   The converter instance.
   */
  public function getConverter();

  /**
   * Get the entity related profile.
   *
   * @return Profile
   *   The profile instance.
   */
  public function getProfile();

  /**
   * Get the entity type.
   *
   * @return string
   */
  public function getEntityType();

  /**
   * Get the entity bundle.
   *
   * @return string
   */
  public function getBundle();

  /**
   * Get the entity options.
   *
   * @return array
   */
  public function getOptions();

  /**
   * Get the translation handler.
   *
   * @return TranslationInterface
   */
  public function getTranslationHandler();

  /**
   * Add an entity to current wrapper.
   *
   * @param $languageId
   *   The MaPS System® language ID.
   * @param $entity
   *   An array defining an existing entity, if applicable.
   */
  public function addEntity($languageId, array $entity = array());

  /**
   * Return the Drupal entity identifier.
   *
   * @todo implement this
   *
   * @return array
   *   An array containing the identifiers, indexed by id language.
   */
  public function getIdentifiers();

  /**
   * Get all Drupal entity wrappers.
   *
   * @return \EntityDrupalWrapper[]
   *   An array of Drupal entity wrappers.
   */
  public function getEntities();

  /**
   * Perform some necessary operations before saving the Drupal entities.
   *
   * @param string $entityType
   *   The entity type.
   * @param string $bundle
   *   The entity bundle.
   */
  public function presave($entityType, $bundle);

  /**
   * Save all Drupal entities.
   */
  public function save();

  /**
   * Delete a series of entities.
   */
  public function deleteEntities(ConverterInterface $converter);

  /**
   * Delete a series of entities if applicable.
   */
  public function unpublishEntities(ConverterInterface $converter);

  /**
   * Whether the entity type supports publication statuses.
   */
  public function hasPublicationFeature();

}
