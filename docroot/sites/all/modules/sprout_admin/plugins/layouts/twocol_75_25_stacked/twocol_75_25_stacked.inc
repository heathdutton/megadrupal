<?php
/**
 * @file   twocol_70_30_stacked.inc
 * @author António P. P. Almeida <appa@perusio.net>
 * @date   Sun Jul 24 20:43:02 2011
 *
 * @brief  The two column 70/30 stacked layout.
 *
 *
 */

// Plugin definition.
$plugin = array(
  'title' => t('Two column 75/25 stacked'),
  'category' => t('Sprout Layouts'),
  'icon' => 'twocol_75_25_stacked.png',
  'theme' => 'twocol_75_25_stacked',
  'css' => 'twocol_75_25_stacked.css',
  'regions' => array(
    'header' => t('Header'),
    'top' => t('Top'),
    'left' => t('Left'),
    'right' => t('Right'),
    'bottom' => t('Bottom'),
  ),
);
