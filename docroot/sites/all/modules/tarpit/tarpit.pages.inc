<?php

/**
 * Page callback. See @tarpit_menu().
 */
function _tarpit_get_page() {
  module_invoke_all('tarpit_reaction', array('args' => func_get_args()));
  return _tarpit_generate_render_array();
}


