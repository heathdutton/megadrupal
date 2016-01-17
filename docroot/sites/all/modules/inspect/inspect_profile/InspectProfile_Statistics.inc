<?php
/**
 * @file
 * Drupal Inspect Profile Statistics class
 */

//D8:use Drupal\Inspect\Inspect;

/**
 * Performance profiling statistics.
 *
 * @category Drupal
 * @package Developer
 * @subpackage Inspect
 */
class InspectProfile_Statistics {
  /**
   * @var array|NULL
   */
  protected static $profileFilters;

  /**
   * Generates statistical reports for the previous day(s), using none and all filters configured to used by cron.
   *
   * Doesn't make new reports if they already exist.
   *
   * @return void
   */
  public static function cron() {
    // Generate statistics, if not done yet.
    if (($previousDays = variable_get('inspect_profile_statscrondays', 0))) {
      $now = getdate();
      $toTimestamp = mktime(0, 0, 0, $now['mon'], $now['mday'], $now['year']); // This morning.
      $fromTimestamp = $toTimestamp - ($previousDays * 24 * 60 * 60);

      if (($cronProfileFilters = variable_get('inspect_profile_statscronprofilefilters', ''))
        && ($cronProfileFilters = drupal_json_decode($cronProfileFilters))
        && ($profileFilters = self::getProfileFilters())) {

        foreach ($cronProfileFilters as $name) {
          if (array_key_exists($name, $profileFilters)) {
            self::make($fromTimestamp, $toTimestamp, $name);
          }
        }
      }
      else {
        self::make($fromTimestamp, $toTimestamp);
      }
    }
  }

  /**
   * Get or generate statistics for a period of time.
   *
   * Default period is yesterday; from morning to night, plus the setting 'day statistics offset'.
   *
   * If non-empty filter: unfiltered statistics will be created prior to creating filtered statistics.
   * If the period is longer than a day, and divisible by 24 hours: statistics will be created for every day in the range.
   * If the effective to-time is later than now: existing report(s) will be replaced, otherwise an existing report will be returned un-altered.
   *
   *  Options:
   *  - (string) profile_filter: default empty (use all profile names)
   *  - (integer|boolean) offset_hours: default zero (~ add the settings day offset hours), positive number ~ add that many hours, FALSE ~ don't offset
   *
   * @param integer $fromTimestamp
   *  - default: yesterday morning at midnight
   *  - Unix timestamp (seconds)
   *  - 1 (one): today's morning at midnight
   * @param integer $toTimestamp
   *  - default: 24 hours after $fromTimestamp
   *  - Unix timestamp (seconds)
   * @param array|string|integer|NULL $options
   *  - default: NULL
   *  - array: list of options
   *  - string: interprets to profile_filter
   *  - integer: interprets to offset_hours
   *  - FALSE: don't add any offset_hours
   * @return InspectProfile_Statistics|NULL
   *  - NULL on error
   */
  public static function make($fromTimestamp = 0, $toTimestamp = 0, $options = NULL) {
    try {
      if (!$fromTimestamp) { // ~ Yesterday.
        $now = getdate();
        $fromTimestamp = mktime(0, 0, 0, $now['mon'], $now['mday'], $now['year']) - (24 * 60 * 60);
      }
      elseif ($fromTimestamp == 1) { // ~ Today.
        $now = getdate();
        $fromTimestamp = mktime(0, 0, 0, $now['mon'], $now['mday'], $now['year']);
      }
      if (!$toTimestamp) { // ~ 24 hours later.
        $toTimestamp = $fromTimestamp + (24 * 60 * 60);
      }

      $offsetHours = 0;
      $profileFilterName = $profileFilter = '';
      if ($options) {
        switch (gettype($options)) {
          case 'array':
            if (isset($options['profile_filter'])) {
              $profileFilterName = '' . $options['profile_filter'];
            }
            if (array_key_exists('offset_hours', $options)) {
              $offsetHours = $options['offset_hours'];
            }
            break;
          case 'string':
            $profileFilterName = $options;
            break;
          case 'integer':
            $offsetHours = $options;
            break;
          default:
            if ($options === FALSE) {
              $offsetHours = FALSE;
            }
        }
      }
      if (!$offsetHours) {
        $offsetHours = $offsetHours === FALSE ? 0 : variable_get('inspect_profile_statoffsethrs', 0);
      }
      if ($offsetHours) {
        $fromTimestamp += ($offsetHours * 60 * 60);
        $toTimestamp += ($offsetHours * 60 * 60);
      }

      if ($profileFilterName) {
        if (!($profileFilter = self::getProfileFilters($profileFilterName))) {
          Inspect::trace(
            NULL, array('type' => 'inspect_profile', 'severity' => WATCHDOG_ERROR,
            'message' => 'Cannot use profile filter[' . $profileFilterName . '] because '
              . ($profileFilter === NULL ? 'no filters defined' : 'that filter isnt defined'))
          );
          return NULL;
        }
      }

      // Check that we haven't stat'ed that period already.
      if ($toTimestamp >= REQUEST_TIME) {
        // Delete existing stats if to-time is now or later.
        db_delete('inspect_profile_statistics')
          ->condition('from_timestamp', $fromTimestamp)
          ->condition('to_timestamp', $toTimestamp)
          ->condition('profile_filter_name', $profileFilterName)
          ->execute();
      }
      elseif (($data = db_select('inspect_profile_statistics', 'stats')
        ->fields('stats', array('data'))
        ->condition('stats.from_timestamp', $fromTimestamp)
        ->condition('stats.to_timestamp', $toTimestamp)
        ->condition('stats.profile_filter_name', $profileFilterName)
        ->execute()
        ->fetchField())) {
        return unserialize($data);
      }

      // Create new and store it.
      $o = new InspectProfile_Statistics($fromTimestamp, $toTimestamp, $profileFilterName);
      try {
        db_insert('inspect_profile_statistics')
          ->fields(array(
            'from_timestamp' => $fromTimestamp,
            'to_timestamp' => $toTimestamp,
            'profile_filter_name' => $profileFilterName,
            'profile_filter' => $profileFilter,
            'severity_threshold' => $o->severityThreshold,
            'completed' => $o->completed_size,
            'aborted' => $o->aborted_size,
            'failed' => $o->failed_size,
            'data' => serialize($o),
            'created' => time(),
          ))
          ->execute();

        watchdog(
          'inspect_profile',
          t('Created statistical report for the period @from - @to using @profile_filter.'),
          array(
            '@from' => date('Y-m-d H:i:s', $fromTimestamp), '@to' => date('Y-m-d H:i:s', $toTimestamp),
            '@profile_filter' => !$profileFilterName ? t('no profile filter') : t('profile filter \'@name\'', array('@name' => $profileFilterName)),
          ),
          WATCHDOG_INFO
        );
      }
      catch (PDOException $xc) {
        // Test for duplicate key error - things may be a little out of sync, and an attempt to save duplicate statistics should thus be ignored.
        if ((int)$xc->getCode() == 23000) { // Fair chance that it's a duplicate key error, though 23000 may also (MySQL) mean null error.
          Inspect::trace($xc, array('type' => 'inspect_profile', 'severity' => WATCHDOG_NOTICE, 'message' => 'Not an error if duplicate key'));
        }
        else {
          throw $xc;
        }
      }
      return $o;
    }
    catch (Exception $xc) {
      Inspect::trace($xc, array('type' => 'inspect_profile', 'severity' => WATCHDOG_ERROR));
    }
    return NULL;
  }

  /**
   * Push report file to download.
   *
   * @param integer $reportId
   * @return void
   */
  public static function reportDownload($reportId) {
    try {
      $fileName = 'performance-report__'
        . preg_replace('/[^a-z\d_\-]/', '',
          str_replace(array('æ','ø','å','ä','ö','ü', ' '), array('ae','oe','aa','ae','oe','ue', '-'),
            drupal_strtolower(variable_get('site_name'))))
        . '__' . $_SERVER['SERVER_NAME']
        . '__' . $reportId;

      if (!($report = db_select('inspect_profile_statistics', 'stats')
        ->fields('stats') // ~ all fields.
        ->condition('sid', $reportId)
        ->execute()->fetchAssoc())) {
        self::downloadHeaders('txt', $fileName);
        echo (variable_get('inspect_profile_statsfilesnotwin', FALSE) ? '' : "\xEF\xBB\xBF") // UTF-8 byte order marker.
          . t('Performance report @report_id doesnt exist.', array('@report_id' => $reportId));
        exit;
      }
      $fileName .= '__' . date('Ymd', $report['from_timestamp']) . '-' . date('Ymd', $report['to_timestamp'] - 1)
        . '__' . (!$report['profile_filter_name'] ? 'no-profile-filter' :
          preg_replace('/[^a-z\d_\-]/', '',
            str_replace(array('æ','ø','å','ä','ö','ü', ' '), array('ae','oe','aa','ae','oe','ue', '-'),
              drupal_strtolower($report['profile_filter_name']) ) )
        );
      self::downloadHeaders('csv', $fileName);
      $o = unserialize($report['data']);
      $o->printReportCsv($reportId);
    }
    catch (Exception $xc) {
      Inspect::trace($xc, array('type' => 'inspect_profile', 'severity' => WATCHDOG_ERROR));
      echo (variable_get('inspect_profile_statsfilesnotwin', FALSE) ? '' : "\xEF\xBB\xBF") // UTF-8 byte order marker.
        . t('Failed to generate performance report @report_id.', array('@report_id' => $reportId));
    }
    exit;
  }

  /**
   * @param string $fileExtension
   *  - no leading dot
   * @param string $fileName
   * @return void
   */
  protected static function downloadHeaders($fileExtension, $fileName) {
    header('Cache-Control: no-cache, must-revalidate');
    header('Pragma: no-cache');
    header('Expires: Mon, 26 Jan 1997 05:00:00 GMT');
    header('Last-Modified: ' . gmdate('D, d M Y H:i:s') . ' GMT');
    header('Cache-Control: no-store, no-cache, must-revalidate');
    header('Cache-Control: post-check=0, pre-check=0', FALSE);
    header('Pragma: no-cache');
    header('Content-type: text/' . $fileExtension . '; charset=utf-8');
    header('Content-disposition: attachment; filename=' . $fileName . '.' . $fileExtension);
  }

  /**
   * @param string $name
   *  - default: empty (~ get all filters, as array)
   *  - non-empty string: get that filter
   * @return array|string|NULL
   *  - NULL if no filters defined at all
   */
  public static function getProfileFilters($name = '') {
    if (!($filters = self::$profileFilters)) {
      if ($filters === NULL) {
        if (($filters = variable_get('inspect_profile_profilefilters', '')) && ($filters = drupal_json_decode($filters))) {
          self::$profileFilters =& $filters;
        }
      }
      if (!$filters) {
        self::$profileFilters = array(); // Not null, because now we have checked it.
        return NULL;
      }
      self::$profileFilters =& $filters;
    }
    if (!$name) {
      return $filters;
    }
    return isset($filters[$name]) ? $filters[$name] : '';
  }

  /**
   * @var integer $fromTimestampMilli
   */
  public $fromTimestampMilli = 0;
  /**
   * @var integer $toTimestampMilli
   */
  public $toTimestampMilli = 0;

  /**
   * A profile is considered failed when it contains an event of that watchdog severity (or worse).
   *
   * @var integer $severityThreshold
   */
  public $severityThreshold = 0;

  /**
   * @var string $profileFilterName
   */
  public $profileFilterName = '';

  /**
   * @var string $profileFilter
   */
  public $profileFilter = '';

  /**
   * @var integer $completed_size
   */
  public $completed_size = 0;
  /**
   * @var integer $aborted_size
   */
  public $aborted_size = 0;
  /**
   * @var integer $failed_size
   */
  public $failed_size = 0;

  /**
   * Successful profiles that reached their end points (* or same profile name as begin).
   *
   * @var array $completed
   */
  public $completed = array();
  /**
   * Successful profiles that didn't reach their end points (* or same profile name as begin).
   *
   * @var array $aborted
   */
  public $aborted = array();
  /**
   * Profiles that contained event(s) of the threshold severity (or worse).
   *
   * @var array $failed
   */
  public $failed = array();

  /**
   * Generates statistics of one or more periods.
   *
   * If non-empty filter: unfiltered statistics objects will be created prior to creating filtered statistics.
   * If the period is longer than a day, and divisible by 24 hours: statistics objects will be created for every day in the range.
   *
   * @see InspectProfile_Statistics::make()
   * @throws Exception
   *  - and propagates Exception
   * @param integer $fromTimestamp
   *  - seconds
   * @param integer $toTimestamp
   *  - seconds
   * @param string $profileFilterName
   *  - default: empty (~ all profiles)
   */
  public function __construct($fromTimestamp, $toTimestamp, $profileFilterName = '') {

    // Convert to milliseconds, because all times in profiler are in that unit.
    $this->fromTimestampMilli = $fromTimestamp * 1000;
    $this->toTimestampMilli = $toTimestamp * 1000;
    $this->severityThreshold = variable_get('inspect_profile_statseveritythreshold', WATCHDOG_ERROR);

    $this->profileFilterName = $profileFilterName;
    $profileFilter = '';
    $filterProfileNames = NULL;
    $exclude = FALSE;
    if ($profileFilterName) {
      if (!($profileFilter = self::getProfileFilters($profileFilterName))) {
        throw new Exception('Cannot use profile filter[' . $profileFilterName . '] because '
          . ($profileFilter === NULL ? 'no filters defined' : 'that filter isnt defined'));
      }
      $this->profileFilter = $s = $profileFilter;
      if ($s{0} === '!') {
        $exclude = TRUE;
        $s = substr($s, 1);
      }
      $filterProfileNames = explode('|', $s);
    }

    // If more full days: get/make individual day statistics.
    $dayAndNight = 24 * 60 * 60;
    $nDays = ($toTimestamp - $fromTimestamp) / $dayAndNight;
    if ($nDays % 1 == 0) {
      if ($nDays > 1) {

        // First make un-filtered accumulate stats.
        $periods = array();
        $from = $fromTimestamp;
        for ($i = 0; $i < $nDays; ++$i) {
          $periods[] = self::make($from, $from += $dayAndNight, FALSE); // ~ don't add offset again, and don't filter.
        }
        $this->accumulateStats($periods);
        $this->aggregateEvents();
        try {
          db_insert('inspect_profile_statistics')
            ->fields(array(
              'from_timestamp' => $fromTimestamp,
              'to_timestamp' => $toTimestamp,
              'profile_filter_name' => '',
              'profile_filter' => '',
              'severity_threshold' => $this->severityThreshold,
              'completed' => $this->completed_size,
              'aborted' => $this->aborted_size,
              'failed' => $this->failed_size,
              'data' => serialize($this),
              'created' => time(),
            ))
            ->execute();
          watchdog(
            'inspect_profile',
            t('Created statistical report for the period @from - @to using no profile filter.'),
            array(
              '@from' => date('Y-m-d H:i:s', $fromTimestamp), '@to' => date('Y-m-d H:i:s', $toTimestamp),
            ),
            WATCHDOG_INFO
          );
        }
        catch (PDOException $xc) {
          // Test for duplicate key error - things may be a little out of sync, and an attempt to save duplicate statistics should thus be ignored.
          if ((int)$xc->getCode() == 23000) { // Fair chance that it's a duplicate key error, though 23000 may also (MySQL) mean null error.
            Inspect::trace($xc, array('type' => 'inspect_profile', 'severity' => WATCHDOG_NOTICE, 'message' => 'Not an error if duplicate key'));
          }
          else {
            throw $xc;
          }
        }

        // Then make filtered accumulate stats.
        $this->completed = array();
        $this->aborted = array();
        $this->failed = array();
        $this->completed_size = 0;
        $this->aborted_size = 0;
        $this->failed_size = 0;

        $periods = array();
        $from = $fromTimestamp;
        for ($i = 0; $i < $nDays; ++$i) {
          $periods[] = self::make($from, $from += $dayAndNight, array('offset_hours' => FALSE, 'profile_filter' => $profileFilterName));
        }
        $this->accumulateStats($periods);
        $this->filterProfiles($filterProfileNames, $exclude);
        $this->aggregateEvents();
        return;
      }
      elseif ($profileFilter) {
        // Create unfiltered period stats.
        $period = self::make($fromTimestamp, $toTimestamp, FALSE); // ~ don't add offset again, and don't filter.
        // Clone to self.
        foreach ($period as $prop => $val) {
          $this->{$prop} = $val;
        }
        // But preserve some properties.
        $this->fromTimestampMilli = $fromTimestamp * 1000;
        $this->toTimestampMilli = $toTimestamp * 1000;
        $this->profileFilterName = $profileFilterName;
        $this->profileFilter = $profileFilter;
        // Filter this.
        $this->filterProfiles($filterProfileNames, $exclude);
        // Create aggregated events list (events across, not caring for, profiles).
        $this->aggregateEvents();
        return;
      }
    }
    $this->buildFromLog();
    $this->aggregateEvents();
  }

  /**
   * Builds statistics object by reading and analysing data directly from the inspect_profile table.
   *
   * @return void
   */
  protected function buildFromLog() {
    $completed = NULL;
    $aborted = NULL;
    $failed = NULL;

    // List profiles having erring event(s); we expect these to be the least of all profiles.
    // SELECT DISTINCT profile_id
    // FROM inspect_profile
    // WHERE type = 'event'
    // AND severity > 0
    // AND timestamp_milli >= 1380492000000  AND timestamp_milli < 1380578400000
    $failedIds = db_select('inspect_profile', 'p')
      ->fields('p', array('profile_id'))
      ->distinct()
      ->condition('type', 'event')
      ->condition('severity', 0, '>')
      ->condition('severity', $this->severityThreshold, '<=')
      ->condition('timestamp_milli', $this->fromTimestampMilli, '>=')->condition('timestamp_milli', $this->toTimestampMilli, '<')
      ->execute()->fetchCol();
    // Find their profile names, and remove profiles that didn't start within the period (looking for profile_begin, not event).
    if ($failedIds) {
      $failed = db_select('inspect_profile', 'p')
        ->fields('p', array('profile_id', 'name'))
        ->distinct()
        ->condition('type', 'profile_begin')
        ->condition('timestamp_milli', $this->fromTimestampMilli, '>=')->condition('timestamp_milli', $this->toTimestampMilli, '<')
        ->condition('profile_id', $failedIds, 'IN')
        ->execute()->fetchAllKeyed();
      $this->failed_size = count($failed);
      $failedIds = array_keys($failed);
    }

    // List profiles having no error(s).
    $query = db_select('inspect_profile', 'p')
      ->fields('p', array('profile_id', 'name'))
      //->distinct()
      ->condition('type', 'profile_begin')
      ->condition('timestamp_milli', $this->fromTimestampMilli, '>=')->condition('timestamp_milli', $this->toTimestampMilli, '<');
    if ($failedIds) {
      $query->condition('profile_id', $failedIds, 'NOT IN');
    }
    $succeeded = $query->execute()->fetchAllKeyed();

    // List successful profiles completed.
    if ($succeeded) {
      $succeededIds = array_keys($succeeded);
      // SELECT DISTINCT p_begin.profile_id, p_being.name
      // FROM inspect_profile AS p_begin
      // LEFT JOIN inspect_profile AS p_end ON (p_end.profile_id = p_begin.profile_id AND p_end.name = p_begin.name)
      // WHERE p_begin.type = 'profile_begin'
      // AND p_begin.timestamp_milli >= 1380492000000  AND p_begin.timestamp_milli < 1380578400000
      // AND p_begin.profile_id NOT IN('1I4ThmuCUo5ZmyI', ...)
      // AND p_end.type = 'profile_end'
      $query = db_select('inspect_profile', 'p_begin')
        ->fields('p_begin', array('profile_id', 'name'))
        ->distinct();
      $query->leftJoin('inspect_profile', 'p_end', 'p_end.profile_id = p_begin.profile_id');
      $completed = $query
        ->condition('p_begin.type', 'profile_begin')
        ->condition('p_begin.profile_id', $succeededIds, 'IN')
        ->condition('p_end.type', 'profile_end')
        ->execute()
        ->fetchAllKeyed();

      // List non-erring profiles aborted (~ without profile_end).
      if (($this->completed_size = count($completed)) != count($succeededIds)) {
        $aborted = $succeeded; // Copy.
        foreach ($succeededIds as $profileId) {
          if (array_key_exists($profileId, $completed)) {
            unset($aborted[$profileId]);
          }
        }
        $this->aborted_size = count($aborted);
      }
    }

    if ($completed) {
      $this->completed['profiles'] = $this->eventsFromLog($completed);
    }
    if ($aborted) {
      $this->aborted['profiles'] = $this->eventsFromLog($aborted);
    }
    if ($failed) {
      $this->failed['profiles'] = $this->eventsFromLog($failed, TRUE);
    }
  }

  /**
   * Sorts profiles by frequency, and stats their events (and optionally their errors).
   *
   * @param array $profiles
   * @param boolean $errors
   *  - default: FALSE (~ don't stat error codes.
   * @return array
   */
  protected function eventsFromLog(array $profiles, $errors = FALSE) {
    // Count instances of each profile name, and sort array by the count (DESC) and then by key (ASC).
    $instances = array_count_values($profiles);
    //ksort($event_count); arsort($event_count); ...doesnt work. And that array_multisort() is utterly obscure, but works.
    array_multisort(array_values($instances), SORT_DESC, array_keys($instances), SORT_ASC, $instances);

    // Find profile ids of each profile name.
    $idsByName = array_fill_keys(array_keys($instances), array());
    foreach ($profiles as $id => $name) {
      $idsByName[$name][] = $id;
    }

    // Find and stat events of each profile.
    foreach ($instances as $profileName => &$value) {
      // SELECT name, SUM(duration) AS total, MAX(duration) AS longest, MIN(duration) AS shortest, count(*) AS quantity, AVG(duration) AS mean,
      // 1000/count(*) AS frequency, SUM(duration)/1000 AS relative_mean
      // FROM inspect_profile
      // WHERE profile_id IN('1I4ThmuCUo5ZmyI', ...)
      //   AND type = 'event'
      // GROUP BY name  ORDER BY total DESC
      $result = db_query(
        'SELECT name, SUM(duration) AS total, MAX(duration) AS longest, MIN(duration) AS shortest, count(*) AS quantity, AVG(duration) AS mean,'
        . 'count(*)/' . $value . ' AS frequency, SUM(duration)/' . $value . ' AS relative_mean'
        . ' FROM {inspect_profile} WHERE profile_id IN(:ids) AND type = \'event\' GROUP BY name ORDER BY total DESC',
        array(':ids' => $idsByName[$profileName]),
        array('fetch' => PDO::FETCH_ASSOC)
      );
      $events = array();
      while(($row = $result->fetchAssoc())) {
        $events[ $row['name'] ] = array(
          'total' => (float)$row['total'],
          'longest' => (float)$row['longest'],
          'shortest' => (float)$row['shortest'],
          'quantity' => (int)$row['quantity'],
          'mean' => (float)$row['mean'],
          'frequency' => (float)$row['frequency'],
          'relative_mean' => (float)$row['relative_mean'],
        );
      }

      // Overwrite $value (by reference) with an array.
      $value = array(
        'quantity' => $value,
        'events' => $events,
      );

      // Stat error codes by frequency, finding first event having an error code (for each event).
      if ($errors) {
        // SELECT profile_id, error_code
        // FROM inspect_profile
        // WHERE profile_id IN('1I4ThmuCUo5ZmyI', ...)
        // AND type = 'event'
        // AND error_code > 0
        // ORDER BY profile_id, since_profile_start
        $result = db_query(
          'SELECT profile_id, error_code FROM {inspect_profile}'
          . ' WHERE profile_id IN(:ids) AND type = \'event\' AND error_code > 0 ORDER BY profile_id ASC, since_profile_start ASC',
          array(':ids' => $idsByName[$profileName]),
          array('fetch' => PDO::FETCH_ASSOC)
        );
        $error_codes = array();
        $last_profileId = '';
        while(($row = $result->fetchAssoc())) {
          if ($row['profile_id'] != $last_profileId) {
            $last_profileId = $row['profile_id'];
            if (isset($error_codes[ $row['error_code'] ])) {
              ++$error_codes[ $row['error_code'] ];
            }
            else {
              $error_codes[ $row['error_code'] ] = 1;
            }
          }
        }
        arsort($error_codes);

        $value['errors'] = $error_codes;
      }
    }
    unset($value);

    return $instances;
  }

  /**
   * Merges data of more InspectProfile_Statistics objects.
   *
   * @param array $stats
   * @return void
   */
  protected function accumulateStats(array $stats) {
    foreach ($stats as $o) {
      $this->completed_size += $o->completed_size;
      $this->aborted_size += $o->aborted_size;
      $this->failed_size += $o->failed_size;

      $profilesStatuses = array('completed', 'aborted', 'failed');
      foreach ($profilesStatuses as $status) {
        if ($o->{$status}) {
          $those_profiles =& $o->{$status}['profiles'];
          if (!$this->{$status}) {
            $this->{$status}['profiles'] = array();
          }
          $profiles =& $this->{$status}['profiles'];

          foreach ($those_profiles as $profileName => $profileProps) {
            if (!array_key_exists($profileName, $profiles)) {
              $profiles[$profileName] = $profileProps;
            }
            else {
              $profiles[$profileName]['quantity'] += $profileProps['quantity'];

              $those_events =& $profileProps['events'];
              $events =& $profiles[$profileName]['events'];
              foreach ($those_events as $eventName => $eventProps) {
                if (!array_key_exists($eventName, $events)) {
                  $events[$eventName] = $eventProps;
                }
                else {
                  $accu =& $events[$eventName];
                  $accu['total'] += $eventProps['total'];
                  if ($eventProps['longest'] > $accu['longest']) {
                    $accu['longest'] = $eventProps['longest'];
                  }
                  if ($eventProps['shortest'] < $accu['shortest']) {
                    $accu['shortest'] = $eventProps['shortest'];
                  }
                  $accu['quantity'] += $eventProps['quantity'];
                  unset($accu);
                }
              }
              unset($those_events, $events);

              if ($status == 'failed') {
                $those_errors =& $profileProps['errors'];
                $errors =& $profiles[$profileName]['errors'];
                foreach ($those_errors as $errorCode => $errorQuantity) {
                  if (!isset($errors[$errorCode])) {
                    $errors[$errorCode] = $errorQuantity;
                  }
                  else {
                    $errors[$errorCode] += $errorQuantity;
                  }
                }
                arsort($errors);
                unset($those_errors, $errors);
              }
            }
          }

          // Sort by profile quantity (DESC) and then by profile name (ASC).
          $profileNames = array();
          $profileQuantities = array();
          foreach ($profiles as $profileName => $profileProps) {
            $profileNames[] = $profileName;
            $profileQuantities[] = $profileProps['quantity'];
          }
          array_multisort($profileQuantities, SORT_DESC, $profileNames, SORT_ASC, $profiles);

          unset($those_profiles, $profiles);
        }
      }
    }
  }

  /**
   * Removes profiles.
   *
   * @param array $filterNames
   * @param boolean $exclude
   *  - default: FALSE
   */
  protected function filterProfiles($filterNames, $exclude = FALSE) {
    $profilesStatuses = array('completed', 'aborted', 'failed');
    foreach ($profilesStatuses as $status) {
      if ($this->{$status}) {
        $profiles =& $this->{$status}['profiles'];
        $subtractSize = 0;
        if (!$exclude) {
          $profileNames = array_keys($profiles);
          foreach ($profileNames as $profileName) {
            if (!in_array($profileName, $filterNames)) {
              $subtractSize += $profiles[$profileName]['quantity'];
              unset($profiles[$profileName]);
            }
          }
        }
        else {
          foreach ($filterNames as $profileName) {
            if (array_key_exists($profileName, $profiles)) {
              $subtractSize += $profiles[$profileName]['quantity'];
              unset($profiles[$profileName]);
            }
          }
        }
        $this->{$status . '_size'} -= $subtractSize;
        unset($profiles);
      }
    }
  }

  /**
   * Combines events of all (filtered) profiles.
   *
   * @return void
   */
  protected function aggregateEvents() {
    $profilesStatuses = array('completed', 'aborted', 'failed');
    foreach ($profilesStatuses as $status) {
      if ($this->{$status}) {
        $size = $this->{$status . '_size'};
        $profiles =& $this->{$status}['profiles'];

        // If only one profile, we simply copy it.
        if ($size == 1) {
          $profile = reset($profiles);
          $this->{$status}['events_aggregated'] = $profile['events'];
          return;
        }

        $eventsAggregated = array();
        $errorsAggregated = array();
        foreach ($profiles as $profileProps) {
          foreach ($profileProps['events'] as $eventName => $eventProps) {
            if (!array_key_exists($eventName, $eventsAggregated)) {
              $eventsAggregated[$eventName] = $eventProps;
              unset($eventsAggregated[$eventName]['frequency'], $eventsAggregated[$eventName]['relative_mean']);
            }
            else {
              $aggr =& $eventsAggregated[$eventName];
              $aggr['total'] += $eventProps['total'];
              if ($eventProps['longest'] > $aggr['longest']) {
                $aggr['longest'] = $eventProps['longest'];
              }
              if ($eventProps['shortest'] < $aggr['shortest']) {
                $aggr['shortest'] = $eventProps['shortest'];
              }
              $aggr['quantity'] += $eventProps['quantity'];
              unset($aggr);
            }
          }
          if ($status == 'failed') {
            foreach ($profileProps['errors'] as $errorCode => $errorQuantity) {
              if (!isset($errorsAggregated[$errorCode])) {
                $errorsAggregated[$errorCode] = $errorQuantity;
              }
              else {
                $errorsAggregated[$errorCode] += $errorQuantity;
              }
            }
          }
        }
        unset($profiles);

        $totals = array();
        foreach ($eventsAggregated as $eventName => &$eventProps) {
          $totals[$eventName] = $eventProps['total'];
          $eventProps['mean'] = $eventProps['total'] / $eventProps['quantity'];
          $eventProps['frequency'] = $eventProps['quantity'] / $size;
          $eventProps['relative_mean'] = $eventProps['total'] / $size;
        }
        unset($eventProps);
        arsort($totals);
        $aggregationSorted = array();
        $eventNames = array_keys($totals);
        foreach ($eventNames as $eventName) {
          $aggregationSorted[$eventName] = $eventsAggregated[$eventName];
        }
        unset($eventsAggregated);
        $this->{$status}['events_aggregated'] = $aggregationSorted;

        if ($status == 'failed') {
          arsort($errorsAggregated);
          $this->{$status}['errors_aggregated'] = $errorsAggregated;
        }
      }
    }
  }

  /**
   * Prints (echo) semicolon-separated report.
   *
   * @param integer $reportId
   *  - default: zero
   * @return void
   */
  public function printReportCsv($reportId = 0) {
    $windowsCompatible = !variable_get('inspect_profile_statsfilesnotwin', FALSE);
    if ($windowsCompatible) {
      echo "\xEF\xBB\xBF"; // UTF-8 byte order marker
    }
    $sep = ';';
    $dlm = '"';
    $d_d = $dlm . $sep . $dlm;
    $nl = $windowsCompatible ? "\r\n" : "\n";

    if ($reportId) {
      echo $dlm . 'Report id' . $d_d . $reportId . $dlm . $nl;
    }
    echo $dlm . variable_get('site_name') . $dlm . $nl
      . $dlm . $_SERVER['SERVER_NAME'] . $dlm . $nl
      . $dlm . date('Y-m-d H:i:s' , (int)floor($this->fromTimestampMilli / 1000)) . $dlm . $nl
      . $dlm . date('Y-m-d H:i:s' , (int)floor($this->toTimestampMilli / 1000) - 1) . $dlm . $nl
      . $dlm . $this->profileFilterName . $dlm . $nl
      . $dlm . ' ' . $dlm . $nl
      . $dlm . 'Profiles completed' . $d_d . $this->completed_size . $dlm . $nl
      . $dlm . 'Profiles aborted' . $d_d . $this->aborted_size . $dlm . $nl
      . $dlm . 'Profiles failed' . $d_d . $this->failed_size . $dlm . $nl;

    $profilesStatuses = array('completed', 'aborted', 'failed');
    foreach ($profilesStatuses as $status) {
      if ($this->{$status}) {
        // Profiles.
        echo $dlm . ' ' . $dlm . $nl
          . $dlm . ' ' . $dlm . $nl
          . $dlm . ucfirst($status) . ' profiles' . $d_d . 'Event name'
          . $d_d . 'Total' . $d_d . 'Longest' . $d_d . 'Shortest' . $d_d . 'Quantity' . $d_d . 'Mean' . $d_d . 'Frequency' . $d_d . 'Relative mean'
          . $dlm . $nl;

        if (!empty($this->{$status}['profiles']) && ($profiles =& $this->{$status}['profiles'])) {
          foreach ($profiles as $profileName => $profileProps) {
            $prefix = $dlm . $profileName . $d_d;
            echo $prefix . ' ' . $d_d . ' ' . $d_d . ' ' . $d_d . ' ' . $d_d . $profileProps['quantity'] . $dlm . $nl;
            $events =& $profileProps['events'];
            foreach ($events as $eventName => $eventProps) {
              echo $prefix . $eventName
                . $d_d . round($eventProps['total'], 4)
                . $d_d . round($eventProps['longest'], 4)
                . $d_d . round($eventProps['shortest'], 4)
                . $d_d . round($eventProps['quantity'], 4)
                . $d_d . round($eventProps['mean'], 4)
                . $d_d . round($eventProps['frequency'], 4)
                . $d_d . round($eventProps['relative_mean'], 4) . $dlm . $nl;
            }
            unset($events);
          }
        }
        unset($profiles);

        // Events aggregated.
        echo $dlm . ' ' . $dlm . $nl
          . $dlm . ucfirst($status) . ' - events aggregated'  . $d_d . 'Event name'
          . $d_d . 'Total' . $d_d . 'Longest' . $d_d . 'Shortest' . $d_d . 'Quantity' . $d_d . 'Mean' . $d_d . 'Frequency' . $d_d . 'Relative mean' . $dlm . $nl;

        $prefix = $dlm . ' ' . $d_d;
        if (!empty($this->{$status}['events_aggregated']) && ($events =& $this->{$status}['events_aggregated'])) {
          foreach ($events as $eventName => $eventProps) {
            echo $prefix . $eventName
              . $d_d . round($eventProps['total'], 4)
              . $d_d . round($eventProps['longest'], 4)
              . $d_d . round($eventProps['shortest'], 4)
              . $d_d . round($eventProps['quantity'], 4)
              . $d_d . round($eventProps['mean'], 4)
              . $d_d . round($eventProps['frequency'], 4)
              . $d_d . round($eventProps['relative_mean'], 4) . $dlm . $nl;
          }
        }
        unset($events);

        // Errors.
        if ($status == 'failed') {
          $translator = '';
          $knownErrors = array();
          if (($func = variable_get('inspect_profile_statserrortranslator', '')) && function_exists($func)) {
            $translator = $func;
          }
          $noErrorInfo = array('module' => '', 'name' => '');

          echo $dlm . ' ' . $dlm . $nl
            . $dlm . 'Errors in profiles' . $d_d . 'Error name' . $d_d . 'Module' . $d_d . 'Error code' . $d_d . 'Quantity' . $dlm . $nl;

          if (!empty($this->{$status}['profiles']) && ($profiles =& $this->{$status}['profiles'])) {
            foreach ($profiles as $profileName => $profileProps) {
              $prefix = $dlm . $profileName . $d_d;
              $errors =& $profileProps['errors'];
              foreach ($errors as $errorCode => $errorQuantity) {
                if ($translator) {
                  if (isset($knownErrors[$errorCode])) {
                    $errorInfo = $knownErrors[$errorCode];
                  }
                  elseif (is_array($errorInfo = $translator($errorCode))) {
                    $knownErrors[$errorCode] = $errorInfo;
                  }
                  else {
                    $errorInfo = $noErrorInfo;
                  }
                }
                else {
                  $errorInfo = $noErrorInfo;
                }
                echo $prefix
                  . $errorInfo['name'] . $d_d . $errorInfo['module'] . $d_d . $errorCode . $d_d . $errorQuantity . $dlm . $nl;
              }
              unset($errors);
            }
          }
          unset($profiles);

          // Errors aggregated.
          echo $dlm . ' ' . $dlm . $nl
            . $dlm . 'Errors aggregated' . $d_d . 'Error name' . $d_d . 'Module' . $d_d . 'Error code' . $d_d . 'Quantity' . $dlm . $nl;
          if (!empty($this->{$status}['errors_aggregated'])) {
            $prefix = $dlm . ' ' . $d_d;
            $errors =& $this->{$status}['errors_aggregated'];
            foreach ($errors as $errorCode => $errorQuantity) {
              if ($translator) {
                if (isset($knownErrors[$errorCode])) {
                  $errorInfo = $knownErrors[$errorCode];
                }
                elseif (is_array($errorInfo = $translator($errorCode))) {
                  $knownErrors[$errorCode] = $errorInfo;
                }
                else {
                  $errorInfo = $noErrorInfo;
                }
              }
              else {
                $errorInfo = $noErrorInfo;
              }
              echo $prefix
                . $errorInfo['name'] . $d_d . $errorInfo['module'] . $d_d . $errorCode . $d_d . $errorQuantity . $dlm . $nl;
            }
            unset($errors);
          }
        }
      }
    }
  }
}
