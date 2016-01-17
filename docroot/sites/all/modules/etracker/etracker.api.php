<?php

/**
 * @file
 * Hooks provided by the etracker module.
 */

function hook_etracker_default_variables_alter(&$variables) {
  $variables['et_pagename'] = 'My own page title';
}

function hook_etracker_variables_alter(&$variables) {
  if ('some condition') {
    $variables['et_target'] = array();
  }
}
