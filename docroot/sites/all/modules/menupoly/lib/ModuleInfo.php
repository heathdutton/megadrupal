<?php

class menupoly_ModuleInfo {

  /**
   * @param array &$existing
   *
   * @return array[]
   *   Theme hook definitions.
   */
  function hook_theme(&$existing) {

    // Add theme hook suggestion patterns for the core theme functions used in
    // this module. We can't add them during hook_theme_registry_alter() because
    // we will already have missed the opportunity for the theme engine's
    // theme_hook() to process the pattern. And we can't run the pattern ourselves
    // because we aren't given the type, theme and path in that hook.
    $existing['menu_tree']['pattern'] = 'menu_tree__';
    $existing['menu_item']['pattern'] = 'menu_item__';
    $existing['menu_item_link']['pattern'] = 'menu_item_link__';

    return array(
      'menupoly' => array(
        'variables' => array('menupoly' => NULL),
        'pattern' => 'menupoly',
      ),
    );
  }
}
