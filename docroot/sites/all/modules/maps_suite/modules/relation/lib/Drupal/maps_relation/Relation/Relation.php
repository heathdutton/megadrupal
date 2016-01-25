<?php

/**
 * @file
 * MaPS Import Relation class.
 */

namespace Drupal\maps_relation\Relation;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Operation\Entity\Delete\Delete as EntityDelete;

class Relation {
  
  /**
   * The relation id.
   *
   * @var id
   */
  private $id;
  
  /**
   * The related converter.
   * 
   * @var ConverterInterface;
   */
  private $converter;
  
  /**
   * The relation type.
   * 
   * @var string
   */
  private $relation_type;
  
  /**
   * The relation endpoints.
   * 
   * @var array
   */
  private $endpoints = array();
  
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
   * Set the relation id.
   * 
   * @param int
   *   The relation id.
   */
  public function setId($id) {
    $this->id = $id;
  }
  
  /**
   * Set the relation converter.
   * 
   * @param ConverterInterface $converter.
   *   The related converter.
   */
  public function setConverter($converter) {
    $this->converter = $converter;
  }
  
  /**
   * Set the relation type.
   * 
   * @param string
   *   The relation type.
   */
  public function setRelationType($relation_type) {
    $this->relation_type = $relation_type;
  }
  
  /**
   * Set the relation endpoints.
   * 
   * @param array $endpoints.
   *   The relation endpoints.
   */
  public function setEndpoints($endpoints) {
    $this->endpoints = $endpoints;
  }
  
  /**
   * Get the relation id.
   * 
   * @return int
   *   The relation id.
   */
  public function getId() {
    return $this->id;
  }
  
  /**
   * Get the relation converter.
   * 
   * @return ConverterInterface
   *   The related converter.
   */
  public function getConverter() {
    return $this->converter;
  }
  
  /**
   * Get the relation type.
   * 
   * @return string
   *   The relation type.
   */
  public function getRelationType() {
    return $this->relation_type;
  }
  
  /**
   * Get the relation endpoints.
   * 
   * @return array
   *   The relation endpoints.
   */
  public function getEndpoints() {
    return $this->endpoints;
  }
  
  /**
   * Save the relation.
   */
  public function save() {
    $record = new \stdClass();
    $record->cid = $this->getConverter()->getCid();
    $record->relation_type = $this->relation_type;
    $record->endpoints = $this->endpoints;

    $pk = array();
    
    if ($this->getId()) {
      $record->id = $this->getId();
      $pk = 'id';
    }
    
    drupal_write_record('maps_import_relation', $record, $pk);
    
    return $record->id;
  }
  
  /**
   * Load all relations for the given converter.
   * 
   * @param ConverterInterface $converter
   *   The related converter.
   * 
   * @return array
   *   The loaded relations.
   */
  public static function loadAll(ConverterInterface $converter = NULL) {
    $query = db_select('maps_import_relation', 'r')
      ->fields('r');
    
    if (!is_null($converter)) {
      $query->condition('cid', $converter->getCid());
    }
      
    $rows = $query->execute()
      ->fetchAllAssoc('id', \PDO::FETCH_ASSOC);
    
    $return = array();
    foreach ($rows as $id => $values) {
      $values['endpoints'] = unserialize($values['endpoints']);
      $values['converter'] = maps_import_converter_load($values['cid']);
      
      $return[$id] = new Relation($values);
    }
    
    return $return;
  }
 
  /**
   * Load a relation.
   * 
   * @param ConverterInterface $converter
   *   The converter.
   * @param $id
   *   The relation id.
   * 
   * @return Relation
   *   The loaded relation.
   */
  public static function load($id, ConverterInterface $converter = NULL) {
    $relations = self::loadAll($converter);

    return isset($relations[$id]) ? $relations[$id] : FALSE;
  }
  
  /**
   * Delete the relation.
   */
  public function delete($delete_entities = FALSE) {
    if ($delete_entities) {
      $operation = new EntityDelete($this->getMappingConverter());

      if (!empty($GLOBALS['drupal_test_info']['test_run_id'])) {
        $operation->process();
      }
      else {
        batch_set(array(
          'operations' => $operation->batchOperations(),
          'title' => t('Deleting all entities created by the converter %id.', array('%title' => $this->getId())),
        ));
      }
    }  

    // Delete the relation.
    db_delete('maps_import_relation')
      ->condition('id', $this->getId())
      ->execute();
    
    // Delete all child converters link to this relation.
    db_delete('maps_import_converter')
      ->condition('pid', $this->getConverter()->getProfile()->getPid())
      ->condition('name', "relation_{$this->getId()}")
      ->execute();
  }
  
  /**
   * Get the converter used for the relation mapping.
   * 
   * @return ConverterInterface
   *   The converter used for the relation mapping.
   */
  public function getMappingConverter() {
    $result = db_select('maps_import_converter', 'c')
      ->fields('c')
      ->condition('pid', $this->getConverter()->getProfile()->getPid())
      ->condition('name', "relation_{$this->getId()}")
      ->execute()
      ->fetch();

    return maps_import_converter_load($result->cid);
  }
  
  /**
   * Return the available endpoints.
   * 
   * @return array
   *   The available endpoints.
   */
  public static function getAvailableEndpoints(ConverterInterface $converter) {
    return array(
      'current_entity' => t('Current entity'),
      'property:source_id' => t('Source id'),
      'property:parent_id' => t('Parent id'),
    );
  }
  
}
