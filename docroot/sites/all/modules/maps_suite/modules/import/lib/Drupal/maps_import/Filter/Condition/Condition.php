<?php

/**
 * @file
 * Define the base class for a filter Condition.
 */

namespace Drupal\maps_import\Filter\Condition;

use Drupal\maps_import\Converter\Converter;
use Drupal\maps_import\Filter\Filter;

/**
 * Class for MaPS Import Filter conditions.
 */
abstract class Condition implements ConditionInterface {

  /**
   * The filter ID.
   *
   * @var int
   */
  private $id = NULL;

  /**
   * The related converter.
   *
   * @var Converter
   */
  private $converter;

  /**
   * The filter parent ID.
   *
   * @var int
   */
  private $parentId = NULL;

  /**
   * The filter extra informations.
   *
   * @var array
   */
  private $extra = NULL;

  /**
   * The filter weight.
   *
   * @var int
   */
  private $weight = 0;

  /**
   * @inheritdoc
   */
  public function __construct(Converter $converter, array $properties = array()) {
    $this->converter = $converter;

    foreach (array('id', 'parent_id', 'extra', 'weight') as $propertyName) {
      if (array_key_exists($propertyName, $properties)) {
        $camelCase = maps_suite_drupal2camelcase($propertyName, FALSE);
        $this->{$camelCase} = $properties[$propertyName];
      }
    }
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
  public function getId() {
    return $this->id;
  }

  /**
   * @inheritdoc
   */
  public function getParentId() {
    return $this->parentId;
  }

  /**
   * @inheritdoc
   */
  public function setParentId($parentId) {
    $this->parentId = $parentId;
  }

  /**
   * @inheritdoc
   */
  public function getExtra() {
    return $this->extra;
  }

  /**
   * @inheritdoc
   */
  public function setExtra(array $extra) {
    $this->extra = $extra;
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
  public function getChildren() {
    $children = array();
  	$parents = array($this->getId());

  	$conditions = $this->getConverter()->getFilter()->getFlattenConditions();
    foreach ($conditions as $condition) {
    	if (in_array($condition->getParentId(), $parents)) {
    		$children[] = $condition;
    		if (!in_array($condition->getId(), $parents)) {
    			$parents[] = $condition->getId();
    		}
    	}
    }

    return $children;
  }

  /**
   * Save object in database.
   * 
   * @return int
   *   The created filter id.
   */
  public function save() {
    $record = array(
      'id' => $this->getId(),
      'cid' => $this->getConverter()->getCid(),
      'parent_id' => $this->getParentId(),
      'type' => $this->getType(),
      'extra' => $this->getExtra(),
      'weight' => $this->getWeight(),
      'class' => get_class($this),
    );

    $update = isset($record['id']) ? array('id') : array();
    drupal_write_record('maps_import_converter_conditions', $record, $update);
    return $record['id'];
  }

  /**
   * Delete the current condition.
   *
   * @return int
   *   The number of records that were deleted.
   */
  public function delete() {
    $ids = array($this->getId());

    foreach ($this->getChildren() as $child) {
      $ids[] = $child->getId();
    }

    $this->getConverter()->unsetFilter();
    return db_delete('maps_import_converter_conditions')->condition('id', $ids)->execute();
  }

  /**
   * Form save function.
   *
   * @param $form
   * @param &$form_state
   */
  public function formSave($form, &$form_state) {
    $this->save();
  }

  /**
   * Return the condition form array.
   *
   * @param $form
   * @param &$form_state
   */
  public function conditionForm($form, &$form_state) {
    $form_state['converter'] = $this->getConverter();

    $form['id'] = array('#type' => 'value', '#value' => $this->getId());
    $form['cid'] = array('#type' => 'value', '#value' => $this->getConverter()->getCid());
    $form['pid'] = array('#type' => 'value', '#value' => $this->getConverter()->getProfile()->getPid());

    $form['parent_id'] = array(
      '#type' => 'select',
      '#title' => t('Parent container'),
      '#options' => array('' => t('None')) + $this->getConverter()->getConditionContainers(),
      '#default_value' => $this->getParentId(),
    );

    $form['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#delta' => 10,
      '#default_value' => $this->getWeight(),
    );

    return $this->form($form, $form_state);
  }

}

