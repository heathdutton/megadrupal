<?php

/**
 * Implements hook_fusion_apply_group_PLUGIN_info().
 */
function jbase5_fusion_apply_group_general_info() {
  $groups['rounded_corners'] = array(
    'title' => t('Rounded Corners'),
    'description' => t('Block rounded corner styles.'),
    'weight' => -15,
  );

  return $groups;
}

/**
 * Implements hook_fusion_apply_skin_PLUGIN_info().
 */
function jbase5_fusion_apply_skin_general_info() {

  $skins = array();

  $skins['jbase5_skins_rounded_corners'] = array(
    'title' => t('Rounded corners'),
    'description' => t('Default is no background color or rounded corners'),
    'type' => 'radios',
    'group' => 'rounded_corners',
    'default status' => TRUE,
    'options' => array(
      'rounded-corner-blue' => array(
        'title' => 'Blue background and corners',
        'class' => array('jbase5-rounded-corner', 'jbase5-rounded-corner-blue'),
      ),
      'rounded-corner-blue-dual' => array(
        'title' => 'Blue background with light blue body and corners',
        'class' => array('jbase5-rounded-corner', 'jbase5-rounded-corner-blue-dual'),
      ),
    ),
  );  
  
  $skins['jbase5_skins_general_styles'] = array(
    'title' => t('General styles'),
    'description' => t('These are some generally useful options for blocks'),
    'theme hooks' => array('block'),
    'type' => 'checkboxes',
    'group' => 'general',
    'default status' => TRUE,
    'options' => array(
      'jbase5-callout' => array(
        'title' => t('Text: large, bold callout style'),
        'class' => array('jbase5-callout'),
      ),
      'jbase5-bold-links' => array(
        'title' => t('Links: bold all links'),
        'class' => array('jbase5-bold-links'),
      ),
    ),
  );  

  return $skins;
}
