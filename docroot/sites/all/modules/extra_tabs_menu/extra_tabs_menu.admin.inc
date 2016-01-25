<?php
/**
 * @file
 * Admin form related functions.
 */

/**
 * Admin configuration form.
 */
function extra_tabs_menu_config_form() {
  $form = array();
  $tab_config = variable_get('extra_tabs_config', array());

  // Add an empty tab configuration area to the settings form.
  $tab_config[] = array(
    'tab_paths' => NULL,
    'tab_menu_settings' => NULL,
  );

  $index = 0;
  $form['tab_settings']['#tree'] = TRUE;
  $menu_options = menu_parent_options(menu_get_menus(), array('mlid' => 0));

  foreach ($tab_config as $tc) {
    $form['tab_settings'][$index++] = array(
      '#type' => 'fieldset',
      '#title' => t('Tab Configuration @index', array('@index' => $index)),
      'tab_paths' => array(
        '#type' => 'textarea',
        '#title' => t('Paths'),
        '#default_value' => $tc['tab_paths'],
        '#description' => t("Paths matching the pattern entered here will
          display extra tabs. Specify pages by using their paths. Enter one
          path per line. The '*' character is a wildcard.
          Example paths are %blog for the blog page and %blog-wildcard for
          every personal blog. %front is the front page.",
          array(
            '%blog' => 'blog',
            '%blog-wildcard' => 'blog/*',
            '%front' => '<front>',
          )
        ),
      ),
      'tab_menu_settings' => array(
        '#type' => 'select',
        '#title' => t('Menu'),
        '#default_value' => $tc['tab_menu_settings'],
        '#options' => $menu_options,
        '#empty_option' => t('None'),
        '#description' => t('If a menu is selected, the full menu contents will
          be displayed. Otherwise children of the selected link will be
          displayed as tabs.'
        ),
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'][] = 'extra_tabs_menu_config_submit';
  return $form;
}

/**
 * Submit handler for extra_tabs_menu config form.
 */
function extra_tabs_menu_config_submit(&$form, &$form_state) {
  $settings = $form_state['values']['tab_settings'];
  foreach ($settings as $key => $value) {
    if (!$value['tab_paths'] || !$value['tab_menu_settings']) {
      unset($settings[$key]);
    }
  }
  variable_set('extra_tabs_config', $settings);
  drupal_set_message(t('Extra tabs menu configuration has been updated.'));
}
