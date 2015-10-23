<?php

// Put the logo path into JavaScript for the live preview.
drupal_add_js(array('color' => array('logo' => theme_get_setting('logo', 'landing_page_theme'))), 'setting');

$info = array(
  // Available colors and color labels used in theme.
  'fields' => array(
    'headerbg' => t('Header background'),
    'headertxt' => t('Header text'),
    'headerlinks' => t('Header Links color'),
    'headerhover' => t('Header :hover color'),
    'highlightedbg' => t('Highlighted background'),
    'highlightedtxt' => t('Highlighted text'),
    'highlightedlinks' => t('Highlighted Links color'),
    'highlightedhover' => t('Highlighted :hover color'),
    'helpbg' => t('Help background'),
    'helptxt' => t('Help text'),
    'helplinks' => t('Help Links color'),
    'helphover' => t('Help :hover color'),
    'prefixbg' => t('Prefix background'),
    'prefixtxt' => t('Prefix text'),
    'prefixlinks' => t('Prefix Links color'),
    'prefixhover' => t('Prefix :hover color'),
    'contentbg' => t('Content background'),
    'contenttxt' => t('Content text'),
    'contentlinks' => t('Content Links color'),
    'contenthover' => t('Content :hover color'),
    'suffixbg' => t('Suffix background'),
    'suffixtxt' => t('Suffix text'),
    'suffixlinks' => t('Suffix Links color'),
    'suffixhover' => t('Suffix :hover color'),
    'featuredbg' => t('Featured background'),
    'featuredtxt' => t('Featured text'),
    'featuredlinks' => t('Featured Links color'),
    'featuredhover' => t('Featured :hover color'),
    'footerbg' => t('Footer background'),
    'footertxt' => t('Footer text'),
    'footerlinks' => t('Footer Links color'),
    'footerhover' => t('Footer :hover color'),
   ),
  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Blue (Default)'),
      'colors' => array(
        'headerbg' => '#1919B3',
        'headertxt' => '#eeeeee',
        'headerlinks' => '#eeeeee',
        'headerhover' => '#ffffff',

        'highlightedbg' => '#12127D',
        'highlightedtxt' => '#eeeeee',
        'highlightedlinks' => '#eeeeee',
        'highlightedhover' => '#ffffff',

        'helpbg' => '#C8C8FE',
        'helptxt' => '#1919B3', 
        'helplinks' => '#12127D',
        'helphover' => '#12127D',

        'prefixbg' => '#9191FE',
        'prefixtxt' => '#1919B3', 
        'prefixlinks' => '#12127D',
        'prefixhover' => '#12127D',

        'contentbg' => '#1919B3', 
        'contenttxt' => '#eeeeee',
        'contentlinks' => '#eeeeee',
        'contenthover' => '#ffffff',

        'suffixbg' => '#9191FE',
        'suffixtxt' => '#1919B3',
        'suffixlinks' => '#12127D',
        'suffixhover' => '#12127D',

        'featuredbg' => '#C8C8FE',
        'featuredtxt' => '#1919B3',
        'featuredlinks' => '#12127D',
        'featuredhover' => '#12127D',

        'footerbg' => '#12127D',
        'footertxt' => '#eeeeee',
        'footerlinks' => '#eeeeee',
        'footerhover' => '#ffffff',
      ),
    ),
    'green' => array(
      'title' => t('Green'),
      'colors' => array(
        'headerbg' => '#33FF00',
        'headertxt' => '#eeeeee',
        'headerlinks' => '#eeeeee',
        'headerhover' => '#ffffff',

        'highlightedbg' => '#24B200',
        'highlightedtxt' => '#eeeeee',
        'highlightedlinks' => '#eeeeee',
        'highlightedhover' => '#ffffff',

        'helpbg' => '#CCFEBF',
        'helptxt' => '#33FF00',
        'helplinks' => '#24B200',
        'helphover' => '#24B200',

        'prefixbg' => '#99FE80',
        'prefixtxt' => '#33FF00',
        'prefixlinks' => '#24B200',
        'prefixhover' => '#24B200', 

        'contentbg' => '#33FF00', 
        'contenttxt' => '#eeeeee',
        'contentlinks' => '#eeeeee',
        'contenthover' => '#ffffff', 

        'suffixbg' => '#99FE80',
        'suffixtxt' => '#33FF00',
        'suffixlinks' => '#24B200',
        'suffixhover' => '#24B200', 

        'featuredbg' => '#CCFEBF',
        'featuredtxt' => '#33FF00',
        'featuredlinks' => '#24B200',
        'featuredhover' => '#24B200', 

        'footerbg' => '#24B200',
        'footertxt' => '#eeeeee',
        'footerlinks' => '#eeeeee',
        'footerhover' => '#ffffff', 
      ),
    ),
    'yellow' => array(
      'title' => t('Yellow'),
      'colors' => array(
        'headerbg' => '#FFB200',
        'headertxt' => '#eeeeee',
        'headerlinks' => '#eeeeee',
        'headerhover' => '#ffffff', 

        'highlightedbg' => '#B27D00',
        'highlightedtxt' => '#eeeeee',
        'highlightedlinks' => '#eeeeee',
        'highlightedhover' => '#ffffff',

        'helpbg' => '#FEECBF',
        'helptxt' => '#FFB200',
        'helplinks' => '#B27D00',
        'helphover' => '#B27D00',

        'prefixbg' => '#FED980',
        'prefixtxt' => '#FFB200', 
        'prefixlinks' => '#B27D00',
        'prefixhover' => '#B27D00',

        'contentbg' => '#FFB200', 
        'contenttxt' => '#eeeeee',
        'contentlinks' => '#eeeeee',
        'contenthover' => '#ffffff',

        'suffixbg' => '#FED980',
        'suffixtxt' => '#FFB200',
        'suffixlinks' => '#B27D00',
        'suffixhover' => '#B27D00',

        'featuredbg' => '#FEECBF',
        'featuredtxt' => '#FFB200',
        'featuredlinks' => '#B27D00',
        'featuredhover' => '#B27D00',

        'footerbg' => '#B27D00',
        'footertxt' => '#eeeeee',
        'footerlinks' => '#eeeeee',
        'footerhover' => '#ffffff',
      ),
    ),
    'magento' => array(
      'title' => t('Magento'),
      'colors' => array(
        'headerbg' => '#E60066',
        'headertxt' => '#eeeeee',
        'headerlinks' => '#eeeeee',
        'headerhover' => '#ffffff',

        'highlightedbg' => '#A10048',
        'highlightedtxt' => '#eeeeee',
        'highlightedlinks' => '#eeeeee',
        'highlightedhover' => '#ffffff',

        'helpbg' => '#FEBFDC',
        'helptxt' => '#E60066', 
        'helplinks' => '#A10048',
        'helphover' => '#A10048',

        'prefixbg' => '#FE80B9',
        'prefixtxt' => '#E60066', 
        'prefixlinks' => '#A10048',
        'prefixhover' => '#A10048',

        'contentbg' => '#E60066', 
        'contenttxt' => '#eee',
        'contentlinks' => '#eee',
        'contenthover' => '#fff',

        'suffixbg' => '#FE80B9',
        'suffixtxt' => '#E60066',
        'suffixlinks' => '#A10048',
        'suffixhover' => '#A10048',

        'featuredbg' => '#FEBFDC',
        'featuredtxt' => '#E60066',
        'featuredlinks' => '#A10048',
        'featuredhover' => '#A10048',
        
        'footerbg' => '#A10048',
        'footertxt' => '#eee',
        'footerlinks' => '#eee',
        'footerhover' => '#fff',
      ),
    ),
  ),

  // CSS files (excluding @import) to rewrite with new color scheme.
  'css' => array(
    'css/colors.css',
  ),

  // Files to copy.
  'copy' => array(
    'logo.png',
  ),

  // Gradient definitions.
  'gradients' => array(
    array(
      // (x, y, width, height).
      'dimension' => array(0, 0, 0, 0),
      // Direction of gradient ('vertical' or 'horizontal').
      'direction' => 'vertical',
      // Keys of colors to use for the gradient.
      'colors' => array('top', 'bottom'),
    ),
  ),

  // Color areas to fill (x, y, width, height).
  'fill' => array(),

  // Coordinates of all the theme slices (x, y, width, height)
  // with their filename as used in the stylesheet.
  'slices' => array(),

  // Reference color used for blending. Matches the base.png's colors.
  'blend_target' => '#ffffff',

  // Preview files.
  'preview_css' => 'color/preview.css',
  'preview_js' => 'color/preview.js',
  'preview_html' => 'color/preview.html',

  // Base file for image generation.
  'base_image' => 'color/base.png',
);
