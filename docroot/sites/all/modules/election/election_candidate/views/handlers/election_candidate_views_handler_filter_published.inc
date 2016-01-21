<?php
/**
 * @file
 * Views handler to filter election candidates by 'published' status.
 */

class election_candidate_views_handler_filter_published extends views_handler_filter {

  public function admin_summary() {
    return;
  }

  public function operator_form(&$form, &$form_state) {
    return;
  }

  /**
   * Overrides parent::can_expose().
   *
   * It makes no sense to expose this filter.
   */
  public function can_expose() {
    return FALSE;
  }

  public function query() {
    // Join to the election table: candidates should not be displayed if the
    // election is not published.
    $join = new views_join();
    $join->construct('election', 'election_candidate', 'election_id', 'election_id');
    $candidate_table = $this->ensure_my_table();
    $election_table = $this->query->add_relationship('election', $join, $this->table_alias, $this->relationship);
    // Join to the election post table: candidates should not be displayed if
    // the post is not published.
    $join = new views_join();
    $join->construct('election_post', 'election_candidate', 'post_id', 'post_id');
    $post_table = $this->query->add_relationship('election_post', $join, $this->table_alias, $this->relationship);
    $this->query->add_where_expression($this->options['group'], "($candidate_table.published = 1 AND $post_table.published = 1 AND $election_table.published = 1) OR ($election_table.uid = ***CURRENT_USER*** AND ***CURRENT_USER*** <> 0) OR ***EDIT_ANY_ELECTION*** = 1");
  }
}
