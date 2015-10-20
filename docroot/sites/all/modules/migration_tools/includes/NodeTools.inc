<?php
/**
 * @file
 * Tools for handling nodes and fields.
 */

class NodeTools {

  /**
   * Sets the $entity->body filter setting for all body fields of all languages.
   *
   * @param object $entity
   *   Node entity object. Altered by reference
   * @param string $filter_machine_name
   *   The filter machine name to assign. (example: 'full_html')
   */
  public static function reassignBodyFilter($entity, $filter_machine_name) {
    if (!empty($entity->body) && is_array($entity->body)) {
      foreach ($entity->body as &$language) {
        foreach ($language as $lang => &$body) {
          // It is possible that $body is not an array.
          if (is_array($body)) {
            $body['value_format'] = $filter_machine_name;
            $body['format'] = $filter_machine_name;
          }
        }
        // Break the reference.
        unset($body);
      }
      // Break the reference.
      unset($language);
    }
  }
}
