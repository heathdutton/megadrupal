<?php

/**
 * @file
 * Color modules main settings file.
 */

$info = array(

  // Available colors and color labels used in theme.
  'fields' => array(
    'base' => t("Base"),
    'background' => t("Background"),
    'text' => t('Text'),
    'link' => t('Link'),
    'linkhover' => t('Hovered Link'),
    'darkestgray' => t('Darkest Gray'),
    '2nddarkestgray' => t('2nd Darkest Gray'),
    '3rddarkestgray' => t('3rd Darkest Gray'),
    'mediumgray' => t('Medium Gray'),
    '2ndmediumgray' => t('2nd Medium Gray'),
    '3rdmediumgray' => t('3rd Medium Gray'),
    'lightestgray' => t('Lightest Gray'),
    '2ndlightestgray' => t('2nd Lightest Gray'),
    '3rdlightestgray' => t('3rd Lightest Gray'),
  ),

  // Pre-defined color schemes.
  'schemes' => array(
    'default' => array(
      'title' => t('Gray (Default)'),
      'colors' => array(
        'base' => '#343434',
        'background' => '#f3f3f3',
        'text' => '#000000',
        'link' => '#af0000',
        'linkhover' => '#336699',
        'darkestgray' => '#343434',
        '2nddarkestgray' => '#404040',
        '3rddarkestgray' => '#494949',
        'mediumgray' => '#5b5b5b',
        '2ndmediumgray' => '#616161',
        '3rdmediumgray' => '#a9a9a9',
        'lightestgray' => '#b0b0b0',
        '2ndlightestgray' => '#d8d8d8',
        '3rdlightestgray' => '#f3f3f3',
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
