<?php
// Plugin definition
$plugin = array(
  'title' => t('Selby'),
  'icon' => 'selby.png',
  'category' => t('Responsive'),
  'theme' => 'selby',
  'css' => 'selby.css',
  'regions' => array(
    'sidebar' => t('Sidebar'),
    'contentheader' => t('Content Header'),
    'contentcolumn1' => t('Content Column 1'),
    'contentcolumn2' => t('Content Column 2'),
    'contentfooter' => t('Content Footer'),
  ),
);
