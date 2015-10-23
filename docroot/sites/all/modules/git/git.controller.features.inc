<?php

/**
 * @file
 *  Git features controller classes.
 */

/**
 * Adds saving of entity to revert logic.
 */
class GitFeaturesController extends EntityDefaultFeaturesController {
  function revert($module = NULL) {
    if ($defaults = features_get_default($this->type, $module)) {
      foreach ($defaults as $name => $entity) {
        entity_delete($this->type, $name);
        // Added this line.
        $entity->save();
      }
    }
  }
}
