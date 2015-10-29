<?php

/**
 * @file
 * Just an example DruTeX submodule.
 *
 * For developers.
 */

/**
 * Implements hook_info().
 */
function drutex_example_info($format = -1) {
  return (object) array(
    'title' => t('Exampe submodule'),
    'description' => t('Provides some senseless stuff.'),
    'toggle' => TRUE, /* tells drutex if the user can activate/deactivate this submodule */
    'weight' => 20 /* helps ordering the fieldsets on the configuration page */,
  );
}

/**
 * Implements hook_defaults().
 */
function drutex_example_defaults() {
  $D['drutex_blahtex_active'] = FALSE;
  $D['drutex_example_sometext'] = 'This is the default text for drutex_example_sometext_$format.';

  return $D;
}

/**
 * Implements hook_node2html().
 */
function drutex_example_node2html() {
  $E = array();

  /* blabla{hello world} => <xyz>hellp world</xyz> */
  $E[] = (object) array(
    'type' => 'latex-command',
    'pattern' => 'blabla',
    'replacement' => '<xyz>$1</xyz>',
  );

  /* abcde => edcba */
  $E[] = (object) array(
    'pattern' => '@abcde@s',
    'replacement' => 'edcba',
    'weight' => 100,
    /* type is regex! */
  );

  return $E;
}

/**
 * Implements hook_filter_settings().
 */
function drutex_example_filter_settings($format = -1) {
  $form["drutex_example_sometext_$format"] = array(
    '#type' => 'textarea',
    '#title' => t('Some text'),
    '#rows' => 4,
    '#cols' => 50,
    '#default_value' => drutex_var_get("drutex_example_sometext_$format"),
  );

  return $form;
}

/**
 * Implements hook_perm().
 */
function drutex_example_perm() {
  return array('access examples');
}
