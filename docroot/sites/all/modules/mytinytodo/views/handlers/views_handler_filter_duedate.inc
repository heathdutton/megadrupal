<?php

/**
 * Filter to handle due dates stored as a string.
 */
class views_handler_filter_duedate extends views_handler_filter_date {
    function op_between($field) {
        $field = "UNIX_TIMESTAMP( STR_TO_DATE($field, '%Y-%m-%d') )";
        parent::op_between($field);
    }

    function op_simple($field) {
        $field = "UNIX_TIMESTAMP( STR_TO_DATE($field, '%Y-%m-%d') )";
        parent::op_simple($field);
    }
}
