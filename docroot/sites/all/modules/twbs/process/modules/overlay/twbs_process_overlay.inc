<?php

/**
 * Implements twbs_process_HOOK() for overlay.tpl.php
 *
 * Places the rendered HTML for the page body into a top level variable.
 *
 * @see twbs_preprocess_overlay()
 * @see overlay.tpl.php
 */
function twbs_process_overlay(&$variables) {
  $variables['page'] = $variables['page']['#children'];
}
