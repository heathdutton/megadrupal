<?php

function etracker_action_pagename_set($title) {
  etracker_add_variable('et_pagename', $title);
}

function etracker_action_area_set($area) {
  if (is_scalar($area)) {
    $value = array($area);
  }
  else {
    if (is_object($area)) {
      $area = array($area);
    }
    $areas = array();
    foreach ($area as $item) {
      if (!empty($item->name)) {
        $areas[] = $item->name;
      }
    }
    $value = array_reverse($areas);
  }

  if (empty($value)) {
    return;
  }

  etracker_add_variable('et_areas', $value);
}

function etracker_action_target_history_reset() {
  _drupal_session_delete_cookie('et_target_history');
}

function etracker_action_target_set($target) {
  etracker_action_target_history_reset();
  etracker_action_target_append($target);
}

function etracker_action_target_append($target) {
  etracker_add_variable('et_target', $target);
}

function etracker_action_interest_level_set($level) {
  etracker_add_variable('et_ilevel', $level);
}
