<?php
/**
 * @file
 * The panels layout plugin for the panels one column Parallaxe layout.
 */

$plugin = array(
  'title' => t('Parallaxe One column'),
  'category' => t('Parallaxe - 1 column'),
  'icon' => 'p_one.png',
  'theme' => 'p_one',
  'admin css' => 'p_one.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'one_main_top' => t('Top (conditional)'),
    'one_main_1' => t('Main 1'),
    'one_main_middle' => t('Middle (conditional)'),
    'one_main_2' => t('Main 2 (conditional)'),
    'one_main_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'one',
);

/**
 * Preprocess variables for p_one.tpl.php.
 */
function template_preprocess_p_one(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
