<?php

/**
 * @file
 * Class for mapping items.
 */

namespace Drupal\maps_import\Mapping\Item;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Cache\Data\MappingItems as CacheMappingItems;
use Drupal\maps_import\Mapping\Target\Drupal\Field\FieldInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\Property\PropertyInterface;

class Item implements ItemInterface {

  /**
   * The mapping item id
   */
  private $id = NULL;

  /**
   * The MaPS System® property / Attribute.
   *
   * @var PropertyInterface
   */
  private $property;

  /**
   * The Drupal field.
   *
   * @var FieldInterface
   */
  private $field;

  /**
   * A boolean indicating whether the mapped value is static.
   *
   * @var bool
   */
  private $static;

  /**
   * A boolean indicatin wether the mapped field is required.
   * @todo Take care of the fields settings: required option
   */
  private $required;

  /**
   * The weight of the mapping.
   *
   * @var int
   */
  private $weight;

  /**
   * The extra options.
   *
   * @var array
   */
  private $options = array();

  /**
   * The related converter.
   *
   * @var ConverterInterface
   */
  private $converter;

  /**
   * @inheritdoc
   */
  public function __construct($definition = array()) {
    foreach ($definition as $key => $value) {
      $this->{$key} = $value;
    }
  }

  /**
   * @inheritdoc
   */
  public function getId() {
    return $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getProperty() {
    return $this->property;
  }

  /**
   * @inheritdoc
   */
  public function getField() {
    return $this->field;
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
  public function isRequired() {
    return (bool) $this->required;
  }

  /**
   * @inheritdoc
   */
  public function isStatic() {
    return (bool) $this->static;
  }

  /**
   * @inheritdoc
   */
  public function getOptions() {
    return is_array($this->options) ? $this->options : unserialize($this->options);
  }

  /**
   * @inheritdoc
   */
  public function save() {
    $record = array(
      'id' => $this->id,
      'cid' => $this->converter->getCid(),
      'property_id' => $this->getProperty()->getKey(),
      'field_name' => $this->getField()->getId(),
      'static' => (int) $this->isStatic(),
      'required' => isset($this->required) ? $this->required : 0,
      'weight' => isset($this->weight) ? $this->weight : 0,
      'options' => $this->options,
      'type' => $this->getType(),
    );

    $update = isset($this->id) ? array('id', 'type') : array();
    drupal_write_record('maps_import_mapping_item', $record, $update);

    CacheMappingItems::getInstance()->clearBinCache();

    return $record['id'];
  }

  /**
   * @inheritdoc
   */
  public function delete() {
    db_delete('maps_import_mapping_item')
      ->condition('id', $this->id)
      ->condition('type', $this->getType())
      ->execute();

    CacheMappingItems::getInstance()->clearBinCache();
  }

  /**
   * @inheritdoc
   */
  public function hasOptions() {
    return $this->property->hasOptions() || $this->field->hasOptions();
  }

  /**
   * @inheritdoc
   */
  public function getSourceOptions() {
    $options = $this->getOptions();
    return isset($options['source']) ? $options['source'] : $this->property->getOptionsDefault();
  }

  /**
   * @inheritdoc
   */
  public function getTargetOptions() {
    $options = $this->getOptions();
    return isset($options['target']) ? $options['target'] : $this->field->getOptionsDefault();
  }

  /**
   * @inheritdoc
   */
  public function getSourceOptionsForm($form, &$form_state) {
    $values = $this->getSourceOptions();
    return $this->property->optionsForm($form, $form_state, $values);
  }

  /**
   * @inheritdoc
   */
  public function getTargetOptionsForm($form, &$form_state) {
    $values = $this->getTargetOptions();
    return $this->field->optionsForm($form, $form_state, $values);
  }

  /**
   * @inheritdoc
   */
  public function optionsForm($form, &$form_state) {
    $form = array();
    $options = $this->getOptions();

    if ($this->getProperty()->hasOptions()) {
      $form['source'] = $this->getSourceOptionsForm($form, $form_state);
    }

    if ($this->getField()->hasOptions()) {
      $form['target'] = $this->getTargetOptionsForm($form, $form_state);
    }

    return $form;
  }

  /**
   * @inheritdoc
   */
  public function optionsFormValidate($form, &$form_state) {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function optionsFormSubmit($form, &$form_state) {
    foreach ($this->getSourceOptions() as $source => $value) {
      $this->options['source'][$source] = $form_state['values'][$source];
    }

    foreach ($this->getTargetOptions() as $target => $value) {
      $this->options['target'][$target] = $form_state['values'][$target];
    }

    return db_update('maps_import_mapping_item')
      ->fields(array(
        'options' => serialize($this->options),
      ))
      ->condition('id', $form_state['item']->getId())
      ->execute();
  }

  /**
   * Return the related converter.
   *
   * @return ConverterInterface
   *   The related converter.
   */
  public function getConverter() {
    return $this->converter;
  }

  /**
   * Return the mapping item type, aka "link" or "object".
   * Notice that the "object" type stand for "object" and "media".
   *
   * @return string
   *   The mapping item type.
   */
  public function getType() {
    return $this->getConverter()->getType() == 'link' ? 'link' : 'object';
  }

  /**
   * Set the item options.
   *
   * @param array $options
   *   The item options.
   */
  public function setOptions($options = array()) {
    $this->options = $options;
  }

  /**
   * Set the item id.
   *
   * @param int $id
   *   The item id.
   */
  public function setId($id) {
    $this->id = $id;
  }

}
