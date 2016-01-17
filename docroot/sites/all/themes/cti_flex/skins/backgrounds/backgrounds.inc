<?php

/**
 * Implements hook_skinr_group_PLUGIN_info().
 */
function cti_flex_skinr_group_backgrounds_info() {
  $groups['block_styles'] = array(
    'title' => t('CTI Flex Block Styles'),
    'description' => t('Set background colors for block headers and content'),
  ); 
  return $groups;
}


/**
* Implements hook_skinr_skin_plugin_info().
*/
function cti_flex_skinr_skin_backgrounds_info() {
  $skins['cti_flex_background_color'] = array(
    'title' => t('Block Background'),
    'type' => 'select',
    'group' => 'block_styles',
    'default status' => 1,
    'options' => array(
      'bg_color_00' => array('title' => t('No Background'), 'class' => array('no-bg')),
      'bg_color_01' => array('title' => t('Light Gray'), 'class' => array('lt-gray-bg')),
      'bg_color_02' => array('title' => t('Medium Gray'), 'class' => array('med-gray-bg')),
      'bg_color_03' => array('title' => t('Light Blue'), 'class' => array('lt-blue-bg')),
      'bg_color_03b' => array('title' => t('Medium Blue'), 'class' => array('med-blue-bg')),
      'bg_color_04' => array('title' => t('Light Green'), 'class' => array('lt-green-bg')),
      'bg_color_04b' => array('title' => t('Medium Green'), 'class' => array('med-green-bg')),
      'bg_color_05' => array('title' => t('Light Purple'), 'class' => array('lt-purple-bg')),
      'bg_color_05b' => array('title' => t('Medium Purple'), 'class' => array('med-purple-bg')),
      'bg_color_06' => array('title' => t('Light Teal'), 'class' => array('lt-teal-bg')),
      'bg_color_06b' => array('title' => t('Medium Teal'), 'class' => array('med-teal-bg')),
      'bg_color_07' => array('title' => t('Light Orange'), 'class' => array('lt-orange-bg')),
    ),
  );
  $skins['cti_flex_header_background'] = array(
    'title' => t('Block Header Background'),
    'type' => 'select',
    'group' => 'block_styles',
    'default status' => 1,
    'options' => array(
      'color_00' => array('title' => t('No Background'), 'class' => array('no-header-bg')),
      'color_01' => array('title' => t('Black'), 'class' => array('black-header')),
      'color_02' => array('title' => t('Red'), 'class' => array('red-header')),
      'color_03' => array('title' => t('Blue'), 'class' => array('blue-header')),
      'color_04' => array('title' => t('Green'), 'class' => array('green-header')),
      'color_05' => array('title' => t('Teal'), 'class' => array('teal-header')),
      'color_06' => array('title' => t('Purple'), 'class' => array('purple-header')),
      'color_07' => array('title' => t('Orange'), 'class' => array('orange-header')),
    ),
  );
  return $skins;
}