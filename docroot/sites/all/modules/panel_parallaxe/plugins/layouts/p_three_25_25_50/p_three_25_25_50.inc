<?php
/**
 * @file
 * The panels layout plugin for the panels 25-25-50 Parallaxe layout.
 */

$plugin = array(
  'title'    => t('Parallaxe Three column 25/25/50'),
  'category' => t('Parallaxe - 3 column'),
  'icon'     => 'p_three_25_25_50.png',
  'theme'    => 'p_three_25_25_50',
  'admin css' => 'p_three_25_25_50.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'three_25_25_50_top'    => t('Top (conditional)'),
    'three_25_25_50_first_1'  => t('Left 1'),
    'three_25_25_50_second_1' => t('Center 1'),
    'three_25_25_50_third_1'  => t('Right 1'),
    'three_25_25_50_middle' => t('Middle (conditional)'),
    'three_25_25_50_first_2'  => t('Left 2'),
    'three_25_25_50_second_2' => t('Center 2'),
    'three_25_25_50_third_2'  => t('Right 2'),
    'three_25_25_50_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'three',
  'options' => array(
    'three-25-25-50' => 'default',
    'three-25-25-50-stack-bottom' => 'stack bottom',
    'three-25-25-50-stack-top' => 'stack top',
    'three-25-25-50-stack' => 'stack',
  ),
  'styles' => array(
    'three-25-25-50' => array(
      'css' => array(
        '25' => array('.three-25-25-50 > .region' => 'width:25%'),
        '50' => array('.three-25-25-50 > div.region-three-25-25-50-third' => 'width:50%'),
      ),
    ),
    'three-25-25-50-stack-bottom' => array(
      'css' => array(
        '50' => array('.three-25-25-50 > .region' => 'width:50%'),
        'fdw' => array('.three-25-25-50 > .region-three-25-25-50-third' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-25-25-50-stack-top' => array(
      'css' => array(
        'fdw' => array('.three-25-25-50 > .region-three-25-25-50-first' => 'float:none;display:block;width:100%;clear:both'),
        '25' => array('.three-25-25-50 > div.region-three-25-25-50-second' => 'width:25%'),
        '75' => array('.three-25-25-50 > div.region-three-25-25-50-third' => 'width:75%'),
      ),
    ),
    'three-25-25-50-stack' => array(
      'css' => array(
        'fdw' => array('.three-25-25-50 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for p_three_25_25_50.tpl.php.
 */
function template_preprocess_p_three_25_25_50(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
