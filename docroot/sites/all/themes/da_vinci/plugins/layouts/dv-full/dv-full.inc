<?php
$plugin = array(
  'title' => t('DV - Full'),
  'category' => t('Da Vinci - Layouts'),
  'icon' => 'dv-full.png',
  'theme' => 'dv-full',
  'admin theme' => 'admin-dv-full',
  'theme arguments' => array('id', 'content'),
  'regions' => array(
    'content' => t('Content'),
    'footer' => t('Footer'),
  ),
);
