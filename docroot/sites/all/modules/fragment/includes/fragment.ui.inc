<?php

/**
 * @file
 * Provides a controller for building a fragment overview form.
 */

/**
 * Fragment controller for providing UI.
 */
class FragmentUIController extends EntityDefaultUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Modify the entity edit menu item to be usable as a contextual link.
    // Can be removed when http://drupal.org/node/1598652 lands.
    // Need to reconstruct the wildcard :(
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%entity_object';
    $items[$this->path . '/manage/' . $wildcard . '/edit']['context'] = MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE;

    // Fix access to fragment admin page.
    $items['admin/content/fragments']['access callback'] = 'user_access';
    $items['admin/content/fragments']['access arguments'] = array('administer fragments');

    return $items;
  }
}
