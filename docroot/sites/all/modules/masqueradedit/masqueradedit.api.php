<?php

/**
 * @file
 * Hooks provided by the Masqueradedit Module.
 */

/**
 * Hook the access callback for the Masqueradedit module.
 *
 * Return an access Boolean that will be returned to define user access.
 */
function hook_masqueradedit_access_alter(&$node, &$access) {
  if ($access == TRUE && $node->type == 'basic_page') {
    $access = FALSE;
  }
}


/**
 * Hook the redirect URL Masqueradedit module switch function.
 *
 * Return an internal URL link that user will visit after starting Masquerade.
 */
function hook_masqueradedit_destination_alter(&$node, &$destination) {
  $destination = 'node/1/edit';
}

/**
 * Hook the redirect URL Masqueradedit module unswitch function.
 *
 * Return an internal URL link that user will visit after stopping Masquerade.
 */
function hook_masqueradedit_back_destination_alter(&$form, &$destination) {
  $destination = 'node/1';
}
