<?php
/**
 * @file
 * Example API documentation.
 */

/**
 * Allows you to modify the files that blocksit looks for during load.
 *
 * @param array $files
 *   An array of files, prepopulated with the libraries path.
 */
function hook_views_blocksit_file_candidates(&$files) {
  $files[] = 'my/custom/path/blocksit-custom.js';
}
