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
  'title' => t('Two column 50/50'),
  'category' => t('Sprout Layouts'),
  'icon' => 'twocol_50_50.png',
  'theme' => 'twocol_50_50',
  'css' => 'twocol_50_50.css',
  'regions' => array(
    'left' => t('Left'),
    'right' => t('Right'),
  ),
);
