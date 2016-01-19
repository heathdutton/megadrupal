<?php

namespace Drupal\panelizer_deploy;

interface ConverterInterface {
  /**
   * Run a conversion to UUID
   *
   * @param array $objects
   */
  public function convertToUUID(&$objects);

  /**
   * Run a conversion to Id
   *
   * @param array $objects
   */
  public function convertToID(&$objects);

  /**
   * Get the name of the key
   *
   * @return string
   */
  public function getKeyField();

  /**
   * Get the name of the uuid field
   *
   * @return string
   */
  public function getUUIDField();
}