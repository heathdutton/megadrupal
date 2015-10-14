<?php

/**
 * Implements hook_skinr_group_PLUGIN_info().
 */
function skins_skinr_group_core_info() {
  $groups['menus'] = array(
    'title' => t('Menus'),
    'description' => t('Menu and navigation styles.'),
  );

  return $groups;
}

/**
 * Implements hook_skinr_skin_PLUGIN_info().
 */
function skins_skinr_skin_core_info() {
  $skins = array();
  
  $skins['skins_core_skins_block_positioning'] = array(
    'title' => t('Block position'),
    'type' => 'select',
    'description' => t('Change the position of this block (default is to float to the left)'),
    'group' => 'layout',
    'theme hooks' => array('block'),
    'default status' => TRUE,
    'options' => array(
      'skins-core-left' => array(
        'title' => t('Float block to the left'),
        'class' => array('skins-core-left'),
      ),
      'skins-core-right' => array(
        'title' => t('Float block to the right'),
        'class' => array('skins-core-right'),
      ),
      'skins-core-center' => array(
        'title' => t('Position block in the center'),
        'class' => array('skins-core-center'),
      ),
      'skins-core-clear' => array(
        'title' => t('Clear floats (block drops to a new line)'),
        'class' => array('skins-core-clear'),
      ),      
    ),
  );
  
  $skins['skins_core_skins_content_alignment'] = array(
    'title' => t('Content alignment'),
    'description' => t('Default is left aligned content'),
    'type' => 'select',
    'group' => 'layout',
    'default status' => TRUE,
    'options' => array(
      'skins-core-right' => array(
        'title' => t('Center align content within its container'),
        'class' => array('skins-core-center-content'),
      ),
      'skins-core-center' => array(
        'title' => t('Right align content within its container'),
        'class' => array('skins-core-right-content'),
      ),   
    ),
  );  
  
  $skins['skins_core_skins_general_styles'] = array(
    'title' => t('General styles'),
    // 'description' => t('These are some generally useful options for blocks'),
    'theme hooks' => array('block'),
    'type' => 'checkboxes',
    'group' => 'general',
    'default status' => TRUE,
    'options' => array(
      'skins-core-callout' => array(
        'title' => t('Text: large, bold callout style'),
        'class' => array('skins-core-callout'),
      ),
      'skins-core-bold-links' => array(
        'title' => t('Links: bold all links'),
        'class' => array('skins-core-bold-links'),
      ),
      'skins-core-padding' => array(
        'title' => 'Padding: add 30px extra padding inside block',
        'class' => array('skins-core-padding'),
      ),
      'skins-core-border' => array(
        'title' => 'Border: add 1px border and 10px padding',
        'class' => array('skins-core-border'),
      ),
    ),
  );  
  
  $skins['skins_core_equal_heights'] = array(
    'title' => t('Equal heights'),
    'description' => t('Make blocks in a region the same height'),
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'type' => 'checkboxes',
    'group' => 'layout',
    'default status' => TRUE,
    'attached' => array(
       'js' => array('js/jquery.equalheights.js'), 
    ),
    'options' => array(
      'equal-heights' => array(
        'title' => t('Equal heights: equalize the heights of blocks with this style in the same region'),
        'class' => array('equal-heights'),
      ),
    ),
  );    
  
  $skins['skins_core_multicol'] = array(
    'title' => t('List columns'),
    'type' => 'select',
    'description' => t('Put items in lists (menus, list views, etc.) in multiple columns'),    
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel', 'views_view'),    
    'group' => 'general',
    'default status' => TRUE,
    'options' => array(
      'skins-core-2-col-list' => array(
        'title' => t('2-column list/menu (50%/50%)'),
        'class' => array('skins-core-2-col-list', 'clearfix'),
      ),
      'skins-core-3-col-list' => array(
        'title' => t('3-column list/menu (33%/33%/33%)'),
        'class' => array('skins-core-3-col-list', 'clearfix'),      
      )
    ),
  );   
  
  $skins['skins_core_menu'] = array(
    'title' => t('Menu Layout'),
    'type' => 'select',
    'description' => t('Different layouts and alignment options for your menus'),    
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),    
    'group' => 'menus',
    'default status' => TRUE,
    'options' => array(
      'skins-core-inline-menu' => array(
        'title' => t('Single line menu with separators'),
        'class' => array('skins-core-inline-menu'),
      ),
      'skins-core-multicol-menu' => array(
        'title' => t('Multi-column menu with bold headers (set menu items to Expanded)'),
        'class' => array('skins-core-multicol-menu', 'clearfix')
      )
    ),
  );  
  
  $skins['skins_core_list_styles'] = array(
    'title' => t('List/menu styles'),
    'type' => 'checkboxes',
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel', 'views_view'),    
    'group' => 'menus',
    'default status' => TRUE,
    'options' => array(
      'skins-core-list-bottom-border' => array(
        'title' => t('Bottom border (no bullets)'),
        'class' => array('skins-core-list-bottom-border'),
      ),
      'skins-core-multicol-menu' => array(
        'title' => t('Extra vertical spacing (no bullets)'),
        'class' => array('skins-core-list-vertical-spacing')
      )
    ),
  );  
    
  $skins['skins_core_float_imagefield'] = array(
    'title' => t('Image floating styles'),
    'type' => 'select',
    'description' => t('These options will float image field images in content to the left or right and add margins'),
    'theme hooks' => array('block', 'node', 'views_view'),    
    'group' => 'layout',
    'default status' => TRUE,
    'options' => array(
      'skins_core_float_imagefield_left' => array(
        'title' => t('Float ImageField left'),
        'class' => array('skins-core-float-imagefield-left'),
      ),
      'skins_core_float_imagefield_right' => array(
        'title' => t('Float ImageField right'),
        'class' => array('skins-core-float-imagefield-right'),
      ),
    ),
  );   
  
  $skins['skins_core_login'] = array(
    'title' => t('User login block styles'),
    'theme hooks' => array('block', 'panels_display', 'panels_pane', 'panels_panel'),
    'group' => 'box',
    'default status' => TRUE,
    'attached' => array(
       'js' => array('js/jquery.overlabel.js'),
    ),
    'options' => array(
      'skins_core_horizontal' => array(
        'title' => t('Horizontal: puts user login block on a single line with label overlays (good for Header Top region)'),
        'class' => array('skins-core-horiz-login'),
      ),
    ),
  );
  
  return $skins;
}

