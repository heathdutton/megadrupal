<?php

/**
 * @file
 * Wrapper against Converter class for CTools exportable.
 */

namespace Drupal\maps_import\Plugins\CTools\ExportUI;

use Drupal\maps_import\Converter\Child\ChildInterface as ChildConverterInterface;
use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Filter\Condition\ConditionInterface;
use Drupal\maps_import\Profile\Profile as MapsImportProfile;

/**
 * Profile Exportable class.
 */
class ConverterExportable extends Exportable {

  /**
   * The converter object.
   *
   * @var ConverterInterface
   */
  protected $converter;

  /**
   * The profile exportable object.
   *
   * @var ProfileExportable
   */
  protected $profile;

  /**
   * The list of filter conditions.
   *
   * @var array
   */
  protected $conditions = array();

  /**
   * The list of mapping items.
   *
   * @var array
   */
  protected $mapping = array();

  /**
   * The name of the parent converter, if any.
   *
   * @var string
   */
  protected $parent_name;

  /**
   * Class constructor.
   *
   * This class is not supposed to handle empty converters, but only real ones.
   */
  public function __construct(ProfileExportable $profile = NULL, ConverterInterface $converter = NULL, $type = EXPORT_IN_DATABASE) {
    $this->profile = $profile;
    parent::__construct($converter, $type);
  }

  /**
   * Magic method __set_state().
   */
  public static function __set_state(array $array) {
    $schema = drupal_get_schema('maps_import_converter');
    $class = get_called_class();
    $instance = new $class();

    // We absolutely need the converter class.
    if (!isset($array['class']) || !is_subclass_of($array['class'], 'Drupal\maps_import\Converter\ConverterInterface')) {
      throw new \InvalidArgumentException('The ConverterExportable constructor requires a Drupal\maps_import\Converter\ConverterInterface object.');
    }

    // Create an empty converter with an empty profile.
    $instance->setWrappedObject(new $array['class'](new MapsImportProfile()));

    foreach (array_keys($schema['fields']) as $key) {
      if (isset($array[$key])) {
        $instance->{$key} = $array[$key];
      }
    }

    if (isset($array['parent_name'])) {
      $instance->setParentName($array['parent_name']);
    }

    if (isset($array['conditions'])) {
      $instance->conditions = $array['conditions'];
    }

    if (isset($array['mapping_items'])) {
      $instance->mapping = $array['mapping_items'];
    }

    return $instance;
  }

  /**
   * @inheritdoc
   *
   * @return MapsImportProfile
   */
  public function getWrappedObject() {
    return $this->converter;
  }

  /**
   * @inheritdoc
   *
   * @param MapsImportProfile
   */
  public function setWrappedObject($object = NULL) {
    $this->converter = $object;
  }

  /**
   * Get the converter object.
   *
   * This is a alias of the getWrappedObject() method, but its
   * name is really more explicit.
   *
   * @return ConverterInterface
   */
  public function getConverter() {
    return $this->converter;
  }

  /**
   * Get the profile exportable object.
   *
   * @return ProfileExportable
   */
  public function getProfile() {
    return $this->profile;
  }

  /**
   * Set the profile exportable object.
   *
   * @rparam ProfileExportable
   */
  public function setProfile(ProfileExportable $profile) {
    $this->profile = $profile;

    if ($this->getConverter()) {
      $this->getConverter()->setProfile($profile->getProfile());
    }
  }

  /**
   * Get the parent converter name.
   */
  public function getParentName() {
    return $this->parent_name;
  }

  /**
   * Set the parent converter name.
   *
   * @param string $name
   */
  public function setParentName($name) {
    $this->parent_name = $name;
  }

  /**
   * Get the converter class.
   *
   * Implements this method in the behalf of the real Converter object.
   *
   * @return string
   */
  public function getClass() {
    return get_class($this->getConverter());
  }

  /**
   * Add a mapping item to the list.
   *
   * @param array $item
   *   The item definition.
   */
  public function addMappingItem(array $item) {
    $this->mapping[] = $item;
  }

  /**
   * Get the mapping items.
   *
   * @return array
   */
  public function getMappingItems() {
    return $this->mapping;
  }

  /**
   * Add a filter condition to the list.
   *
   * @param array $condition
   *   The condition definition.
   */
  public function addFilterCondition(array $condition) {
    $this->conditions[] = $condition;
  }

  /**
   * Get the filter conditions.
   *
   * @return array
   */
  public function getFilterConditions() {
    return $this->conditions;
  }

  /**
   * Convert the converter object into an array.
   *
   * @return array
   */
  public function toArray() {
    $data = array();
    $schema = drupal_get_schema('maps_import_converter');

    foreach ($schema['fields'] as $column => $info) {
      if ($info['type'] === 'serial') {
        continue;
      }

      $value = $this->__get($column);

      if (isset($value)) {
        $data[$column] = $value;
      }
    }

    return $data;
  }

  /**
   * Whether the converter is a child of another converter.
   */
  public function isChildConverter() {
    return $this->getConverter() instanceof ChildConverterInterface;
  }

  /**
   * Ensure the parent converter object is correctly set.
   */
  public function ensureParent() {
    if ($this->getProfile() && $this->isChildConverter()) {
      if ($parentName = $this->getParentName()) {
        foreach ($this->getProfile()->getConverters() as $name => $converter) {
          if ($name === $parentName && $converter->getCid()) {
            $this->getConverter()->setParent($converter->getConverter());
            return TRUE;
          }
        }
      }

      return FALSE;
    }

    return TRUE;
  }

  /**
   * (@inheritdoc)
   */
  public function export($prefix = '') {
    $output = array();
    $output[] = get_class($this) . '::__set_state(array(';
    $output[] = "  'class' => " . ctools_var_export($this->getClass()) . ',';
    $output = array_merge($output, $this->exportFromSchema('maps_import_converter', '  '));

    if ($this->isChildConverter()) {
      $output[] = "  'parent_name' => " . ctools_var_export($this->getParent()->getName()) . ',';
    }

    // Conditions.
    $output[] = "  'conditions' => array(";

    foreach ($this->getFilter() as $condition) {
      $array = $this->condition2array($condition);
      $code = '    ' . ctools_var_export($array, '    ');
      $output = array_merge($output, explode("\n", $code . ','));
    }

    $output[] = "  ),";

    // Mapping.
    $output[] = "  'mapping_items' => array(";

    foreach ($this->getMapping()->getItems('all') as $item) {
      $code = '    ' . ctools_var_export(
        array(
          'property_id' => $item->getProperty()->getKey(),
          'field_name' => $item->getField()->getKey(),
          'static' => (int) $item->isStatic(),
          'required' => $item->isRequired(),
          'weight' => $item->getWeight(),
          'options' => $item->getOptions(),
          'type' => $item->getType(),
        ),
        '    ');
      $output = array_merge($output, explode("\n", $code . ','));
    }

    $output[] = "  ),";
    $output[] = '))';

    return implode("\n$prefix", $output);
  }

  /**
   * Export a filter condition.
   *
   * @return array
   */
  protected function condition2array(ConditionInterface $condition) {
    $array = array(
      'type' => $condition->getType(),
      'extra' => $condition->getExtra(),
      'weight' => $condition->getWeight(),
      'class' => get_class($condition),
    );

    if ($children = $condition->getChildren()) {
      foreach ($children as $childCondition) {
        $array['children'][] = $this->condition2array($childCondition);
      }
    }

    return $array;
  }

}
