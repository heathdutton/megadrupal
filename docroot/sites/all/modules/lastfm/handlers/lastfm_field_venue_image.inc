<?php
// $Id;

/**
 * @file
 *   Views field handler for last.fm venue image fields.
 */

/**
 * Views field handler for last.fm venue image fields.
 *
 * Just override the default handler to provide the 'Mega Large' image size.
 */
class lastfm_field_venue_image extends lastfm_field_image {
  
  /**
   * Venues have a 'mega' option in addition to the standard ones.
   */
  function get_image_sizes() {
    $options = parent::get_image_sizes();
    $options['mega'] = 'Mega Large';
    return $options;
  }
}
