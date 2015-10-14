<?php

$module = 'biblereadings';
$module_path = drupal_get_path('module', $module);
$plugin_path = $module_path . '/plugins/mobile_content_panes/biblereadings';
$class_path = $module_path . '/includes/classes/mobi.webmobilize';

$plugin = array(
    'key' => 'biblereadings',
    'name' => t('Daily Bible Readings'),
    'allow_multiple' => false,
    'module' => $module,
    'handler' => array(
        'class' => 'BibleReadingsPage',
        'path' => $class_path,
        'file' => 'BibleReadingsPage.class.php',
    ),
    //"js" => "$plugin_path/biblereadings.js",
    "css" => "$plugin_path/biblereadings.css",
    //"admin js" => "$plugin_path/biblereadings_admin.js",
    //"admin css" => "$plugin_path/biblereadings_admin.css",
);
