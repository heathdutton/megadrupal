<?php
/**
 * @file
 * Drupal Inspect Profile class
 */

//D8:use Drupal\Inspect\Inspect;

/**
 * Execution time profiler.
 *
 * @static
 * @singleton
 * @category Drupal
 * @package Developer
 * @subpackage Inspect
 */
class InspectProfile {
  /**
   * Instance to enable filing stats at object destruction.
   *
   * Boolean FALSE, if user not allowed to profile.
   *
   * @type InspectProfile|boolean|NULL
   */
  protected static $o;

  /**
   * We only instantiate to get the __destruct() method, which makes it possible to do stuff at real shutdown.
   *
   * @return InspectProfile
   */
  protected function __construct() {}

  /**
   * Files simple event statistics.
   *
   * Files to directory private://module/inspect/profiles.
   *
   * To limit the number of files, and still avoid concurrent writing, appends to file named [date]_user_[user id];
   * plus, for anonymous user only, _[inspect session number].
   *
   * @return void
   */
  public function __destruct() {
    if (self::$events) {
      file_put_contents(self::$profileDir . self::$profileFile, self::statSimple('file'), FILE_APPEND);
    }
  }

  /**
   * Duration marker for event interval that hasn't been stopped.
   *
   * @type integer
   */
  const INTERVAL_UNSTOPPED = -1;

  /**
   * Duration marker for event interval that hadn't been started.
   *
   * @type integer
   */
  const INTERVAL_UNSTARTED = -2;

  /**
   * @var boolean|NULL $init
   */
  protected static $init;

  /**
   * @var integer $_uid
   */
  protected static $uid = 0;

  /**
   * @var string $_profileDir
   */
  protected static $profileDir;

  /**
   * Filename.
   *
   * @var string $_profileFile
   */
  protected static $profileFile;

  /**
   * @var float $_ownTime_event
   */
  protected static $ownTime_event = 0;

  /**
   * @var float $_ownTime_total
   */
  protected static $ownTime_total = 0;

  /**
   * @var array $_events
   */
  protected static $events = array();

  /**
   * @var array $_started
   */
  protected static $started = array();

  /**
   * @var array $_durations
   */
  protected static $durations = array();

  /**
   * @var array $_names;
   */
  protected static $names = array();

  /**
   * Highest recorded severity in request.
   *
   * @var integer $_severity;
   */
  protected static $severity = 0;

  /**
   * Current multi-request profile, if any.
   *
   *  Buckets:
   *  - 0: id
   *  - 1: name
   *  - 2: timestamp (in millisecs) of beginning
   *
   * @var array|NULL $_profileCurrent;
   */
  protected static $profileCurrent;

  /**
   * End current multi-request profile at shutdown.
   *
   * @var array|NULL $_profileEndAtShutdown;
   */
  protected static $profileEndAtShutdown;

  /**
   * @var boolean|NULL
   */
  protected static $logRequestShutdown;


  /**
   * @return boolean
   *  - FALSE if no permission
   */
  protected static function init() {
    if (self::$init) {
      return TRUE;
    }
    if (self::$init === FALSE) {
      return FALSE;
    }

    // Not permitted, or set to file at shutdown but cannot write files.
    if (!user_access('inspect profile')) {
      return (self::$init = FALSE);
    }

    // If filing simple (non-profile) events at shutdown.
    if (variable_get('inspect_profile_statsimplefileshutdown', FALSE)) {
      // Ensure the dir for filing.
      if (!(self::$profileDir = Inspect::ensureDirectory('profiles'))) {
        return (self::$init = FALSE);
      }

      // Instantiate, to get __destruct().
      self::$o = new InspectProfile();

      // $GLOBALS['user'] is not available at time of __destruct().
      self::$uid = $uid = $GLOBALS['user']->uid;

      // Make file name.
      self::$profileFile = '/profile_'
        . date('Ymd', REQUEST_TIME)
        . '_user_' . $uid
        . ($uid ? '' : ('_' . Inspect::sessionCounters('session'))) // For anonymous user: avoid filing concurrency by using the session number.
        . '.log';
    }

    //  Measure own time, per event.
    self::$init = TRUE; // Do flag successful initialisation first, otherwise perpetual loop!
    self::event('a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event('a', 'a');
    self::event(NULL, 'a');
    $a = self::$durations['a'];
    sort($a);
    array_shift($a);
    array_shift($a);
    array_pop($a);
    array_pop($a);
    self::$ownTime_event = array_sum($a) / 24; // Should really be 16, but then own time seem to be to long
    self::$events = self::$started = self::$durations = array();

    // Write profile event records to database at shutdown.
    drupal_register_shutdown_function('_inspect_profile_shutdown');

    return TRUE;
  }

  /**
   * Register event interval start and/or stop.
   * 
   *  Stop $options (all optional):
   *  - (boolean) if_started: default FALSE, also supported as array bucket value, 'if_started'
   *  - (string) position: default empty
   *  - (integer) severity: default zero
   *  - (number|string) error_code: default empty
   *  - (string) log: default empty
   *
   * Example, profiling in a loop:
   * @code
   * $le = 10;
   * InspectProfile::event('loop'); // Start first interval.
   * for ($i = 0; $i < $le; $i++) {
   *   $pow = pow($i, $i);
   *   InspectProfile::event(
   *     $i < $le - 1 ? 'loop' : '', // Start new interval, except for last iteration.
   *     'loop', // Do always stop current interval.
   *     array('log' => $i . ' power ' . $i . ': ' . $pow)
   *   );
   * }
   * @endcode
   * @param string|NULL $start_name
   *  - default: NULL (~ don't start)
   * @param string|NULL $stop_name
   *  - default: NULL (~ don't stop)
   * @param string|array|NULL $options
   *  - default: NULL
   *  - string: interprets to position
   *  - array: list of options, if start+stop the options only count for the stopping event
   *  - array (stop options): (optional:boolean) if_started, (optional:integer) severity, (optional:string) log
   *  - array (start options): (optional:string) log
   * @return boolean|NULL
   *   - NULL if no permission
   *   - FALSE on error; $start_name and $stop_name both empty
   */
  public static function event($start_name = NULL, $stop_name = NULL, $options = NULL) {
    $t = microtime(TRUE) * 1000;
    //  check init
    $init = FALSE;
    if (!self::$init && (self::$init === FALSE || !($init = self::init()))) {
      return NULL;
    }
    if ($init) {
      $t = microtime(TRUE) * 1000; // Reset to compensate for initialization.
    }
    $rtrn = FALSE;

    if (!$start_name && !$stop_name) {
      return FALSE;
    }

    $position = $startLog = $stopLog = $error_code = '';
    $severity = $ifStarted = 0;

    if ($options) {
      if (is_array($options)) {
        // The 'position' options is the same for start and stop.
        if (!empty($options['position'])) {
          $position = $options['position'];
        }
        // Stop's optionals, and if stop then a 'log' option is for stop.
        if ($stop_name) {
          if (!empty($options['if_started']) || in_array('if_started', $options, TRUE)) {
            $ifStarted = TRUE;
          }
          if (!empty($options['severity'])) {
            $severity = $options['severity'];
            // Do only look for error_code and error_category if there's a severity.
            if (!empty($options['error_code'])) {
              $error_code = $options['error_code'];
            }
          }
          if (!empty($options['log'])) {
            $stopLog = $options['log'];
          }
        }
        // Start may have 'log' option, if no stop.
        elseif ($start_name && !empty($options['log'])) {
          $startLog = $options['log'];
        }
      }
      else {
        $position = $options;
      }
    }

    //  Get own-time, and add to total own-time.
    self::$ownTime_total += ($tP = self::$ownTime_event);

    $ss =& self::$started;
    if ($stop_name && $stop_name != $start_name) {
      $rtrn = TRUE;
      if (isset($ss[$stop_name])) {
        //$tDur = $t - $ss[$stop_name] - $tP;
        if (($tDur = $t - $ss[$stop_name]) > $tP) { // More than own time, subtract that.
          $tDur -= $tP;
        }
        elseif ($tDur > $tP / 2) { // More than half of own time, subtract that.
          $tDur -= $tP / 2;
        }
        //  Register interval duration, for total and average calculation.
        if (isset(self::$durations[$stop_name])) {
          self::$durations[$stop_name][] = $tDur;
        }
        else {
          self::$durations[$stop_name] = array(
            $tDur
          );
        }
        unset($ss[$stop_name]);
        // Modify overall severity recorded (in request)?
        if ($severity && $severity > self::$severity) {
          self::$severity = $severity;
        }
      }
      elseif ($ifStarted) {
        return TRUE;
      }
      else { // Error, hadnt been started previously, and no ignore flag.
        self::$names[$stop_name] = drupal_strlen($stop_name);
        $tDur = self::INTERVAL_UNSTARTED;
        // Modify overall severity recorded (in request)?
        if ($severity && $severity > self::$severity) {
          self::$severity = $severity;
        }
      }
      self::$events[] = array(
        $stop_name,
        $t, // time
        $tDur, // interval duration
        $severity,
        $error_code,
        drupal_substr($stopLog, 0, 255),
        $position,
      );
    }
    if ($start_name) {
      $rtrn = TRUE;
      //  if currently already started: stop current interval
      if (isset($ss[$start_name])) {
        if (($tDur = $t - $ss[$start_name]) > $tP) { // More than own time, subtract that.
          $tDur -= $tP;
        }
        elseif ($tDur > $tP / 2) { // More than half of own time, subtract that.
          $tDur -= $tP / 2;
        }
        //  Register interval duration, for total and average computation.
        if (isset(self::$durations[$start_name])) {
          self::$durations[$start_name][] = $tDur;
        }
        else {
          self::$durations[$start_name] = array(
              $tDur
          );
        }
        self::$events[] = array(
          $start_name,
          $t, // Time.
          $tDur, // Interval duration.
          $severity,
          $error_code,
          drupal_substr($stopLog, 0, 255),
          $position,
        );
      }
      else {
        self::$names[$start_name] = drupal_strlen($start_name);
      }
      $ss[$start_name] = $t;
      self::$events[] = array(
        $start_name,
        $t, // Time.
        self::INTERVAL_UNSTOPPED, // Interval duration.
        0, // Severity.
        '', // Error code.
        drupal_substr($startLog, 0, 255),
        $position,
      );
    }
    return $rtrn;
  }

  /**
   * Register event interval start.
   *
   *  Array value of $options (any number of options):
   *  - (string) position (default empty)
   *  - (string) log, will be truncated to 255 (default empty)
   *
   * Slightly slower than InspectProfile::event(), because involves one more function call.
   *
   * @param string $name
   *  - do not use these chars in names: quote carriage-return newline tab pipe
   * @param string|array|NULL $options
   *  - default: NULL
   *  - string: interprets to position
   *  - array: list of options
   * @return boolean|NULL
   *   - NULL if no permission
   *   - FALSE on error
   */
  public static function start($name, $options = NULL) {
    return self::event($name, NULL, $options);
  }

  /**
   * Register event interval stop.
   *
   *  Array value of $options (any number of options):
   *  - (boolean) if_started: default FALSE, also supported as array bucket value, 'if_started'
   *  - (string) position: default empty
   *  - (integer) severity: default zero
   *  - (number|string) error_code: default empty
   *  - (string) log: default empty
   *
   * Slightly slower than InspectProfile::event(), because involves one more function call.
   *
   * @param string $name
   *  - do not use these chars in names: quote carriage-return newline tab pipe
   * @param string|array|NULL $options
   *  - default: NULL
   *  - string: interprets to position
   *  - array: list of options
   * @return boolean|NULL
   *   - NULL if no permission
   *   - FALSE on error
   */
  public static function stop($name, $options = NULL) {
    return self::event(NULL, $name, $options);
  }

  /**
   * Resolve begin/end options arg.
   *
   * @param mixed $options
   * @param string $type
   *  - default: 'begin'
   * @return array
   */
  protected static function resolveOptions($options, $type = 'begin') {
    $opts = array('log' => '', 'position' => '');
    if (!$options) {
      return $opts;
    }
    if (!is_array($options)) { // ~ string or stringable number.
      $opts['position'] = $options;
      return $opts;
    }
    $opts = $options;
    // Support flags set as value instead of key.
    if ($type == 'begin') {
      if (in_array('if_name_not_begun', $opts, TRUE)) {
        $opts['if_name_not_begun'] = TRUE;
      }
      elseif (in_array('if_none_begun', $opts, TRUE)) {
        $opts['if_none_begun'] = TRUE;
      }
    }
    else {
      if (in_array('at_exit', $opts, TRUE)) {
        $opts['at_exit'] = TRUE;
      }
      elseif (!empty($opts['at_shutdown']) || in_array('at_shutdown', $opts, TRUE)) {
        $opts['at_exit'] = TRUE;
      }
    }
    if (!array_key_exists('position', $opts)) {
      $opts['position'] = '';
    }
    if (!empty($opts['message'])) {
      $opts['log'] = $opts['message'];
    }
    elseif (!array_key_exists('log', $opts)) {
      $opts['log'] = '';
    }
    return $opts;
  }

  /**
   * Begin multi-request profile.
   *
   * Also ends current multi-request profile, if any.
   * Events of multi-request profiles are being logged to the inspect_profile table.
   *
   *  Array value of $options (any number of options):
   *  - (boolean) if_name_not_begun (default falsy, also supported as array bucket value 'if_name_not_begun')
   *  - (boolean) if_none_begun (default falsy, also supported as array bucket value 'if_none_begun')
   *  - (string) position (default empty)
   *  - (string) log, will be truncated to 255 (default empty)
   *  - (string) message, alias of log
   *
   * @param string $name
   *  - '*' is an illegal value
   * @param string|array|NULL $options
   *  - default: NULL
   *  - string: interprets to position
   *  - array: list of options
   * @return void
   */
  public static function beginProfile($name, $options = NULL) {
    $tEnd = microtime(TRUE) * 1000;
    // Init.
    if (!self::$init && (self::$init === FALSE || !self::init())) {
      return;
    }

    $opts = self::resolveOptions($options);

    try {
      if ($name == '*') {
        throw new Exception('Arg $name cannot be star, when begin\'ning (only when end\'ing).');
      }
      $stateExist = module_exists('state');
      // Get previous (currently running) profile, if any.
      if (!($currentProfile = self::$profileCurrent)) {
        if ($stateExist) {
          $currentProfile = State::sessionGet('module', 'inspect_profile');
        }
        elseif (drupal_session_started() && isset($_SESSION['module']['inspect_profile'])) {
          $currentProfile = $_SESSION['module']['inspect_profile'];
        }
      }
      if ($currentProfile) {
        // Get out if any profile running?
        if (!empty($opts['if_none_begun'])) {
          return;
        }
        // Get out if this profile already running?
        if (!empty($opts['if_name_not_begun']) && $name == $currentProfile[1]) {
          return;
        }
        // End current.
        self::end($currentProfile, $tEnd, $opts['log'], $opts['position'], TRUE); // TRUE ~ We're starting a new profile right away.
      }

      // Start new.
      $c = explode('.', uniqid('', TRUE));
      $profileId = Inspect::baseConvert($c[0], 16, 62) . Inspect::baseConvert($c[1], 16, 62);
      $profile = array(
        $profileId,
        $name, // Needed later if '*' ending profile.
        $tStart = microtime(TRUE) * 1000,
      );

      // Set in session, and set fallback in cookie (for orphaned events).
      if ($stateExist) {
        State::sessionSet('module', 'inspect_profile', $profile);
        // For orphaned events.
        State::cookieSet('inspect_profile', $profileId . ':' . $tStart, array('httponly' => TRUE));
      }
      elseif ($currentProfile) { // The key path exists, no reason to set it safely (doing all the checks of the next block).
        $_SESSION['module']['inspect_profile'] = $profile;
        // For orphaned events.
        setcookie('inspect_profile', $profileId . ':' . $tStart, 0, $GLOBALS['base_path'], $GLOBALS['cookie_domain'], $GLOBALS['is_https'], TRUE);
      }
      else {
        // Set session var safely.
        if (!($sessionStarted = drupal_session_started())) {
          drupal_session_start();
        }
        if (!$sessionStarted || !array_key_exists('module', $_SESSION)) {
          $_SESSION['module'] = array(
            'inspect_profile' => $profile
          );
        }
        else {
          $_SESSION['module']['inspect_profile'] = $profile;
        }
        // For orphaned events.
        setcookie('inspect_profile', $profileId . ':' . $tStart, 0, $GLOBALS['base_path'], $GLOBALS['cookie_domain'], $GLOBALS['is_https'], TRUE);
      }

      // Log.
      $sessionCounters = Inspect::sessionCounters();
      db_insert('inspect_profile')
        ->fields(array(
          'uid' => $GLOBALS['user']->uid,
          'session_id' => $sessionCounters['session'],
          'profile_id' => $profileId,
          'type' => 'profile_begin',
          'page_load_no' => $sessionCounters['page_load'],
          'request_no' => $sessionCounters['request'],
          'since_request_start' => $tStart - Inspect::requestTimeMilli(),
          'name' => $name,
          'log_start' => $opts['log'],
          'location' => $GLOBALS['base_root'] . request_uri(),
          'referer' => isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : '',
          'hostname' => substr(ip_address(), 0, 128),
          'position_start' => $opts['position'],
          'timestamp_milli' => round($tStart),
        ))
        ->execute();
    }
    catch (Exception $xc) {
      Inspect::trace($xc);
    }
  }

  /**
   * End multi-request profile.
   *
   * @throws PDOException
   * @param array|NULL $profile
   *  - default: NULL (~ self::$profileCurrent)
   * @param float $tEnd
   *  - default: zero
   * @param string $log
   *  - default: empty
   * @param string $position
   *  - default: empty
   * @param boolean $beforeStartingNew
   *  - default: FALSE (~ we're not starting a new profile right after this)
   * @return void
   */
  protected static function end($profile = NULL, $tEnd = 0.0, $log = '', $position = '', $beforeStartingNew = FALSE) {
    if (!$profile) {
      $profile = self::$profileCurrent;
    }
    if (!$tEnd) {
      $tEnd = microtime(TRUE) * 1000;
    }
    $uid = $GLOBALS['user']->uid;
    $sessionCounters = Inspect::sessionCounters();
    // Log.
    $profileId = $profile[0];
    $profileName = $profile[1];
    $profileStarted = $profile[2];
    $query = db_insert('inspect_profile')
      ->fields(array(
        'uid',
        'session_id',
        'profile_id',
        'type',
        'page_load_no',
        'request_no',
        'since_profile_start',
        'since_request_start',
        'duration',
        'name',
        'log_stop',
        'location',
        'referer',
        'hostname',
        'position_start',
        'position_stop',
        'timestamp_milli',
      ));
    // Log all events.
    if (($nEvents = count($records = self::listEventsForLog($profile)))) {
      for ($i = 0; $i < $nEvents; ++$i) {
        $query->values($records[$i]);
      }
      // Clear event lists. But don't clear severity, because we still want that logged at shutdown.
      self::$events = self::$started = self::$durations = self::$names = array();
    }
    // Log profile_end.
    $query->values(array(
      'uid' => $uid,
      'session_id' => $sessionCounters['session'],
      'profile_id' => $profileId,
      'type' => 'profile_end',
      'page_load_no' => $sessionCounters['page_load'],
      'request_no' => $sessionCounters['request'],
      'since_profile_start' => ($duration = $tEnd - $profileStarted),
      'since_request_start' => $tEnd - Inspect::requestTimeMilli(),
      'duration' => $duration,
      'name' => $profileName,
      'log_stop' => $log,
      // Do only save these three once, to keep query short; only log them here if there was no events.
      'location' => $nEvents ? '' : ($GLOBALS['base_root'] . request_uri()),
      'referer' => $nEvents ? '' : (isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : ''),
      'hostname' => $nEvents ? '' : substr(ip_address(), 0, 128),
      'position_start' => '',
      'position_stop' => $position,
      'timestamp_milli' => round($tEnd),
    ));
    $query->execute();

    // Clear current profile var.
    self::$profileCurrent = NULL;

    // If not starting new profile.
    if (!$beforeStartingNew) {

      // If the ending profile had events in this request: request_shutdown time must be logged at shutdown, for statistics.
      if ($nEvents) {
        // Flag that request_shutdown must be logged, even if there's no profile running at the time of shutdown.
        self::$logRequestShutdown = TRUE;
      }

      // Remove fallback cookie.
      if (module_exists('state')) {
        State::cookieRemove('inspect_profile', array('httponly' => TRUE));
      }
      else {
        setcookie('inspect_profile', '', REQUEST_TIME - 3600, $GLOBALS['base_path'], $GLOBALS['cookie_domain'], $GLOBALS['is_https'], TRUE);
      }
    }
  }

  /**
   * End multi-request profile.
   *
   *  Array value of $options (any number of options):
   *  - (boolean) at_exit (default falsy, also supported as array bucket value 'at_exit')
   *  - (boolean) at_shutdown, alias of at_exit (default falsy)
   *  - (string) position (default empty)
   *  - (string) log, will be truncated to 255 (default empty)
   *  - (string) message, alias of log
   *
   * @param string $name
   *  - default: star (~ wildcard, end any profile)
   *  - non-empty: end profile by that name
   * @param string|array|object|NULL $options
   *  - default: NULL
   *  - string: interprets to position
   *  - array|object: list of options
   * @return void
   */
  public static function endProfile($name = '*', $options = NULL) {
    // NB: A profile will not get an end point if it 'passes' a logout (because current session will be destroyed).
    // Then the mechanism for extracting data must use last request_shutdown as end point.

    $tEnd = microtime(TRUE) * 1000;
    // Init.
    if (!self::$init && (self::$init === FALSE || !self::init())) {
      return;
    }

    $opts = self::resolveOptions($options, 'end');

    // Wait until shutdown?
    if (!empty($opts['at_exit'])) {
      unset($opts['at_exit']);
      $opts['name'] = $name;
      self::$profileEndAtShutdown = $opts;
      return;
    }

    // Deliberately don't get from self::$profileCurrent, because the profile may have been ended by another request in the meantime.
    $currentProfile = NULL;
    if (module_exists('state')) {
      $currentProfile = State::sessionGet('module', 'inspect_profile');
    }
    elseif (drupal_session_started() && isset($_SESSION['module']['inspect_profile'])) {
      $currentProfile = $_SESSION['module']['inspect_profile'];
    }
    if ($currentProfile) {
      // If not wildcard and not current.
      if ($name != '*' && $name != $currentProfile[1]) {
        return;
      }
      // Remove profile from session.
      unset($_SESSION['module']['inspect_profile']);
      // Clean-up.
      if (!$_SESSION['module']) {
        unset($_SESSION['module']);
      }
      self::end($currentProfile, $tEnd, $opts['log'], $opts['position']);
    }
  }


  /**
   * Log single-request profile statistics to watchdog, if permitted and any events have been registered.
   *
   * @param string $message
   *  - default: empty
   * @return boolean|NULL
   *   - NULL if no events or user isnt permitted to log inspections
   *   - FALSE on error
   */
  public static function log($message = '') {
    if (!self::$events || !user_access('inspect log')) {
      return NULL;
    }

    $tagStart = $tagEnd = '';
    if (($u = 'pre')) {
      $tagStart = '<' . $u . ' class="module-inspect-collapsible inspect-profile">';
      $tagEnd = '</' . $u . '>';
    }
    $nl = "\n";
    $message = $tagStart
      . ( empty($message) ? '' : (check_plain($message) . ':' . $nl) )
      . '[Inspect profile - ' . join(':', Inspect::sessionCounters()) . ']' . $nl
      . self::statSimple('log')
      . $tagEnd;

    // Check output length.
    $outputMax = Inspect::outputMax();
    $severity = WATCHDOG_INFO;
    if (strlen($message) > $outputMax) { // Deliberately not drupal_strlen(); need 'physical' length, not (possibly shorter) multibyte length.
      $message = 'Message length [' . strlen($message) // Deliberately not drupal_strlen().
        . '] exceeds max allowed [' . $outputMax . ']';
      $severity = WATCHDOG_WARNING;
      $return = FALSE;
    }
    else {
      $return = TRUE;
    }

    watchdog(
      'inspect profile',
      $message, // Will generate error when extracting strings for translation, but doing replacing just the message doesnt make sense.
      NULL,
      $severity
    );
    return $return;
  }

  /**
   * File single-request profile statistics to watchdog, if permitted and any events have been registered.
   *
   * @param string $message
   *  - default: empty
   * @param boolean $by_user
   *   - default: FALSE (~ logs to common log file)
   *   - truthy: logs to user specific log file
   * @return boolean|NULL
   *  - NULL if user isnt permitted to log inspections
   *  - FALSE: too long message, or failing to file
   */
  public static function file($message = '', $by_user = FALSE) {
    if (!self::$events || (!user_access('inspect log') && !drupal_is_cli())) {
      return NULL;
    }

    $nl = "\n";
    return Inspect::fileMessage(
      (!$message ? '' : ($message . ':' . $nl) ) . self::statSimple('get'),
      'inspect profile',
      WATCHDOG_INFO,
      $by_user
    );
  }

  /**
   * Get single-request profile statistics as string, if permitted and any events have been registered.
   *
   * @param string $message
   *  - default: empty
   * @return string
   *   - empty if no events or user isnt permitted to get inspections, or on error
   */
  public static function get($message = '') {
    if (!self::$events || (!user_access('inspect get') && !drupal_is_cli())) {
      return '';
    }

    $tagStart = $tagEnd = '';
    if (($u = 'pre')) {
      $tagStart = '<' . $u . ' class="module-inspect-collapsible inspect-profile">';
      $tagEnd = '</' . $u . '>';
    }
    $nl = "\n";
    return $tagStart
      . ( empty($message) ? '' : (check_plain($message) . ':' . $nl) )
      . '[Inspect profile]' . $nl
      . self::statSimple('get')
      . $tagEnd;
  }


  /**
   * Creates simple statistics of events (_not_ profiles).
   *
   * @param string $target
   * @return string
   */
  protected static function statSimple($target) {
    $reqEnd = microtime(TRUE) * 1000;
    $file = $target == 'file';
    $delim = ' | ';
    $nl = "\n";

    $events =& self::$events;
    $nEvents = count($events);

    $reqTotal = $reqEnd - ($reqStart = Inspect::requestTimeMilli()) - self::$ownTime_total;

    $s = (!$file ? '' :
        ($nl . join(':', Inspect::sessionCounters())
        . ' - ' . date('Y-m-d H:i:s', floor($reqStart / 1000)) . ' - user: ' . self::$uid
        . ' --------------------------------------------------'
        . $nl . (isset($GLOBALS['_SERVER']) && !empty($_SERVER['REQUEST_METHOD']) ? $_SERVER['REQUEST_METHOD'] : 'CLI') . ' ' . request_uri()
        . $nl . $nl)
      )
      . 'Unit: milliseconds'
      . $nl . 'Total request time' . ($file ? '' : ' (so far)') . ': ' . number_format($reqTotal, 4, '.', '')
      . $nl . 'Subtracted estimated own time, total: ' . number_format(self::$ownTime_total, 4, '.', '')
      . ', per event: ' . number_format(self::$ownTime_event, 4, '.', '')
      . $nl . 'Do not use these chars in names: " \r \n \t |';

    //  Calculate totals and means.
    $durations =& self::$durations;
    $totals = array();
    $stats = array();
    foreach ($durations as $nm => $a) {
      $le = count($a);
      $mTrnc = $mean = ($totals[$nm] = array_sum($a)) / $le;
      if ($le > 1) {
        sort($a);
        $sml = $a[0];
        $big = $a[$le - 1];
      }
      else {
        $sml = $big = self::INTERVAL_UNSTARTED;
      }
      if ($le > 10) {
        $trnc = floor($le / 10);
        $mTrnc = array_sum(array_slice($a, $trnc, $le - ($trnc * 2))) / ($le - ($trnc * 2));
      }
      elseif ($le > 4) {
        array_shift($a);
        array_pop($a);
        $mTrnc = array_sum($a) / ($le - 2);
      }
      $stats[$nm] = array(
        $le,
        $mean,
        $mTrnc,
        $big,
        $sml
      );
    }

    //  Find longest name, for making padded names.
    $padNames = self::$names; // copy
    $lNms = 4; // ~ Name
    foreach ($padNames as $le) {
      if ($le > $lNms) {
        $lNms = $le;
      }
    }
    if (!$file && $lNms > 32) {
      $lNms = 32;
    }
    foreach ($padNames as $nm => &$v) {
      if ($v < $lNms) {
        $v = str_pad($nm, $lNms, ' ');
      }
      elseif (!$file && $v > 32) {
        $v = drupal_substr($nm, 0, 32);
      }
      else {
        $v = $nm;
      }
    }
    unset($v);

    //  List summations.
    $s .= $nl . ' ' . $nl . 'Interval durations cumulative' . $nl
      . join(
        $delim,
        array(
          str_pad('name', $lNms, ' '),
          'total      ',
          'total % ',
          'count',
          'mean %  ',
          'mean       ',
          'trunc. mean',
          'longest    ',
          'shortest   '
        )
      );
    arsort($totals);
    foreach ($totals as $nm => $n) {
      $s .= $nl . $padNames[$nm]
        . $delim . str_pad(number_format($n, 4, '.', ''), 11, ' ', STR_PAD_LEFT) // total
        . $delim . str_pad(number_format(($n * 100) / $reqTotal, 4, '.', ''), 8, ' ', STR_PAD_LEFT) // total percent
        . $delim . str_pad($stats[$nm][0], 5, ' ', STR_PAD_LEFT) // count
        . $delim . str_pad(number_format((($v = $stats[$nm][1]) * 100) / $reqTotal, 4, '.', ''), 8, ' ', STR_PAD_LEFT) // mean %
        . $delim . str_pad(number_format($v, 4, '.', ''), 11, ' ', STR_PAD_LEFT) // mean
        . $delim . str_pad(number_format($v = $stats[$nm][2], 4, '.', ''), 11, ' ', STR_PAD_LEFT) // truncated mean
        . $delim . ($stats[$nm][3] < 0 ? '           ' :
          str_pad(number_format($stats[$nm][3], 4, '.', ''), 11, ' ', STR_PAD_LEFT)) // longest
        . $delim . ($stats[$nm][4] < 0 ? '           ' :
          str_pad(number_format($stats[$nm][4], 4, '.', ''), 11, ' ', STR_PAD_LEFT)) // shortest
        ;
    }

    //  List all events.
    $s .= $nl . ' ' . $nl . 'All events: ' . $nEvents . $nl;
    //  Prevent too large output.
    if ($nEvents > ($maxEvents = floor(Inspect::outputMax() / 400))) {
      return $s . '- number of events exceed ' . $maxEvents . ', will not list events' . ($file ? $nl : '');
    }
    $s .= join(
      $delim,
      array('event', str_pad('name', $lNms, ' '),
        'since req. ', 'since last ', 'duration   ', 'versus mean', '%       ', 'log')
      );
    $tLast = 0;
    $unStarted = self::INTERVAL_UNSTARTED;
    for ($i = 0; $i < $nEvents; $i++) {
      $a = $events[$i]; // name | time | duration | severity | log
      $nm = $a[0];
      $s .= $nl . str_pad($i + 1, 5, ' ', STR_PAD_LEFT) // event no
        . $delim . $padNames[$nm]
        . $delim . str_pad(number_format(($t = $a[1]) - $reqStart, 4, '.', ''), 11, ' ', STR_PAD_LEFT) // since request
        . $delim . ($i && $t > $tLast ? str_pad(number_format($t - $tLast, 4, '.', ''), 11, ' ', STR_PAD_LEFT) : '           ')
        . $delim . (($tDur = $a[2]) < 0 ? ($tDur < $unStarted ? '           ' : ('    ' . $unStarted . '     ')) : // stop that hadnt been started
          str_pad(number_format($tDur, 4, '.', ''), 11, ' ', STR_PAD_LEFT)) // duration
        . $delim . ($tDur < 0 || $stats[$nm][0] == 1 ? '           ' :
          str_pad(number_format($tDur - $stats[$nm][1], 4, '.', ''), 11, ' ', STR_PAD_LEFT)) // versus mean
        . $delim . ($tDur < 0 ? '        ' : str_pad(number_format(($tDur * 100) / $reqTotal, 4, '.', ''), 8, ' ', STR_PAD_LEFT))
        . $delim . (!strlen($a[5]) ? '' :
          str_replace(array('"', "\r", "\n", "\t", '|'), array("'", '', ' ', ' ', '/'), $a[5]) // log
        );
      $tLast = $t;
    }
    $s .= $nl . '%: Percent of total request time' . ($file ? $nl : ' &#183; ') . 'Duration ' . $unStarted . ': Stop preceded by no start'
      . $nl . 'Truncated mean ignores 5 through 25 percent of the outliers at each end (5-10% for populations >= 10)'
      . ($file ? '' : (
        $nl . 'Names and logs are shortened to 32 (except in filed profile)'
        . $nl . 'File: private://module/inspect/profiles' . self::$profileFile
      ) );
    return $s . ($file ? ($nl . $nl) : '');
  }

  /**
   * @param array|NULL $profile
   *  - default: NULL
   * @return array
   */
  protected static function listEventsForLog($profile = NULL) {
    // Every event is (optimally) listed twice in the events list, as a start record and stop record.
    // Here we reduce the records to a single record per event.
    $records = array();
    $events =& self::$events;
    if ($events) {
      $uid = $GLOBALS['user']->uid;
      $sessionCounters = Inspect::sessionCounters();
      $profileId = $profile ? $profile[0] : '';
      $profileStarted = $profile ? $profile[2] : 0;

      $unStopped = self::INTERVAL_UNSTOPPED;
      $unStarted = self::INTERVAL_UNSTARTED;

      $recordIndex = -1;
      $recordsStarted = array();
      foreach ($events as &$event) {
        $name = $event[0];
        $started = $event[1];
        $duration = $event[2];
        $severity = $event[3];
        $error_code = $event[4];
        $log = $event[5];
        $position = $event[6];
        $isStart = FALSE;
        if ($duration == $unStopped) { // A start.
          $isStart = TRUE;
          ++$recordIndex;
          $recordsStarted[ $name ] = $recordIndex;
        }
        elseif ($duration == $unStarted) { // A stop preceded by no start.
          ++$recordIndex;
        }
        else { // A regular stop.
          //  Update previously recorded (started) event, don't write separate new record.
          if (array_key_exists($name, $recordsStarted)) {
            $index = $recordsStarted[ $name ];
            $records[ $index ]['duration'] = $duration;
            $records[ $index ]['severity'] = $severity;
            $records[ $index ]['error_code'] = $error_code;
            $records[ $index ]['log_stop'] = $log;
            $records[ $index ]['position_stop'] = $position;
            unset($recordsStarted[ $name ]);
          }
          // else... ignore; error, but nothing much we can do about it here.
          continue;
        }
        $records[] = array(
          'uid' => $uid,
          'session_id' => $sessionCounters['session'],
          'profile_id' => $profileId,
          'type' => 'event',
          'page_load_no' => $sessionCounters['page_load'],
          'request_no' => $sessionCounters['request'],
          'since_profile_start' => $profileStarted ? ($started - $profileStarted) : 0,
          'since_request_start' => $started - Inspect::requestTimeMilli(),
          'duration' => $duration,
          'name' => $name,
          'severity' => $severity,
          'error_code' => $error_code,
          'log_start' => $isStart ? $log : '',
          'log_stop' => !$isStart ? $log : '',
          // Do only save these three once, to keep query short.
          'location' => $recordIndex ? '' : ($GLOBALS['base_root'] . request_uri()),
          'referer' => $recordIndex ? '' : (isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : ''),
          'hostname' => $recordIndex ? '' : substr(ip_address(), 0, 128),
          'position_start' => $isStart ? $position : '',
          'position_stop' => !$isStart ? $position : '',
          'timestamp_milli' => round($started),
        );
      }
      unset($event); // Clear reference.
    }
    return $records;
  }

  /**
   * Ends current profile, if there's a profile running and endProfile() was called during this request with 'at_exit' option.
   *
   * Called by hook_exit() implementation.
   *
   * @see inspect_profile_exit().
   * @see InspectProfile::endProfile().
   * @return void
   */
  public static function systemExit() {
    // We cannot do this at shutdown, because the profile can't be removed from session vars at that time.
    if (($opts = self::$profileEndAtShutdown)) {
      self::$profileEndAtShutdown = NULL;
      if (!($profile = self::$profileCurrent)) {
        if (drupal_session_started() && isset($_SESSION['module']['inspect_profile'])) {
          $profile = $_SESSION['module']['inspect_profile'];
          unset($_SESSION['module']['inspect_profile']);
          // Clean-up.
          if (!$_SESSION['module']) {
            unset($_SESSION['module']);
          }
        }
      }
      if ($profile && ($opts['name'] == '*' || $opts['name'] == $profile[1])) {
        self::end($profile, 0.0, $opts['log'], $opts['position']);
      }
    }
  }

  /**
   * Writes current event records to database.
   *
   * Registered as shutdown function first time an event gets started/stopped (or a profile gets begun/ended).
   *
   * @see inspect_profile_shutdown()
   * @param array|NULL $profile
   *  - default: NULL (~ get from session var, if any)
   * @param boolean $shutdown
   *  - default: FALSE
   * @return void
   */
  public static function logEvents($profile = NULL, $shutdown = FALSE) {
    // Log events, if any.
    $records = NULL;
    if (self::$events) {
      if (!$profile && !($profile = self::$profileCurrent)) {
        $profileFallback = NULL;
        if (module_exists('state')) {
          if (!($profile = State::sessionGet('module', 'inspect_profile'))) {
            $profileFallback = State::cookieGet('inspect_profile');
          }
        }
        elseif (drupal_session_started() && isset($_SESSION['module']['inspect_profile'])) {
          $profile = $_SESSION['module']['inspect_profile'];
        }
        else {
          $profileFallback = !empty($GLOBALS['_COOKIE']) && array_key_exists('inspect_profile', $_COOKIE) ? $_COOKIE['inspect_profile'] : NULL;
        }
        // Fallback on profile in cookie.
        // These events would otherwise be orphaned; because this HTTP request came in after another request
        // - which ended the profile (and didn't begin another profile).
        if (!$profile && $profileFallback && preg_match('/^[a-zA-Z\d]+\:\d+(\.\d+)?$/', $profileFallback)) {
          $profileFallback = explode(':', $profileFallback);
          $profile = array(
            $profileFallback[0], // Profile id.
            '', // Name; we don't need it here (only used when ending profile using '*').
            (float)$profileFallback[1], // Profile started.
          );
        }
      }
      // This will also log events if orphaned (no profile).
      $records = self::listEventsForLog($profile);
    }
    // If at shutdown: log request_shutdown, even if there are no events
    // (if a profile ended previously during this request: we want request_shutdown anyway, for statistics).
    if ($records || ($shutdown && self::$logRequestShutdown)) {
      try {
        $query = db_insert('inspect_profile')
          ->fields(array(
            'uid',
            'session_id',
            'profile_id',
            'type',
            'page_load_no',
            'request_no',
            'since_profile_start',
            'since_request_start',
            'duration',
            'name',
            'severity',
            'error_code',
            'log_start',
            'log_stop',
            'location',
            'referer',
            'hostname',
            'position_start',
            'position_stop',
            'timestamp_milli',
          ));
        if ($records) {
          $le = count($records);
          for ($i = 0; $i < $le; ++$i) {
            $query->values($records[$i]);
          }
        }
        if ($shutdown) {
          $uid = $GLOBALS['user']->uid;
          $sessionCounters = Inspect::sessionCounters();
          $requestStarted = Inspect::requestTimeMilli();
          $tEnd = microtime(TRUE) * 1000;
          $query->values(array(
            'uid' => $uid,
            'session_id' => $sessionCounters['session'],
            'profile_id' => '', // N/A: There may have been more than one profile in the request.
            'type' => 'request_shutdown',
            'page_load_no' => $sessionCounters['page_load'],
            'request_no' => $sessionCounters['request'],
            'since_profile_start' => -1, // N/A: There may have been than more one profile in the request.
            'since_request_start' => $tEnd - $requestStarted,
            'duration' => $tEnd - $requestStarted,
            'name' => '',
            'severity' => self::$severity,
            'error_code' => '',
            'log_start' => '',
            'log_stop' => '',
            // Do only save these three once, to keep query short.
            'location' => '',
            'referer' => '',
            'hostname' => '',
            'position_start' => '',
            'position_stop' => '',
            'timestamp_milli' => round($tEnd),
          ));
        }
        $query->execute();
      }
      catch (Exception $xc) {
        Inspect::trace($xc);
      }
    }
  }

}
