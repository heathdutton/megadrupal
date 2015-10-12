<?php

//$Id$;
/**
 * @file
 * 
 */

function jagedown_jagedown_editor() {
  $editor['jagedown'] = array(
    'title' => 'JageDown',
    'vendor url' => 'https://github.com/olragon/JageDown',
    'download url' => 'https://github.com/olragon/JageDown/downloads',
    'library path' => wysiwyg_get_path('JageDown'),
    'libraries' => array(
      'source' => array(
        'title' => 'Source',
        'files' => array('Markdown.Converter.js', 'Markdown.Sanitizer.js', 'Markdown.Editor.js'),
      ),
    ),
    'version callback' => 'jagedown_jagedown_version',
    'themes callback' => 'jagedown_jagedown_themes',
    'settings callback' => 'jagedown_jagedown_settings',
    'plugin callback' => 'jagedown_jagedown_plugins',
    'versions' => array(
      'lastest' => array(
        'js files' => array('jagedown-latest.js'),
        'css files' => array('jagedown-latest.css'),
      )
    )
  );
  return $editor;
}

function jagedown_jagedown_version($version) {
  return 'latest';
}

function jagedown_jagedown_themes($editor, $profile) {
  return array('default', 'bootstrap');
}

function jagedown_jagedown_settings($editor, $config, $theme) {
  return array(
    'delay' => false,
  );
}

function jagedown_jagedown_plugins($editor) {
  return array();
}