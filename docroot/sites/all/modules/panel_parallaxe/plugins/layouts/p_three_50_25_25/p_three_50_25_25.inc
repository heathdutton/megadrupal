<?php
/**
 * @file
 * The panels layout plugin for the panels 50-25-25 Parallaxe layout.
 */

$plugin = array(
  'title' => t('Parallaxe Three column 50/25/25'),
  'category' => t('Parallaxe - 3 column'),
  'icon' => 'p_three_50_25_25.png',
  'theme' => 'p_three_50_25_25',
  'admin css' => 'p_three_50_25_25.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'three_50_25_25_top' => t('Top (conditional)'),
    'three_50_25_25_first_1' => t('Left 1'),
    'three_50_25_25_second_1' => t('Center 1'),
    'three_50_25_25_third_1' => t('Right 1'),
    'three_50_25_25_middle' => t('Middle (conditional)'),
    'three_50_25_25_first_2' => t('Left 2 (conditional)'),
    'three_50_25_25_second_2' => t('Center 2 (conditional)'),
    'three_50_25_25_third_2' => t('Right 2 (conditional)'),
    'three_50_25_25_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'three',
  'options' => array(
    'three-50-25-25' => 'default',
    'three-50-25-25-stack-bottom' => 'stack bottom',
    'three-50-25-25-stack-top' => 'stack top',
    'three-50-25-25-stack' => 'stack',
  ),
  'styles' => array(
    'three-50-25-25' => array(
      'css' => array(
        '25' => array('.three-50-25-25 > .region' => 'width:25%'),
        '50' => array('.three-50-25-25 > div.region-three-50-25-25-first' => 'width:50%'),
      ),
    ),
    'three-50-25-25-stack-bottom' => array(
      'css' => array(
        '75' => array('.three-50-25-25 > div.region-three-50-25-25-first' => 'width:75%'),
        '25' => array('.three-50-25-25 > div.region-three-50-25-25-second' => 'width:25%'),
        'fdw' => array('.three-50-25-25 > .region-three-50-25-25-third' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-50-25-25-stack-top' => array(
      'css' => array(
        '50' => array('.three-50-25-25 > .region-three-50-25-25-second,.three-50-25-25 .region-three-50-25-25-third' => 'width:50%'),
        'fdw' => array('.three-50-25-25 > .region-three-50-25-25-first' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-50-25-25-stack' => array(
      'css' => array(
        'fdw' => array('.three-50-25-25 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for p_three_50_25_25.tpl.php.
 */
function template_preprocess_p_three_50_25_25(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
