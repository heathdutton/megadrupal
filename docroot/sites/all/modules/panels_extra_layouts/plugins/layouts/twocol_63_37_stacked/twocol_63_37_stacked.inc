<?php
/**
 * @file   twocol-63-37-stacked.tpl.php
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Tue Dec 18 9:15:00 2012
 *
 * @brief  Template for the 63/37 panels layout.
 *
 *
 */

// Plugin definition.
$plugin = array(
  'title' => t('Two column 63/37 stacked'),
  'category' => t('Columns: 2'),
  'icon' => 'twocol_63_37_stacked.png',
  'theme' => 'twocol_63_37_stacked',
  'css' => 'twocol_63_37_stacked.css',
  'regions' => array(
    'header' => t('header'),
    'left' => t('left'),
    'right' => t('right'),
    'footer' => t('footer'),
  ),
);
