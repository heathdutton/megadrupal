<?php

/**
 * @file
 * Context plugin file to provide changing the menu as a context reaction.
 */

/**
 * Expose main-menus as context reactions.
 */
class context_reaction_menus extends context_reaction {
  /**
   * Grab the available menus and provide them as a reaction for context.
   *
   * @param $context
   *   The context as passed from context module.
   *
   * @return array
   *   The FAPI array as read by context module.
   */
  public function options_form($context) {

    $menus = menu_get_menus();

    // fetch the settings that have been saved already, if any.
    $settings = $this->fetch_from_context($context);

    $form = array(
      '#tree' => TRUE,

      '#title' => t('Active Menus'),
      'main_menu' => array(
        '#title' => t('Main Menu'),
        '#description' => t('Choose a main-menu to activate when this context is active.'),
        '#type' => 'select',
        // allow disabling the menu
        '#empty_option' => t('No Main Menu links'),
        '#options' => $menus,
        '#default_value' => isset($settings['main_menu']) ? $settings['main_menu'] : variable_get('menu_main_links_source', 'main-menu'),
      ),

      'secondary_menu' => array(
        '#title' => t('Secondary Menu'),
        '#description' => t('Choose a secondary menu to activate when this context is active.'),
        '#type' => 'select',
        // allow disabling the menu
        '#empty_option' => t('No Secondary links'),
        '#options' => $menus,
        '#default_value' => isset($settings['secondary_menu']) ? $settings['secondary_menu'] : variable_get('menu_secondary_links_source', 'user-menu'),
      ),
    );
    return $form;
  }

  /**
   * Return the required main menu based on the context.
   */
  public function execute() {
    global $conf;

    foreach ($this->get_contexts() as $context) {

      // We could variable_set these, but variable_set will update the db as
      // well as the local instance (i.e. this page load), which we don't
      // want - we only want to update the page instance for when this
      // context is matched. Therefore, we will use the global $conf instead
      // of variable_set().

      if (isset($context->reactions['menus']['main_menu'])) {
        $conf['menu_main_links_source'] = $context->reactions['menus']['main_menu'];
      }

      if (isset($context->reactions['menus']['secondary_menu'])) {
        $conf['menu_secondary_links_source'] = $context->reactions['menus']['secondary_menu'];
      }

    }
  }
}
