<?php

/**
 * @file
 * A hack of a sort handler, for putting content written by the acting user on
 * top.
 */

class courseplanner_handler_sort_author extends views_handler_sort {
  function can_expose() {
     return FALSE;
  }

  // Sorts on comparison of the node UID with the UID of the acting user.
  function query() {
    $this->ensure_my_table();
    global $user;
    $formula = $user->uid . '=' . $this->real_field;
    $this->query->add_orderby(NULL, $formula, 'DESC');
  }
}
