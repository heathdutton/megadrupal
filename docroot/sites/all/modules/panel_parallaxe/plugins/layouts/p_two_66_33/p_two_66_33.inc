<?php
/**
 * @file
 * The panels layout plugin for the panels 66-33 Parallaxe layout.
 */

$plugin = array(
  'title' => t('Parallaxe two column 66/33'),
  'category' => t('Parallaxe - 2 column'),
  'icon' => 'p_two_66_33.png',
  'theme' => 'p_two_66_33',
  'admin css' => 'p_two_66_33.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'two_66_33_top'    => t('Top (conditional)'),
    'two_66_33_first_1'  => t('Left 1'),
    'two_66_33_second_1' => t('Right 1'),
    'two_66_33_middle' => t('Middle (conditional)'),
    'two_66_33_first_2'  => t('Left 2'),
    'two_66_33_second_2' => t('Right 2'),
    'two_66_33_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'two',
  'options' => array(
    'two-66-33' => 'default',
    'two-66-33-stack' => 'stack',
  ),
  'styles' => array(
    'two-66-33' => array(
      'css' => array(
        '66' => array('.two-66-33 > .region-two-66-33-first' => 'width:66.666666%'),
        '33' => array('.two-66-33 > .region-two-66-33-second' => 'width:33.333333%'),
      ),
    ),
    'two-66-33-stack' => array(
      'css' => array(
        'fdw' => array('.two-66-33 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for p_two_66_33.tpl.php.
 */
function template_preprocess_p_two_66_33(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
