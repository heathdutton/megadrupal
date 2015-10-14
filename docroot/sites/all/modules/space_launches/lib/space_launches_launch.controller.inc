<?php

/**
 * @file
 * The controller for the Space Launches Launch entity.
 */
class SpaceLaunchesLaunchEntityController extends EntityAPIController {

  /**
   * Saves a launch entity.
   *
   * @param SpaceLaunchesLaunch $launch
   *   The entity to save.
   *
   * @return SpaceLaunchesLaunch
   *   The saved entity.
   */
  public function save($launch) {
    $launch->updated = time();

    if (isset($launch->is_new) && $launch->is_new) {
      $launch->created = time();
    }

    parent::save($launch);

    return $launch;
  }

}
