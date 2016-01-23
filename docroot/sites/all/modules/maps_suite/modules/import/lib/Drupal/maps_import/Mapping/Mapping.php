<?php

/**
 * @file
 * @todo make this exportable using ctools module.
 * Abstract class for Mapping.
 */

namespace Drupal\maps_import\Mapping;

use Drupal\maps_import\Cache\Data\DrupalFields as CacheDrupalFields;
use Drupal\maps_import\Cache\Data\MapsAttributes as CacheMapsAttributes;
use Drupal\maps_import\Cache\Data\MappingItems as CacheMappingItems;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Mapping\Mapper\Mapper;
use Drupal\maps_import\Mapping\Source\MapsSystem as MS;
use Drupal\maps_import\Mapping\Target\Drupal\Field;
use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_suite\Log\Context\Context as LogContext;

/**
 * Base MaPS Import Mapping class.
 */
abstract class Mapping implements MappingInterface {

  /**
   * Stock the current MaPS System® entity.
   *
   * @var MS\Entity
   */
  public static $currentEntity;

  /**
   * The related converter.
   *
   * @var ConverterInterface
   */
  private $converter;

  /**
   * The array of mapping items.
   *
   * @var ArrayObject
   */
  protected $items;

  /**
   * The mapping type.
   *
   * @var string
   */
  protected $type;

  /**
   * @inheritdoc
   */
  public function __construct(ConverterInterface $converter) {
    $this->converter = $converter;
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
  public function getType() {
    return $this->type;
  }

  /**
   * @inheritdoc
   */
  public function process(MS\Entity $entity) {
    self::$currentEntity = $entity;

    Mapper::log()
      ->addContext(new LogContext('source'), 'child')
      ->addContext(new LogContext('properties'), 'child');

    try {
      // Set raw values in MaPS Entity.
      foreach ($this->getItems() as $item) {
        $entity->setRawValues($item->getProperty(), $item->getOptions(), $this->getConverter());
      }

      // Retrieve existing entities.
      $converterUid = $this->getConverter()->getParentId() > 0 ? $this->getConverter()->getParent()->getUid() : $this->getConverter()->getUid();
      $uidProperty = MS\PropertyWrapper::load($this->getConverter(), $converterUid);
      $uids = $uidProperty->getValues($entity);
      $uid = reset($uids);

      $existingEntities = $this->getMappedEntities($uid);

      // Retrieve entity information and Drupal Entity class.
      $entityInfo = entity_get_info($this->getConverter()->getEntityType());
      $drupalEntityClass = (class_exists($entityInfo['maps_import_handler'])) ? $entityInfo['maps_import_handler'] : 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Entity';

      Mapper::log()
        ->moveToParent('source')
        ->moveToParent()
        ->addContext(new LogContext('target'), 'child')
        ->addContext(new LogContext('fields'), 'child');

      // Create a new Drupal Entity.
      $drupalEntity = new $drupalEntityClass($this->getConverter(), $existingEntities);

      // If the entity is not published and should be deleted, do not process it.
      if (!$entity->isPublished()) {
        $options = $this->getConverter()->getParentId() ? $this->getConverter()->getParent()->getOptions() : $this->getConverter()->getOptions();

        // If the entity does not handle publication feature, delete it. If it
        // handles such feature but is configured to be deleted, process it now.
        if (!$drupalEntity->hasPublicationFeature() || (!empty($options['status']) && $options['status'] === 'delete')) {
          // @todo why passing the converter to this methos, since the converter
          // is already passed to the constructor???
          $drupalEntity->deleteEntities($this->getConverter());
          return array();
        }
      }

      // Set values in Drupal Entity.
      foreach ($this->getItems() as $item) {
        if (!$drupalEntity->getTranslationHandler()->setValue($item->getField(), $item->getProperty(), $entity, $item->isRequired())) {
          return FALSE;
        }
      }

      try {
        // Save the Drupal Entity.
        $drupalEntity->presave($this->getConverter()->getEntityType(), $this->getConverter()->getBundle());
        $drupalEntity->save();

        // Insert the identifiers in maps_import tables.
        $identifiers = $drupalEntity->getIdentifiers();
        foreach ($identifiers as $id_language => $identifier) {
          $op = 'update';

          $fields = array(
            'entity_type' => $this->getConverter()->getEntityType(),
            'bundle' => $this->getConverter()->getBundle(),
            'uid' => $uid,
            'uid_scope' => $this->getConverter()->getUidScope(),
            'entity_id' => $identifier,
            'id_language' => (!empty($id_language)) ? $id_language : 0,
            'pid' => $this->getConverter()->getProfile()->getPid(),
            'cid' => $this->getConverter()->getCid(),
          );

          // We check if there is already these values in DB_ENTITIES_TABLE;
          $query_select = db_select(MappingInterface::DB_ENTITIES_TABLE)
            ->fields(MappingInterface::DB_ENTITIES_TABLE, array('id'));
          foreach ($fields as $index => $value) {
            $query_select->condition($index, $value);
          }
          $id = $query_select->execute()->fetchField();

          if (!$id) {
            $id = db_insert(MappingInterface::DB_ENTITIES_TABLE)
              ->fields($fields)
              ->execute();
            $op = 'insert';
          }

          // Check if the entry already exists in the ids table.
          $result = db_select('maps_import_' . str_replace('_mapping', '', $this->getType()) . '_ids', 'm')
            ->fields('m')
            ->condition('maps_id', $entity->getId())
            ->condition('correspondence_id', $id)
            ->execute()
            ->fetchAll();

          $pk = !empty($result) ? array('maps_id', 'correspondence_id') : array();

          $record = array(
            'maps_id' => $entity->getId(),
            'correspondence_id' => $id,
            'updated' => $entity->getUpdated(),
          );

          drupal_write_record('maps_import_' . str_replace('_mapping', '', $this->getType()) . '_ids', $record, $pk);

          // Post process.
          foreach ($this->getItems() as $item) {
            $item->getField()->postProcess($this->getConverter(), $entity, $drupalEntity);
          }

          // Manage delayed entities.
          foreach ($this->getItems('delayed') as $item) {
            $options = $item->getOptions();

            foreach ($entity->getRelatedEntities() as $relatedEntity) {
              // Retrieve the parent id.
              $parentId = $relatedEntity[$item->getProperty()->getId()];
              $parentEntityIds = self::getEntityIdFromMapsId($this->getConverter()->getType(), $parentId, $this->getConverter()->getUidScope(), $this->getConverter()->getPid());

              if (!$parentEntityIds || !$parentEntities = entity_load($options['entity_type'], $parentEntityIds)) {
                continue;
              }

              // Get the values.
              $values = array();

              foreach ($parentEntities as $parentEntity) {
                $wrapper = entity_metadata_wrapper($options['entity_type'], $parentEntity);
                $values[] = $item->getField()->sanitize($identifier);

                // Check if the entity has the given field.
                if (!isset($wrapper->{$item->getField()->getId()})) {
                  continue;
                }

                // Replace old datas
                if ($options['update_mode'] == 'replace') {
                  $wrapperValues = $wrapper->value();
                  foreach (reset($wrapperValues->{$item->getField()->getId()}) as $wrapperValue) {
                    $value = reset($wrapperValue);
                    if (empty($values[$value])) {
                      $values[$value] = $value;
                    }
                  }
                  $wrapper->{$item->getField()->getId()}->set(NULL);
                }

                // Set the values in the wrapper.
                foreach ($values as $value) {
                  $wrapper->{$item->getField()->getId()}[$value]->set($value);
                }

                try {
                  $wrapper->save();
                } catch (\Exception $e) {
                  Mapper::log()->addMessageFromException($e);
                }
              }
            }
          }
        }

        // Check if the entity is published. Deletion has been performed before
        // if applicable, so here we just call the "unpublish" method.
        if (!$entity->isPublished()) {
          $drupalEntity->unpublishEntities($this->getConverter());
        }

        // Allow to interact with the Drupal entities once they are fully
        // mapped and saved.
        foreach (module_implements('maps_import_entity_mapping_finished') as $module) {
          $function = $module . '_maps_import_entity_mapping_finished';

          if (function_exists($function)) {
            $function($drupalEntity, $entity, $this->getConverter()->getEntityType(), $this->getConverter()->getBundle(), $this->getConverter());
          }
        }
      } catch (\EntityMalformedException $e) {
        Mapper::log()
          ->addContext(new LogContext('error'), 'child')
          ->addMessageFromException($e);
      }
    } catch (MappingException $e) {
      Mapper::log()
        ->addContext(new LogContext('error'), 'child')
        ->addMessageFromException($e);

      return array();
    }

    return array($entity, $drupalEntity);
  }

  /**
   * @inheritdoc
   */
  public function getItems($type = 'default') {
    return CacheMappingItems::getInstance()->load(array(
      'converter' => $this->getConverter(),
      'type' => $type,
    ));
  }

  /**
   * @inheritdoc
   */
  public function getItem($id) {
    $items = $this->getItems();
    return isset($items[$id]) ? $items[$id] : FALSE;
  }

  /**
   * @inheritdoc
   */
  public function getSourceAttributes() {
    return CacheMapsAttributes::getInstance()->load(array(
      'profile' => $this->getConverter()->getProfile(),
    ));
  }

  /**
   * @inheritdoc
   */
  public function getSourceAttribute($id) {
    $attributes = $this->getSourceAttributes();
    return isset($attributes[$id]) ? $attributes[$id] : new MS\Attribute\Broken(array('id' => $id));
  }

  /**
   * Build the property handlers from property definitions.
   *
   * @param $properties
   *   The list of properties.
   *
   * @return array
   */
  protected function buildSourcePropertyHandlers(array $properties) {
    $handlers = array();

    foreach ($properties as $id => $property) {
      $property += array('class' => 'DefaultProperty', 'id' => $id);
      $class = __NAMESPACE__ . '\\Source\\MapsSystem\\Property\\' . $property['class'];

      if (class_exists($class) && @is_subclass_of($class, __NAMESPACE__ . '\\Source\\MapsSystem\\PropertyWrapper')) {
        $handlers['property:' . $id] = new $class($property);
      }
    }

    return $handlers;
  }

  /**
   * @inheritdoc
   */
  public function getSourceProperty($name) {
    $properties = $this->getSourceProperties();
    return isset($properties[$name]) ? $properties[$name] : new MS\Property\Broken();
  }

  /**
   * @inheritdoc
   */
  public function getTargetFields() {
    return CacheDrupalFields::getInstance()->load(array(
      'entity_type' => $this->getConverter()->getEntityType(),
      'bundle' => $this->getConverter()->getBundle(),
    ));
  }

  /**
   * @inheritdoc
   */
  public function getTargetField($name) {
    $fields = $this->getTargetFields();

    // @todo create the Broken class for Drupal fields and add an additional
    // use statement on the top of current file with an alias of BrokenField.
    return isset($fields[$name]) ? $fields[$name] : new Field\Broken(array());
  }

  /**
   * @inheritdoc
   */
  public function getMappedEntities($uid) {
    $query = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e')
      ->fields('e')
      ->condition('e.entity_type', $this->getConverter()->getEntityType())
      ->condition('e.bundle', $this->getConverter()->getBundle())
      ->condition('e.uid', $uid)
      ->condition(db_or()
        ->condition('e.uid_scope', array(0, 1))
        ->condition(db_and()
          ->condition('e.uid_scope', 2)
          ->condition('e.pid', $this->getConverter()->getProfile()->getPid())))
      ->orderBy('abs(e.pid - ' . $this->getConverter()->getProfile()->getPid() . ')', 'ASC')
      ->orderBy('abs(e.cid - ' . $this->getConverter()->getCid() . ')', 'ASC');

    // @todo this could override some other existing contents...
    $result = $query->execute()->fetchAllAssoc('id_language', \PDO::FETCH_ASSOC);
    return $result ? $result : array();
  }

  /**
   * @inheritdoc
   */
  public static function getEntityFromMapsId($type, $mapsId, $uid_scope = 1, $pid = 0) {
    $table = 'maps_import_' . $type . '_ids';

    $query = db_select(MappingInterface::DB_ENTITIES_TABLE, 'e');
    $query->join($table, 't', 'e.id = t.correspondence_id');
    $query
      ->fields('e')
      ->condition(db_or()
        ->condition('e.uid_scope', array(0, 1))
        ->condition(db_and()
          ->condition('e.uid_scope', 2)
          ->condition('e.pid', $pid)))
      ->condition('t.maps_id', $mapsId);

    $result = $query->execute()->fetchAllAssoc('id_language', \PDO::FETCH_ASSOC);

    return !empty($result) ? $result : array();
  }

  /**
   * @inheritdoc
   */
  public static function getEntityIdFromMapsId($type, $mapsId, $uid_scope = 1, $pid = 0) {
    if ($result = self::getEntityFromMapsId($type, $mapsId, $uid_scope, $pid)) {
      foreach ($result as &$row) {
        $row = $row['entity_id'];
      }
    }

    return !empty($result) ? $result : array();
  }

  /**
   * @inheritdoc
   */
  public static function getCurrentEntity() {
    return self::$currentEntity;
  }

  /**
   * Generate a file uri.
   *
   * @param Profile $profile
   *   The related profile.
   * @param $path
   *   The path to the given file type.
   * @param $preset
   *   The file preset.
   * @param $filename
   *   The file name.
   *
   * @return string
   *   The file uri.
   */
  public function getFileUri($path, $preset, $filename) {
    return "{$this->getConverter()->getProfile()->getMediaAccessibility()}://{$this->getConverter()->getProfile()->getMediaDirectory()}/{$path}/{$preset}{$filename}";
  }

}
