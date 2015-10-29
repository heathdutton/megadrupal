<?php

/**
 * @file
 * Provide an Eukleides environment.
 *
 * http://www.eukleides.org/
 */

/**
 * Implements hook_info().
 */
function drutex_eukleides_info($format = -1) {
  return (object) array(
    'title' => t('Eukleides'),
    'description' => t('Provides an Eukleides environment.'),
    'toggle' => TRUE,
    'weight' => 16,
  );
}

/**
 * Implements hook_defaults().
 */
function drutex_eukleides_defaults() {
  $D['drutex_eukleides_active'] = FALSE;
  return $D;
}

/**
 * Implements hook_node2html().
 */
function drutex_eukleides_node2html() {
  $E = array();

  $E[] = (object) array(
    'pattern' => '@<eukleides([^>]*)>(.+?)</eukleides>@se',
    'replacement' => "drutex_eukleides('$2', '<var>format</var>', '$1')",
    'weight' => -10,
    'info' => t('Eukleides environment'),
    'id' => 'eukleides',
  );

  return $E;
}
