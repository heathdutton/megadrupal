<?php

/**
 * @file
 * Contains the filter condition related interfaces
 */

namespace Drupal\maps_import\Filter\Condition;

use Drupal\maps_import\Converter\Converter;

/**
 * Interface for MaPS Import Filter conditions.
 */
interface ConditionInterface {

  /**
   * AND operator.
   */
  const CONDITION_AND = 'and';

  /**
   * OR operator.
   */
  const CONDITION_OR = 'or';

  /**
   * Class constructor.
   *
   * @param $converter
   *   The related converter instance.
   * @param $properties
   *   An array off properties that defines the condition.
   *
   * @return Condition
   */
  public function __construct(Converter $converter, array $properties = array());

  /**
   * Get the condition untranslated title.
   *
   * @return string
   */
  public function getTitle();

  /**
   * Get the converter instance.
   *
   * @return Converter
   */
  public function getConverter();

  /**
   * Get the condition ID.
   *
   * @return int
   */
  public function getId();

  /**
   * Get the condition type.
   *
   * @return string
   */
  public function getType();

  /**
   * Get the parent condition ID.
   *
   * @return int
   */
  public function getParentId();

  /**
   * Set the parent condition ID.
   */
  public function setParentId($parentId);

  /**
   * Get the condition extra data.
   *
   * @return int
   */
  public function getExtra();

  /**
   * Set the condition extra data.
   */
  public function setExtra(array $extra);

  /**
   * Get the condition weight.
   *
   * @return int
   */
  public function getWeight();

  /**
   * Set the condition weight.
   */
  public function setWeight($weight);

  /**
   * Return the children of the current condition.
   *
   * @return array
   *   An array of Condition instances.
   */
  public function getChildren();

  /**
   * Specify if the condition is a container or not.
   *
   * @return bool
   */
  public function isContainer();

  /**
   * Return a formated label for display.
   *
   * @return string
   */
  public function getLabel();

}
