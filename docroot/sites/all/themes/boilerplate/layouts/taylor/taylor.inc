<?php
// Plugin definition
$plugin = array(
  'title' => t('Taylor'),
  'icon' => 'taylor.png',
  'category' => t('Responsive'),
  'theme' => 'taylor',
  'css' => 'taylor.css',
  'regions' => array(
    'header' => t('Header'),
    'half' => t('Half Column'),
    'quarter1' => t('Quarter 1 Column'),
    'quarter2' => t('Quarter 2 Column'),
    'footer' => t('Footer'),
  ),
);
