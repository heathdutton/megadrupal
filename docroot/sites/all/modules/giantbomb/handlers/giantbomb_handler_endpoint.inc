<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombBaseEndpoint {
  function object_properties($type) {
    
  }

  /**
   * Test if an object property is a list, and if it is a special list.
   *
   * @param string $endpoint
   *   A Giant Bomb endpoint name.
   * @param string $type
   *   Source type.
   * @param string $property
   *   Property name.
   * @return string|bool
   *   List this property is an alias of, or FALSE.
   */
  function property_is_list($type, $property) {
    return FALSE;
  }
}