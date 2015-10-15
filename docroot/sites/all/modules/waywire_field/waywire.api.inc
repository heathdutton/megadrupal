<?php

/**
 * @file
 * Hooks provided by the Date module.
 */

/**
 * Alter the default value for a date argument.
 *
 * @param object $entity
 *   The entity which is displaying the video.
 * @param array $element
 *   All the videos that are going to be shown on the page.
 */
function hook_waywire_video_alter($entity, array &$element) {
  if ($entity->type == 'video') {
    switch ($entity->nid) {
      case '58398':
        // Change the player to Multi View Player.
        foreach ($element as $key => $item) {
          $element[$key]['#player'] = 'mvp';
        }
        break;
    }
  }
}
