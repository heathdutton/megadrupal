<?php
// $Id: devel_info_custom_theme.inc,v 1.1.2.1 2010/07/14 11:51:51 flanker Exp $

/**
 * @file
 *   custom_theme block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_custom_theme_block_help() {

  $output = '<p>'. t("This block displays list of modules which override the system default theme sorted by module's call-up. To find out, it executes <em>hook_init()</em> function of each module.") .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_custom_theme_block_info() {

  $block = array(
    'info' => t('DI: Custom Theme'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_custom_theme_block_configure() {

  $config = devel_info_custom_theme_get_variable();

  $form = array();
  $form['custom_theme_block_behavior'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block behavior'),
    '#description' => t('Configure block behavior.'),
    '#default_value' => array(
      $config['block_content'],
      $config['block_footer'],
    ),
    '#options' => array(
      'block_content' => t('Show block content.'),
      'block_footer' => t('Show block footer links.'),
    ),
  );
  $form['block_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block content'),
  );
  // Custom theme
  $form['block_content']['custom_theme_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('Blacklist'),
    '#description' => t('Enter one module per line of each modules that should not execute <em>hook_custom_theme()</em>. Example modules are <em>og</em> for the Organic Groups module and <em>views</em> for the Views module.'),
    '#default_value' => $config['blacklist'],
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_custom_theme_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['custom_theme_block_behavior']['block_content'];
  $var['block_footer'] = $edit['custom_theme_block_behavior']['block_footer'];
  // Custom theme
  $var['blacklist'] = trim($edit['custom_theme_blacklist']);

  variable_set('devel_info_custom_theme', serialize($var));
}


/**
 * Block content.
 */
function devel_info_custom_theme_block_view() {

  $config = devel_info_custom_theme_get_variable();

  if ($config['block_content']) {

    $blacklist = preg_split("/\r\n|\n|\r|\s/",  $config['blacklist'], -1, PREG_SPLIT_NO_EMPTY);

    $item = array();
    $item[] = t('Default theme: %theme', array('%theme' => variable_get('theme_default', 'garland')));

    foreach (module_implements('custom_theme') as $module) {

      if (!in_array($module, $blacklist)) {

        $custom_theme = module_invoke($module, 'custom_theme');
        if (!is_null($custom_theme)) {
          $item[] = t('Theme %theme is set by %module', array('%theme' => $custom_theme, '%module' => $module .'_custom_theme()'));
        }
      }
    }

    $custom_theme = menu_get_custom_theme();
    $item[] = t('Custom theme: %theme', array('%theme' => !is_null($custom_theme) ? $custom_theme : ''));
  }

  $block = array();
  $block['subject'] = t('DI: Custom Theme');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= devel_info_get_block_link('custom_theme', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_custom_theme_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_custom_theme_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_custom_theme', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_custom_theme_block_uninstall() {

  variable_del('devel_info_custom_theme');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_custom_theme_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // Custom theme
  $default['blacklist'] = "ctools\nog\nviews";

  $custom = variable_get('devel_info_custom_theme', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}
