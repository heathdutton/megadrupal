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
  'title' => t('Two column 65/35 Stacked'),
  'category' => t('Sprout Layouts'),
  'icon' => 'twocol_65_35_stacked.png',
  'theme' => 'twocol_65_35_stacked',
  'css' => 'twocol_65_35_stacked.css',
  'regions' => array(
    'top' => t('Top'),
    'left' => t('Left'),
    'right' => t('Right'),
    'bottom' => t('Bottom'),
  ),
);
