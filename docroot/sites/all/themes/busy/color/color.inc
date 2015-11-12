<?php
 
$info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t('Header Sidebar'),
    'gradient_block' => t('Block with Gradient Background'),
    'link' => t('Link'),
    'header_bar' => t('Header Background Bar'),
    'text' => t('Copy Text'),
    'link_hover' => t('Link Hover'),
    'menu_active' => t('Menu Active'),
    'content_header' => t('Header Content Area'),
    'block_featured_header' => t('Header Featured Block'),
    'block_featured_text' => t('Copy Text Featured Block'),
  ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Monoblue (default)'),
      'colors' => array(
        'base' => '#266a9c',
        'gradient_block' => '#266a9b',
        'link' => '#1d4c7f',
        'header_bar' => '#266a9e',
        'text' => '#6e8497',
        'link_hover' => '#1d4c7e',
        'menu_active' => '#6d7e8f',
        'content_header' => '#1d4c7f',
        'block_featured_header' => '#e3f1ff',
        'block_featured_text' => '#b6d7f2',
      ),
    ),
    'blackwhite' => array(
      'title' => t('Black and white'),
      'colors' => array(
        'base' => '#444444',
        'gradient_block' => '#444444',
        'link' => '#23bbf5',
        'header_bar' => '#444444',
        'text' => '#777777',
        'link_hover' => '#1d4c7e',
        'menu_active' => '#0091e2',
        'content_header' => '#0091e2',
        'block_featured_header' => '#eeeeee',
        'block_featured_text' => '#dddddd',
      ),
    ),
    'green' => array(
      'title' => t('Green'),
      'colors' => array(
        'base' => '#444444',
        'gradient_block' => '#444445',
        'link' => '#23bbf5',
        'header_bar' => '#444444',
        'text' => '#777777',
        'link_hover' => '#1d4c7e',
        'menu_active' => '#0091e2',
        'content_header' => '#0091e2',
        'block_featured_header' => '#eeeeee',
        'block_featured_text' => '#dddddd',
      ),
    ),
  ),
 
  // Images to copy over.
  'copy' => array(    
    'logo.png',
    'screenshot.png',
  ),
 
  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/style.css',   
  ),

  //// Gradient definitions.
  'gradients' => array(
   array(
      // (x, y, width, height).
    'dimension' => array(0, 0, 0, 0),
     // Direction of gradient ('vertical' or 'horizontal').
     'direction' => 'vertical',
      // Keys of colors to use for the gradient.
     'colors' => array('link', 'text'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(
    'gradient_block' => array(0, 0, 10, 250),
    'header_bar' => array(0, 250, 68, 146),
    'base' => array(0, 1569, 1000, 31),
  ),
 
  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
   'slices' => array(
  'images/navigation-background.png' => array(0, 0, 10, 250),
  'images/bg-color-bar-low.jpg' => array(0, 250, 10, 146),
  'images/footer_bg.jpg' => array(0, 1590, 1000, 10),
  'images/bg-shadow-color-bar-left.jpg'  => array(10, 250, 29, 146),
  'images/bg-shadow-color-bar-right.jpg'  => array(39, 250, 29, 146),
  'images/input-bg-dark.jpg' => array(0, 1569, 500, 21), 
  ),
 
  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',
 
  // Preview files.
  'preview_image' => 'color/preview.png',
  'preview_css' => 'color/preview.css',
 
  // Base file for image generation.
  'base_image' => 'color/base.png',
);
