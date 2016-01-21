<?php

/**
 * @file
 * Theme the adminpages for the TabT-module.
 */

/**
 * Theme the teamlist on the adminpage.
 */
function theme_tabt_admin_teams_teamlist($variables) {
  $form = $variables['form'];
  $header = array(t('Team ID'), t('Teamname'), t('Division'), t('URL Alias'), t('Title'));
  $rows = array();
  foreach (element_children($form['teams']) as $key) {
    $row = array();
    $row[] = drupal_render($form['teams'][$key]['teamid']);
    $row[] = drupal_render($form['teams'][$key]['teamletter']);
    $row[] = drupal_render($form['teams'][$key]['division']);
    $row[] = drupal_render($form['teams'][$key]['alias']);
    $row[] = drupal_render($form['teams'][$key]['title']);
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}