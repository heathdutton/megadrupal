<?php

function theme_hub_hint($vars) {
  $tree = implode("']['", $vars['parents']);

  $output  = '<div class="hub-hint">';
  $output .= '$hub[\'' . $tree . '\']';
  $output .= '</div>';
  
  return $output;
}