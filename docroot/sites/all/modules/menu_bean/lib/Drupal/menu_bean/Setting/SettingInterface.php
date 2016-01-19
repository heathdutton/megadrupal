<?php

/**
 * @file
 * Interface for setting plugin
 */

namespace Drupal\menu_bean\Setting;

interface SettingInterface {

  /**
   * Provides form
   *
   * @param \Bean $bean
   * @param $form
   * @param $form_state
   */
  public function form(\Bean $bean, &$form, &$form_state);

  /**
   * Alters the menu tree
   *
   * @param array $tree
   * @param \Bean $bean
   */
  public function alterTree(array &$tree, \Bean $bean);
}