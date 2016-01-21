<?php
// $Id;

/**
 * @file
 *   Views argument validator for last.fm API event arguments.
 */

/**
 * Views argument validator for last.fm API event arguments.
 *
 * Because of the way we have implemented arguments, and because of the way 
 * the last.fm API works, there are lots of combinations of arguments that 
 * don't make sense. We try and detect these here and throw errors if we 
 * find conflicts.
 */
class lastfm_views_plugin_argument_validate_events extends views_plugin_argument_validate {

  /**
   * Validate that the arguments are combined in such a way that they don't
   * collide. For events this means that you can only have one argument of 
   * type user, artist, venue, or one combination of lat/long/distance/location.
   */
  function validate_argument($arg) {
    //kpr($this);
    
    $unique_args = array('artist', 'user', 'event');
    $arg_keys = array_keys($this->view->argument);

    if (count(array_intersect($unique_args, $arg_keys)) > 1) {
      drupal_set_message('You may only use one of the following arguments at a time when querying events: artist, user, event.');
      return FALSE;
    }

    if (in_array('lat', $arg_keys) && !in_array('long', $arg_keys)) {
      drupal_set_message('When using a latitude argument you must also use a longitude argument.');
      return FALSE;
    }

    if (in_array('long', $arg_keys) && !in_array('lat', $arg_keys)) {
      drupal_set_message('When using a longitude argument you must also use a latitude argument.');
      return FALSE;
    }

    return TRUE;
  }
}
