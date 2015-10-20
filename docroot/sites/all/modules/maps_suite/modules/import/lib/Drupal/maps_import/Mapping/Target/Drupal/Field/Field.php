<?php

/**
 * @file
 * Abstract class for Drupal Field.
 */

namespace Drupal\maps_import\Mapping\Target\Drupal\Field;

use Drupal\maps_import\Converter\ConverterInterface;

abstract class Field implements FieldInterface {

  /**
   * The field id.
   *
   * @var string
   */
  private $id;

  /**
   * The field name.
   *
   * @var string
   */
  private $name;

  /**
   * The field label.
   *
   * @var string
   */
  private $label;

  /**
   * The field description.
   *
   * @var string
   */
  private $description;

  /**
   * The field type
   *
   * @var string
   */
  private $type;

  /**
   * The array of options.
   *
   * @var array
   */
  private $options;

  /**
   * Whether the property is translatable.
   *
   * @var boolean
   */
  protected $translatable = FALSE;

  /**
   * @inheritdoc
   */
  public function __construct(array $definition) {
    foreach ($definition as $name => $property) {
      $this->{$name} = $property;
    }
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
  public function getKey() {
    return $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    return $this->label;
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
  public function getId() {
    return $this->id;
  }

  /**
   * @inheritdoc
   */
  public function isTranslatable() {
    return (bool) $this->translatable;
  }

  /**
   * @inheritdoc
   */
  public function isMultiple() {
    return 0 === strpos($this->type, 'list<');
  }

  /**
   * @inheritdoc
   */
  public function getCardinality () {
    $field = field_info_field($this->getId());
    return $field['cardinality'];
  }

  /**
   * @inheritdoc
   *
   * @todo remove that!
   */
  public static function load(ConverterInterface $converter, $name) {
    return $converter->getMapping()->getTargetField($name);
  }

  /**
   * @inheritdoc
   */
  public function getDescription(array $granularity, $separator = ' - ') {
    $description = array();

    if (in_array('label', $granularity)) {
      $description[] = $this->getLabel();
    }

    if (in_array('id', $granularity)) {
      $description[] = $this->getId();
    }

    if (in_array('type', $granularity)) {
      $description[] = $this->getType();
    }

    if (in_array('translatable', $granularity) && $this->isTranslatable()) {
      $description[] = t('Translatable');
    }

    if (in_array('multiple', $granularity)) {
      $description[] = $this->isMultiple() ? t('Multiple') : t('Single value');
    }

    return implode($separator, $description);
  }

  /**
   * @inheritdoc
   */
  public function sanitize($values) {
    // Force the return to be an array
    // for allowing us to have the same process for multiple and single values.

//    $values = array($values);

    return $values;
  }

  /**
   * @inheritdoc
   */
  public function hasOptions() {
    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function getOptionsDefault() {
    return array();
  }

  /**
   * @inheritdoc
   */
  public function postProcess(ConverterInterface $converter, $mapsEntity, $drupalEntity) {
    return TRUE;
  }

  /**
   * Set the field options.
   *
   * @param array $options
   */
  public function setOptions(array $options) {
    $this->options = $options;
  }

  /**
   * Get the field options.
   *
   * @return array
   */
  public function getOptions() {
    return $this->options;
  }

}
