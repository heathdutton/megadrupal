<?php
/**
 * @file
 * Provide election post ID argument handler.
 */

/**
 * Argument handler to accept a post ID.
 */
class election_post_views_handler_argument_post_id extends views_handler_argument_numeric {

  /**
   * Overrides views_handler_title_query().
   *
   * Get the title of the post.
   */
  public function title_query() {
    $titles = array();
    $query = db_select('election_post')
      ->fields('election_post', array('title'))
      ->condition('post_id', $this->value, 'IN');
    $result = $query->execute();
    foreach ($result as $item) {
      $titles[] = check_plain($item->title);
    }
    return $titles;
  }

}
