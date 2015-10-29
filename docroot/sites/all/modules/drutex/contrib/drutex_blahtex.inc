<?php

/**
 * @file
 * Provide environments to create MathML with the blahtex binary.
 *
 * Blahtex is a tool to convert LaTeXmaths to MathML, but - of course -
 * not every LaTeXmath command/environment is supported.
 */

/**
 * Implements hook_info().
 */
function drutex_blahtex_info($format = -1) {
  return (object) array(
    'title' => t('blahtex (MathML)'),
    'description' => t('Provides math environments to create MathML with blahtex.'),
    'toggle' => TRUE,
    'weight' => 16,
  );
}

/**
 * Implements hook_defaults().
 */
function drutex_blahtex_defaults() {
  $D['drutex_blahtex_active'] = FALSE;

  return $D;
}

/**
 * Implements hook_node2html().
 */
function drutex_blahtex_node2html() {
  $E = array();

  /* Inline math between $ and $ */
  $E[] = (object) array(
    'pattern' => '@(?<!\\\\)\$(.*?[^\\\\])\$@se',
    'replacement' => "drutex_blahtex('$1', '<var>format</var>')",
    'weight' => -20,
  );

  return $E;
}

/**
 * Passes $text to blahtex.
 */
function drutex_blahtex($text, $format = -1) {
  $text = _drutex_unescape($text);

  $text = shell_exec("echo \"$text\" | blahtex --mathml");

  $text = preg_replace('/\n/s', ' ', $text);

  return $text;
}

