<?php
/**
 * @file
 * User profile comments argument handler.
 */

// @codingStandardsIgnoreStart
class user_profile_comments_argument extends views_handler_argument_numeric {
  /**
   * Generate a query and a countquery from all of the information.
   */
  public function query($group_by = false) {
    $this->ensure_my_table();

    // Add certain condition.
    if (!empty($this->argument)) {
      $this->add_join_to_primary_table();
      $this->add_user_profile_condition();
    }
  }

  /**
   * Add join for node table.
   */
  protected function add_join_to_primary_table() {
    if (!array_key_exists('user_profile_comment', $this->query->relationships)) {
      $join = new views_join();
      $join->construct('node', 'comment', 'nid', 'nid', array(), 'INNER');
      $this->query->add_relationship('user_profile_comment', $join, 'comment');
    }
  }

  /**
   * Add condition.
   */
  protected function add_user_profile_condition() {
    $this->query->add_where(0, 'user_profile_comment.uid', $this->argument);
  }
}
// @codingStandardsIgnoreEnd
