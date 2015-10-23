<?php

/**
 * @file
 * Class that loads Drupal\maps_import\Converter\Converter objects.
 */

namespace Drupal\maps_import\Cache\Object;

use Drupal\maps_import\Exception\CacheException;
use Drupal\maps_import\Exception\ConverterException;

/**
 * The MaPS Import Cache class related to converters.
 */
class Converter extends Object {

  /**
   * @inheritdoc
   */
  protected function useCacheBin() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  protected function getCacheKey() {
    return 'maps_import:converters';
  }

  /**
   * @inheritdoc
   */
  protected function getTable() {
    return 'maps_import_converter';
  }

  /**
   * @inheritdoc
   */
  protected function getDefaultColumn() {
    return 'cid';
  }

  /**
   * @inheritdoc
   */
  protected function getInterfaceName() {
    return 'Drupal\\maps_import\\Converter\\ConverterInterface';
  }

  /**
   * @inheritdoc
   */
  protected function getClassName($data) {
    if (!isset($data->class) || !is_subclass_of($data->class, $this->getInterfaceName())) {
      throw new CacheException('The class defined in the column %column is either empty or not valid.', 0, array('%column' => 'class'), array('$data' => $data));
    }

    return $data->class;
  }

  /**
   * @inheritdoc
   */
  protected function createInstance(&$data) {
    $class = $this->getClassName($data);

    if (!$profile = Profile::getInstance()->loadSingle($data->pid)) {
      throw new ConverterException('There is no profile with an ID of @pid.', 0, array('@pid' => $data->pid));
    }

    return new $class($profile);
  }

}
