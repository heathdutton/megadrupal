<?php
$plugin = array(
  'title' => t('DV - left col'),
  'category' => t('Da Vinci - Layouts'),
  'icon' => 'dv-left-col.png',
  'theme' => 'dv-left-col',
  'admin theme' => 'admin-dv-left-col',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'content' => t('Content'),
    'left' => t('Left side'),
    'footer' => t('Footer'),
  ),
);
