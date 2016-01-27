<?php
$plugin = array(
  'title' => t('DV - right col'),
  'category' => t('Da Vinci - Layouts'),
  'icon' => 'dv-right-col.png',
  'theme' => 'dv-right-col',
  'admin theme' => 'admin-dv-right-col',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'content' => t('Content'),
    'right' => t('Right side'),
    'footer' => t('Footer'),
  ),
);
