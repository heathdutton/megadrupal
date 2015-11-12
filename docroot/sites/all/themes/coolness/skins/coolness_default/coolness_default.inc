<?php

/**
 * Implements hook_skinr_skin_PLUGIN_info().
 */
function coolness_skinr_skin_coolness_default_info() {
  $skins = array();
  
  $skins['coolness_block_background'] = array(
    'title' => t('Sidebar Block Background'),
    'description' => t('Select a background color.'),
    'type' => 'select',
    'theme hooks' => array('block'),
    'group' => 'box',
    'attached' => array('css' => array('coolness-default.css')),
    'default status' => 1,
    'options' => array(
      'option_1' => array(
        'title' => t('Gray background'),
        'class' => array('coolness-block-background-gray'),
      ),
      'option_2' => array(
        'title' => t('Blue background'),
        'class' => array('coolness-block-background-blue'),
      ),
      'option_3' => array(
        'title' => t('Green background'),
        'class' => array('coolness-block-background-green'),
      ),
      'option_4' => array(
        'title' => t('Orange background'),
        'class' => array('coolness-block-background-orange'),
      ),      
    ),
  );

  $skins['coolness_block_borders'] = array(
    'title' => t('Sidebar Block Border'),
    'description' => t('Select a border color.'),
    'type' => 'select',
    'theme hooks' => array('block'),
    'group' => 'box',
    'default status' => 1,
    'options' => array(
      'option_1' => array(
        'title' => t('Gray border'),
        'class' => array('coolness-block-border-gray'),
      ),
      'option_2' => array(
        'title' => t('Blue border'),
        'class' => array('coolness-block-border-blue'),
      ),
      'option_3' => array(
        'title' => t('Green border'),
        'class' => array('coolness-block-border-green'),
      ),
      'option_4' => array(
        'title' => t('Orange border'),
        'class' => array('coolness-block-border-orange'),
      ),      
    ),
  );
  
  $skins['coolness_image_border'] = array(
    'title' => t('Image border'),
    'type' => 'checkboxes',
    'description' => t('Add a colored border around images.'),
    'theme hooks' => array('block', 'node', 'panels_display', 'panels_pane', 'panels_panel', 'views_view'),    
    'group' => 'box',
    'default status' => TRUE,
    'options' => array(
      'coolness_image_border' => array(
        'title' => t('Add border to images.'),
        'class' => array('coolness-image-border'),
      ),
    ),
  );
  
  $skins['coolness_float_imagefield'] = array(
    'title' => t('Image floating styles'),
    'type' => 'select',
    'description' => t('These options will float ImageField images in content to the left or right and add margins'),
    'theme hooks' => array('block', 'node', 'views_view'),    
    'group' => 'layout',
    'default status' => TRUE,
    'options' => array(
      'coolness_float_imagefield_left' => array(
        'title' => t('Float ImageField left'),
        'class' => array('coolness-float-imagefield-left'),
      ),
      'coolness_float_imagefield_right' => array(
        'title' => t('Float ImageField right'),
        'class' => array('coolness-float-imagefield-right'),
      ),
    ),
  );
  
  return $skins;
}
