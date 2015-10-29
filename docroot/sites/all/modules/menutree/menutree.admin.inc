<?php
/**
 * @file
 * Administrative page callbacks for menutree module.
 */

/**
 * Form builder; display menutree settings form.
 */
function menutree_settings($form, &$form_state) {
  $form['#attached']['js'][] = drupal_get_path('module', 'menutree') . '/menutree.admin.js';

  $form['menutree'] = array(
    '#type' => 'vertical_tabs',
  );

  $items = array_merge(array('<none>'), range(-10, 10));
  $weights = array_combine($items, $items);

  foreach (menu_get_menus() as $menu_name => $menu_title) {
    $form['menutree'][$menu_name] = array(
      '#type' => 'fieldset',
      '#title' => $menu_title,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('The path <a href="@link">@path</a> will provide a complete tree of all menu items in this menu.  If you wish to set a custom title or header text, do so here.', array(
        '@link' => url('menutree/' . $menu_name),
        '@path' => 'menutree/' . $menu_name,
      )),
      '#attributes' => array('class' => array('menutree-tab-pane')),
    );
    $form['menutree'][$menu_name]['menutree_title_' . $menu_name] = array(
      '#type' => 'textfield',
      '#title' => t('Page title'),
      '#default_value' => variable_get('menutree_title_' . $menu_name, ''),
      '#description' => t('A page title that is displayed instead of the root menu item title.'),
      '#attributes' => array('class' => array('menutree-title')),
    );
    $form['menutree'][$menu_name]['menutree_intro_text_' . $menu_name] = array(
      '#type' => 'textarea',
      '#title' => t('Intro text'),
      '#default_value' => variable_get('menutree_intro_text_' . $menu_name, ''),
      '#resizable' => TRUE,
      '#description' => t('An intro text that is displayed below the page title.'),
      '#attributes' => array('class' => array('menutree-intro-text')),
    );
    $form['menutree'][$menu_name]['menutree_all_weight_' . $menu_name] = array(
      '#type' => 'select',
      '#title' => t('Order in main index'),
      '#options' => $weights,
      '#default_value' => variable_get('menutree_all_weight_' . $menu_name, '<none>'),
      '#description' => t('The path <a href="@link">@path</a> will provide multiple menu trees on a single page.  You can specify which and in what order here.  Set the weight to "&lt;none&gt;" to exclude this menu.', array(
        '@link' => url('menutree/all'),
        '@path' => 'menutree/all',
      )),
      '#attributes' => array('class' => array('menutree-weight')),
    );
  }

  return system_settings_form($form);
}
