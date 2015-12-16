<?php
// Plugin definition
$plugin = array(
  'title' => t('Folders'),
  'icon' => 'folders.png',
  'category' => t('Folders'),
  'theme' => 'folders',
  'css' => 'folders.css',
  'regions' => array(
    'folderlayout' => t('Folder Layout'),
    'content' => t('Content'),
    'sidebar' => t('Sidebar'),
  ),
);
