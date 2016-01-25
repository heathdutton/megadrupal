<?php

/**
 * @file
 * Class that loads Drupal\maps_import\Converter\Converter links.
 */

namespace Drupal\maps_links\Cache\Object;

use Drupal\maps_import\Cache\Object\Profile;
use Drupal\maps_import\Cache\Object\Object;
use Drupal\maps_import\Exception\ConverterException;

/**
 * The MaPS Import Cache class related to converters.
 */
class Link extends Object {

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
    return 'maps_links:converters';
  }

  /**
   * @inheritdoc
   */
  protected function getTable() {
    return 'maps_links_converter';
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
    return 'Drupal\\maps_links\\Converter\\LinkInterface';
  }

  /**
   * @inheritdoc
   */
  protected function getClassName($data) {
    return 'Drupal\\maps_links\\Converter\\Link';
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
