<?php
/**
 * @file
 * The panels layout plugin for the panels 25-50-25 Parallaxe layout.
 */

$plugin = array(
  'title'    => t('Parallaxe Three column 25/50/25'),
  'category' => t('Parallaxe - 3 column'),
  'icon'     => 'p_three_25_50_25.png',
  'theme'    => 'p_three_25_50_25',
  'admin css' => 'p_three_25_50_25.admin.css',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'three_25_50_25_top'    => t('Top (conditional)'),
    'three_25_50_25_first_1'  => t('Left 1'),
    'three_25_50_25_second_1' => t('Center 1'),
    'three_25_50_25_third_1'  => t('Right 1'),
    'three_25_50_25_middle'    => t('Middle (conditional)'),
    'three_25_50_25_first_2'  => t('Left 2 (conditional)'),
    'three_25_50_25_second_2' => t('Center 2 (conditional)'),
    'three_25_50_25_third_2'  => t('Right 2 (conditional)'),
    'three_25_50_25_bottom' => t('Bottom (conditional)'),
  ),
  'type' => 'three',
  'options' => array(
    'three-25-50-25' => 'default',
    'three-25-50-25-stack-bottom' => 'stack bottom',
    'three-25-50-25-stack-top' => 'stack top',
    'three-25-50-25-stack' => 'stack',
  ),
  'styles' => array(
    'three-25-50-25' => array(
      'css' => array(
        '25' => array('.three-25-50-25 > .region' => 'width:25%'),
        '50' => array('.three-25-50-25 > div.region-three-25-50-25-second' => 'width:50%'),
      ),
    ),
    'three-25-50-25-stack-bottom' => array(
      'css' => array(
        '25' => array('.three-25-50-25 > div.region-three-25-50-25-first' => 'width:25%'),
        '75' => array('.three-25-50-25 > div.region-three-25-50-25-second' => 'width:75%'),
        'fdw' => array('.three-25-50-25 > .region-three-25-50-25-third' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
    'three-25-50-25-stack-middle' => array(
      'css' => array(
        'fdw' => array('.three-25-50-25 > .region-three-25-50-25-first' => 'float:none;display:block;width:100%;clear:both'),
        '75' => array('.three-25-50-25 > div.region-three-25-50-25-second' => 'width:75%'),
        '25' => array('.three-25-50-25 > div.region-three-25-50-25-third' => 'width:25%'),
      ),
    ),
    'three-25-50-25-stack-top' => array(
      'css' => array(
        'fdw' => array('.three-25-50-25 > .region-three-25-50-25-first' => 'float:none;display:block;width:100%;clear:both', '.three-25-50-25 > .region-three-25-50-25-fourth' => 'float:none;display:block;width:100%;clear:both'),
        '75' => array('.three-25-50-25 > div.region-three-25-50-25-second' => 'width:75%', '.three-25-50-25 > div.region-three-25-50-25-fifth' => 'width:75%'),
        '25' => array('.three-25-50-25 > div.region-three-25-50-25-third' => 'width:25%', '.three-25-50-25 > div.region-three-25-50-25-sixt' => 'width:25%'),
      ),
    ),
    'three-25-50-25-stack' => array(
      'css' => array(
        'fdw' => array('.three-25-50-25 > .region' => 'float:none;display:block;width:100%;clear:both'),
      ),
    ),
  ),
);

/**
 * Preprocess variables for three-25-50-25-100-25-50-25.tpl.php.
 */
function template_preprocess_p_three_25_50_25(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
