<?php

/**
 * @file
 * Level Plugin class
 */

namespace Drupal\menu_bean\Setting;

class LevelSetting implements SettingInterface {

  /**
   * Provides form
   *
   * @param \Bean $bean
   * @param $form
   * @param $form_state
   */
  public function form(\Bean $bean, &$form, &$form_state) {
    $form['level'] = array(
      '#type' => 'select',
      '#title' => t('Starting level'),
      '#default_value' => $bean->level,
      '#options' => array(
        '1'  => t('1st level (primary)'),
        '2'  => t('2nd level (secondary)'),
        '3'  => t('3rd level (tertiary)'),
        '4'  => t('4th level'),
        '5'  => t('5th level'),
        '6'  => t('6th level'),
        '7'  => t('7th level'),
        '8'  => t('8th level'),
        '9'  => t('9th level'),
      ),
      '#description' => t('Blocks that start with the 1st level will always be visible. Blocks that start with the 2nd level or deeper will only be visible when the trail to the active menu item is in the block’s tree.'),
    );

    return $form;
  }

  /**
   * Alters the menu tree
   *
   * @param $tree
   * @param \Bean $bean
   * @param array
   */
  public function alterTree(array &$tree, \Bean $bean) {
    // Prune the tree along the active trail to the specified level.
    if ($bean->level > 1) {
      $this->menu_bean_tree_prune_tree($tree, $bean->level);
    }
  }

  /**
   * Prune a tree so that it begins at the specified level.
   *
   * Mostly from menu_block
   *
   * @param $tree array
   *   array The menu tree to prune.
   * @param $level
   *   int The level of the original tree that will start the pruned tree.
   */
  protected function menu_bean_tree_prune_tree(array &$tree, $level) {
    // Trim the upper levels down to the one desired.
    for ($i = 1; $i < $level; $i++) {
      $found_active_trail = FALSE;
      // Examine each element at this level for the active trail.
      foreach ($tree AS $key => &$value) {
        if ($tree[$key]['link']['in_active_trail']) {
          // Get the title for the pruned tree.
          //menu_block_set_title($tree[$key]['link']);
          // Prune the tree to the children of the item in the active trail.
          $tree = $tree[$key]['below'] ? $tree[$key]['below'] : array();
          $found_active_trail = TRUE;
          break;
        }
      }
      // If we don't find the active trail, the active item isn't in the tree we want.
      if (!$found_active_trail) {
        $tree = array();
        break;
      }
    }
  }
}