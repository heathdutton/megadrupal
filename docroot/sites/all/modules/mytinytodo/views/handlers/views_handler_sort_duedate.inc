<?php

/**
 * Basic sort handler for due dates.
 */
class views_handler_sort_duedate extends views_handler_sort_date {
    /**
     * Called to add the sort to a query.
     */
    function query() {
        $this->ensure_my_table();

        $field = "UNIX_TIMESTAMP( STR_TO_DATE({$this->real_field}, '%Y-%m-%d') )";
        $table_field = "UNIX_TIMESTAMP( STR_TO_DATE({$this->table_alias}.{$this->real_field}, '%Y-%m-%d') )";

        switch ($this->options['granularity']) {
            case 'second':
            default:
                $formula = views_date_sql_format('YmdHis', $table_field);
            case 'minute':
                $formula = views_date_sql_format('YmdHi', $table_field);
                break;
            case 'hour':
                $formula = views_date_sql_format('YmdH', $table_field);
                break;
            case 'day':
                $formula = views_date_sql_format('Ymd', $table_field);
                break;
            case 'month':
                $formula = views_date_sql_format('Ym', $table_field);
                break;
            case 'year':
                $formula = views_date_sql_format('Y', $table_field);
                break;
        }

        // Add the field.
        $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias . '_' . $this->field . '_' . $this->options['granularity']);
    }
}
