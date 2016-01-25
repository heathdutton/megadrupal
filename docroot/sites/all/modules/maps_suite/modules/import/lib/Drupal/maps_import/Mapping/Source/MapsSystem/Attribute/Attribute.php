<?php

/**
 * @file
 * Class that defines operation on MaPS Object's attribute.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Attribute;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_import\Profile\Profile;

/**
 * MaPS Import Mapping class related to MaPS System® attributes.
 */
abstract class Attribute extends PropertyWrapper {

  /**
   * Whether the property is translatable.
   *
   * @var boolean
   */
  protected $translatable = FALSE;

  /**
   * Whether the property may have multiple values.
   *
   * @var boolean
   */
  protected $multiple = FALSE;

  /**
   * The criteria id.
   *
   * @var int
   */
  protected $id_criteria = NULL;

  /**
   * The MaPS System® attribute code.
   *
   * @var string
   */
  protected $code;

  /**
   * @inheritdoc
   */
  public function __construct(array $definition = array()) {
    parent::__construct($definition);
    $this->id = $definition['id'];

    $this->code = isset($definition['code']) ? $definition['code'] : FALSE;
    $this->multiple = isset($definition['multiple']) ? (bool) $definition['multiple'] : FALSE;
    $this->translatable = isset($definition['localisable']) ? (bool) $definition['localisable'] : FALSE;
    $this->typeCode = isset($definition['attribute_type_code']) ? $definition['attribute_type_code'] : FALSE;
    $this->id_criteria = isset($definition['idcriteria']) ? (int) $definition['idcriteria'] : NULL;

    if (isset($definition['data'])) {
      $this->unpackData(unserialize($definition['data']));
    }
  }

  /**
   * @inheritdoc
   */
  public function getDescription(array $granularity, $separator = ' - ') {
    $description = array();

    if (in_array('title', $granularity)) {
      $description[] = $this->getTranslatedTitle();
    }

    if (in_array('key', $granularity)) {
      $description[] = $this->getKey();
    }

    if (in_array('id', $granularity)) {
      $description[] = $this->getId();
    }

    if (in_array('code', $granularity)) {
      $description[] = $this->getCode();
    }

    if (in_array('type', $granularity)) {
      $description[] = $this->getTypeCode();
    }

    if (in_array('translatable', $granularity) && $this->translatable) {
      $description[] = t('Translatable');
    }

    if (in_array('multiple', $granularity)) {
      $description[] = $this->multiple ? t('Multiple') : t('Single value');
    }

    return implode($separator, $description);
  }

  /**
   * Get the attribute code.
   *
   * @return string
   */
  public function getCode() {
  	return $this->code;
  }

  /**
   * Unpack the attribute properties from the data array
   *
   * @param $data
   *    The attribute properties array.
   */
  public function unpackData($data) {
    foreach ($this->dataInfo() as $target => $info) {
      if (array_key_exists($info['key'], $data)) {
        $this->{$target} = !empty($info['boolean']) ? (bool) $data[$info['key']] : $data[$info['key']];
      }
    }
  }

  /**
   * Get the list of properties that need to be extracted.
   */
  public function dataInfo() {
    return array(
      'type_id' => array('key' => 'idattribute_type'),
      'type_code' => array('key' => 'attribute_type_code'),
      'multiple' => array('key' => 'multiple', 'boolean' => TRUE),
      'translatable' => array('key' => 'localisable', 'boolean' => TRUE),
    );
  }

  /**
   * @inheritdoc
   */
  public function getTranslatedTitle() {
    // Attribute title translation is handled by MaPS System®.
    return $this->title;
  }

  /**
   * @inheritdoc
   */
  public function getGroupLabel() {
    return t('Object attributes');
  }

  /**
   * @inheritdoc
   */
  public function getKey() {
    return 'attribute:' . $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getSelectLabel() {
    return $this->getDescription(array('title', 'id', 'code', 'type', 'translatable', 'multiple'));
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    return $this->getDescription(array('title', 'key', 'code', 'type', 'translatable', 'multiple'));
  }

  /**
   * @inheritdoc
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter) {
    $attributes = $entity->getAttributes();

    return (array_key_exists($this->id, $attributes)) ? $this->processValues($attributes[$this->id], $entity, $currentConverter) : NULL;
  }

  /**
   * @inheritdoc
   */
  public function exists(Profile $profile) {
    $results = $profile->getConfiguration(array(
      'id' => $this->getId(),
      'type' => 'attribute',
    ));

    return count($results) > 0;
  }

  /**
   * Get the id criteria.
   *
   * @return int
   *   The id criteria.
   */
  public function getIdCriteria() {
    return $this->id_criteria;
  }

}
