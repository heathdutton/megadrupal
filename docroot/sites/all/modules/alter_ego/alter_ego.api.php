<?php

/**
 * @file
 * Hooks provided by the alter_ego API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provides a hook to return a custom picture url for module defined avatars.
 *
 * @param Avatar $avatar
 *
 * @return string
 *   Return the URL to the specific avatar's picture.
 *
 */
function alter_ego_avatar_picture_url($avatar) {
  if ($avatar->type == 'generic') {
    return $avatar->picture;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
