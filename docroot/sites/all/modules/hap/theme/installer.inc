<?php

function theme_hap_installer_tools($variables) {
  $form = $variables['element'];
  // Individual table headers.
  $rows = array();
  // Iterate through all the modules, which are
  // children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $tool = $form[$key];
    $row = array();

    // Checkbox
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($tool));

    // Name
    $row[] = '<label for="' . $tool['#id'] . '"><strong>' . $tool['#installer_name'] . '</strong></label>';

    // Version
    $row[] = $tool['#installer_version'];

    // Description
    $row[] = array('data' => $tool['#installer_description'], 'class' => array('description'));

    // Help & Configure
    foreach (array('help', 'configure') as $key) {
      $path = $tool['#installer_' . $key];
      $row[] = array('data' => $path ? l($key, $path) : '', 'class' => array($key));
    }

    $rows[] = array(
      'data' => $row,
      'class' => $tool['#value'] ? array('enabled') : array(),
    );
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows, 'sticky' => FALSE, 'empty' => t('No Tools found.')));
}
