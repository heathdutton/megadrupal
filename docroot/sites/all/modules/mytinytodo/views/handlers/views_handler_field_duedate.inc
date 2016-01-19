<?php
/**
 * Field handler to present a duedate as a date.
 */
class views_handler_field_duedate extends views_handler_field_date {
    function get_value($values, $field = null) {
        $date = parent::get_value($values);
        return strtotime($date);
    }
}
