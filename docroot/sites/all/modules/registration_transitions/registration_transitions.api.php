<?php

/**
 * Implements hook_registration_transitions_access_alter().
 *
 * $context contains:
 *  - registration: the Registration object
 *  - meta: the transition info, with:
 *     - from
 *     - to
 *     - .. and more
 *  - account: the user doing the transition
 */
function HOOK_registration_transitions_access_alter(&$access, $context) {
  $access = $context['registration']->registration_id % 2 == 0;
}
