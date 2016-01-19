<?php

/**
 * @file
 * Helpfile for the settings tab of the dynOZ module
 */

/**
 * Creates the help text for the settings tab
 *
 * @return
 *   HTML with the help text
 */
function dynoz_help_text() {
  $base = rtrim(base_path(), '/');

  $output = <<<EOF
<h2>How to use dynOZ in drupal</h2>
<ul>
<li>create a free account at <a href="http://www.dynoz.net">dynoz.net</a></li>
<li>set and activate hours of your facility at <a href="http://www.dynoz.net">dynoz.net</a></li>
<li>copy webkey of your facility at <a href="http://www.dynoz.net">dynoz.net</a> in clipboard</li>
<li>set number of blocks in drupal to the number of different views on your hours managed at <a href="http://www.dynoz.net">dynoz.net</a></li>
<li>configure each block in drupal by pasting the webkey and choosing the view</li>
</ul>

EOF;

  return $output;
}
