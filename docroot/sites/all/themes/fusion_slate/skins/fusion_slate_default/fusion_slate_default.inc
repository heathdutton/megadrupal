<?php
/**
 * Default skins settings.
 */

/**
 * Implements hook_fusion_apply_skin_PLUGIN_info().
 */
function fusion_slate_fusion_apply_skin_fusion_slate_default_info() {

  $skins = array();

  $skins['fusion_slate_background'] = array(
    'title' => t('Background Styles'),
    'type' => 'radios',
    'theme hooks' => array(
      'block',
      'panels_display',
      'panels_pane',
      'panels_panel',
      'views-view'),
    'group' => 'box',
    'attached' => array('css' => array('fusion_slate_default.css')),
    'default status' => TRUE,
    'options' => array(
      'dark-transparent-background' => array(
        'title' => 'Dark transparent Background (Use only in the Preface Top region with a banner background)',
        'class' => array('dark-transparent-background'),
      ),
    ),
  );

  $skins['fusion_slate_titles'] = array(
    'title' => t('Title Styles'),
    'type' => 'radios',
    'theme hooks' => array(
      'block',
      'panels_display',
      'panels_pane',
      'panels_panel',
      'views-view'),
    'group' => 'box',
    'attached' => array('css' => array('fusion_slate_default.css')),
    'default status' => TRUE,
    'options' => array(
      'title-white-bold-first' => array(
        'title' => 'White title with bold first word and capital letters',
        'class' => array('title-white-bold-first'),
      ),
      'title-dual-color-arrow title-dual' => array(
        'title' => 'Dual color bold title and capital letters with arrow',
        'class' => array('title-dual-color-arrow', 'title-dual'),
      ),
      'title-yellow-bold' => array(
        'title' => 'Yellow title with bold and capital letters',
        'class' => array('title-yellow-bold'),
      ),
    ),
  );

  $skins['fusion_slate_corners'] = array(
    'title' => t('Rounded Corner Styles'),
    'type' => 'radios',
    'theme hooks' => array(
      'block',
      'panels_display',
      'panels_pane',
      'panels_panel',
      'views-view'),
    'group' => 'box',
    'attached' => array('css' => array('fusion_slate_default.css')),
    'default status' => TRUE,
    'options' => array(
      'rounded-corners-gray-background' => array(
        'title' => 'Rounded corners with gray background',
        'class' => array('rounded-corners-gray-background'),
      ),
      'rounded-corners-gradient-background' => array(
        'title' => 'Rounded corners with gradient background and border',
        'class' => array('rounded-corners-gradient-background'),
      ),
    ),
  );

  $skins['fusion_slate_banners'] = array(
    'title' => t('Banner Styles'),
    'type' => 'radios',
    'theme hooks' => array('page'),
    'group' => 'box',
    'attached' => array('css' => array('fusion_slate_default.css')),
    'default status' => TRUE,
    'options' => array(
      'banner-background-beachstones' => array(
        'title' => 'Beach stones',
        'class' => array('banner-background-beachstones', 'banner-background'),
      ),
      'banner-background-citystreetlights' => array(
        'title' => 'City street lights',
        'class' => array('banner-background-citystreetlights', 'banner-background'),
      ),
      'banner-background-greekheads' => array(
        'title' => 'Greek heads',
        'class' => array('banner-background-greekheads', 'banner-background'),
      ),
      'banner-background-seascape' => array(
        'title' => 'Seascape',
        'class' => array('banner-background-seascape', 'banner-background'),
      ),
      'banner-background-windows' => array(
        'title' => 'Windows',
        'class' => array('banner-background-windows', 'banner-background'),
      ),
    ),
  );

  return $skins;
}
