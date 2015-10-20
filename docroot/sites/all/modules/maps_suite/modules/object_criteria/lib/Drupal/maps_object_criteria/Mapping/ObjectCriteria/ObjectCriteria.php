<?php

namespace Drupal\maps_object_criteria\Mapping\ObjectCriteria;

use Drupal\maps_import\Converter\ConverterInterface;

class ObjectCriteria {

  /**
   * The object criteria mapping id.
   *
   * @var int
   */
  private $id;

  /**
   * The related converter.
   *
   * @var ConverterInterface
   */
  private $converter;

  /**
   * The attribute id.
   *
   * @var int
   */
  private $attribute_id;

  /**
   * The relation type name.
   *
   * @var string
   */
  private $relation_type;

  /**
   * The related relation type field name.
   *
   * @var string
   */
  private $field_name;

  /**
   * The class constructor.
   */
  public function __construct($definition = array()) {
    foreach ($definition as $key => $value) {
      $method = 'set' . maps_suite_drupal2camelcase($key, TRUE);
      if (method_exists($this, $method)) {
        $this->$method($value);
      }
    }
  }

  /**
   * @return int
   */
  public function getAttributeId() {
    return $this->attribute_id;
  }

  /**
   * @param int $attribute_id
   */
  public function setAttributeId($attribute_id) {
    $this->attribute_id = $attribute_id;
  }

  /**
   * @return int
   */
  public function getId() {
    return $this->id;
  }

  /**
   * @param int $id
   */
  public function setId($id) {
    $this->id = $id;
  }

  /**
   * @return ConverterInterface
   */
  public function getConverter() {
    return $this->converter;
  }

  /**
   * @param ConverterInterface $converter
   */
  public function setConverter($converter) {
    $this->converter = $converter;
  }

  /**
   * @return string
   */
  public function getRelationType() {
    return $this->relation_type;
  }

  /**
   * @param string $relation_type
   */
  public function setRelationType($relation_type) {
    $this->relation_type = $relation_type;
  }

  /**
   * @return string
   */
  public function getFieldName() {
    return $this->field_name;
  }

  /**
   * @param string $field_name
   */
  public function setFieldName($field_name) {
    $this->field_name = $field_name;
  }

  /**
   * Save the object criteria mapping.
   */
  public function save() {
    $record = new \stdClass();
    $record->cid = $this->getConverter()->getCid();
    $record->relation_type = $this->relation_type;
    $record->attribute_id = $this->attribute_id;
    $record->field_name = $this->field_name;

    $pk = array();

    if ($this->getId()) {
      $record->id = $this->getId();
      $pk = 'id';
    }

    drupal_write_record('maps_object_criteria', $record, $pk);
    $this->id = $record->id;

    return $this->id;
  }

  public static function load($id) {
    $result = self::loadAllConditions(array('id' => $id));
    return is_array($result) ? reset($result) : null;
  }

  /**
   * Load all object criterias related to a converter.
   *
   * @param \Drupal\maps_import\Converter\ConverterInterface $converter
   * @return array
   */
  public static function loadAllForConverter(ConverterInterface $converter) {
    return self::loadAllConditions(array('cid' => $converter->getCid()));
  }

  /**
   * Load object criterias from an array of conditions.
   *
   * @param array $conditions
   * @return array
   */
  public static function loadAllConditions($conditions = array()) {
    $query = db_select('maps_object_criteria', 'mocm')
      ->fields('mocm');

    foreach ($conditions as $key => $value) {
      $query->condition($key, $value);
    }

    $result = $query->execute()
      ->fetchAll();

    $return = array();
    foreach ($result as $mapping) {
      $definition = array(
        'id' => $mapping->id,
        'converter' => maps_import_converter_load($mapping->cid),
        'attribute_id' => $mapping->attribute_id,
        'relation_type' => $mapping->relation_type,
        'field_name' => $mapping->field_name,
      );

      $return[] = new ObjectCriteria($definition);
    }

    return $return;
  }

  /**
   * Save a mapping into database.
   *
   * @param $relation_id
   * @param $id_object_criteria
   * @param $entity_uid
   */
  public function saveMapping($relation_id, $id_object_criteria, $entity_uid) {
    $record = new \stdClass();
    $record->object_criteria_id = $this->getId();
    $record->relation_id = $relation_id;
    $record->id_criteria = $id_object_criteria;
    $record->uid = $entity_uid;

    drupal_write_record('maps_object_criteria_mapping', $record, array());
  }

  /**
   * Get a created relation from an object criteria id and an entity uid.
   *
   * @param int $id_object_criteria
   *   The object criteria id.
   * @param string $entity_uid
   *   The entity uid.
   *
   * @return mixed The relation object.
   */
  public function getRelation($id_object_criteria, $entity_uid) {
    $mapping = db_select('maps_object_criteria_mapping', 'mocm')
      ->fields('mocm')
      ->condition('id_criteria', $id_object_criteria)
      ->condition('uid', $entity_uid)
      ->condition('object_criteria_id', $this->getId())
      ->execute()
      ->fetchAssoc();

    if (!is_null($mapping['relation_id'])) {
      return relation_load($mapping['relation_id']);
    }

    return null;
  }

  /**
   * Get all relations created from this mapping.
   *
   * @return array
   */
  public function getAllRelations() {
    $mapping = db_select('maps_object_criteria_mapping', 'mocm')
      ->fields('mocm')
      ->condition('object_criteria_id', $this->getId())
      ->execute()
      ->fetchAllAssoc('relation_id');

    $return = array();
    foreach ($mapping as $element) {
      $return[] = relation_load($element->relation_id);
    }

    return $return;
  }

  /**
   * Delete.
   * @todo implements batch API.
   *
   * @param string $mode
   */
  public function delete($mode = 'unlink') {
    if ($mode === 'delete') {
      // We have to delete all the created relations.
      $relations = $this->getAllRelations();

      foreach ($relations as $relation) {
        relation_delete($relation->rid);
      }
    }

    db_delete('maps_object_criteria_mapping')
      ->condition('object_criteria_id', $this->getId())
      ->execute();

    db_delete('maps_object_criteria')
      ->condition('id', $this->getId())
      ->execute();
  }

}