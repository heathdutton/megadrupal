<?php
/**
 * Default include file for fusion skins.
 */

/**
 * Implements hook_fusion_apply_skin_PLUGIN_info().
 */
function fusion_prosper_fusion_apply_skin_fusion_prosper_default_info() {

  $skins = array();

  $skins['fusion_prosper_general_styles'] = array(
    'title' => t('General Styles'),
    'type' => 'radios',
    'attached' => array('css' => array('fusion-prosper-default.css')),
    'default status' => TRUE,
    'group' => 'box',
    'options' => array(
      'general_1' => array(
        'title' => t('Gray rounded title background, plain white block'),
        'class' => array('prosper-gray-rounded-plain', 'prosper-rounded-title')),
      'general_2' => array(
        'title' => t('Gray rounded title background, list styling'),
        'class' => array('prosper-gray-rounded-style', 'prosper-rounded-title')),
      'general_3' => array(
        'title' => t('Rounded corner dark background'),
        'class' => array('prosper-grayborder-darkbackground')),
      'general_4' => array(
        'title' => t('Light gray background and border'),
        'class' => array('prosper-lightgraybackground-border')),
      'general_5' => array(
        'title' => t('Medium gray background with bottom border'),
        'class' => array('prosper-mediumgraybackground-bottomborder')),
      'general_6' => array(
        'title' => t('Gradient background with padding and thick border'),
        'class' => array('prosper-gradient-border')),
      'general_7' => array(
        'title' => t('Menu List'),
        'class' => array('prosper-menu-list')),
    ),
  );

  $skins['fusion_prosper_blocks'] = array(
    'title' => t('Block Styles'),
    'type' => 'radios',
    'attached' => array('css' => array('fusion-prosper-default.css')),
    'default status' => TRUE,
    'group' => 'box',
    'options' => array(
      'block_1' => array(
        'title' => t('Shopping cart - Light background'),
        'class' => array('prosper-shoppingcart-light')),
      'block_2' => array(
        'title' => t('Shopping cart - Dark background'),
        'class' => array('prosper-shoppingcart-dark')),
      'block_3' => array(
        'title' => t('Tabbed block - also set Quicktabs style to Default'),
        'class' => array('prosper-quicktabs')),
    ),
  );

  $skins['fusion_prosper_comments'] = array(
    'title' => t('Comments'),
    'type' => 'radios',
    'attached' => array('css' => array('fusion-prosper-default.css')),
    'default status' => TRUE,
    'group' => 'box',
    'options' => array(
      'comment_1' => array(
        'title' => t('Simple with white background and separators'),
        'class' => array('prosper-comments-clean')),
      'comment_2' => array(
        'title' => t('Speech bubbles with pointed edge'),
        'class' => array('prosper-comments-edgy')),
    ),
  );

  $skins['fusion_prosper_common_styles'] = array(
    'title' => t('Common Styles'),
    'type' => 'checkboxes',
    'attached' => array('css' => array('fusion-prosper-default.css')),
    'default status' => TRUE,
    'group' => 'box',
    'options' => array(
      'common_1' => array(
        'title' => t('border around image'),
        'class' => array('prosper-gray-border-image')),
    ),
  );

  $skins['fusion_prosper_header_nav'] = array(
    'title' => t('Header Navigation'),
    'type' => 'checkboxes',
    'attached' => array('css' => array('fusion-prosper-default.css')),
    'default status' => TRUE,
    'group' => 'box',
    'options' => array(
      'header_nav_1' => array(
        'title' => t('inline user menu (for header menu only)'),
        'class' => array('user-menu')),
    ),
  );

  return $skins;
}
