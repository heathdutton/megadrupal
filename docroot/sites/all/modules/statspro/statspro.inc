<?php

/**
 * @file
 *   statspro class for Statistics Pro module.
 *
 */

/**
 * Manages the data saving and retrieval according to the user defined
 * parameters.
 */
class StatsPro {

  protected $period;
  protected $last_run;
  protected $current_field;
  protected $fields = array();

  public $absolute_amounts = array();

  /*
   * Day results.
   *
   */
  protected $days;

  function __construct() {
    $this->set_fields();
    if (module_exists('statistics')) {
      $this->absolute_amounts = array(
        'pi',
        'upi',
      );
    }
    else {
      $this->absolute_amounts = array();
    }
    $this->absolute_amounts += array(
      'error',
      'uerror',
      'warning',
      'uwarning',
      'emergency',
      'uemergency',
      'alert',
      'ualert',
      'critical',
      'ucritical',
    );
  }

  /**
   * Set all available data field to member variable.
   */
  protected function set_fields() {
    // During cache clearance, this function can cause an infinite loop!
    // In order to prevent this, we increment a session variable and if it
    // reaches 5 (arbitrary, but suggests we are looping), we don't process
    // this function.
    if (isset($_SESSION['statspro_loop_counter'])) {
      $_SESSION['statspro_loop_counter']++;
    }
    else {
      $_SESSION['statspro_loop_counter'] = 1;
    }
    if ($_SESSION['statspro_loop_counter'] >= 5) {
      drupal_set_message("Probable infinite loop detected by Statistics Pro module and averted.");
      unset($_SESSION['statspro_loop_counter']);
      return;
    }
    
    $this->fields = array(
      'nuser' => user_access('administer users') ? l(t('User registrations'), 'admin/user/user') : t('User registrations'),
      'auser' => t('User online'),
      'nnode' => user_access('administer nodes') ? l(t('New nodes'), 'admin/content/node') : t('New nodes'),
      'cnode' => user_access('administer nodes') ? l(t('Changed nodes'), 'admin/content/node') : t('Changed nodes'),
//       'comment' => user_access('administer comments') ? l(t('Comments'), 'admin/content/comment') : t('Comments'),
    );
    if (module_exists('statistics')) {
      $this->fields += array(
        'pi' => l(t('Page impressions'), 'admin/reports/hits'),
        'upi' => l(t('Page impressions for authenticated users'), 'admin/reports/hits'),
      );
    }
    $this->fields += array(
      'error' => user_access('access site reports') ? l(t('Errors'), 'admin/reports/dblog') : t('Errors'),
      'uerror' => user_access('access site reports') ? l(t('Errors for authenticated users'), 'admin/reports/dblog') : t('Errors for authenticated users'),
      'warning' => user_access('access site reports') ? l(t('Warnings'), 'admin/reports/dblog') : t('Warnings'),
      'uwarning' => user_access('access site reports') ? l(t('Warnings for authenticated users'), 'admin/reports/dblog') : t('Warnings for authenticated users'),
      'emergency' => user_access('access site reports') ? l(t('Emergencies'), 'admin/reports/dblog') : t('Emergencies'),
      'uemergency' => user_access('access site reports') ? l(t('Emergencies for authenticated users'), 'admin/reports/dblog') : t('Emergencies for authenticated users'),
      'alert' => user_access('access site reports') ? l(t('Alerts'), 'admin/reports/dblog') : t('Alerts'),
      'ualert' => user_access('access site reports') ? l(t('Alerts for authenticated users'), 'admin/reports/dblog') : t('Alerts for authenticated users'),
      'critical' => user_access('access site reports') ? l(t('Critical conditions'), 'admin/reports/dblog') : t('Critical conditions'),
      'ucritical' => user_access('access site reports') ? l(t('Critical conditions for authenticated users'), 'admin/reports/dblog') : t('Critical conditions for authenticated users'),
    );
    
    // If we reach here, then we are not looping
    if (isset($_SESSION['statspro_loop_counter'])) {
      unset($_SESSION['statspro_loop_counter']);
    }
  }

  /**
   * Get data fields.
   */
  public function get_fields() {
    return $this->fields;
  }

  protected function get_calc_fields($whitelist, $blacklist) {
    if ((is_array($whitelist)) && (is_array($blacklist))) {
      die('whitelist and blacklist cannot be used togther.');
    }
    elseif (is_array($whitelist)) {
      $fields = array();
      foreach ($this->fields as $field => $desc) {
        if (in_array($field, $whitelist)) {
          $fields[$field] = $desc;
        }
      }
    }
    elseif (is_array($blacklist)) {
      $fields = array();
      foreach ($this->fields as $field => $desc) {
        if (!in_array($field, $blacklist)) {
          $fields[$field] = $desc;
        }
      }
    }
    else {
      // all fields
      $fields = $this->fields;
    }

    return $fields;
  }

  public function get_aggregate_stat($mode, $period = NULL) {
    $rc = FALSE;

    $where_sql = ($period === NULL) ? '' : 'WHERE ' . $this->get_period('timestamp', TRUE);

    if (in_array($mode, $this->absolute_amounts)) {
      $where_sql = ($period === NULL) ? '' : 'WHERE ' . $this->get_period();
      // TODO Please convert this statement to the D7 database API syntax.
      $rc = array(
        'subject' => strip_tags($this->fields[$mode]),
        'amount' => db_query("SELECT SUM(%s) FROM {statspro} " . $where_sql, $mode)->fetchField(),
      );
    }
    elseif ($mode == 'users') {
      $where_sql = ($period === NULL) ? '' : 'WHERE ' . $this->get_period('created', TRUE);
      // TODO Please convert this statement to the D7 database API syntax.
      $rc = array(
        'subject' => t('Amount of users'),
        'amount' => db_query("SELECT COUNT(*) FROM {users} " . $where_sql)->fetchField(),
      );
    }
    elseif ($mode == 'terms') {
      $rc = array(
        'subject' => t('Terms'),
        'amount' => $this->generate_term_stats(),
      );
    }
    elseif ($mode == 'nodes') {
      $where_sql = ($period === NULL) ? '' : $this->get_period('created', TRUE) . ' AND ';
      $where_sql = 'WHERE ' . $where_sql . ' status = 1';
      // TODO Please convert this statement to the D7 database API syntax.
      $rc = array(
        'subject' => t('Amount of nodes'),
        'amount' => db_query("SELECT COUNT(*) FROM {node} " . $where_sql)->fetchField(),
      );
    }
    elseif ($mode == 'node_types') {
      $rc = array(
        'subject' => t('Amount of node types'),
        'amount' => db_query("SELECT COUNT(*) FROM {node_type}")->fetchField(),
      );
    }
    elseif ($mode == 'comments') {
      $where_sql = ($period === NULL) ? '' : 'WHERE ' . $this->get_period('timestamp', TRUE);
      // TODO Please convert this statement to the D7 database API syntax.
      $rc = array(
        'subject' => t('Amount of comments'),
        'amount' => db_query("SELECT COUNT(*) FROM {comments} " . $where_sql)->fetchField(),
      );
    }
    elseif ($mode == 'aliases') {
      $rc = array(
        'subject' => t('Amount of aliases'),
        'amount' => db_query("SELECT COUNT(*) FROM {url_alias}")->fetchField(),
      );
    }
    elseif ($mode == 'sessions') {
      $where_sql = ($period === NULL) ? '' : 'WHERE ' . $this->get_period('timestamp', TRUE);
      // TODO Please convert this statement to the D7 database API syntax.
      $rc = array(
        'subject' => t('Amount of sessions'),
        'amount' => db_query("SELECT COUNT(*) FROM {sessions} " . $where_sql)->fetchField(),
      );
    }
    elseif ($mode == 'modules') {
      $modules = system_rebuild_module_data();
      $amount = 0;
      foreach ($modules as $values) {
        if ($values->status) {
          $amount++;
        }
      }
      $rc = array(
        'subject' => t('Amount of modules'),
        'amount' => $amount,
      );
    }

    return $rc;
  }

  /**
   * Get saved statistics for display.
   */
  public function get_stats($period, $whitelist = NULL, $blacklist = NULL) {
    $this->period = $period;

    $with_data = FALSE;

    // build field list
    $fields = $this->get_calc_fields($whitelist, $blacklist);

    // initialize values
    $data = array();

    foreach ($fields as $field => $desc) {
      $data[$field] = 0;
    }

    // get values
    $sql_fields = 'day, nuser, auser, nnode, cnode, comment';
    if (module_exists('statistics')) {
      $sql_fields .= ', pi, upi';
    }
    $sql_fields .= ', error, uerror,
          warning, uwarning, emergency, uemergency, alert, ualert, critical,
          ucritical';
    $sql = "
      SELECT
        $sql_fields
        FROM {statspro}
        WHERE " . $this->get_period();
    // TODO Please convert this statement to the D7 database API syntax.
    $result = db_query($sql);
    foreach ($result as $row) {
      foreach ($fields as $field => $desc) {
        $data[$field] += $row->$field;
        if ((!$with_data) && ($row->$field > 0)) {
          $with_data = TRUE;
        }
      }
    }

    if ($with_data) {
      // prepare return array
      $rows = array();
      foreach ($fields as $field => $desc) {
        $rows[] = array($desc, $data[$field]);
      }
      return $rows;
    }
  }

  /**
   * Returns a string with a WHERE compliant representation of a min and max day
   * selection.
   *
   * @param <string> $field
   * @param <int> $min
   * @param <int> $max
   * @return <string> SQL min and max WHERE.
   */
  protected function get_min_max_period($field, $min, $max) {
    return sprintf('(%s >= %d AND %1$s <= %d)', $field, $min, $max);
  }

  /**
   * Returns a SQL WHERE for the selected week.
   *
   * Weeks starts at mondays and ends at sundays.
   *
   * For date('w') we get 0 (for Sunday) through 6 (for Saturday).
   *
   * @param <int> $reference Defines the selected week.
   * @return <string> SQL min and max WHERE for week.
   */
  protected function get_week_from_reference($reference, $field, $use_timestamp) {
    $min = $this->get_first_day_of_week_from_reference($reference, $use_timestamp);
    $day_of_week = date('w', $reference);
    $max = ($day_of_week == 0)  ? 'today' : 'next sunday';
    $max = strtotime($max, $reference);
    if (!$use_timestamp) {
      $max = date('Y-m-d', $max);
      return sprintf('(%s >= \'%s\' AND %1$s <= \'%s\')', $field, $min, $max);
    }
    return $this->get_min_max_period($field, $min, $max);
  }

  /**
   * Returns a YYYY-MM-DD date representation of the first day of same the week
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> First day of the same week of the reference day.
   */
  protected function get_first_day_of_week_from_reference($reference, $use_timestamp) {
    $day_of_week = date('w', $reference);
    $first = ($day_of_week == 1) ? 'today' : 'previous monday';
    $first = strtotime($first, $reference);
    if (!$use_timestamp) {
      $first = date('Y-m-d', $first);
    }
    return $first;
  }

  /**
   * Returns a SQL WHERE for the selected month.
   *
   * @param <int> $reference Defines the selected month.
   * @return <string> SQL min and max WHERE for month.
   */
  protected function get_month_from_reference($reference, $field, $use_timestamp) {
    $min = $this->get_first_day_of_month_from_reference($reference, $use_timestamp);
    $max = $this->get_last_day_of_month_from_reference($reference, $use_timestamp);
    if (!$use_timestamp) {
      return sprintf('(%s >= \'%s\' AND %1$s <= \'%s\')', $field, $min, $max);
    }
    return $this->get_min_max_period($field, $min, $max);
  }

  /**
   * Returns a YYYY-MM-DD date representation of the first day of same the month
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> First day of the same month of the reference day.
   */
  protected function get_first_day_of_month_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $month = date('m', $reference);
    $first = $year . '-' . $month . '-01';
    if ($use_timestamp) {
      return strtotime($first);
    }
    return $first;
  }

  /**
   * Returns a YYYY-MM-DD date representation of the last day of same the month
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> Last day of the same month of the reference day.
   */
  protected function get_last_day_of_month_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $month = date('m', $reference);
    if ($use_timestamp) {
      $last = ($month == 12) ?
        ($year + 1) . '-01-01' :
        sprintf('%04u-%02u-01', $year, $month + 1);
    }
    else {
      $last = ($month == 12) ?
        $year . '-12-31' :
        sprintf('%04u-%02u-00', $year, $month + 1);
    }
    $last = strtotime($last);
    if (!$use_timestamp) {
      $last = date('Y-m-d', $last);
    }
    return $last;
  }

  /**
   * Returns a YYYY-MM-DD date representation of the first day of same the quarter
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> First day of the same quarter of the reference day.
   */
  protected function get_first_day_of_quarter_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $first_month_of_quarter = $this->get_first_month_of_quarter_from_reference($reference);
    $first = sprintf('%04u-%02u-01', $year, $first_month_of_quarter);
    if ($use_timestamp) {
      $first = strtotime($first);
    }
    return $first;
  }

  /**
   * Returns a YYYY-MM-DD date representation of the last day of same the quarter
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> Last day of the same quarter of the reference day.
   */
  protected function get_last_day_of_quarter_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $last_month_of_quarter = $this->get_first_month_of_quarter_from_reference($reference);
    if ($use_timestamp) {
      $last = ($last_month_of_quarter == 12) ?
        ($year + 1) . '-01-01' :
        sprintf('%04u-%02u-01', $year, $last_month_of_quarter + 1);
    }
    else {
      $last = ($last_month_of_quarter == 12) ?
        $year . '-12-31' :
        sprintf('%04u-%02u-00', $year, $last_month_of_quarter + 1);
    }
    $last = strtotime($last);
    if (!$use_timestamp) {
      $last = date('Y-m-d', $last);
    }
    return $last;
  }

  /**
   * Returns the first month of the same the quarter as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> First month of the same quarter of the reference day.
   */
  protected function get_first_month_of_quarter_from_reference($reference) {
    $month = date('m', $reference);
    $first_month = floor(($month - 1) / 3);
    $first_month = (int) (($first_month * 3) + 1);
    return sprintf('%02u', $first_month);
  }

  /**
   * Returns the last month of same the quarter as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> Last month of the same quarter of the reference day.
   */
  protected function get_last_month_of_quarter_from_reference($reference) {
    $month = date('m', $reference);
    $last_month = floor(($month - 1) / 3);
    $last_month = (int) (($last_month + 1) * 3);
    return sprintf('%02u', $last_month);
  }

  /**
   * Returns a YYYY-MM-DD date representation of the first day of same the year
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> First day of the same year of the reference day.
   */
  protected function get_first_day_of_year_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $first = sprintf('%04u-01-01', $year);
    if ($use_timestamp) {
      $first = strtotime($first);
    }
    return $first;
  }

  /**
   * Returns a YYYY-MM-DD date representation of the last day of same the quarter
   * as the reference day.
   *
   * @param <int> $reference A timestamp representaing the reference day.
   * @return <int> Last day of the same quarter of the reference day.
   */
  protected function get_last_day_of_year_from_reference($reference, $use_timestamp) {
    $year = date('Y', $reference);
    $last = $use_timestamp ? sprintf('%04u-01-01', $year + 1) : sprintf('%04u-12-31', $year);
    if ($use_timestamp) {
      $last = strtotime($last);
    }
    return $last;
  }

  /**
   *
   * Generate SQL fragment for period query.
   *
   */
  public function get_period($field = 'day', $use_timestamp = FALSE, $period = FALSE) {
    if ($period !== FALSE) {
      $this->period = $period;
    }
    $now = REQUEST_TIME;
    if (is_numeric($this->period)) {
      $timestamp_starting_date = strtotime(sprintf('-%u day', $this->period), $now);
      if ($use_timestamp) {
        return sprintf('%s >= %u', $field, $timestamp_starting_date);
      }
      else {
        $starting_date = date('Y-m-d', $timestamp_starting_date);
        return sprintf('%s >= \'%s\'', $field, $starting_date);
      }
    }
    else {
      switch ($this->period) {
        case 'today':
          if ($use_timestamp) {
            $timestamp_begining_today = strtotime('today', $now);
            return sprintf('%s >= %u', $field, $timestamp_begining_today);
          }
          else {
            $today = date('Y-m-d', $now);
            return sprintf('%s = \'%s\'', $field, $today);
          }
          break;

        case 'yesterday':
          if ($use_timestamp) {
            $min = strtotime('-1 day', $now);
            $max = strtotime('today', $now);
            return $this->get_min_max_period($field, $min, $max);
          }
          else {
            $day = date('Y-m-d', strtotime('-1 day', $now));
            return sprintf('%s = \'%s\'', $field, $day);
          }
          break;

        case 'week_current':
          $min = $this->get_first_day_of_week_from_reference($now, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'week_last':
          $reference = strtotime('-1 week', $now);
          return $this->get_week_from_reference($reference, $field, $use_timestamp);
          break;

        case 'week_last2':
          $reference = strtotime('-2 week', $now);
          return $this->get_week_from_reference($reference, $field, $use_timestamp);
          break;

        case 'month_current':
          $min = $this->get_first_day_of_month_from_reference($now, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'month_last':
          $reference = strtotime('-1 month', $now);
          return $this->get_month_from_reference($reference, $field, $use_timestamp);
          break;

        case 'month_last3':
          $reference = strtotime('-3 month', $now);
          $min = $this->get_first_day_of_month_from_reference($reference, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'month_last6':
          $reference = strtotime('-6 month', $now);
          $min = $this->get_first_day_of_month_from_reference($reference, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'quarter_current':
          $min = $this->get_first_day_of_quarter_from_reference($now, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'quarter_last':
          $reference = strtotime('-3 month', $now);
          $min = $this->get_first_day_of_quarter_from_reference($reference, $use_timestamp);
          $max = $this->get_last_day_of_quarter_from_reference($reference, $use_timestamp);
          if ($use_timestamp) {
            return $this->get_min_max_period($field, $min, $max);
          }
          else {
            return sprintf('(%s >= \'%s\' AND %1$s <= \'%s\')', $field, $min, $max);
          }
          break;

        case 'year_current':
          $min = $this->get_first_day_of_year_from_reference($now, $use_timestamp);
          if ($use_timestamp) {
            return sprintf('%s >= %u', $field, $min);
          }
          else {
            return sprintf('%s >= \'%s\'', $field, $min);
          }
          break;

        case 'year_last':
          $reference = strtotime('-1 year', $now);
          $min = $this->get_first_day_of_year_from_reference($reference, $use_timestamp);
          $max = $this->get_last_day_of_year_from_reference($reference, $use_timestamp);
          if ($use_timestamp) {
            return $this->get_min_max_period($field, $min, $max);
          }
          else {
            return sprintf('(%s >= \'%s\' AND %1$s <= \'%s\')', $field, $min, $max);
          }
          break;

        default:
          watchdog('statspro', "Unknown period '@period'.", array('@period' => $this->period), WATCHDOG_ERROR);
          return FALSE;
          break;
      }
    }
  }

  /**
   * Get data out of drupal source tables.
   */
  public function get_days_data($last_run) {
    $this->last_run = $last_run;
    
    // reset days
    $this->days = array();

    foreach ($this->fields as $field => $desc) {
      $this->current_field = $field;
      $method = 'get_data_' . $this->current_field;
      $this->$method();
    }

    return $this->days;
  }

  /**
   * Add day data.
   *
   * @param string  $date
   * @param int     $amount
   */
  protected function add_day_data($date, $amount) {
    if (!empty($date)) {
      $amount = (int) $amount;
      if ((isset($this->days[$date][$this->current_field])) &&
        ($this->days[$date][$this->current_field] > 0)) {
        $this->days[$date][$this->current_field] += $amount;
      }
      else {
        $this->days[$date][$this->current_field] = $amount;
      }
    }
  }

  /**
   * Calculates the number of new users registered per day since the last run.
   */
  protected function get_data_nuser() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(created)) AS dategroup
        FROM {users}
        WHERE uid > :uid
          AND created >= :created
        GROUP BY dategroup', array(':uid' => 0, ':created' => $this->last_run));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Calculates the number of authenticated users that accessed the site per day
   * since the last run.
   */
  protected function get_data_auser() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(access)) AS dategroup
        FROM {users}
        WHERE uid > :uid
          AND access >= :access
        GROUP BY dategroup', array(':uid' => 0, ':access' => $this->last_run));

    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Returns the number of new nodes created per day since the last run.
   */
  protected function get_data_nnode() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(created)) AS dategroup
        FROM {node}
        WHERE created >= :created
        GROUP BY dategroup', array(':created' => $this->last_run));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Returns the number of nodes changed per day since the last run.
   */
  protected function get_data_cnode() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(changed)) AS dategroup
        FROM {node}
        WHERE changed >= :changed
          AND created <> :created
        GROUP BY dategroup', array(':changed' => $this->last_run, ':created' => $this->last_run));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Count nodes for terms.
   *
   * @return int
   */
  public function generate_term_stats() {
    // get nodes
    
    // D6 version of query:
//     db_query(
//       'INSERT INTO {statspro_term} (tid, ncount)
//         SELECT tid, COUNT(*) AS num
//         FROM {term_node}
//         GROUP BY tid'
//     );

    // D7 version of query?
    $query = db_select('taxonomy_term_node')
    ->fields(array('tid'))
    ->groupBy('tid');
    $query->addExpression('COUNT(*)', 'num');
    
    db_insert('statspro_term')
    ->from($query)
    ->execute();

    return db_affected_rows();
  }

  /**
   * Returns the number of new comments created per day since the last run.
   */
  protected function get_data_comment() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(timestamp)) AS dategroup
        FROM {comments}
        WHERE timestamp >= :timestamp
        GROUP BY dategroup', array(':timestamp' => $this->last_run));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Returns the number of total page impressions per day since the last run.
   */
  protected function get_data_pi() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(timestamp)) AS dategroup
        FROM {accesslog}
        WHERE timestamp >= :timestamp
        GROUP BY dategroup', array(':timestamp' => $this->last_run));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Returns the number of page impressions for authenticated users per day
   * since the last run.
   */
  protected function get_data_upi() {
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(timestamp)) AS dategroup
        FROM {accesslog}
        WHERE timestamp >= :timestamp
          AND uid > :uid
        GROUP BY dategroup', array(':timestamp' => $this->last_run, ':uid' => 0));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Returns the number of total warnings per day since the last run.
   */
  protected function get_data_warning() {
    $this->get_watchdog_data(WATCHDOG_WARNING, TRUE);
  }

  /**
   * Returns the number of warnings generated by authenticated users per day
   * since the last run.
   */
  protected function get_data_uwarning() {
    $this->get_watchdog_data(WATCHDOG_WARNING, FALSE);
  }

  /**
   * Returns the number of total errors per day since the last run.
   */
  protected function get_data_error() {
    $this->get_watchdog_data(WATCHDOG_ERROR, TRUE);
  }

  /**
   * Returns the number of errors generated by authenticated users per day
   * since the last run.
   */
  protected function get_data_uerror() {
    $this->get_watchdog_data(WATCHDOG_ERROR, FALSE);
  }

  /**
   * Returns the number of total emergencies per day since the last run.
   */
  protected function get_data_emergency() {
    $this->get_watchdog_data(WATCHDOG_EMERGENCY, TRUE);
  }

  /**
   * Returns the number of emergency generated by authenticated users per day
   * since the last run.
   */
  protected function get_data_uemergency() {
    $this->get_watchdog_data(WATCHDOG_EMERGENCY, FALSE);
  }

  /**
   * Returns the number of total alerts per day since the last run.
   */
  protected function get_data_alert() {
    $this->get_watchdog_data(WATCHDOG_ALERT, TRUE);
  }

  /**
   * Returns the number of alerts generated by authenticated users per day
   * since the last run.
   */
  protected function get_data_ualert() {
    $this->get_watchdog_data(WATCHDOG_ALERT, FALSE);
  }

  /**
   * Returns the number of total critical conditions per day since the last run.
   */
  protected function get_data_critical() {
    $this->get_watchdog_data(WATCHDOG_CRITICAL, TRUE);
  }

  /**
   * Returns the number of critical conditions generated by authenticated users
   * per day since the last run.
   */
  protected function get_data_ucritical() {
    $this->get_watchdog_data(WATCHDOG_CRITICAL, FALSE);
  }

  /**
   * Helper function for all data gathering related to the watchdog table.
   *
   * @param int $watchdog_level
   * @param bool $get_all_users Indicates if all users should be included or
   *   only authenticated ones (uid > 0).
   */
  protected function get_watchdog_data($watchdog_level, $get_all_users) {
    $where_user = $get_all_users ? '' : 'AND uid > 0';
    $result = db_query('SELECT COUNT(*) AS num, DATE(FROM_UNIXTIME(timestamp)) AS dategroup
        FROM {watchdog}
        WHERE severity = :severity
          AND timestamp >= :timestamp
          :where_user
        GROUP BY dategroup', 
      array(':severity' => $watchdog_level, ':timestamp' => $this->last_run, ':where_user' => $where_user));
    foreach ($result as $row) {
      $this->add_day_data($row->dategroup, $row->num);
    }
  }

  /**
   * Save day information to database.
   *
   * @param string  $date
   * @param array   $values
   */
  public function store_day($date, $values) {
    $found = db_query("SELECT COUNT(*)
        FROM {statspro}
        WHERE day = :day", array(':day' => $date))->fetchField();

    // update row
    if ($found) {
      $sql_fields = array();
      $sql_values = array();
      foreach ($this->fields as $field => $desc) {
        $sql_fields[] = $field . ' = ' . $field . ' + %d';
        $sql_values[] = isset($values[$field]) ? (int) $values[$field] : 0;
      }

      // add date to argument array
      $sql_values[] = $date;

      // TODO Please convert this statement to the D7 database API syntax.
      db_query(
        "UPDATE {statspro} SET
          " . implode(', ', $sql_fields) . "
          WHERE day = '%s'",
        $sql_values
      );
    }
    // new row
    else {
      $sql_fields = array('day');  // e.g. array('day', 'nuser', 'auser', 'nnode', 'cnode', 'pi', 'upi', 'error', 'warning')
      $sql_values = array($date);  // e.g. array('2013-01-22', 0, 1, 0, 0, 59, 56, 6, 6)
      // e.g. $field = "nuser", $desc = "User registrations"
      foreach ($this->fields as $field => $desc) {
        $sql_fields[] = $field;
        $sql_values[] = isset($values[$field]) ? (int) $values[$field] : 0;
      }
      $query = db_insert('statspro');
      $query->fields($sql_fields, $sql_values);
      $query->execute();
    }
  }

  public function get_qt_days_per_period($period) {
    if (is_numeric($period)) {
      return $period;
    }
    else {
      switch ($period) {
        case 'today':
        case 'yesterday':
          return 1;
          break;

        case 'week_current':
        case 'week_last':
        case 'week_last2':
          return 7;
          break;

        case 'month_current':
        case 'month_last':
          return 30;
          break;

        case 'month_last3':
          return 90;
          break;

        case 'month_last6':
          return 180;
          break;

        case 'quarter_current':
        case 'quarter_last':
          return 90;
          break;

        case 'year_current':
        case 'year_last':
          return 365;
          break;

        default:
          watchdog('statspro', "Unknown period '@period'.", array('@period' => $period), WATCHDOG_ERROR);
          return FALSE;
          break;
      }
    }
  }
}
