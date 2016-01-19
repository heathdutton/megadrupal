<?php

/**
 * @file
 * 118n Plugin class
 */

namespace Drupal\menu_bean\Setting;

class i18nSetting implements SettingInterface {
  /**
   * Provides form
   *
   * @param \Bean $bean
   * @param $form
   * @param $form_state
   */
  public function form(\Bean $bean, &$form, &$form_state) {}

  /**
   * Alters the menu tree
   *
   * @param array $tree
   * @param \Bean $bean
   */
  public function alterTree(array &$tree, \Bean $bean) {
    // Localize the tree.
    if (module_exists('i18n_menu')) {
      $tree = i18n_menu_localize_tree($tree);
    }
  }
}