<?php
/**
 * @file
 * Views field handler for displaying the current user's eligibility to vote.
 */

class election_vote_views_handler_field_eligibility extends views_handler_field {

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $post = election_post_load($this->get_value($values));
    return theme('election_vote_eligibility', array('post' => $post));
  }

}
