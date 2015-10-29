<?php

/**
 * @file
 * Provide verbatim environments (these will be hidden by all further DruTeX parsing).
 *
 * Note that a missing subhook_info() means that this submodule cannot be disabled (without hacking).
 */

/**
 * Implements hook_info().
 */
function drutex_verbatim_info($format = -1) {
  return (object) array(
    'title' => t('Numbering / Referencing'),
    'description' => t('Provide verbatim environments (these will be hidden by all further DruTeX parsing).'),
    'toggle' => FALSE,
    'weight' => 20,
  );
}

/**
 * Implements hook_defaults().
 */
function drutex_verbatim_defaults() {
  $D['drutex_verbatim_active'] = TRUE;
  return $D;
}

/**
 * Implements hook_node2html().
 */
function drutex_verbatim_node2html() {
  $E = array();

  /* <code>*</code> */
  $E[] = (object) array(
    'pattern' => '@<code>(.+?)</code>@se',
    'replacement' => "_drutex_hide('set', '$0')",
    'weight' => -100,
  );

  /* <notex>*</notex> */
  $E[] = (object) array(
    'pattern' => '@<notex>(.+?)</notex>@se',
    'replacement' => "_drutex_hide('set', '$1')",
    'weight' => -100,
  );

  return $E;
}
