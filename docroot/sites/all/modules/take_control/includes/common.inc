<?php

function take_control_admin_menu_build_first_item(&$items, $item, $relpath) {
  static $is_first = true;

  if ($is_first) {
    $items['admin/config/content/take_control'] = array(
        'title' => 'Take Control',
        'description' => 'Administer Take Control, and add-on modules.',
        'page callback' => $item['page callback'],
        'page arguments' => $item['page arguments'],
        'access arguments' => array(
            'administer take control'),
        'file' => $item['file']);

    $items['admin/config/content/take_control/' . $relpath] = array(
        'title' => $item['title'],
        'description' => $item['description'],
        'weight' => -1,
        'access arguments' => $item['access arguments'],
        'file' => $item['file'],
        'type' => MENU_DEFAULT_LOCAL_TASK);

    $is_first = false;
  }
  else {
    $item['type'] = MENU_LOCAL_TASK;
    $items['admin/config/content/take_control/' . $relpath] = $item;
  }
}

function take_control_menu_build_first_item(&$items, $item, $relpath) {
  static $is_first = true;

  if ($is_first) {
    $items['admin/structure/take_control'] = array(
        'title' => 'Take Control',
        'description' => 'Drupal File system Management through Take Control add-on modules.',
        'page callback' => $item['page callback'],
        'page arguments' => $item['page arguments'],
        'access arguments' => array(
            'access take control'));

    $items['admin/structure/take_control/' . $relpath] = array(
        'title' => $item['title'],
        'description' => $item['description'],
        'weight' => -1,
        'access arguments' => $item['access arguments'],
        'type' => MENU_DEFAULT_LOCAL_TASK);

    $is_first = false;
  }
  else {
    $item['type'] = MENU_LOCAL_TASK;
    $items['admin/structure/take_control/' . $relpath] = $item;
  }
}

function take_control_random_string($lenth = 10) {
  // makes a random alpha numeric string of a given lenth
  $chars = array_merge(range('A', 'Z'), range('a', 'z'), range(0, 9), array(
      '@',
      '%',
      '^',
      '*'));
  $out = '';
  for ($c = 0; $c < $lenth; $c++) {
    $out .= $chars[mt_rand(0, count($chars) - 1)];
  }
  return $out;
}

