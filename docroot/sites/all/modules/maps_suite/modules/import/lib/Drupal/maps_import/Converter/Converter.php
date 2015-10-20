<?php

/**
 * @file
 * Abstract class that represents a converter.
 */

namespace Drupal\maps_import\Converter;

use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Exception\ConverterException;
use Drupal\maps_import\Filter\FilterInterface;
use Drupal\maps_import\Mapping\MappingInterface;
use Drupal\maps_import\Operation\Entity\Delete\Delete as EntityDelete;
use Drupal\maps_import\Profile\Profile;

abstract class Converter implements ConverterInterface {

  /**
   * The converter ID.
   *
   * @var int
   */
  private $cid = NULL;

  /**
   * The MaPS profile ID.
   *
   * @var Profile
   */
  private $profile;

  /**
   * The converter title.
   *
   * @var string
   */
  private $title = '';

  /**
   * The converter name.
   *
   * @var string
   */
  private $name = '';

  /**
   * The converter description.
   *
   * @var string
   */
  private $description = '';

  /**
   * The unique ID for imported objects.
   *
   * @var string
   */
  private $uid = NULL;

  /**
   * The unique ID scope.
   *
   * @var string
   */
  private $uid_scope = self::SCOPE_GLOBAL;

  /**
   * The entity type.
   *
   * @var string
   */
  private $entity_type = NULL;

  /**
   * The entity bundle.
   *
   * @var string
   */
  private $bundle = NULL;

  /**
   * The converter weight.
   *
   * @var int
   */
  private $weight = 0;

  /**
   * The converter options.
   *
   * @var array
   */
  private $options = array();

  /**
   * The converter filter
   *
   * @var FilterInterface
   */
  private $filter = NULL;

  /**
   * The parent id.
   *
   * @var int
   */
  private $parentId = 0;

  /**
   * The mapping object.
   *
   * @var MappingInterface
   */
  protected $mapping;

  /**
   * The table name that stores this converter data.
   *
   * @var string
   */
  protected $baseTable = 'maps_import_converter';

  /**
   * @inheritdoc
   */
  public function __construct(Profile $profile) {
    $this->profile = $profile;
  }

  /**
   * @inheritdoc
   */
  public function getCondition($condition_id) {
    foreach ($this->getFilter()->getFlattenConditions() as $condition) {
      if ((int) $condition->getId() === (int) $condition_id) {
        return $condition;
      }
    }
  }

  /**
   * Get the conditions that are also containers.
   *
   * @return array
   *   An array whose keys are container ids and whose values are
   *   container titles.
   */
  public function getConditionContainers() {
    $containers = array();

    foreach ($this->getFilter()->getFlattenConditions() as $key => $condition) {
      if ($condition->isContainer()) {
        // @todo use getter and setter for "depth".
        $containers[$condition->getId()] = str_repeat('--', $condition->depth)  . $condition->getLabel();
      }
    }

    return $containers;
  }

  /**
   * @inheritdoc
   */
  public function getCid() {
    return $this->cid;
  }

  /**
   * @inheritdoc
   */
  public function setCid($cid) {
    $this->cid = $cid;
  }

  /**
   * @inheritdoc
   */
  public function getPid() {
    return $this->getProfile()->getPid();
  }

  /**
   * @inheritdoc
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * @inheritdoc
   */
  public function setProfile(Profile $profile) {
    $this->profile = $profile;
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * @inheritdoc
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * @inheritdoc
   */
  public function getName() {
    return $this->name;
  }

  /**
   * @inheritdoc
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * @inheritdoc
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * @inheritdoc
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * @inheritdoc
   */
  public function setUid($uid) {
    $this->uid = $uid;
  }

  /**
   * @inheritdoc
   */
  public function getUidScope() {
    return (int) $this->uid_scope;
  }

  /**
   * @inheritdoc
   */
  public function setUidScope($uid_scope) {
    $this->uid_scope = $uid_scope;
  }

  /**
   * @inheritdoc
   */
  public function getEntityType() {
    return $this->entity_type;
  }

  /**
   * @inheritdoc
   */
  public function setEntityType($entity_type) {
    $this->entity_type = $entity_type;
  }

  /**
   * @inheritdoc
   */
  public function getBundle() {
    return $this->bundle;
  }

  /**
   * @inheritdoc
   */
  public function setBundle($bundle) {
    $this->bundle = $bundle;
  }

  /**
   * @inheritdoc
   */
  public function getWeight() {
    return $this->weight;
  }

  /**
   * @inheritdoc
   */
  public function setWeight($weight) {
    $this->weight = $weight;
  }

  /**
   * @inheritdoc
   */
  public function getOptions() {
    return $this->options;
  }

  /**
   * @inheritdoc
   */
  public function setOptions($options) {
    if (is_array($options)) {
      $this->options = $options;
    }
  }

  /**
   * @inheritdoc
   *
   * @todo use a Cache class for this.
   */
  public function getFilter() {
    $records = maps_suite_get_records('maps_import_converter_conditions', 'id', array('cid' => (int) $this->getCid()), \PDO::FETCH_ASSOC);
    $data = array();

    foreach ($records as $element) {
      $data[] = new $element['class']($this, $element);
    }

    $filterClass = 'Drupal\\maps_import\\Filter\\' . maps_suite_drupal2camelcase($this->getType());
    $this->filter = new $filterClass($data);

    return $this->filter;
  }

  /**
   * Unset the Filter instance.
   */
  public function unsetFilter() {
    if (isset($this->filter)) {
      unset($this->filter);
    }
  }

  /**
   * Get information about entity that are supported by
   * Entity API.
   *
   * @return array
   */
  public function entityInfo() {
    return array_intersect_key(entity_get_info(), entity_get_property_info());
  }

  /**
   * Get some configuration items.
   *
   * @param $types
   *   Either an array of settings types or a string.
   *
   * @return array
   *   An array of settings grouped by type then keyed
   *   by their ID.
   */
  public function getConfigSettings($types) {
    static $settings = array();

    if (is_array($types)) {
      $types = array_unique(array_filter($types));
      sort($types);
      $key = implode(':', $types);
    }
    else {
      $key = $types;
    }

    if (!isset($settings[$key])) {
      $settings[$key] = $this->getProfile()->getConfigurationTypes($types, $this->getProfile()->getLanguage());
    }

    return $settings[$key];
  }

  /**
   * @inheritdoc
   */
  public function getParentId() {
    return (int) $this->parentId;
  }

  /**
   * @inheritdoc
   */
  public function setParentId($parentId) {
    if ($this instanceof Child\ChildInterface) {
      $this->parentId = (int) $parentId;
    }
  }

  /**
   * @inheritdoc
   */
  public function delete(array $options = array()) {
    $options += array(
      'mode' => 'unlink',
      'new_cid' => NULL,
      'exception' => FALSE,
      'check_converter' => TRUE,
    );

    if ($options['mode'] === 'reassign') {
      if (empty($options['new_cid']) || ($options['check_converter'] && !CacheConverter::getInstance()->loadSingle($options['new_cid']))) {
        if ($options['exception']) {
          throw new ConverterException('The converter ID "@new_id" is not valid or do not match any existing converter. It is impossible to reassign any entity to such cid, the deletion of the converter @title (@id) has been skipped.', 0, array('@new_id' => $options['new_cid'], '@title' => $this->getTitle(), '@id' => $this->getCid()));
        }

        if (empty($options['new_cid']) || $options['check_converter']) {
          // Fallback to the default mode.
          $options['mode'] = 'unlink';
        }
      }
    }

    $to_delete_tables = array(
      'maps_import_converter',
      'maps_import_converter_conditions',
      'maps_import_mapping_item',
    );

    // Get children converters.
    $converters = db_select('maps_import_converter')
      ->fields('maps_import_converter', array('cid'))
      ->condition('parent_id', $this->getCid())
      ->execute()
      ->fetchAllAssoc('cid');

    $converters = array_keys($converters);
    $converters[] = $this->getCid();

    if ($options['mode'] === 'delete') {
      $operation = new EntityDelete($this);

      if (!empty($GLOBALS['drupal_test_info']['test_run_id'])) {
        $operation->process();
      }
      else {
        batch_set(array(
          'operations' => $operation->batchOperations(),
          'title' => t('Deleting all entities created by the converter %title.', array('%title' => $this->getTitle())),
        ));
      }
    }

    if ($options['mode'] === 'unlink') {
      $to_delete_tables[] = MappingInterface::DB_ENTITIES_TABLE;

      $rows = db_select(MappingInterface::DB_ENTITIES_TABLE)
        ->fields(MappingInterface::DB_ENTITIES_TABLE, array('id', 'entity_id'))
        ->condition('cid', $converters)
        ->execute()
        ->fetchAllKeyed();

      if ($correspondence_ids = array_keys($rows)) {
        db_delete('maps_import_object_ids')
          ->condition('correspondence_id', $correspondence_ids)
          ->execute();

        db_delete('maps_import_media_ids')
          ->condition('correspondence_id', $correspondence_ids)
          ->execute();
      }
    }

    // Remove stored data.
    foreach ($to_delete_tables as $table) {
      db_delete($table)
        ->condition('cid', $converters)
        ->execute();
    }

    // Re-assign entities to another converter.
    if ($options['mode'] === 'reassign') {
      db_update(MappingInterface::DB_ENTITIES_TABLE)
        ->fields(array('cid' => $options['new_cid']))
        ->condition('cid', $this->getCid())
        ->execute();
    }
  }

}
