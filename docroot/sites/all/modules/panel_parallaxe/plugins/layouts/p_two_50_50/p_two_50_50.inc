<?php
/**
 * @file
 * The panels layout plugin for the panels 50-50 Parallaxe layout.
 */

$plugin = array(
  'title'    => t('Parallaxe Two column brick'),
  'category' => t('Parallaxe - 2 column'),
  'icon'     => 'p_two_50_50.png',
  'theme'    => 'p_two_50_50',
  'admin css' => 'p_two_50_50.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'two_50_50_top'     => t('Top (conditional)'),
    'two_50_50_left_1'  => t('Left 1'),
    'two_50_50_right_1' => t('Right 1'),
    'two_50_50_middle'  => t('Middle (conditional)'),
    'two_50_50_left_2'  => t('Left 2 (conditional)'),
    'two_50_50_right_2' => t('Right 2 (conditional)'),
    'two_50_50_bottom'  => t('Bottom (conditional)'),
  ),
  'type' => 'two',
  'options' => array(
    'two-brick' => 'default',
    'two-brick-stack' => 'stack',
  ),
  'styles' => array(
    'two-brick' => array(
      'css' => array(
        'fn' => array('.two-brick > .panel-row' => 'float:none'),
        '50' => array('.two-brick > .panel-row > .region' => 'width:50%'),
      ),
    ),
    'two-brick-stack' => array(
      'css' => array(
        'fdw' => array('.two-brick > .panel-row > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for p_two_50_50.tpl.php.
 */
function template_preprocess_p_two_50_50(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
