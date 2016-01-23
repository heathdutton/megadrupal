<?php

/**
 * @file
 * Level Plugin class
 */

namespace Drupal\menu_bean\Setting;

class ExpandedSetting implements SettingInterface {

  /**
   * Provides form
   *
   * @param \Bean $bean
   * @param $form
   * @param $form_state
   */
  public function form(\Bean $bean, &$form, &$form_state) {
    $form['expanded'] = array(
      '#type' => 'checkbox',
      '#title' => t('<strong>Expand all children</strong> of this tree.'),
      '#default_value' => $bean->expanded,
    );
  }

  /**
   * Alters the menu tree
   *
   * @param array $tree
   * @param \Bean $bean
   * @return array
   */
  public function alterTree(array &$tree, \Bean $bean) {
    // Get the menu
    if ($bean->expanded) {
      // Get the full, un-pruned tree.
      $tree_array = menu_tree_all_data($bean->menu_name);
      // And add the active trail data back to the full tree.
      menu_bean_tree_add_active_path($tree);
    }
    else {
      // Get the tree pruned for just the active trail.
      $tree_array = menu_tree_page_data($bean->menu_name);
    }

    return $tree_array;
  }


}