<?php
/**
 * @file
 * The panels layout plugin for the panels 3x33 Parallaxe layout.
 */

$plugin = array(
  'title'    => t('Parallaxe Three column 3x33'),
  'category' => t('Parallaxe - 3 column'),
  'icon'     => 'p_three_3x33.png',
  'theme'    => 'p_three_3x33',
  'admin css' => 'p_three_3x33.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'three_33_top'    => t('Top (conditional)'),
    'three_33_first_1'  => t('Left 1'),
    'three_33_second_1' => t('Center 1'),
    'three_33_third_1'  => t('Right 1'),
    'three_33_middle' => t('Middle (conditional)'),
    'three_33_first_2'  => t('Left 2'),
    'three_33_second_2' => t('Center 2'),
    'three_33_third_2'  => t('Right 2'),
    'three_33_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'three',
  'options' => array(
    'three-3x33' => 'default',
    'three-3x33-5050-stack-bottom' => 'even stack bottom',
    'three-3x33-5050-stack-middle' => 'even stack middle',
    'three-3x33-stack-bottom' => 'stack bottom',
    'three-3x33-stack-middle' => 'stack middle',
    'three-3x33-stack-top' => 'stack top',
    'three-3x33-stack' => 'stack',
  ),
  'styles' => array(
    'three-3x33' => array(
      'css' => array(
        '33' => array('.three-3x33 > .region' => 'width:33.333333%'),
      ),
    ),
    'three-3x33-5050-stack-bottom' => array(
      'css' => array(
        '50' => array('.three-3x33 > .region' => 'width:50%'),
        'fdw' => array('.three-3x33 > .region-three-33-third' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-3x33-stack-bottom' => array(
      'css' => array(
        '66' => array('.three-3x33 > .region-three-33-first' => 'width:66.666666%'),
        '33' => array('.three-3x33 > .region-three-33-second' => 'width:33.333333%'),
        'fdw' => array('.three-3x33 > .region-three-33-third' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-3x33-stack-middle' => array(
      'css' => array(
        '50' => array('.three-3x33 > .region' => 'width:50%'),
        'fdw' => array('.three-3x33 > div.region-three-33-first' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-3x33-stack-top' => array(
      'css' => array(
        '50' => array('.three-3x33 > .region' => 'width:50%'),
        'fdw' => array('.three-3x33 > div.region-three-33-first' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-3x33-stack' => array(
      'css' => array(
        'fdw' => array('.three-3x33 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for p_three_3x33.tpl.php.
 */
function template_preprocess_p_three_3x33(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
