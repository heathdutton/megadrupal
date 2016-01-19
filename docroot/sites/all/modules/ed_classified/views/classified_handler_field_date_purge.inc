<?php
/**
 * @file
 * Expiration date field handler for views
 *
 * @copyright (c) 2010 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */

class classified_handler_field_date_purge extends views_handler_field_date {

  function render($values) {

    $grace = _classified_get('grace');
    $date_format = _classified_get('date-format');

    // Save expires.
    $expires = $values->classified_node_expires;

    // Compute purge.
    $values->classified_node_expires += $grace * 24*60*60;

    // Render purge.
    $ret = parent::render($values);

    // Restore expires.
    $values->classified_node_expires = $expires;
    return $ret;
  }
}
