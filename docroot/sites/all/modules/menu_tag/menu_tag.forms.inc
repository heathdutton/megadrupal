<?php

/**
 * @file
 * Different form functions for menu_tag module.
 */

/**
 * Add the menu tag to a menu item edit form.
 *
 * @param $form
 *   The menu item edit form passed by reference.
 */
function menu_tag_menu_item_form_alter(array &$form) {
  menu_tag_add_form_field($form);
  $form['menu_tag']['#title'] = t('Menu tag');
  $form['menu_tag']['#description'] = t('Specify menu tag for this menu item.');
  $form['#submit'][] = 'menu_tag_menu_item_form_submit';
}

/**
 * Form submit handler for menu item link form.
 *
 * Saves menu tag to menu_links table.
 */
function menu_tag_menu_item_form_submit($form, &$form_state) {
  module_load_include('inc', 'menu_tag', 'menu_tag.db');
  // Empty menu tag is also valid state and we will save it.
  menu_tag_db_save_menu_item_tag($form_state['values']['mlid'], $form_state['values']['menu_tag']);
  menu_tag_db_cache_clear($form_state['values']['menu_name']);
}

/**
 * Add the menu tag to a node menu item edit form.
 *
 * @param $form
 *   The node menu item edit form passed by reference.
 */
function menu_tag_menu_item_node_form_alter(array &$form) {
  $link_element =& $form['menu']['link'];
  menu_tag_add_form_field($link_element);
  $link_element['menu_tag']['#title'] = t('Menu tag');
  $link_element['menu_tag']['#description'] = t('Specify menu tag for this menu item.');
  // We will not use submit handler for saving, saving is done in 
  // menu_tag_node_insert().
}

/**
 * Add menu tag select option to a menu block edit form.
 */
function menu_tag_menu_block_form_alter(array &$form) {
  $form['menu_tag'] = menu_tag_filter_select_element($form['delta']['#value']);  
  $form['#submit'][] = 'menu_tag_menu_block_form_submit';
}

/**
 * Form submit handler for menu block form.
 *
 * Saves menu tag for this block.
 */
function menu_tag_menu_block_form_submit($form, &$form_state) {
  module_load_include('inc', 'menu_tag', 'menu_tag.db');
  menu_tag_db_save_menu_tag($form_state['values']['delta'], $form_state['values']['menu_tag']);
}

/**
 * Add menu tag select options to a menu configure for for main and secondary 
 * menus.
 */
function menu_tag_menu_configure_form_alter(array &$form) {
  $form['menu_tag'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu tags'),
  );
  $form['menu_tag']['menu_tag_main_menu'] = menu_tag_filter_select_element('main_menu');
  $form['menu_tag']['menu_tag_main_menu']['#title'] = t('Main menu');
  $form['menu_tag']['menu_tag_secondary_menu'] = menu_tag_filter_select_element('secondary_menu');
  $form['menu_tag']['menu_tag_secondary_menu']['#title'] = t('Secondary menu');
  
  $form['#submit'][] = 'menu_tag_menu_configure_form_submit';
}

/**
 * Form submit handler for menu configure form.
 *
 * Saves menu tags for main and secondary menu.
 */
function menu_tag_menu_configure_form_submit($form, &$form_state) {
  module_load_include('inc', 'menu_tag', 'menu_tag.db');
  menu_tag_db_save_menu_tag('main_menu', $form_state['values']['menu_tag_main_menu']);
  menu_tag_db_save_menu_tag('secondary_menu', $form_state['values']['menu_tag_secondary_menu']);
}

/**
 * Adds menu tag edit fields to menu overiview form.
 */
function menu_tag_menu_overview_form_alter(array &$form) {
  // Add menu tag field for all menu items.
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['hidden'])) {
      $element = &$form[$mlid];
      menu_tag_add_form_field($element);
      $element['menu_tag']['#size'] = 15;
    }
  }
  
  // We need new theme function to correctly render table form, because module
  // can not overide existing theme_menu_overview_form except if we want to 
  // implement evil hook_theme_registry_alter.
  $form['#theme'] = 'menu_tag_menu_overview_form';
  
  $form['#submit'][] = 'menu_tag_menu_overview_form_submit';
}

/**
 * Returns HTML for the menu overview form into a table and adds menu tag 
 * fields.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see theme_menu_overview_form()
 * 
 * @ingroup themeable
 */
function theme_menu_tag_menu_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');

  $header = array(
    t('Menu link'),
    array('data' => t('Enabled'), 'class' => array('checkbox')),
    array('data' => t('Menu Tag'), 'class' => array('textfield')),
    t('Weight'),
    array('data' => t('Operations'), 'colspan' => '3'),
  );

  $rows = array();
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['hidden'])) {
      $element = &$form[$mlid];
      // Build a list of operations.
      $operations = array();
      foreach (element_children($element['operations']) as $op) {
        $operations[] = array('data' => drupal_render($element['operations'][$op]), 'class' => array('menu-operations'));
      }
      while (count($operations) < 2) {
        $operations[] = '';
      }

      // Add special classes to be used for tabledrag.js.
      $element['plid']['#attributes']['class'] = array('menu-plid');
      $element['mlid']['#attributes']['class'] = array('menu-mlid');
      $element['weight']['#attributes']['class'] = array('menu-weight');

      // Change the parent field to a hidden. This allows any value but hides the field.
      $element['plid']['#type'] = 'hidden';

      $row = array();
      $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
      $row[] = array('data' => drupal_render($element['hidden']), 'class' => array('checkbox', 'menu-enabled'));
      $row[] = array('data' => drupal_render($element['menu_tag']), 'class' => array('tetfield'));
      $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
      $row = array_merge($row, $operations);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }
  $output = '';
  if (empty($rows)) {
    $rows[] = array(array('data' => $form['#empty_text'], 'colspan' => '7'));
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'menu-overview')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Form submit handler for menu overview form.
 */
function menu_tag_menu_overview_form_submit($form, &$form_state) {
  module_load_include('inc', 'menu_tag', 'menu_tag.db');

  // Save all menu tags.
  foreach ($form_state['values'] as $menu_item) {
    if (is_array($menu_item) && isset($menu_item['hidden'])) {
      menu_tag_db_save_menu_item_tag($menu_item['mlid'], $menu_item['menu_tag']);
    }
  }
  
  // We need to be sure that this menu cache is cleared so new menu tag values
  // will be added to cache.
  menu_cache_clear($form['#menu']['menu_name']);
  menu_tag_db_cache_clear($form['#menu']['menu_name']);
}

/* Helpers */

/**
 * Adds menu tag field to form array.
 */
function menu_tag_add_form_field(array &$form) {
  $default_value = '';
  if ($form['mlid']['#value']) {
    // Load existing menu tag from db.
    module_load_include('inc', 'menu_tag', 'menu_tag.db');
    $default_value = menu_tag_db_load_menu_item_tag($form['mlid']['#value']);
  }
  
  $form['menu_tag'] = array(
    '#type' => 'textfield',
    '#default_value' => $default_value,
    '#autocomplete_path' => 'menu_tag/autocomplete',
  );
}

/**
 * Returns menu tag form select element.
 * 
 * @param type $menu_tag_id
 *   Menu tag id - typicaly it is menu block delta.
 */
function menu_tag_filter_select_element($menu_tag_id) {
  module_load_include('inc', 'menu_tag', 'menu_tag.db');
  
  // Menu tag options.
  $options = array(
    'no-filter'   => t('Don\'t apply meny tag filter'),
    'without-tag' => t('Display only items without menu tag'),
  );
  // @todo - Do we need check_plain here for options?
  $options += menu_tag_db_load_menu_tags();
  
  return array(
    '#type' => 'select',
    '#title' => t('Menu tag filter'),
    '#options' => $options,
    '#default_value' => menu_tag_db_load_menu_tag($menu_tag_id),
    '#description' => t('Set this to menu tag you want to filter menu links in this menu.'),
  );
}
