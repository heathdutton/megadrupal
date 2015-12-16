<?php
 
$info = array();
 
// Define the possible replaceable items and their labels.
$info['fields'] = array(
  'pageheader'  => t('Header background color'),
  'btn'         => t('Button background color'),
  'table'       => t('Table header background color'),
  'link'        => t('Link color'),
  'text'        => t('Text color'),
);

/// Color schemes for the site.
$info['schemes'] = array(
  // Define the default scheme.
  'default' => array(
    // Scheme title.
    'title' => t('Blue Lagoon (Default)'),
    // Scheme colors (Keys are coming from $info['fields']).
    'colors' => array(
      'pageheader'  => '#2385c2',
      'btn'         => '#0072b9',
      'table'       => '#5ab5ee',
      'link'        => '#2385c2',
      'text'        => '#494949',
    ),
  ),
  // Second scheme.
  'chocolate' => array(
    'title' => t('Belgian Chocolate'),
    'colors' => array(
      'pageheader'  => '#6c420e',
      'btn'         => '#971702',
      'table'       => '#d5b048',
      'link'        => '#6c420e',
      'text'        => '#494949',
    ),
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
);
