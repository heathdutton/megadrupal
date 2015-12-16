<?php

/**
 * @file
 * Child converter class for link converters.
 */

namespace Drupal\maps_import\Converter\Child;

use Drupal\maps_import\Converter as ParentConverter;

/**
 * Media Child Converter class.
 */
class Media extends ParentConverter\Media implements ChildInterface {

  /**
   * The parent converter.
   *
   * @var ParentConverter\ConverterInterface
   */
  protected $parentConverter;

  /**
   * @inheritdoc
   */
  public function getParent() {
    if (!isset($this->parentConverter)) {
      $this->parentConverter = $this->getParentId() ? maps_import_converter_load($this->getParentId()) : FALSE;
    }

    return $this->parentConverter;
  }

  /**
   * @inheritdoc
   */
  public function setParent(ParentConverter\ConverterInterface $converter) {
    $this->parentConverter = $converter;
    $this->setParentId($converter->getCid());
  }

  /**
   * @inheritdoc
   */
  public function getUid() {
    if ($converter = $this->getParent()) {
      return $converter->getUid() . ':' . $this->getCid();
    }
  }

  /**
   * @inheritdoc
   */
  public function getUidScope() {
    return ParentConverter\ConverterInterface::SCOPE_INHERIT;
  }

}
