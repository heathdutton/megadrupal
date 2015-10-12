<?php
/**
 * @file
 * Views field handler for displaying the user's vote eligibility as a Boolean.
 */

class election_vote_views_handler_field_eligibility_boolean extends views_handler_field_boolean {

  /**
   * Overrides parent::get_value().
   */
  public function get_value($values, $field = NULL) {
    $post_id = parent::get_value($values);
    if ($field === NULL) {
      $post = election_post_load($post_id);
      return election_vote_access('vote', $post);
    }
    return $post_id;
  }

}
