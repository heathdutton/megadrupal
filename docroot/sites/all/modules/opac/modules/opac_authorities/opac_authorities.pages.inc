<?php
/**
 * @file
 * Page callbacks for opac authorities module.
 */

/**
 * Authority view callback.
 *
 * @param object $authority
 *   Loaded opac authority entity.
 *
 * @return string
 *   HTML output.
 */
function opac_authority_view($authority) {
  drupal_set_title(entity_label('opac_authority', $authority));
  return entity_view('opac_authority', array(entity_id('opac_authority', $authority) => $authority), 'full');
}
