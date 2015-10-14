<?php

namespace Drupal\panelizer_deploy;

/**
 * Class which tracks the id to uuid conversion
 */
class ConverterMap {
  protected $type_map = array();
  protected $object_map = array();

  /**
   * Add a Converter Type
   *
   * @param ConverterInterface $type
   * @param string $key
   *
   * @return ConverterMap
   */
  function addConverterType(ConverterInterface $type, $key) {
    $this->type_map[$key] = $type;
    return $this;
  }

  /**
   * Get a ConverterType
   *
   * @param string $key
   * @throws ConverterException
   * @return ConverterInterface
   */
  function getConverterType($key) {
    if (empty($this->type_map[$key])) {
      throw new ConverterException("Converter Type ($key) has not been set.");
    }
    return $this->type_map[$key];
  }

  /**
   * Add an object to the list of object to convert
   *
   * @param $object
   * @param $type string
   * @throws ConverterException
   * @return $this
   */
  public function addObject(&$object, $type) {
    // We are just testing to see if the convertertype is set
    $this->getConverterType($type);

    if (is_array($object)) {
      foreach ($object as &$thing) {
        $this->object_map[$type][] = &$thing;
      }
    } else {
      $this->object_map[$type][] = &$object;
    }

    return $this;
  }

  public function convertToUUID() {
    foreach ($this->object_map as $type => &$objects) {
      $this->type_map[$type]->convertToUUID($objects);
    }
  }

  public function convertToID() {
    foreach ($this->object_map as $type => &$objects) {
      $this->type_map[$type]->convertToID($objects);
    }
  }

  public function getObject($type, $id) {
    return $this->object_map[$type][$id];
  }
}

class ConverterException extends \Exception {};