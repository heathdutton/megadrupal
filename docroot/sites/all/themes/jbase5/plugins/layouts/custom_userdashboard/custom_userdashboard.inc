<?php

// Plugin definition
$plugin = array(
  'title' => t('Custom User Dashboard'),
  'icon' => 'custom_userdashboard.png',
  'theme' => 'custom_userdashboard',
  'css' => 'custom_userdashboard.css',
  'regions' => array(
    'content_top' => t('Content top'),
    'header_top_col_first' => t('Header top first column'),
    'header_top_col_middle' => t('Header top middle column'),
    'header_top_col_last' => t('Header top last column'),
    'header_row' => t('Header row'),
    'header_bottom_col_first' => t('Header bottom first column'),
    'header_bottom_col_last' => t('Header bottom last column'),
    'content_bottom' => t('Content bottom')
  ),
);
