<?php

/**
 * @file
 * The controller for the avatar entity.
 */

class DrpgAvatarEntityController extends EntityAPIController {

  /**
   * Saves a avatar entity.
   *
   * @param DrpgAvatar $avatar
   *   The avatar to save.
   *
   * @return DrpgAvatar
   *   The saved avatar.
   */
  public function save($avatar) {
    $avatar->updated = time();

    if (isset($avatar->is_new) && $avatar->is_new) {
      $avatar->created = time();
    }

    parent::save($avatar);

    return $avatar;
  }

}
