<?php

function crush_command_ls($parsed_command) {
  $dirs = $parsed_command['args'];
  $current_path = $parsed_command['current_path'];
  $messages = array();
  if (count($dirs) == 0 || is_string($dirs)) {
    $dirs = array('.');
  }
  if (count($dirs) == 1) {
    $messages = array_merge($messages, _crush_command_ls($dirs[0], $current_path));
  }
  else {
    foreach ($dirs as $dir) {
      $messages[] = $dir . ':';
      $messages = array_merge($messages, _crush_command_ls($dir, $current_path));
    }
  }
  return $messages;
}

function _crush_command_ls($dir, $current_path) {
  $messages = array();
  $parts = array_filter(explode('/', $dir));
  $current_parts = array_filter(explode('/', $current_path));
  if ($dir[0] == '/') {
    $current_parts = array();
  }
  foreach ($parts as $part) {
    if ($part == '.') {
      // Do nothing.
    }
    elseif ($part == '..') {
      array_pop($current_parts);
    }
    else {
      $current_parts[] = $part;
    }
  }
  $path = implode('/', $current_parts);
  if (($item = menu_get_item($path)) || !strlen($path)) {
    if (!strlen($path)) {
      $resolved_path = '';
      $resolved_count = 1;
    }
    else {
      $resolved_path = $item['path'] . '/';
      $resolved_count = count($current_parts) + 1;
    }
    $select = db_select('menu_router', 'mr');
    $select->fields('mr', array('path'));
    $select->condition('mr.path', db_like($resolved_path) . '%', 'LIKE');
    $select->condition('mr.number_parts', $resolved_count);
    $items = $select->execute()->fetchAll();
    $paths = array();
    if (count($items)) {
      foreach ($items as $item) {
        $paths[] = str_replace($resolved_path, '', $item->path);
      }
      $messages[] = implode(' ', $paths);
    }
    else {
      $messages[] = t('No items found.');
    }
  }
  else {
    $messages[] = t('Cannot access !dir: no such page.', array('!dir' => $path));
  }
  return $messages;
}

