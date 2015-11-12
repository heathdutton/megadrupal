<?php
/**
 * @file
 * Context reaction to add menu parent contents to tabs.
 */

/**
 * Class ExtraTabsMenuContextReaction.
 */
class ExtraTabsMenuContextReaction extends context_reaction {
  /**
   * Options form.
   */
  public function options_form($context) {
    $form = array('#tree' => TRUE);
    $item = array('mlid' => 0);
    $menu_options = menu_parent_options(menu_get_menus(), $item);
    $form['menu_parent'] = array(
      '#type' => 'select',
      '#title' => t('Menu Parent'),
      '#options' => $menu_options,
    );

    if (isset($context->reactions['extra_tabs_menu_reaction']['menu_parent'])) {
      $form['menu_parent']['#default_value'] = $context->reactions['extra_tabs_menu_reaction']['menu_parent'];
    }

    return $form;
  }

  /**
   * Get a list of menu parents whose links should be converted to local tasks.
   *
   * @return array
   *   A list of menu parents in the format of menu_name:mlid.
   */
  public function execute() {
    $parents = array();
    foreach ($this->get_contexts() as $contexts) {
      if (isset($contexts->reactions[$this->plugin]['menu_parent'])) {
        $parents[] = $contexts->reactions[$this->plugin]['menu_parent'];
      }
    }
    return $parents;
  }

}
