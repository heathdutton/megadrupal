<?php

/**
 * @file
 * Class that defines a Drupal target Entity.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal;

use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface as DrupalEntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\Translation\TranslationInterface;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Converter\Converter;

class Entity implements EntityInterface {

  /**
   * The converter instance
   *
   * @var Converter
   */
  protected $converter;

  /**
   * The translation handler.
   *
   * @var TranslationInterface
   */
  protected $translationHandler;

  /**
   * The list of entities that represent the current wrapper.
   *
   * @var array
   */
  protected $entities;

  /**
   * The primary key in the related table.
   *
   * @var string
   */
  protected $primaryKey = 'id';

  /**
   * @inheritdoc
   */
  public function __construct(ConverterInterface $converter, array $existingEntities = array()) {
    $this->converter = $converter;

    $class = (class_exists($this->getTranslationClass())) ? $this->getTranslationClass() : __NAMESPACE__ . '\\Translation\\NoTranslation';
    $this->translationHandler =  new $class($this, $existingEntities);
  }

  /**
   * @inheritdoc
   */
  public function getConverter() {
    return $this->converter;
  }

  /**
   * @inheritdoc
   */
  public function getProfile() {
    return $this->getConverter()->getProfile();
  }

  /**
   * @inheritdoc
   */
  public function getEntityType() {
    return $this->getConverter()->getEntityType();
  }

  /**
   * @inheritdoc
   */
  public function getBundle() {
    return $this->getConverter()->getBundle();
  }

  /**
   * @inheritdoc
   */
  public function getOptions() {
    return $this->getConverter()->getOptions();
  }

  /**
   * @inheritdoc
   */
  public function getTranslationHandler() {
    return $this->translationHandler;
  }

  /**
   * @inheritdoc
   */
  public function addEntity($languageId, array $entity = array()) {
    $id = isset($entity['entity_id']) ? $entity['entity_id'] : NULL;
    $this->entities[$languageId] = $this->createEntity($languageId, $id);
  }

  /**
   * Get the expected translation class.
   *
   * Child classes should override this method when applicable.
   *
   * @return string
   *   The translation handler full class name.
   */
  protected function getTranslationClass() {
    return 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Translation\\NoTranslation';
  }

  /**
   * Create a new entity.
   *
   * @return EntityDrupalWrapper
   *   An entity wrapper object.
   */
  protected function createEntity($languageId, $entityId = NULL) {
    if (entity_type_supports($this->getEntityType(), 'create')) {
      $info = entity_get_info($this->getEntityType());
      $values = array();

      if (isset($entityId)) {
        $id_key = isset($info['entity keys']['name']) ? $info['entity keys']['name'] : $info['entity keys']['id'];
        $values[$id_key] = $entityId;
      }

      if (!empty($info['entity keys']['bundle'])) {
        $values[$info['entity keys']['bundle']] = $this->getBundle();
      }
      $entity = entity_create($this->getEntityType(), $values);

      if (isset($entityId)) {
        if ($entity->original = entity_load_unchanged($this->getEntityType(), $entityId)) {
          $entity->is_new = FALSE;

          if (!empty($info['entity keys']['revision'])) {
            $revision_key = $info['entity keys']['revision'];
            $entity->$revision_key = $entity->original->$revision_key;
          }
        }
      }

      // Set the language.
      $entity->language = $languageId == DrupalEntityInterface::LANGUAGE_NONE ? LANGUAGE_NONE : $this->getProfile()->getLangcode($languageId);

      $wrapper = entity_metadata_wrapper($this->getEntityType(), $entity);

      // Set wrapper language and return the wrapper.
      return $wrapper->language($entity->language);
    }

    return NULL;
  }

  /**
   * @inheritdoc
   *
   * @todo why passing the converter to this methos, since the converter
   * is already passed to the constructor???
   */
  public function deleteEntities(ConverterInterface $converter) {
    if ($ids = $this->getIdentifiers()) {
      entity_delete_multiple($converter->getEntityType(), $ids);
    }
  }

  /**
   * @inheritdoc
   *
   * @todo why passing the converter to this methos, since the converter
   * is already passed to the constructor???
   */
  public function unpublishEntities(ConverterInterface $converter) {
    $this->deleteEntities($converter);
  }

  /**
   * @inheritdoc
   */
  public function hasPublicationFeature() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function getIdentifiers() {
    if (empty($this->entities)) {
      return array();
    }

    $ids = array();
    foreach ($this->entities as $languageId => $wrapper) {
      // In case of the entity is not a wrapper
      // @check is this case normal ?
      if ($wrapper->getIdentifier()) {
        $ids[$languageId] = $wrapper->getIdentifier();
      }
    }

    return $ids;
  }

  /**
   * @inheritdoc
   */
  public function getEntities() {
    return $this->entities;
  }

  /**
   * @inheritdoc
   */
  public function presave($entityType, $bundle) {
    $languages = $this->getProfile()->getLanguages(TRUE);
    $default_language_id = $this->getProfile()->getDefaultLanguage();

    foreach ($this->getEntities() as $wrapper) {
      module_invoke_all('maps_import_entity_presave', $wrapper, $entityType, $bundle);

      $entity = $wrapper->value();

      if ($handler = entity_translation_get_handler($this->getEntityType(), $entity)) {
        foreach ($languages as $language_id => $langcode) {
          $translation = array(
            'translate' => 0,
            'status' => 1,
            'language' => $langcode,
          );

          if ($language_id == $default_language_id) {
            $handler->setOriginalLanguage($languages[$default_language_id]);
          }
          else {
            $translation['source'] = $languages[$default_language_id];
          }

          if (!empty($entity->uid)) {
            $translation['uid'] = $entity->uid;
          }

          $handler->setTranslation($translation);
        }

        $info = entity_get_info($this->getEntityType());
        $translationsKey = isset($info['entity keys']['translations']) ? $info['entity keys']['translations'] : NULL;

        $entity->{$translationsKey} = $handler->getTranslations();
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function save() {
    foreach ($this->getEntities() as $entity) {
      if (get_class($entity) == 'EntityDrupalWrapper') {
        $entity->save();
      }
    }
  }

}
