<?php

/**
 * @file
 * Basic Color module settings
 */

$info = array();

// Define the possible replaceable items and their labels.
$info['fields'] = array(
  'main'     => t('Main color'),
  'lighter'  => t('Main color lighter'),
  'lightest' => t('Main color lightest'),
  'layer'    => t('Backgroundlayer'),
);

// Color schemes for the site.
$info['schemes']  = array(
  // Define the default scheme.
  'default'       => array(
    // Scheme title.
    'title'       => t('Blue (rijksoverheid.nl)'),
    // Scheme colors (Keys are coming from $info['fields'] and default required fields link, text and baee).
    'colors'      => array(
      'link'      => '#154273',
      'text'      => '#000',
      'base'      => '#ffffff',
      'main'      => '#01689b',
      'lighter'   => '#cce0f1',
      'lightest'  => '#e5f0f9',
      'layer'     => '#f3f3f3',
    ),
  ),
  'dark-brown'    => array(
    'title'       => t('Dark brown'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#623327',
      'lighter'   => '#d1c1be',
      'lightest'  => '#e8e1df',
      'layer'     => '#f3f3f3',
    ),
  ),
  'purple'    => array(
    'title'       => t('Purple'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#42145f',
      'lighter'   => '#c6b8cf',
      'lightest'  => '#e3dce7',
      'layer'     => '#f3f3f3',
    ),
  ),
  'dark-green'    => array(
    'title'       => t('Dark green'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#275937',
      'lighter'   => '#becdc3',
      'lightest'  => '#dfe6e1',
      'layer'     => '#f3f3f3',
    ),
  ),
  'ruby'    => array(
    'title'       => t('Ruby'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#ca005a',
      'lighter'   => '#efb2ce',
      'lightest'  => '#f7d9e7',
      'layer'     => '#f3f3f3',
    ),
  ),
  'red'    => array(
    'title'       => t('Red'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#d52b1e',
      'lighter'   => '#f2bfbb',
      'lightest'  => '#f9dfdd',
      'layer'     => '#f3f3f3',
    ),
  ),
  'violet'    => array(
    'title'       => t('Violet'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#a90061',
      'lighter'   => '#e5b2cf',
      'lightest'  => '#f2d9e7',
      'layer'     => '#f3f3f3',
    ),
  ),
  'moss-green'    => array(
    'title'       => t('Moss green'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#777c00',
      'lighter'   => '#d6d7b2',
      'lightest'  => '#ebebd9',
      'layer'     => '#f3f3f3',
    ),
  ),
  'brown'    => array(
    'title'       => t('Brown'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#94710a',
      'lighter'   => '#dfd4b5',
      'lightest'  => '#efeada',
      'layer'     => '#f3f3f3',
    ),
  ),
  'green'    => array(
    'title'       => t('Green'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#39870c',
      'lighter'   => '#c3dbb6',
      'lightest'  => '#e1eddb',
      'layer'     => '#f3f3f3',
    ),
  ),
  'azure'    => array(
    'title'       => t('Azure'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#007bc7',
      'lighter'   => '#b2d7ee',
      'lightest'  => '#d9ebf7',
      'layer'     => '#f3f3f3',
    ),
  ),
  'light-blue'    => array(
    'title'       => t('Light blue'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#8fcae7',
      'lighter'   => '#ddeff8',
      'lightest'  => '#eef7fb',
      'layer'     => '#f3f3f3',
    ),
  ),
  'pink'    => array(
    'title'       => t('Pink'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#f092cd',
      'lighter'   => '#fadef0',
      'lightest'  => '#fdeff8',
      'layer'     => '#f3f3f3',
    ),
  ),
  'yellow'    => array(
    'title'       => t('Yellow'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#f9e11e',
      'lighter'   => '#fdf6bb',
      'lightest'  => '#fefbdd',
      'layer'     => '#f3f3f3',
    ),
  ),
  'dark-yellow'    => array(
    'title'       => t('Dark yellow'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#ffb612',
      'lighter'   => '#ffe9b7',
      'lightest'  => '#fff4dc',
      'layer'     => '#f3f3f3',
    ),
  ),
  'mint-green'    => array(
    'title'       => t('Mint green'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#76d2b6',
      'lighter'   => '#d6f1e9',
      'lightest'  => '#ebf8f4',
      'layer'     => '#f3f3f3',
    ),
  ),
  'orange'    => array(
    'title'       => t('Orange'),
    'colors'      => array(
      'base'      => '#008CFF',
      'link'      => '#ffffff',
      'text'      => '#ffffff',
      'main'      => '#e17000',
      'lighter'   => '#f6d4b2',
      'lightest'  => '#fbead9',
      'layer'     => '#f3f3f3',
    ),
  ),
);

// Define the CSS file(s) that we want the Color module to use as a base.
$info['css'] = array(
  'css/rijkshuisstijl-wysiwyg.css',
  'css/rijkshuisstijl.css',
  'css/print.css',
);

/***** Advanced Color settings - Defaults.. *****/

/**
 * Default settings for the advanced stuff.
 * No need to edit these if you just want to play around with the colors.
 * Color wants these, otherwise it's not going to play.
 *
 */

// Files we want to copy along with the CSS files, let's define these later.
$info['copy'] = array();

// Files used in the scheme preview.
$info['preview_css'] = 'color/preview.css';
//$info['preview_js'] = 'color/preview.js';
$info['preview_html'] = 'color/preview.html';

// Gradients
$info['gradients'] = array();
$info['blend_target'] = '#fff';

// Color areas to fill (x, y, width, height).
$info['fill'] = array();

// Coordinates of all the theme slices (x, y, width, height)
// with their filename as used in the stylesheet.
$info['slices'] = array();

// Base file for image generation.
$info['base_image'] = 'color/base.png';
