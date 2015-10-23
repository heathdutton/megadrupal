<?php
namespace entity_context\core;

/**
 * Base class for extension by individual field handlers.
 */
abstract class FieldHandler {
  protected $_wrapper = NULL;
  protected $_field_group = '';
  protected $_context = array();

  /**
   * Constructor.
   * @param [type] $wrapper     [description]
   * @param [type] $field_group [description]
   * @param [type] $context     [description]
   */
  public function __construct($wrapper, $field_group, $context) {
    $this->setWrapper($wrapper);
    $this->setFieldGroup($field_group);
    $this->setContext($context);
  }

  /**
   * Returns the final value.
   * @return string Value to be populated into UDF field.
   */
  abstract public function getValue();

  /**
   * Concatenates array of values into a single string.
   */
  public function getValueString($value) {
    if (!is_array($value)) {
      $value = array($value);
    }

    return trim(implode(',', $value));
  }

  /**
   * Setter for Wrapper object.
   * @param [type] $wrapper [description]
   */
  protected function setWrapper($wrapper) {
    $this->_wrapper = $wrapper;
  }

  /**
   * Getter for Wrapper object.
   * @return [type] [description]
   */
  protected function getWrapper() {
    return $this->_wrapper;
  }

  /**
   * Setter for Field Group string.
   * @param [type] $context [description]
   */
  protected function setFieldGroup($field_group) {
    $this->_field_group = $field_group;
  }

  /**
   * Getter for Field Group string.
   * @return [type] [description]
   */
  protected function getFieldGroup() {
    return $this->_field_group;
  }

  /**
   * Setter for Context array.
   * @param [type] $context [description]
   */
  protected function setContext($context) {
    $this->_context = $context;
  }

  /**
   * Getter for Context array.
   * @return [type] [description]
   */
  protected function getContext() {
    return $this->_context;
  }
}