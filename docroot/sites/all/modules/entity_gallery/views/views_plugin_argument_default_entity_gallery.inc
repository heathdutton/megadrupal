<?php

/**
 * @file
 * Contains the entity gallery from URL argument default plugin.
 */

/**
 * Default argument plugin to extract an entity gallery via menu_get_object
 *
 * This plugin actually has no options so it odes not need to do a great deal.
 */
class views_plugin_argument_default_entity_gallery extends views_plugin_argument_default {
  function get_argument() {
    foreach (range(1, 3) as $i) {
      $entity_gallery = menu_get_object('entity_gallery', $i);
      if (!empty($entity_gallery)) {
        return $entity_gallery->egid;
      }
    }

    if (arg(0) == 'entity_gallery' && is_numeric(arg(1))) {
      return arg(1);
    }
  }
}
