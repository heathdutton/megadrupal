<?php

/**
 * @file
 * Sample hooks demonstrating usage in Entity bundle Redirect.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * @defgroup entity_bundle_redirect_hooks Entity bundle Redirect Module Hooks
 * @{
 * Entity bundle Redirect's hooks enable other modules to intercept events
 * within Entity bundle Redirect, such as rewriting the redirection path.
 */

/**
 * Respond to the loading of Webform submissions.
 *
 * @param string $path
 *   The path where indicates the redirection.
 *   Modifications are done by reference.
 * @param array $info
 *   Array with three keys: 'entitiy_type', 'bundle' and 'entity'.
 *   The 'entity_type' indicates the type of that entity (node, user...).
 *   The 'bundle' indicates the bundle of the entity (page, article...).
 *   The 'entity' contains the whole entity.
 */
function hook_entity_bundle_redirect_alter(&$path, $info) {

  // Redirects node blog pages.
  if ($info['entity_type'] == 'node' && $info['bundle'] == 'blog') {
    if ($info['entity']->sticky) {
      // The sticky ones will be redirected to front page.
      $path = url('<front>');
    }
    else {
      // The rest, will be redirected to drupal.org
      $path = url('http://drupal.org');
    }
  }

}
