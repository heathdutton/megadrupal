<?php
/**
 * @file
 * cTools Plugin definition for one column layout
 */

// Plugin definition.
$plugin = array(
  'title' => t('One Column stacked'),
  'category' => t('Columns: 1'),
  'icon' => 'onecol_stacked.png',
  'theme' => 'onecol_stacked',
  'css' => 'onecol_stacked.css',
  'regions' => array(
    'header' => t('Header'),
    'top' => t('Top'),
    'center' => t('Center'),
    'footer' => t('Footer'),
  ),
);
