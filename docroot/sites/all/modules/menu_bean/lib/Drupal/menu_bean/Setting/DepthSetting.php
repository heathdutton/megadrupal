<?php

/**
 * @file
 * Level Plugin class
 */

namespace Drupal\menu_bean\Setting;

class DepthSetting implements SettingInterface {
  /**
   * Provides form
   *
   * @param \Bean $bean
   * @param $form
   * @param $form_state
   */
  public function form(\Bean $bean, &$form, &$form_state) {
    $form['depth'] = array(
      '#type' => 'select',
      '#title' => t('Maximum depth'),
      '#default_value' => $bean->depth,
      '#options' => array(
        '1'  => '1',
        '2'  => '2',
        '3'  => '3',
        '4'  => '4',
        '5'  => '5',
        '6'  => '6',
        '7'  => '7',
        '8'  => '8',
        '9'  => '9',
        '0'  => t('Unlimited'),
      ),
      '#description' => t('From the starting level, specify the maximum depth of the menu tree.'),
    );
  }

  /**
   * Alters the menu tree
   *
   * @param array
   * @param \Bean $bean
   */
  public function alterTree(array &$tree, \Bean $bean) {
    // Trim the branches that extend beyond the specified depth.
    if ($bean->depth > 0) {
      $this->menu_tree_depth_trim($tree, $bean->depth);
    }
  }

  /**
   * Prune a tree so it does not extend beyond the specified depth limit.
   *
   * @param $tree
   *   array The menu tree to prune.
   * @param $depth_limit int
   *  The maximum depth of the returned tree must be a positive integer.
   *
   * @return void
   */
  protected function menu_tree_depth_trim($tree, $depth_limit) {
    // Prevent invalid input from returning a trimmed tree.
    if ($depth_limit < 1) {
      return;
    }

    // Examine each element at this level to find any possible children.
    foreach ($tree AS $key => &$value) {
      if ($tree[$key]['below']) {
        if ($depth_limit > 1) {
          $this->menu_tree_depth_trim($tree[$key]['below'], $depth_limit-1);
        }
        else {
          // Remove the children items.
          $tree[$key]['below'] = FALSE;
        }
      }
      if ($depth_limit == 1 && $tree[$key]['link']['has_children']) {
        // Turn off the menu styling that shows there were children.
        $tree[$key]['link']['has_children'] = FALSE;
        $tree[$key]['link']['leaf_has_children'] = TRUE;
      }
    }
  }
}