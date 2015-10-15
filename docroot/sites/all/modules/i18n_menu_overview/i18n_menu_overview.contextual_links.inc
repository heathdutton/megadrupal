<?php
/**
 * @file
 * Continas contextual links stuff.
 */

/**
 * Callback contextual links.
 */
function _i18n_menu_overview_contextual_handler($block_module) {
  dpm($block_module);

  if ($block_module == 'superfish') {
    dpm('fu');
  }
}
