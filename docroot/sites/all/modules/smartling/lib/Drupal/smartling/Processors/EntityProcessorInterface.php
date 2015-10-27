<?php

/**
 * @file
 * Contains Drupal\smartling\Processors\EntityProcessorInterface.
 */

namespace Drupal\smartling\Processors;

interface EntityProcessorInterface {
  public function updateEntity($content);
  public function exportContent();
  public function exportContentToArray();
}