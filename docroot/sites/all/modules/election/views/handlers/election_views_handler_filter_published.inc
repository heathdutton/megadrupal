<?php
/**
 * @file
 * Views handler to filter elections by 'published' status.
 */

class election_views_handler_filter_published extends views_handler_filter {

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
    $table = $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], "$table.published = 1 OR ($table.uid = ***CURRENT_USER*** AND ***CURRENT_USER*** <> 0) OR ***EDIT_ANY_ELECTION*** = 1");
  }
}
