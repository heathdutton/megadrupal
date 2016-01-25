<?php
/**
 * @file
 * Filter posts by voting status = open.
 */

class election_post_views_handler_filter_vstatus extends views_handler_filter_boolean_operator {

  /**
   * Overrides parent::query().
   */
  public function query() {
    $election_table = 'election';
    $post_table = $this->ensure_my_table();
    $join = new views_join();
    $join->construct($post_table, $election_table, 'election_id', 'election_id');
    $vars = array(
      ':openstatus' => ELECTION_STATUS_OPEN,
      ':closedstatus' => ELECTION_STATUS_CLOSED,
      ':scheduledstatus' => ELECTION_STATUS_SCHEDULED,
      ':rtime' => REQUEST_TIME,
    );
    if (empty($this->value)) {
      $vars[':postclosed'] = ELECTION_POST_STATUS_CLOSED;
      $expression = "$post_table.vstatus_inheritance = :postclosed OR ($election_table.vstatus = :closedstatus OR ($election_table.vstatus = :scheduledstatus AND ($election_table.vopen_time >= :rtime OR $election_table.vclose_time <= :rtime)))";
    }
    else {
      $vars[':postinherit'] = ELECTION_POST_STATUS_INHERIT;
      $expression = "$post_table.vstatus_inheritance = :postinherit AND ($election_table.vstatus = :openstatus OR ($election_table.vstatus = :scheduledstatus AND $election_table.vopen_time < :rtime AND $election_table.vclose_time > :rtime))";
    }
    $this->query->add_where_expression(
      $this->options['group'],
      $expression,
      $vars
    );
  }

}
