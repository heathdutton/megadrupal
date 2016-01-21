<?php

/**
 * Custom date filter.
 * Provides two select boxes for filtering results by month and year.
 * Shows only the years for which there are clicks defined.
 *
 * If there is only one year to be displayed in the select box,
 * then the year is shown INSTEAD of the select box.
 */
class uc_affiliate2_handler_filter_date extends views_handler_filter {
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    return $this->value['month'] . '/' . $this->value['year'];
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'month' => array('default' => ''),
        'year' => array('default' => ''),
      ),
    );

    return $options;
  }

  function value_form(&$form, &$form_state) {
    /**
     * if we have the affiliate uid (for example, if the filter is rendered as exposed in the affiliate center),
     * we use it to limit our queries
     */
    $aid = arg(1);
    if (is_numeric($aid)) {
      $start_query = db_query('SELECT time FROM {uc_affiliate2_clicks} WHERE aid = %d ORDER BY time ASC LIMIT 1', $aid);
      $end_query = db_query('SELECT time FROM {uc_affiliate2_clicks} WHERE aid = %d ORDER BY time DESC LIMIT 1', $aid);
    }
    else {
      $start_query = db_query('SELECT time FROM {uc_affiliate2_clicks} ORDER BY time ASC LIMIT 1');
      $end_query = db_query('SELECT time FROM {uc_affiliate2_clicks} ORDER BY time DESC LIMIT 1');
    }

    $start = db_result($start_query);
    $end = db_result($end_query);

    // no clicks defined
    if (!$start && !$end) {
      $start = $end = time();
    }

    // Calculate the years which can be displayed in the form
    $years = range(date('Y', $start), date('Y', $end));
    $form_years = array_combine($years, $years);

    $form['value']['#tree'] = TRUE;

    $form['value']['month'] = array(
      '#type' => 'select',
      '#title' => t('Month'),
      '#options' => array(
        'all' => t('All'),
        '01' => t('January'),
        '02' => t('February'),
        '03' => t('March'),
        '04' => t('April'),
        '05' => t('May'),
        '06' => t('June'),
        '07' => t('July'),
        '08' => t('August'),
        '09' => t('September'),
        '10' => t('October'),
        '11' => t('November'),
        '12' => t('December'),
      ),
      '#default_value' => !empty($this->value['month']) ? $this->value['month'] : date('m'),
    );

    // special case when there's only one option. Just display it, no need for a select box.
    if (count($form_years) == 1) {
      // Pass the value along
      $form['value']['year'] = array(
        '#type' => 'value',
        '#value' => reset($form_years),
      );

      // Display something to the user
      $form['value']['year_display'] = array(
        '#type' => 'item',
        '#title' => t('Year'),
        '#markup' => reset($form_years),
      );
    }
    else {
      $form['value']['year'] = array(
        '#type' => 'select',
        '#title' => t('Year'),
        '#options' => $form_years,
        '#default_value' => !empty($this->value['year']) ? $this->value['year'] : date('Y', $end),
      );
    }
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    // Don't filter by default (if the filter wasn't submitted)
    if ($this->value['month'] == 'all') {
      $start = mktime(0, 0, 0, 1, 1, $this->value['year']);
      $end = mktime(23, 59, 59, 12, 31, $this->value['year']);
    }
    else {
      $start = mktime(0, 0, 0, $this->value['month'], 1, $this->value['year']);
      $end = mktime(23, 59, 59, $this->value['month'] + 1, 0, $this->value['year']);
    }

    $this->query->add_where($this->options['group'], $field, array($start, $end), 'BETWEEN');
  }
}
