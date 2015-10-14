<?php
/**
 * @file
 * Sasson is a tool-kit for Drupal front-end development.
 *
 * DON'T MODIFY SASSON UNLESS IT'S AN IMPROVMENT - IN THIS CASE, LET US KNOW.
 */

$path_to_sasson = drupal_get_path('theme', 'sasson');
require_once $path_to_sasson . '/includes/assets.inc';     // CSS & JS alters
require_once $path_to_sasson . '/includes/preprocess.inc'; // Preprocess hooks
require_once $path_to_sasson . '/includes/theme.inc';      // Theme functions
require_once $path_to_sasson . '/includes/mail.inc';       // Mail dev-tools
