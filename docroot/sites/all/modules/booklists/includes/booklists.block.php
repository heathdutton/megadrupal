<?php
chdir('../../../../../../');
define('DRUPAL_ROOT', getcwd());
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
// The URL will look something like:
// /booklists/includes/booklists.block.php?b2e=nyt-block-1
// Replace the hyphens with underscores and put "booklists_" on the beginning so that it becomes usable in the module_invoke() call below.
$block_to_enable = 'booklists_' . str_replace('-', '_', $_GET['b2e']);
$block = block_load('booklists', $block_to_enable);
print drupal_render(_block_get_renderable_array(_block_render_blocks(array($block))));