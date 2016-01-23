<?php
/**
 * @file
 * Interface for the Universal Data Object
 */

namespace Drupal\tealium;

interface UniversalDataObjectInterface {

  function setDataSourceValue($name, $value);

  function getDataSourceValue($name);

  function unsetDataSourceValue($name);

}