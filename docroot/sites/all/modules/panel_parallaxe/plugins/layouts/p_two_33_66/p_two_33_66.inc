<?php
/**
 * @file
 * The panels layout plugin for the panels 33-66 Parallaxe layout.
 */

$plugin = array(
  'title' => t('Parallaxe column 33/66'),
  'category' => t('Parallaxe - 2 column'),
  'icon' => 'p_two_33_66.png',
  'theme' => 'p_two_33_66',
  'admin css' => 'p_two_33_66.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'two_33_66_top'    => t('Top (conditional)'),
    'two_33_66_first_1'  => t('Left 1'),
    'two_33_66_second_1' => t('Right 1'),
    'two_33_66_middle' => t('Middle (conditional)'),
    'two_33_66_first_2'  => t('Left 2 (conditional)'),
    'two_33_66_second_2' => t('Right 2 (conditional)'),
    'two_33_66_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'two',
  'options' => array(
    'two-33-66' => 'default',
    'two-33-66-stack' => 'stack',
  ),
  'styles' => array(
    'two-33-66' => array(
      'css' => array(
        '33' => array('.two-33-66 > .region-two-33-66-first' => 'width:33.333333%'),
        '66' => array('.two-33-66 > .region-two-33-66-second' => 'width:66.666666%'),
      ),
    ),
    'two-33-66-stack' => array(
      'css' => array(
        'fdw' => array('.two-33-66 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);


/**
 * Preprocess variables for p_two_33_66.tpl.php.
 */
function template_preprocess_p_two_33_66(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
