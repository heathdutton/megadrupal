<?php
/**
 * Field handler to present a todo priority as a string
 */
class views_handler_field_todo_priority extends views_handler_field {
    function construct() {
        parent::construct();
    }

    function render($values) {
        $value = $this->get_value($values);

        switch ($value) {
            case -1:
                return '<span style="border: 1px solid #F9F9F9; background-color: #37F; color: white;">−1</span>';
            case 0:
                return '<span style="border: 1px solid #F9F9F9; background-color: white; color: black;">±0</span>';
            case 1:
                return '<span style="border: 1px solid #F9F9F9; background-color: #F70; color: white;">+1</span>';
            case 2:
                return '<span style="border: 1px solid #F9F9F9; background-color: #F33; color: white;">+2</span>';
            default:
                return $value;
        }
    }
}
