<?php

/**
 * @file
 * Describe hooks provided by the CloudSponge module.
 */

/**
 * All the contacts of the user's address book
 *
 * @see cloudsponge_incoming_callback()
 */
function hook_cloudsponge_all_contacts($emails, $owner, $source) {
  watchdog('cloudsponge', print_r($emails, TRUE));
  watchdog('cloudsponge', print_r($owner, TRUE));
  watchdog('cloudsponge', print_r($source, TRUE));
}

/**
 * Only the contacts that the user selected.
 *
 * @see cloudsponge_incoming_callback()
 */
function hook_cloudsponge_submit_contacts($emails, $owner, $source) {
  watchdog('cloudsponge', print_r($emails, TRUE));
  watchdog('cloudsponge', print_r($owner, TRUE));
  watchdog('cloudsponge', print_r($source, TRUE));
}