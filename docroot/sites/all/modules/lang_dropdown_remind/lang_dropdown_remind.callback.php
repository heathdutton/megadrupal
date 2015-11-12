<?php

/**
 * @file
 * The callback for the Language Dropdown reminder AJAX preference response.
 *
 * We do this rather than a Drupal menu callback to use as lightweight a Drupal
 * bootstrap as possible.
 */


// Load Drupal's bootstrap.inc.
require_once $_SERVER['DOCUMENT_ROOT'] . '/includes/bootstrap.inc';
require_once getcwd() . '/lang_dropdown_remind.module';

header('Content-Type: application/json; charset=utf-8');
header('Vary: Accept-Language');
print json_encode(lang_dropdown_remind_get_preferences());
exit;
