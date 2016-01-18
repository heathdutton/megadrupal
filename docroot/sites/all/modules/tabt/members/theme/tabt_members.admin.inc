<?php
/**
 * @file
 * Theme the adminpage.
 */

/**
 * Theme the memberlist for registered users on the adminpage.
 */
function theme_tabt_members_admin_members_memberlist($variables) {
  $form = $variables['form'];
  $header = array(t('User'), t('Player ID'), t('TabT Name'), t('Displayed Name'), t('Profile Picture'), t('Operations'));
  $rows = array();
  foreach (element_children($form['tabt_members_members']) as $key) {
    $row = array();
    $row[] = drupal_render($form['tabt_members_members'][$key]['user']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['playerid']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['tabtname']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['name']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['profilepic']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['operations']);
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Theme the memberlist for unregistered players on the adminpage.
 */
function theme_tabt_members_admin_members_memberlist_unregistered($variables) {
  $form = $variables['form'];
  $header = array(t('Player ID'), t('TabT Name'), t('Displayed Name'), t('Profile Picture'));
  $rows = array();
  foreach (element_children($form['tabt_members_members']) as $key) {
    $row = array();
    $row[] = drupal_render($form['tabt_members_members'][$key]['playerid']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['tabtname']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['name']);
    $row[] = drupal_render($form['tabt_members_members'][$key]['profilepic']);
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}
