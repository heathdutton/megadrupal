<?php
/**
 * @file
 * User profile comments filter handler.
 */

// @codingStandardsIgnoreStart
class user_profile_comments_filter extends views_handler_filter_boolean_operator {
  /**
   * Generate a query and a countquery from all of the information.
   */
  public function query() {
    $this->ensure_my_table();

    // Add certain condition.
    if (isset($this->value) && $this->value == 1) {
      $this->add_join_to_primary_table();
      $this->add_user_profile_condition('=');
    }
    elseif (isset($this->value) && $this->value == 0) {
      $this->add_join_to_primary_table();
      $this->add_user_profile_condition('<>');
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
  protected function add_user_profile_condition($type) {
    $this->query->add_where($this->options['group'], 'user_profile_comment.type', 'user_profile_comment', $type);
  }
}
// @codingStandardsIgnoreEnd
