<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * "EntityBasic" entity Views definition.
 */
class EntityBasicViewsController extends EntityDefaultViewsController {

  /**
   * Override views_data().
   */
  public function views_data() {
    $data = parent::views_data();
    return $data;
  }
}
