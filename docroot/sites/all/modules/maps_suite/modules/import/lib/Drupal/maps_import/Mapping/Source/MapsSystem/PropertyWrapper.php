<?php

/**
 * @file
 * Abstract class for managing MaPS System® attributes and properties.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\RelatedEntity\DefaultRelatedEntity;
use Drupal\maps_import\Profile\Profile;

/**
 *
 */
abstract class PropertyWrapper implements PropertyWrapperInterface {

  /**
   * The property id.
   *
   * @var int
   */
  protected $id;

  /**
   * The property title.
   *
   * @var string
   */
  protected $title;

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
   * The available options.
   *
   * @var array
   */
  private $options = array();

  /**
   * The type code.
   *
   * @var string
   */
  protected $typeCode = 'string';

  /**
   * @inheritdoc
   */
  public function __construct(array $definition = array()) {
    $this->id = $definition['id'];

    if (isset($definition['title'])) {
      $this->title = $definition['title'];
    }
  }

  /**
   * @inheritdoc
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Get the attribute type code.
   *
   * @return string
   */
  public function getTypeCode() {
    return isset($this->typeCode) ? $this->typeCode : NULL;
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

    if (in_array('translatable', $granularity) && $this->translatable) {
      $description[] = t('Translatable');
    }

    if (in_array('multiple', $granularity)) {
      $description[] = $this->multiple ? t('Multiple') : t('Single value');
    }

    return implode($separator, $description);
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    return $this->getDescription(array('title', 'id', 'translatable', 'multiple'));
  }

  /**
   * @inheritdoc
   */
  public function getSelectLabel() {
    return $this->getDescription(array('title', 'id', 'translatable', 'multiple'));
  }

  /**
   * @inheritdoc
   * @todo: use the related cache class.
   */
  public static function load(ConverterInterface $converter, $id) {
    $exploded = explode(':', $id);

    if ($exploded[0] === 'media') {
      return Media\Media::createMediaFromMediaType($exploded[1], array('id' => $id));
    }
    if (isset($exploded[2])) {
      return new DefaultRelatedEntity(array('id' => $id, 'parent_id' => $exploded[2]));
    }

    $getter = 'getSource' . maps_suite_drupal2camelcase($exploded[0]);

    return $converter->getMapping()->{$getter}($id);
  }

  /**
   * @inheritdoc
   */
  public function isTranslatable() {
    return $this->translatable;
  }

  /**
   * @inheritdoc
   */
  public function isMultiple() {
    return $this->multiple;
  }

  /**
   * @inheritdoc
   */
  public function getValues(EntityInterface $entity, $language_id = NULL) {
    $values = $entity->getRawValues();

    if (is_null($language_id)) {
      if (isset($values[$this->getKey()])) {
        return $values[$this->getKey()];
      }
    }
    else {
     if (!$this->isTranslatable()) {
        $language_id = 0;
     }

      if ($entity->hasValues($this, $language_id)) {
        return $values[$this->getKey()][$language_id];
      }
    }

    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function processValues($values, EntityInterface $entity, ConverterInterface $currentConverter) {
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
  public function sanitize($values) {
    return $values;
  }

  /**
   * @inheritdoc
   */
  public function exists(Profile $profile) {
  	return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function setOptions(array $options) {
    $this->options = $options;
  }

  /**
   * @inheritdoc
   */
  public function getOptions() {
    return $this->options;
  }
}
