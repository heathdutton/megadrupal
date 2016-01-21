<?php
/**
 * @file
 * Pjirc module functions
 */


function _pjirc_get_colorsdef() {
  $colorsdef = array(
    'color0' => array(
      'title' => 'Color 0',
      'description' => 'Button Highlight / Popup & Close Button Text & Higlight / Scrollbar Highlight (Black)',
      'defvalue' => '#000',
    ),
    'color1' => array(
      'title' => 'Color 1',
      'description' => 'Button Border & Text : ScrollBar Border & arrow : Popup & Close button Border : User List border & Text & icons (Default Black)',
      'defvalue' => '#000',
    ),
    'color2' => array(
      'title' => 'Color 2',
      'description' => 'Popup & Close button shadow (Default Dark Grey)',
      'defvalue' => '#a9a9a9',
    ),
    'color3' => array(
      'title' => 'Color 3',
      'description' => 'Scrollbar shadow (Default Dark Grey)',
      'defvalue' => '#a9a9a9',
    ),
    'color4' => array(
      'title' => 'Color 4',
      'description' => 'Scrollbar de-light (3D Dim colour (Default Light Grey))',
      'defvalue' => '#a8a8a8',
    ),
    'color5' => array(
      'title' => 'Color 5',
      'description' => 'foreground : Buttons Face : Scrollbar Face (Default Light Blue)',
      'defvalue' => '#c0d9d9',
    ),
    'color6' => array(
      'title' => 'Color 6',
      'description' => 'background : Header : Scrollbar Track : Footer background (Default Blue)',
      'defvalue' => '#00f',
    ),
    'color7' => array(
      'title' => 'Color 7',
      'description' => 'selection : Status & Window button active colour (Default Red)',
      'defvalue' => '#f00',
    ),
    'color8' => array(
      'title' => 'Color 8',
      'description' => 'event Color (Default Red)',
      'defvalue' => '#f00',
    ),
    'color9' => array(
      'title' => 'Color 9',
      'description' => 'close button',
      'defvalue' => '#',
    ),
    /*'color10' => array ('title' => 'Color 10', 'description' => 'voice icon', 'defvalue' => '', ),
 'color11' => array ('title' => 'Color 11', 'description' => 'operator icon', 'defvalue' => '', ),
 'color12' => array ('title' => 'Color 12', 'description' => 'halfoperator icon', 'defvalue' => '', ),
 'color13' => array ('title' => 'Color 13', 'description' => 'male ASL', 'defvalue' => '', ),
 'color14' => array ('title' => 'Color 14', 'description' => 'female ASL', 'defvalue' => '', ),
 'color15' => array ('title' => 'Color 15', 'description' => 'unknown ASL', 'defvalue' => '', ),*/
  );

  return $colorsdef;
}
