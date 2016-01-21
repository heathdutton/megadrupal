<?php
/**
 * @file
 * Field handler to present a link to an election post's nomination form.
 */

class election_candidate_views_handler_field_link_nominate extends views_handler_field {

  /**
   * Overrides parent::render().
   */
  public function render($values) {
    $post = election_post_load($values->post_id);
    return theme('election_candidate_nominate_link', array('post' => $post));
  }

}
