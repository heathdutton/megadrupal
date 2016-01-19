<?php

/**
 * Page callback. See @tarpit_menu().
 */
function _tarpit_get_page() {
  module_invoke_all('tarpit_view');

  $depth = \Drupal\tarpit\Config::get('tarpit.depth', 2);
  $path_exploded = explode('/', trim(current_path(), '/'));

  if (count($path_exploded) - 1 > $depth) {
    module_invoke_all('tarpit_reaction');
  }

  return _tarpit_generate_render_array();
}


