<?php

/**
 * @file
 * Drush functions galore! Galore.
 */

/**
 * Implements hook_drush_sql_sync_sanitize().
 */
function twcards_drush_sql_sync_sanitize($site) {
  drush_sql_register_post_sync_op('twcards_leads',
    dt('Leads are sensitive, if only because they contain email addresses.'),
    "DELETE from twcards_leads;");
}

