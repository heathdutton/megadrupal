<?php

class MenuBeanType extends BeanPlugin {
  public function values() {
    $settings = menu_bean_get_settings();
    $values = array(
      'menu_name' => 'main-menu',
    );

    foreach ($settings as $key => $info) {
      $values = array_merge_recursive($values, $info['default_values']);
    }

    return $values + parent::values();
  }

  public function form($bean, $form, &$form_state) {
    $menus = menu_get_menus();
    $form['menu_name'] = array(
      '#type' => 'select',
      '#title' => t('Menu'),
      '#default_value' => $bean->menu_name,
      '#options' => $menus,
    );

    $settings = menu_bean_get_settings();
    foreach ($settings as $key => $class) {
      menu_bean_load_setting_class($key)->form($bean, $form, $form_state);
    }

    return $form;
  }

  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {
    $altertree = array();
    $tree = menu_bean_load_setting_class(menu_bean_get_initial_plugin_key())->alterTree($altertree, $bean);

    $settings = menu_bean_get_settings();
    foreach ($settings as $key => $class) {
      menu_bean_load_setting_class($key)->alterTree($tree, $bean);
    }

    return menu_tree_output($tree);
  }
}