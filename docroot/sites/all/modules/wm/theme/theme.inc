<?php

/**
 * @file
 * Template preprocess functions
 */

/**
 * Preprocess function for _wm_toolbar
 *
 * @param $vars
 */
function template_preprocess_wm_toolbar(&$vars) {
  global $user;

  $vars['profile'] = l($user->name, 'user/' . $user->uid);

  if ($user->picture) {
    $vars['picture'] = theme('user_picture', array('account' => $user));
  }
  else {
    $vars['picture'] = theme('image', array('path' => drupal_get_path('module', 'wm') . '/images/foto.png'));
  }

  $vars['logout_url'] = l(t('Logout'), 'user/logout');
  $vars['image_path'] = drupal_get_path('module', 'wm') . '/images/';

  $blocks = $vars['blocks'];
  $vars['status'] = theme('wm_toolbar_block', array('block' => $blocks['status'], 'title' => FALSE));

  unset($blocks['status']);
  uasort($blocks, 'cmp');

  $scollable = array();
  $shortcuts = '';
  foreach ($blocks as $block) {
    $scollable[] = theme('wm_toolbar_block', array('block' => $block));
    foreach ($block['#items'] as $item) {
      if (isset($item['shortcut']) and $item['shortcut'] == TRUE) {
        $shortcuts .= theme('wm_toolbar_shortcut', array('item' => $item));
      }
    }
  }
  $vars['scollables'] = $scollable;
  $vars['shortcuts'] = $shortcuts;
}

/**
 * Sort blocks by weigth
 *
 * @param $a
 * @param $b
 * @return int
 */
function cmp($a, $b) {
  if ($a['#weigth'] == $b['#weigth']) {
    return 0;
  }
  return ($a['#weigth'] < $b['#weigth']) ? -1 : 1;
}

/**
 * Preprocess function for _wm_toolbar_block
 *
 * @param $vars
 */
function template_preprocess_wm_toolbar_block(&$vars) {
  if ($vars['title'] != FALSE) {
    $vars['title'] = (isset($vars['block']['#title'])) ? $vars['block']['#title'] : FALSE;
  }

  $vars['icon'] = $vars['block']['#icon'];
  $vars['ul_attr'] = array('class' => $vars['block']['#class']);

  $output = '';
  foreach ($vars['block']['#items'] as $key => $item) {
    if (wm_access($key)) {
      $output .= theme('wm_toolbar_item', array('item' => $item));
    }
  }

  if ($output) {
    $vars['items'] = $output;
    $vars['render'] = TRUE;
  }
  else {
    $vars['render'] = FALSE;
  }
}
