<?php
/**
 * Defines a sort handler for SQL date types.
 */
class availability_calendar_handler_sort_sql_date extends views_handler_sort_date {
  /**
   * Override to change the default granularity.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['granularity'] = array('default' => 'day');
    return $options;
  }

  /**
   * Override to reduce the granularity options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['granularity']['#options'] = array(
      'day'    => t('Day'),
      'week'    => t('Week'),
      'month'  => t('Month'),
      'year'   => t('Year'),
    );
  }

  /**
   * Override to handle the week granularity and handle the day granularity in a
   * different way.
   */
  function query() {
    $this->ensure_my_table();
    switch ($this->options['granularity']) {
      case 'day':
      default:
        $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order']);
        break;
      case 'month':
      case 'year':
        parent::query();
        break;
      case 'week':
        // Add the field.
        $db_type = Database::getConnection()->databaseType();
        $field = "$this->table_alias.$this->real_field";
        switch ($db_type) {
          default:
          case 'mysql':
            // @todo: if first day of week is sunday we can use %X%V.
            $formula = "DATE_FORMAT($field, '%x%v')";
            break;
          case 'pgsql':
            $formula = "TO_CHAR($field, 'IYYYIW')";
            break;
          case 'sqlite':
            // %V should be used instead, but I think that will not change the
            // %Y modifier to return the year that belongs to the calculated
            // week number (2013-01-01 will result in 201352 instead of 201252).
            $formula = "strftime($field, %Y%W')";
            break;
        }
        $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias . '_' . $this->field . '_' . $this->options['granularity']);
        break;
    }
  }
}
