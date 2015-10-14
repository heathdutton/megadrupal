<?php
/**
 * @file
 * Contains \SimpleComplex\Inspect\Inspect.
 */

namespace SimpleComplex\Inspect;

/**
 * Better variable dumps and error traces.
 */
class Inspect {

  /**
   * Maximum sub var recursion depth.
   *
   * @var integer
   */
  const DEPTH_MAX = 20;

  /**
   * Default sub var recursion depth.
   *
   * @var integer
   */
  const DEPTH_DEFAULT = 10;

  /**
   * Absolute maximum stack frame depth.
   *
   * @var integer
   */
  const TRACE_LIMIT_MAX = 100;

  /**
   * Default stack frame depth.
   *
   * @var integer
   */
  const TRACE_LIMIT_DEFAULT = 5;

  /**
   * Minimum string truncation.
   *
   * @var integer
   */
  const TRUNCATE_MIN = 100000;

  /**
   * Default string truncation.
   *
   * @var integer
   */
  const TRUNCATE_DEFAULT = 1000;

  /**
   * Absolute max. length of an inspection/trace output.
   *
   * Doesn't apply when logging to standard log (PHP error_log()); then 1Kb if
   * syslog and 4Kb if file log.
   *
   * @var integer
   */
  const OUTPUT_MAX = 2097152;

  /**
   * Default max. length of an inspection/trace output.
   *
   * @var integer
   */
  const OUTPUT_DEFAULT = 1048576;

  /**
   * @var integer
   */
  const ERROR_ALGORITHM = 100;

  /**
   * @var integer
   */
  const ERROR_USER = 101;

  /**
   * @var integer
   */
  const ERROR_OUTPUTLENGTH = 102;

  /**
   * @var integer
   */
  const ERROR_EXECTIME = 103;

  /**
   * Operation kind: inspect|trace.
   *
   * Not overridable by $options argument (constructor check).
   *
   * @var string
   */
  protected $kind = 'inspect';

  /**
   * Output target: get|log|file.
   *
   * Not overridable by $options argument (constructor check).
   *
   * @var string
   */
  protected $target = 'get';

  /**
   * Current object/array bucket key name.
   *
   * Not overridable by $options argument (constructor check).
   *
   * @var string|NULL
   */
  protected $key;

  /**
   * (tracer) Flag that frame limit got reduced due to too long output.
   *
   * Not overridable by $options argument (constructor check).
   *
   * @var integer|NULL
   */
  protected $limitReduced;

  /**
   * Current cumulative string length of the inspection output.
   *
   * Not overridable by $options argument (constructor check).
   *
   * @var integer
   */
  protected $outputLength = 0;

  /**
   * Simple option: Maximum object/array recursion depth.
   *
   * Defaults to static::DEPTH_DEFAULT.
   *
   * @var integer
   */
  public $depth;

  /**
   * (tracer) Simple option: Maximum stack frame depth.
   *
   * Defaults to conf var 'trace_limit' or static::TRACE_LIMIT_DEFAULT.
   *
   * @var integer|NULL
   */
  public $limit;

  /**
   * Security option: Hide values of scalar variables/buckets.
   *
   * @var boolean|NULL
   */
  public $hide_scalars;

  /**
   * Security option: Remove paths from string values.
   *
   * NB: Document root is always removed/hidden.
   *
   * @var boolean|NULL
   */
  public $hide_paths;

  /**
   * Output constriction option: Truncate string values.
   *
   * Defaults to conf var 'truncate' or static::TRUNCATE_DEFAULT.
   *
   * @var integer|NULL
   */
  public $truncate;

  /**
   * Maximum output length, of the instance.
   *
   * @var integer|NULL
   */
  public $output_max;

  /**
   * The (inspect) method is wrapped in one or more wrapping logging methods.
   *
   * @var integer
   */
  public $wrappers = 0;

  /**
   * Logging type.
   *
   * @var string
   */
  public $type = 'inspect';

  /**
   * Alias of type.
   *
   * @deprecated
   *
   * @var string
   */
  public $category = 'inspect';

  /**
   * Default logging severity level.
   *
   * @var integer
   */
  public $severity = 7;

  /**
   * Event or error code.
   *
   * @var integer
   */
  public $code = 0;

  /**
   * '$var_name' or "\$var_name", must be escaped, will be truncated to 255
   * (use 'none' to omit).
   *
   * @var string|NULL
   */
  public $name;

  /**
   * @var string|NULL
   */
  public $message;

  /**
   * Used when target is file.
   *
   * @var boolean|NULL
   */
  public $by_user;

  /**
   * Filter out those key names.
   *
   * @var array|NULL
   */
  public $filter;

  /**
   * List of char|string needles to be used in str_replace.
   *
   * @var array
   */
  public $needles = array("\n", "\r", "\t", "\0", '<', '>', '"', "'");

  /**
   * List of char|string replacers to be used in str_replace.
   *
   * @var array
   */
  public $replacers = array('_NL_', '_CR_', '_TB_', '_NUL_', '&#60;', '&#62;', '&#34;', '&#39;');

  /**
   * Injected PSR-3 logger, if any (the 'logger' option).
   *
   * @var object|NULL
   */
  public $logger;

  /**
   * Formatting option: Char|string used as initial quote.
   *
   * @var string
   */
  public $quote_begin = '`';

  /**
   * Formatting option: Char|string used as final quote.
   *
   * @var string
   */
  public $quote_end = '`';

  /**
   * Formatting option.
   *
   * @var array
   */
  public $delimiters = array(
    "\n", // Before first bucket.
    "\n", // Before later bucket.
    "\n"  // Before last bucket.
  );

  /**
   * Formatting option: Indentation of object/array buckets.
   *
   * @var string
   */
  public $pre_indent = '';

  /**
   * Formatting option: Indentation of object/array buckets - gets multiplied
   * by current depth.
   *
   * @var string
   */
  public $indent = '.  ';

  /**
   * Formatting option: (HT)ML enclosure tag, used to wrap log entry.
   *
   * Defaults to empty when using injected logger.
   *
   * @var string
   */
  public $enclose_tag = 'pre';

  /**
   * Formatting option: Newline character(s).
   *
   * @var string
   */
  public $newline = "\n";

  /**
   * (tracer) Formatting option: Frame delimiter.
   *
   * @var string
   */
  public $trace_spacer = '- - - - - - - - - - - - - - - - - - - - - - - - -';

  /**
   * Formatting option: Omit [Inspect... in output.
   *
   * @var boolean|NULL
   */
  public $no_preface;

  /**
   * Formatting option: Don't display file and line of call to inspect method.
   *
   * @var boolean|NULL
   */
  public $no_fileline;

  /**
   * Complex formatting option: Create one-lined formatting instead of
   * multi-lined.
   *
   * @var boolean|NULL
   */
  public $one_lined;

  /**
   * Overriding/supplemental options, by kind (trace).
   *
   * @var array
   */
  protected static $defaultsByKind = array(
    'trace' => array(
      'depth' => 2,
      'type' => 'inspect trace',
      'pre_indent' => '  ',
      'delimiters' => array(
        "\n", // Before first bucket.
        "\n", // Before later bucket.
        "\n  " // Before last bucket.
      ),
    ),
  );

  /**
   * Overriding/supplemental options, by target (get|file).
   *
   * @var array
   */
  protected static $defaultsByTarget = array(
    'get' => array(
      'hide_paths' => TRUE,
    ),
    'file' => array(
      'needles' => array("\r", "\t"),
      'replacers' => array("_CR_\r", "_TB_\t"),
    ),
  );

  /**
   * Options that may be set by (string) value as well as by key.
   *
   * Will be set as TRUE.
   *
   * @var array
   */
  protected static $optionsByValue = array('hide_scalars', 'hide_paths', 'by_user', 'one_lined', 'no_fileline', 'no_preface');

  /**
   * @param string $target
   *   get|log|file.
   * @param mixed $options
   * @param string $kind
   *   Default: 'inspect'.
   */
  protected function __construct($target, $options = NULL, $kind = 'inspect') {
    $this->target = $target;

    // Initialize class vars.
    if (!static::$init) {
      static::init();
    }

    // Initialize instance output max.
    $this->output_max = static::$outputMax;

    // Important defaults; values must be checked after using arg $options.
    $safeDefaults = array(
      'depth' => $this->depth = static::DEPTH_DEFAULT,
      'truncate' => $this->truncate = static::configGet('inspect', 'truncate', static::TRUNCATE_DEFAULT),
    );
    switch ('' . $kind) {
      case 'trace':
        $this->limit = static::configGet('inspect', 'trace_limit', static::TRACE_LIMIT_DEFAULT);
        break;
      default:
        $kind = 'inspect';
    }

    // Override instance default by 'kind' customization.
    if ($kind != 'inspect' && !empty(static::$defaultsByKind[$kind])) {
      $defaults =& static::$defaultsByKind[$kind];
      foreach ($defaults as $key => $value) {
        $this->{$key} = $value;
      }
      unset($defaults);
    }
    // Override instance defaults by target customization.
    if ($target != 'log' && !empty(static::$defaultsByTarget[$target])) {
      $defaults =& static::$defaultsByTarget[$target];
      foreach ($defaults as $key => $value) {
        $this->{$key} = $value;
      }
      unset($defaults);
    }

    // Resolve overriding options.
    $opts = NULL;
    if ($options !== NULL) {
      switch (gettype($options)) {
        case 'array':
          $opts =& $options;
          break;
        case 'object':
          $opts = get_object_vars($options);
          break;
        case 'string':
          if ($options !== '') {
            if (in_array($options, static::$optionsByValue, TRUE)) {
              $this->{$options} = TRUE;
            }
            else {
              $this->message = $options;
            }
          }
          break;
        case 'integer':
          switch ($kind) {
            case 'trace':
              $this->severity = $options;
              break;
            default:
              $this->depth = $options;
          }
          break;
        default:
          // Ignore argument.
      }
    }

    if ($opts) {
      // Remove (protected instance) properties that aren't allowed to be
      // overridden by $options argument.
      unset($opts['kind'], $opts['target'], $opts['key'], $opts['limitReduced'], $opts['outputLength']);

      // Some options may also be set as value (and thus numeric key)
      // ~ interpreted as truthy boolean.
      $copy = $opts;
      foreach ($copy as $key => $value) {
        if (is_numeric($key) && in_array($value, static::$optionsByValue, TRUE)) {
          unset($opts[$key]);
          $opts[$value] = TRUE;
        }
      }
      unset($copy);

      // output_max may be an (absolute) integer or a float to be multiplied
      // by class var $outputMax.
      if (!empty($opts['output_max']) && ($v = $opts['output_max']) > 0) {
        if ($v < 1) {
          $this->output_max = (int)floor($this->output_max * $v);
        }
        elseif ($v < $this->output_max) {
          $this->output_max = (int)$v;
        }
        // else... ignore.
      }
      unset($opts['output_max'], $v);

      // Support 'category' as alias of 'type'.
      if (empty($opts['type']) && !empty($opts['category'])) {
        $opts['type'] = $opts['category'];
        unset($opts['category']);
      }

      // Filter may be string or array.
      if (!empty($opts['filter'])) {
        if (!is_array($opts['filter'])) {
          $opts['filter'] = array($opts['filter']);
        }
      }

      // Vars that must be array to override.
      $keys = array('needles', 'replacers', 'delimiters');
      foreach ($keys as $key) {
        if (!empty($opts[$key]) && is_array($opts[$key])) {
          $this->{$key} = $opts[$key];
        }
        unset($opts[$key]);
      }
      unset($keys);

      // PSR-3 logger.
      if (!empty($opts['logger'])) {
        // Prevent string value error.
        if (!is_object($opts['logger'])) {
          unset($opts['logger']);
        }
        else {
          // An injected logger propably won't like (HT)ML wrapped message.
          $this->enclose_tag = '';
        }
      }

      // Complex formatting flag.
      if (!empty($opts['one_lined'])) {
        $this->depth = 1;
        $this->name = 'none';
        $this->delimiters = array(
          '', // Before first bucket.
          ', ', // Before later bucket.
          ''  // Before last bucket.
        );
        $this->indent = '';
        $this->enclose_tag = '';
        $this->newline = '';
        $this->no_preface = TRUE;
      }
      unset($opts['one_lined']);

      // Merge the rest.
      if ($opts) {
        foreach ($opts as $key => $value) {
          $this->{$key} = $value;
        }
      }
    }
    // Support 'category' as alias of 'type'.
    $this->category = $this->type;

    // Override options when target:get in CLI mode; don't ever enclose output
    // HTML tag.
    if ($target == 'get' && PHP_SAPI === 'cli') {
      $this->enclose_tag = '';
    }

    // Secure against unintended settings and errors.
    if ($this->depth < 1 || $this->depth > static::DEPTH_MAX) {
      $this->depth = $safeDefaults['depth'];
    }
    if ($this->truncate < 0 || $this->truncate > static::TRUNCATE_MIN) {
      $this->truncate = $safeDefaults['truncate'];
    }
    // Convert string severity to integer, and handle out-of-range/unsupported
    // value.
    $this->severity = static::severity($this->severity);

    switch ($kind) {
      case 'trace':
        if ($this->limit < 1 || $this->limit > static::TRACE_LIMIT_MAX) {
          $this->limit = $safeDefaults['limit'];
        }
        break;
    }
    unset($safeDefaults);

    // Paths must always be hidden when target is get.
    if ($target == 'get' && !$this->hide_paths) {
      $this->hide_paths = TRUE;
    }

    // Escape name and message options - and beware of non-string (array)
    // values.
    if ($this->name && $this->name !== 'none') {
      if (is_scalar($value = $this->name)) {
        // Dont escape if target is file.
        if ($target != 'file') {
          $value = htmlspecialchars(
            $value,
            ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
            'UTF-8',
            FALSE // No double encoding
          );
        }
        $this->name = static::mb_substr($value, 0, 255);
      }
      else {
        $this->name = NULL;
      }
    }
    if ($this->message) {
      if (is_scalar($value = $this->message)) {
        // Dont escape if target is file.
        if ($target != 'file') {
          $value = htmlspecialchars(
            $value,
            ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
            'UTF-8',
            FALSE // No double encoding
          );
        }
        $this->message = static::mb_substr($value, 0, 255);
      }
      else {
        $this->message = '*NON-SCALAR MESSAGE of type[' . gettype($this->message) . ']*';
      }
    }
  }

  /**
   * Reset instance.
   */
  protected function reset() {
    $this->outputLength = 0;
    $this->key = NULL;
  }

  /**
   * Inspect session number, page load number, request number.
   *
   * @see Inspect::sessionCounters()
   * @see Inspect::initSessionCounters()
   * @see Inspect::updatePageLoadNumber()
   *
   * @var array
   */
  protected static $sessionCounters = array(
    // Session initialized to 'na' instead of 'n/a' because regexes (PHP and JS)
    // only allow a-zA-Z\d.
    'session' => 'na',
    'page_load' => 0,
    'request' => 1,
  );

  /**
   * @see Inspect::init()
   *
   * @var boolean|NULL
   */
  protected static $init;

  /**
   * Length of document root or real path (the shorter one of them).
   *
   * @see Inspect::init()
   *
   * @var integer
   */
  protected static $pathLength;

  /**
   * Document root and - if symlinked root real path (__FILE__ vs.
   * SCRIPT_FILENAME) - with trailing slash, and as regular expression(s)
   * (/^.../)
   *
   * @see Inspect::init()
   *
   * @var array
   */
  protected static $paths;

  /**
   * Request time start in milliseconds.
   *
   * @see Inspect::requestTimeMilli()
   * @see Inspect::init()
   *
   * @var float
   */
  protected static $requestTimeMilli = 0;

  /**
   * Maximum output length.
   *
   * @see Inspect::init()
   *
   * @var integer
   */
  protected static $outputMax = 0;

  /**
   * PHP max_execution_time as measured at init().
   *
   * We save the value because it may change during execution; this value
   * will be displayed in error message.
   *
   * @see Inspect::init()
   *
   * @var integer
   */
  protected static $maxExecTime = 0;

  /**
   * nspct() will abort if called later than this time ($_maxExecTimePercent
   * percent of max execution time).
   *
   * -1 means no max.
   *
   * @see Inspect::init()
   *
   * @var integer
   */
  protected static $maxExecTimeout = 0;

  /**
   * Log number in current request.
   *
   * NB: Any logging or filing toplevel method must increase this var.
   *
   * @var integer
   */
  protected static $logNo = 0;

  /**
   * Syslog RFC 5424 severity levels; integer-to-string.
   *
   * @var array
   */
  protected static $severityToString = array(
    'emergency',
    'alert',
    'critical',
    'error',
    'warning',
    'notice',
    'info',
    'debug'
  );

  /**
   * Syslog RFC 5424 severity levels; string-to-integer.
   *
   * @var array
   */
  protected static $severityToInteger = array(
    'emergency' => 0,
    'alert' => 1,
    'critical' => 2,
    'error' => 3,
    'warning' => 4,
    'notice' => 5,
    'info' => 6,
    'debug' => 7
  );

  /**
   * Resolve syslog RFC 5424 severity level.
   *
   * @param integer|string $severity
   *   Example: accepts 3 as well as 'error'.
   * @param boolean $toString
   *   Default: FALSE (~ resolve to integer).
   * @param string $default
   *   Applied on out-of-range integer $severity/unsupported string $severity
   *   Default: critical.
   *
   * @return integer|string
   */
  public static function severity($severity, $toString = FALSE, $default = 'critical') {
    // $severity as integer.
    if (strlen('' . $severity) == 1) {
      // Turn out of range to default.
      if ($severity > 7) {
        if ($default === 'critical') {
          // Trace severity arg error.
          static::trace(
            NULL,
            array(
              'message' => 'Severity arg[' . $severity . '] is out of range(0-7)',
              'severity' => static::$severityToInteger['critical'],
            )
          );
        }
        $severity = static::$severityToInteger[$default];
      }
      return !$toString ? $severity : static::$severityToString[$severity];
    }

    // $severity as string.
    if (!in_array('' . $severity, static::$severityToString)) {
      // Turn unsupported to default.
      if ($default === 'critical') {
        // Trace severity arg error.
        static::trace(
          NULL,
          array(
            'message' => 'Severity arg[' . $severity . '] is not supported',
            'severity' => static::$severityToInteger['critical'],
          )
        );
      }
      $severity = $default;
    }
    return !$toString ? static::$severityToInteger[$severity] : $severity;
  }

  /**
   * Checks if user is allowed to output to that target.
   *
   * @throws \LogicException
   *   UNCAUGHT, if arg $target not supported.
   *
   * @param string $target
   *   Values: log|file|frontend log|get.
   *   Default: get.
   *
   * @return boolean
   */
  public static function permit($target = 'get') {
    static $permit = array(
      'get' => NULL,
      'log' => TRUE,
      'file' => TRUE,
      'frontend log' => NULL,
    );

    // If (previously?) established as TRUE/FALSE (not NULL).
    if (($perm = $permit[$target]) || $perm === FALSE) {
      return $perm;
    }

    if (PHP_SAPI === 'cli') {
      $permit = array(
        'get' => TRUE,
        'log' => TRUE,
        'file' => TRUE,
        'frontend log' => FALSE, // Doesn't make sense.
      );
      return $permit[$target];
    }

    $perm = FALSE;
    switch ('' . $target) {
      case 'get':
      case 'frontend log':
        if (ini_get('display_errors')) {
          $perm = TRUE;
        }
        $permit['get'] = $permit['frontend log'] = $perm;
        break;
      default:
        throw new \LogicException(
          'Permission not found, unsupported output target[' . $target . ']',
          Inspect::ERROR_ALGORITHM
        );
    }

    return $perm;
  }

  /**
   * Load settings.
   */
  protected static function init() {
    if (!static::$init) {
      // Make sure session counting is initialised, even if we don't listen to
      // any request init event (conf session_counters).
      static::sessionCounters();

      // Find real path, and document root (if possible), for removal from
      // absolute paths.
      // The reason document root may differ from real path is symbolic links.
      if (($paths = static::configGet('inspect', 'paths'))) {
        // Last bucket is path length.
        static::$pathLength = array_pop($paths);
        static::$paths = $paths;
      }
      else {
        $docRoot = '';
        $le0 = static::mb_strlen($realPath = static::docRoot()); // getcwd().
        $le1 = !empty($_SERVER['SCRIPT_FILENAME']) ? static::mb_strlen($docRoot = dirname($_SERVER['SCRIPT_FILENAME'])) : 0;
        // Find longest path.
        $osNix = DIRECTORY_SEPARATOR == '/';
        // And remove drive C:, for Windows.
        static::$pathLength = ($le0 < $le1 ? $le0 : $le1) - ($osNix ? 0 : 2);
        static::$paths = $paths = array(
          $osNix ? ('/^' . preg_quote($realPath, '/') . '\//') :
            ('/^(' . $realPath{0} . '\:)?' . str_replace('/', '[\\\\\/]', preg_quote(str_replace('\\', '/', static::mb_substr($realPath, 2)))) . '[\\\\\/]/i')
        );
        if ($le1 && $docRoot != $realPath) {
          static::$paths[] = $osNix ? ('/^' . preg_quote($docRoot, '/') . '\//') :
            ('/^(' . $docRoot{0} . '\:)?' . str_replace('/', '[\\\\\/]', preg_quote(str_replace('\\', '/', static::mb_substr($docRoot, 2)))) . '[\\\\\/]/i');
        }
        // Save, they are kind of expensive to establish.
        $paths[] = static::$pathLength;
        static::configSet('inspect', 'paths', $paths);
      }
      unset($paths);

      // Establish output max length.
      static::$outputMax = static::outputMax();

      // Establish request time start.
      $t = static::requestTimeMilli();

      // Establish max execution time abort - if any max at all.
      if ((static::$maxExecTime = $max = ini_get('max_execution_time'))) {
        static::$maxExecTimeout = floor($t / 1000)
          + floor($max * static::configGet('inspect', 'exectime_percent', 90) / 100);
      }
      else {
        static::$maxExecTimeout = -1;
      }

      // Prepare default options - make get options equal file options if CLI
      // request.
      if (PHP_SAPI === 'cli') {
        // Array append.
        static::$defaultsByTarget['get'] += static::$defaultsByTarget['file'];
      }

      static::$init = TRUE;
    }
  }

  /**
   * The actual variable analyser.
   *
   *   Caller must - prior to calling this method - guarantee to call:
   *   - permit()
   *   - init(), unless static::$init is true
   *
   *   Array/object value of $options (any number of options):
   *   - (integer) depth (default 10, max 10)
   *   - (integer) truncate (default 1000, max 100000)
   *   - (integer|float) output_max: default conf var output_max,
   *     float less than 1 will be multiplied by default max.
   *   - (bool) hide_scalars: hide values of scalar vars, and type of resource
   *     (also supported as value 'hide_scalars')
   *   - (bool) hide_paths: only relevant for log (paths always hidden for get),
   *     also supported as value 'hide_paths')
   *   - (string) name: '$var_name' or "\$var_name", must be escaped, will be
   *      truncated to 255 (default empty, use 'none' to omit)
   *   - (string) message, will be truncated to 255 (default empty)
   *   - (string) type: logging type (default inspect)
   *   - (string) category: alias of type
   *   - (integer) severity: default 7|'debug'
   *   - (string|array) filter: filter out that|those key name(s),
   *     default empty
   *   - (array) needles: additional list of char|string needles to be used
   *     in str_replace
   *   - (array) replacers: additional list of char|string replacers to be used
   *     in str_replace
   *   - (boolean) one_lined: create one-lined formatting (getting only,
   *     and also supported as value 'one_lined')
   *   - (boolean) no_preface: truthy to omit [Inspect... preface
   *   - (string) quote_begin: char|string used as initial quote (default >>)
   *   - (string) quote_end: char|string used as final quote (default <<)
   *   - (array) delimiters: between object/array buckets (default newlines)
   *   - (string) indent: indentation of object/array buckets (default 2 spaces)
   *   - (string) enclose_tag: html enclosure tag for the log as a whole
   *     (default pre)
   *   - (string) newline: newline character (default newline)
   *
   *   String truncation:
   *   - truncate option less than one (hide strings) doesnt apply to strings
   *     that solely consist of digits (stringed integers)
   *   - a string bucket of an object/array keyed pass or password (lowercase)
   *     will always be hidden (no matter it's content)
   *
   *   Removes paths at the beginning of variables:
   *   - always removes document root
   *   - always removes relative and absolute paths when getting (optional when
   *     logging)
   *   - path removal takes place before escaping and truncating (except for
   *     overly long strings)
   *
   *   Reports non-integer numbers:
   *   - double as float
   *   - NaN as NaN and infinite as infinite
   *
   *   Reports string lengths and path removal (path reduced to filename):
   *   - un-truncated string, and when hiding scalar values:
   *     (multibyte length|ascii length)
   *   - truncated string: (multibyte length|ascii length|truncation length)
   *   - if path removed: there will be a fourth flag (exclamation mark),
   *     if no truncation truncation length will be hyphen
   *
   *   Recursive:
   *   - always stops at 20th (Inspect::DEPTH_MAX) recursion or less (~ the
   *     depth option)
   *   - when buckets of an object/array is analyzed, buckets of buckets will be
   *     analyzed to one lesser depth
   *   - detects if a bucket of an object references the object itself
   *     (marked *RECURSION*)
   *   - detects GLOBALS array's self-referencing GLOBALS bucket
   *     (marked *RECURSION*)
   *   - doesnt check identity for arrays (not possible in PHP), nor 'same-ness'
   *     (too heavy, and prone to err)
   *
   * @see Inspect::permit()
   * @see Inspect::init()
   *
   * @throws \RuntimeException
   *   UNCAUGHT, if called after max execution time percentage limiter has been
   *   passed (code: Inspect::ERROR_EXECTIME).
   * @throws \OverflowException
   *   UNCAUGHT, if cumulate inspection output length exceeds maximum
   *   (code: Inspect::ERROR_OUTPUTLENGTH).
   * @throws \LogicException
   *   UNCAUGHT, if current depth exceeds max depth
   *   (code: Inspect::ERROR_ALGORITHM).
   *
   * @param mixed $var
   * @param integer $_curDepth
   *   Default: zero
   *   Recursion counter, do not call it with anything but zero.
   *
   * @return string
   */
  protected function nspct($var, $_curDepth = 0) {
    static $callNo = 0;

    // Execution time check, every 1000th time.
    if (!((++$callNo) % 1000)
      && static::$maxExecTimeout > -1
      && time() > static::$maxExecTimeout
    ) {
      throw new \RuntimeException(
        'Inspection aborted: was called after ' . static::configGet('inspect', 'exectime_percent', 90) . '% of PHP max_execution_time[' . static::$maxExecTime
        . '] had passed, using options depth[' . $this->depth . '] and truncate[' . $this->truncate . '], try less depth or more truncation.',
        Inspect::ERROR_EXECTIME
      );
    }
    // Output length check.
    if ($this->outputLength > $this->output_max) {
      throw new \OverflowException(
        'Inspection aborted: output length ' . $this->outputLength . ' exceeds maximum ' . $this->output_max
        . ', using options depth[' . $this->depth . '] and truncate[' . $this->truncate . '], try less depth or more truncation.',
        Inspect::ERROR_OUTPUTLENGTH
      );
    }
    // Recursion limit check.
    if ($_curDepth > $this->depth) {
      throw new \LogicException(
        'Algo error, current depth ' . $_curDepth . ' exceeds max depth ' . $this->depth . '.',
        Inspect::ERROR_ALGORITHM
      );
    }

    // Object or array.
    $tArr = FALSE;
    $tObj = '';
    if (is_object($var) || ($tArr = is_array($var))) {
      $output = $tArr ? 'array' : ($tObj = get_class($var));
      // Set 'name' prop if not set by caller.
      if (!$_curDepth && !$this->name) {
        $this->name = $output;
      }
      $output = '(' . $output . ':';
      // If at max depth: simply get length of the container.
      if ($_curDepth == $this->depth) {
        if ($tArr) {
          $output .= ($nSubs = count($var)) . ')' . (!$nSubs ? '' : ' [...]');
        }
        else {
          if ($var instanceof \Countable && $var instanceof \Traversable) {
            // Require Traversable too, because otherwise the count may not
            // reflect the foreach.
            $nSubs = count($var);
          }
          // stdClass cannot be counted; count public instance vars as array.
          else {
            $nSubs = count(get_object_vars($var));
          }
          $output .= $nSubs . ')' . (!$nSubs ? '' : ' {...}');
        }
        // Deliberately not multibyte strlen().
        $this->outputLength += strlen($output);
        return $output;
      }
      // Dive into container buckets.
      else {
        $delimFirst = $this->delimiters[0]
          . $this->pre_indent . str_repeat($indent = $this->indent, $_curDepth + 1);
        $delimLater = $this->delimiters[1]
          . $this->pre_indent . str_repeat($indent, $_curDepth + 1);
        $nSubs = 0;
        $subOutput = '';
        // Different iterations for object vs array, because:
        // - array iterate by explicit reference, whereas object may fail
        //   fatally doing that (An iterator cannot be used with foreach
        //   by reference)
        // - they must have different self reference checks
        // - array must be checked for (all) numeric indices
        if ($tArr) {
          // If all numeric indices, use [] instead of {}.
          $numArr = TRUE;
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $output
          );
          foreach ($var as $key => &$sub) { // by explicit reference
            $delim = (++$nSubs) > 1 ? $delimLater : $delimFirst;
            // GLOBALS check, skip self reference.
            if ($key === 'GLOBALS' && is_array($sub) && array_key_exists('GLOBALS', $sub)) {
              $numArr = FALSE;
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . 'GLOBALS: (array) *RECURSION*'
              );
              $subOutput .= $u;
              continue;
            }
            if ($numArr && $key !== $nSubs - 1) { // numeric array check
              $numArr = FALSE;
            }
            if ($this->filter && in_array($key, $this->filter, TRUE)) {
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . $key . ': F'
              );
              $subOutput .= $u;
            }
            else {
              $this->key = $key; // Pass key, for password check.
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . $key . ': '
              );
              // Recursion.
              $subOutput .= $u . $this->nspct($sub, $_curDepth + 1);
            }
          }
          unset($sub); // Clear iteration reference.
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $u = $nSubs . ') '
              . ($numArr ? '[' : '{')
          );
          $output .= $u . $subOutput;
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $u = $this->delimiters[2] . str_repeat($indent, $_curDepth)
              . ($numArr ? ']' : '}')
          );
          return $output . $u;
        }
        else { // object
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $output
          );
          // Not &$sub, iterating object is implicitly by reference.
          foreach ($var as $key => $sub) {
            $delim = (++$nSubs) > 1 ? $delimLater : $delimFirst;
            if ($sub === $var) { // Check for identity (self reference).
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . $key . ': (' . $tObj . ') *RECURSION*'
              );
              $subOutput .= $u;
              continue;
            }
            if ($this->filter && in_array($key, $this->filter, TRUE)) {
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . $key . ': F'
              );
              $subOutput .= $u;
            }
            else {
              // Pass key, for password check.
              $this->key = $key;
              // Deliberately not multibyte strlen().
              $this->outputLength += strlen(
                $u = $delim . $key . ': '
              );
              $subOutput .= $u . $this->nspct($sub, $_curDepth + 1); // recursion
            }
          }
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $u = $nSubs . ') {'
          );
          $output .= $u . $subOutput;
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $u = $this->delimiters[2] . str_repeat($indent, $_curDepth)
              . '}'
          );
          return $output . $u;
        }
      }
    }

    // Scalars, NULL and resource.
    switch (($t = gettype($var))) {

      case 'boolean':
        $this->outputLength += strlen( // Deliberately not multibyte strlen().
          $output = '(' . $t . ')' . (!$this->hide_scalars ? ($var ? ' TRUE' : ' FALSE') : '')
        );
        return $output;

      case 'integer':
      case 'double':
      case 'float':
        if (!$this->hide_scalars && !is_finite($var)) {
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $output = '(' . (is_nan($var) ? 'NaN' : 'infinite') . ')'
          );
          return $output;
        }
        $this->outputLength += strlen( // Deliberately not multibyte strlen().
          $output = '(' . ($t == 'double' ? 'float' : $t) . ')'
            . ( $this->hide_scalars ? '' :
              (!$var ? ' 0' : (' ' . static::numberToString($var)) )
            )
        );
        return $output;

      case 'string':
        // (string:n0|n1|n2): n0 ~ multibyte length, n1 ~ ascii length,
        // n2 only occurs if truncation.
        $output = '(string:';
        // Get multibyte length and ascii length, and check for invalid UTF-8.
        $leRaw = strlen($var); // Deliberately not multibyte strlen().
        if (!static::validUtf8($var)) {
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $output .= '?|' . $leRaw . '|0) *INVALID_UTF8*'
          );
          return $output;
        }
        $leMb = $leRaw ? static::mb_strlen($var) : 0;
        $output .= !$leRaw ? '0|0' : ($leMb . '|' . $leRaw);
        // If empty or hide scalar values.
        if ($this->hide_scalars || !$leRaw) {
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            $output .= ')'
              . ( $this->hide_scalars ? '' : // no quotes if hide value
                (' ' . $this->quote_begin . $this->quote_end) )
          );
          return $output;
        }
        // If very long string, truncate to absolute max before working on the
        // string internals.
        $truncedTo = 0;
        if ($leMb > ($trunc = $this->truncate) && $leMb > ($trunc_min = static::TRUNCATE_MIN)) {
          $var = static::mb_substr($var, 0, $trunc_min);
          $truncedTo = $leMb = $trunc_min;
        }
        // If truncate is zero.
        $pw = $_curDepth > 0 && ($this->key === 'pass' || $this->key === 'password');
        if ($pw || $trunc < 1) {
          // Look for stringed integer (consists solely of digits),
          // but only if not truncated.
          if (!$pw && !$truncedTo && preg_match('/^\d+$/', $var)) {
            // Deliberately not multibyte strlen().
            $this->outputLength += strlen(
              // Stringed integer.
              $output .= ') ' . $this->quote_begin . $var . $this->quote_end
            );
            return $output;
          }
          $this->outputLength += strlen( // Deliberately not multibyte strlen().
            // |0 ~ flag truncation
            $output .= '|0) ' . $this->quote_begin . '...' . $this->quote_end
          );
          return $output;
        }

        // Remove document root (always) and/or path.
        // Do it before escaping (needles may contain dot or slash).
        $pathRem = FALSE;
        if ($leRaw > 3) {
          // *nix /.
          if (DIRECTORY_SEPARATOR == '/') {
            if (strpos($var, '/') !== FALSE) {
              // Remove any path.
              if ($this->hide_paths) {
                // Removes any kind of path - also document root
                // - except when Windows.
                if ($var{0} === '/' // Absolute path.
                  // Relative.
                  || ( $var{0} === '.' && (strpos($var, '../') === 0 || strpos($var, './') === 0) )
                ) {
                  $pathRem = TRUE;
                  $leMb = static::mb_strlen($var = basename($var));
                }
              }
              // Replace document root.
              elseif ($leRaw >= static::$pathLength
                && $var{0} === '/'
                && ($le = static::mb_strlen($var = preg_replace(static::$paths, '', $var))) < $leMb
              ) {
                $pathRem = TRUE;
                $leMb = $le;
                $var = 'document_root/' . $var;
              }
            }
          }
          // Windows \.
          // Doesn't attempt to remove/replace backslashed paths (except
          // document root), because such cannot be detected safely. Backslash
          // matching is close to impossible.
          else {
            // Remove C:/ and ./ and ../.
            if ($this->hide_paths && strpos($var, '/') && preg_match('/^[a-zA-Z]:\/.+|\.\.?\/.+/', $var)) {
              $pathRem = TRUE;
              $leMb = static::mb_strlen($var = basename($var));
            }
            // Replace document root.
            elseif ($leRaw >= static::$pathLength
              && ($le = static::mb_strlen($var = preg_replace(static::$paths, '', $var))) < $leMb
            ) {
              $pathRem = TRUE;
              $leMb = $le;
              if (!$this->hide_paths) {
                $var = 'document_root/' . $var;
              }
            }
          }
        }

        // Replace listed neeedles with harmless symbols.
        $var = str_replace($this->needles, $this->replacers, $var);

        // Escape lower ASCIIs and backslashes - they may not be appropriate for
        // a logging implementation.
        if (!$pathRem && $this->target == 'log') {
          // Lower ASCII.
          $var = addcslashes($var, "\0..\37");
          // Replace singular backslash.
          $var = preg_replace('/(\A|[^\x5C])\x5C([^\x5C]|\z)/', '$1_BS_$2', $var);
        }

        // Escape string, except if target is file.
        if ($this->target != 'file') {
          $var = htmlspecialchars(
            $var,
            ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
            'UTF-8',
            FALSE // No double encoding
          );
        }
        // Truncate string.
        if ($leMb > $trunc) {
          $var = static::mb_substr($var, 0, $trunc);
          $truncedTo = $trunc;
        }

        //@formatter:off
        $this->outputLength += strlen( // Deliberately not multibyte strlen().
          $output .= (!$truncedTo ? (!$pathRem ? '' : '|-|!' ) :
              ('|' . $truncedTo . (!$pathRem ? '' : '|!' ) )
            )
            . ') '
            . $this->quote_begin . (!$pathRem ? '' : '...' ) . $var . (!$truncedTo ? '' : '...') . $this->quote_end
        );
        //@formatter:on
        return $output;

      case 'resource':
        $this->outputLength += strlen( // Deliberately not multibyte strlen().
          $output = '(resource)' . (!$this->hide_scalars ? (' ' . get_resource_type($var) ) : '')
        );
        return $output;

      default: // NULL and unknown type
        $this->outputLength += strlen( // Deliberately not multibyte strlen().
          $output = '(' . $t . ')'
        );
        return $output;
    }
  }

  /**
   * @param \Exception|NULL $exception
   *
   * @return string
   */
  protected function trc($exception = NULL) {
    // For IDE; prevent uninitialised notifications.
    $nFrame = $sTrc = $sTrcEnd = NULL;

    try {
      // Received Exception, by arg.
      if ($exception) {
        if (is_object($exception) && (($xcClass = get_class($exception)) === 'Exception' || is_subclass_of($exception, 'Exception'))) {
          if (!$this->code) {
            $this->code = $exception->getCode();
          }
          $trace = $exception->getTrace();
          if (count($trace) > $this->limit) {
            array_splice($trace, $this->limit);
          }
        }
        else {
          throw new \Exception(
            'arg $exception type[' . (!is_object($exception) ? gettype($exception) : get_class($exception)) . '] is not Exception or falsy',
            Inspect::ERROR_USER
          );
        }
      }
      // Create trace, if none given by arg.
      else {
        $xcClass = '';
        $trace = debug_backtrace();

        // Remove top levels on synthetic trace, first of all this method.
        array_shift($trace);
        // Find first frame whose file isn't named inspect.* (case insensitive).
        $le = count($trace);
        $nFrame = -1;
        for ($i = 0; $i < $le; ++$i) {
          if (!empty($trace[$i]['file']) && strtolower(substr(basename($trace[$i]['file']), 0, 8)) !== 'inspect.') { // Deliberately not multibyte.
            $nFrame = $i;
            break;
          }
        }
        if ($nFrame > -1) {
          $trace = array_slice($trace, $nFrame);
        }

        // Enforce the trace limit.
        if (count($trace) > $this->limit) {
          // Plus one because we need the bucket holding the initial event.
          array_splice($trace, $this->limit + 1);
        }
      }

      // Format as string.
      $delim = $this->delimiters[1];
      $sFunc = '';

      // If exception: resolve its origin.
      if ($xcClass) {
        $sTrc = 'Exception (' . $xcClass . ') - code: ' . $exception->getCode() . $delim;
        $file = trim($exception->getFile());
        $line = (($u = $exception->getLine()) ? trim($u) : '?');
        // Escape exception message; may contain unexpected characters that are
        // inappropriate for a logging implementation.
        $xcMessage = addcslashes(str_replace($this->needles, $this->replacers, $exception->getMessage()), "\0..\37");
        $message = 'message: ' . $xcMessage . $delim;

        // If more severe than debug: pass exception message to overall message,
        // if that is empty.
        if ($this->severity < static::$severityToInteger['debug'] && !$this->message) {
          if ($this->target != 'file') {
            $this->message = static::mb_substr(
              htmlspecialchars(
                $xcMessage,
                ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
                'UTF-8',
                FALSE // No double encoding
              ),
              0,
              255
            );
          }
          else {
            $this->message = static::mb_substr($xcMessage, 0, 255);
          }
        }
        unset($xcMessage);
      }
      else {
        $sTrc = 'Backtrace' . $delim;
        $frame = array_shift($trace);
        if (isset($frame['class'])) {
          $sFunc = 'static method: ' . $frame['class'] . '::' . (isset($frame['function']) ? ($frame['function'] . '()') : '') . $delim;
        }
        elseif (isset($frame['function'])) {
          $sFunc = 'function: ' . $frame['function'] . '()' . $delim;
        }
        $file = isset($frame['file']) ? trim($frame['file']) : 'unknown';
        $line = isset($frame['line']) ? trim($frame['line']) : '?';
        $message = '';
      }

      if ($file != 'unknown' && ($le = static::mb_strlen($file))) {
        $file = $this->hide_paths ? basename($file) : str_replace(
          '\\',
          '/',
          static::mb_strlen($noRoot = preg_replace(static::$paths, '', $file)) < $le ? ('document_root/' . $noRoot) : $file
        );
      }
      $sTrc .= 'file: ' . $file . $delim
        . 'line: ' . $line . $delim
        . $sFunc
        . $message;

      $sTrcEnd = 'END ' . $this->trace_spacer;

      // Iterate stack frames.
      // Deliberately not multibyte strlen().
      $sTrcLength = strlen($sTrc) + strlen($sTrcEnd);
      $nFrame = -1;
      foreach ($trace as &$frame) {
        $sFrm = (++$nFrame) . ' ' . $this->trace_spacer . $delim;

        // File and line.
        if (isset($frame['file']) && ($le = static::mb_strlen($u = trim($frame['file'])))) {
          $file = $this->hide_paths ? basename($u) : str_replace(
            '\\',
            '/',
            static::mb_strlen($noRoot = preg_replace(static::$paths, '', $u)) < $le ? ('document_root/' . $noRoot) : $u
          );
        }
        else {
          $file = 'unknown';
        }
        $sFrm .= 'file: ' . $file . $delim
          . 'line: ' . (isset($frame['line']) ? trim($frame['line']) : '?') . $delim;

        // Class, object, function, type.
        // Deliberately not multibyte strlen().
        $sFunc = isset($frame['function']) && strlen($u = $frame['function']) ? $u : '';
        // Deliberately not multibyte strlen().
        $sCls = isset($frame['class']) && strlen($u = $frame['class']) ? $u : '';
        $sType = isset($frame['type']) ? trim($frame['type']) : '';
        $sObj = '';
        if (isset($frame['object'])) {
          if (($o = $frame['object'])) {
            // We dont know if class bucket is present when object is, but using
            // class is cheaper.
            $sObj = $sCls ? $sCls : get_class($o);
            if ($sFunc) {
              $sFunc = 'method: (' . $sObj . ')' . ($sType ? $sType : '->') . $sFunc;
            }
            // Is this possible at all? Simply object, no method call?
            else {
              $sObj = 'object (' . $sObj . ')';
            }
          }
        }
        elseif ($sFunc) {
          $sFunc = !$sCls ? ('function: ' . $sFunc) : ('static method: ' . $sCls . ($sType ? $sType : '::') . $sFunc);
          // $frame['object'] doesnt exist.
        }
        // else {
        //   $frame['object'] doesnt exist
        //   $frame['function'] may exist, containing non-empty string
        // }
        $sFrm .= ($sFunc ? $sFunc : $sObj) . $delim;
        // Args.
        if (isset($frame['args'])) {
          $le = count($args = $frame['args']);
          $sArgs = '';
          for ($i = 0; $i < $le; $i++) {
            $sArgs .= $delim . $this->pre_indent . $this->nspct($args[$i]);
          }
          $sFrm .= 'args (' . $le . ')'
            . (!$le ? '' : (': ' . $sArgs) )
            . $delim;
        }
        // Skip current frame if total output now exceeds max.
        // Deliberately not multibyte strlen().
        if (($sTrcLength += strlen($sFrm)) > $this->output_max) {
          $this->limitReduced = $nFrame;
          break;
        }
        else {
          $sTrc .= $sFrm;
        }
      }
      return $sTrc . $sTrcEnd;
    }
    catch (\Exception $xc) {
      // Inspect::nspct() may abort too.
      if (($errorCode = $xc->getCode()) == Inspect::ERROR_OUTPUTLENGTH) {
        $this->limitReduced = $nFrame;
        return $sTrc . $sTrcEnd;
      }

      // Other error.
      // If severity is less grave than warning, make it a warning.
      if ($this->severity > static::$severityToInteger['warning']) {
        $this->severity = static::$severityToInteger['warning'];
      }
      $output = str_replace($this->needles, $this->replacers, $xc->getMessage())
        . (!$errorCode ? '' : (' (error ' . $errorCode . ')'));

      if ($this->target != 'file') {
        return htmlspecialchars(
          $output,
          ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
          'UTF-8',
          FALSE // No double encoding
        );
      }
      return $output;
    }
  }

  /**
   * Get (or init) session counter(s).
   *
   * Counters will only be set and updated if conf var session_counters,
   * because this feature uses a cookie, and it also adds a slight performance
   * hit to each and every request.
   *
   *  Counters:
   *  - session: not a number, a custom session id (case-sensitive hex, approx.
   *    16 chars long), or simply 'na' (as in n/a)
   *  - page_load: number of request in session that produced 'page' (non-AJAX)
   *    output
   *  - request: number of requests in session
   *
   * @param string $name
   *   Default: empty (~ get all, as array).
   *   Values: session|page_load|request (~ get single value).
   *
   * @return mixed
   *   Array: all counters.
   *   String: session.
   *   Integer: page_load|request.
   *   NULL: bad arg $name.
   */
  public static function sessionCounters($name = '') {
    static $called;
    // Init first time called.
    if (!$called) {
      $called = TRUE;
      if (static::configGet('inspect', 'session_counters')) {
        if (($c = static::cookieGet('inspect__sc')) && preg_match('/^[a-zA-Z\d]+\:\d{1,5}\:\d{1,5}$/', $c)) {
          $c = explode(':', $c);
          static::$sessionCounters = $counters = array(
            'session' => $c[0], // Session number.
            'page_load' => $c[1], // Page load number.
            'request' => (int)$c[2] + 1 // Request number.
          );
        }
        else {
          $c = explode('.', uniqid('', TRUE));
          static::$sessionCounters = $counters = array(
            'session' => Inspect::baseConvert($c[0], 16, 62) . Inspect::baseConvert($c[1], 16, 62),
            // Will be increased when page delivery is guaranteed.
            'page_load' => 0,
            'request' => 1
          );
        }
        static::cookieSet('inspect__sc', join(':', static::$sessionCounters));
      }
      else {
        $counters = static::$sessionCounters;
      }
    }
    else {
      $counters = static::$sessionCounters;
    }

    if ($name) {
      switch ('' . $name) {
        case 'session':
        case 'page_load':
        case 'request':
          return $counters[$name];
      }
      return NULL;
    }
    return $counters;
  }

  /**
   * Increases session counter's page load number and sets updated session
   * counter cookie.
   *
   * Counters will only be set and updated if conf var session_counters,
   * because this feature uses a cookie, and it also adds a slight performance
   * hit to each and every request.
   */
  public static function updatePageLoadNumber() {
    static $called;
    if (!$called) {
      $called = TRUE;
      if (static::configGet('inspect', 'session_counters')) {
        // Make sure session counting is initialised,
        // even if we don't listen to any request init event.
        static::sessionCounters();

        ++static::$sessionCounters['page_load'];
        static::cookieSet('inspect__sc', join(':', static::$sessionCounters));
      }
    }
  }

  /**
   * Get request time start - in milliseconds - more accurately than the
   * REQUEST_TIME constant.
   *
   * The value is usually slightly higher than REQUEST_TIME, apparantly the
   * latter is a floor'ed value.
   *
   * Uses either $_SERVER['REQUEST_TIME_FLOAT'] (PHP>=5.4), request header
   * X-Request-Received-Processed, or REQUEST_TIME.
   *
   * To enable use of the custom request header (Apache only), add the following
   * to document root .htaccess:
   * @code
   * <IfModule mod_headers.c>
   *   RequestHeader set X-Request-Received-Processing "%t %D"
   * </IfModule>
   * @endcode
   *
   * @return float
   */
  public static function requestTimeMilli() {
    // This method will be obsolete when everybody uses PHP>=5.4.
    if (!($rtm =& static::$requestTimeMilli)) {
      if (!empty($_SERVER['REQUEST_TIME_FLOAT'])) {
        $rtm = round($_SERVER['REQUEST_TIME_FLOAT'] * 1000, 3);
      }
      else {
        $rtm = ($rts = (int) $_SERVER['REQUEST_TIME']) * 1000;
        //@formatter:off
        if (
          (
            (!empty($_SERVER['HTTP_X_REQUEST_RECEIVED_PROCESSING']) && ($v = $_SERVER['HTTP_X_REQUEST_RECEIVED_PROCESSING']))
            || (
              function_exists('apache_request_headers') && ($a = apache_request_headers())
              && array_key_exists('X-Request-Received-Processing', $a) && ($v = $a['X-Request-Received-Processing'])
            )
          )
          // Deliberately not multibyte strlen().
          && strlen($v) < 31 && preg_match('/^t\=\d{16,17}\ D\=\d{1,9}/', $v)
        ) {
          $a = explode(' ', $v);
          // Deliberately not multibyte substr().
          $proc = (float) substr($a[1], 2);
          // If received time can be reduced to positive integer, and that is
          // close to REQUEST_TIME.
          // Deliberately not multibyte strlen().
          if (($iRec = (int) round(($flRec = (float) substr($a[0], 2)) / 1000000)) > 0
            && $iRec <= $rts + 1 && $iRec >= $rts - 1
          ) {
            $rtm = round(($flRec + $proc) / 1000, 3);
          }
        }
        //@formatter:on
      }
    }
    return $rtm;
  }

  /**
   * Inspect variable and log to standard log, if permitted.
   *
   *   Like PHP native var_dump()/var_export() except:
   *   - is controlled by permissions
   *   - has much more options, and far better formatting (JSONesque)
   *   - doesnt fail on inspecting $GLOBALS
   *   - aborts if producing too large output, or taking too much time
   *   - doesnt echo, logs
   *
   *   Integer value of $options (depth):
   *   - the value controls how deeply the variable is going to be analyzed
   *
   *   Array/object value of $options (any number of options):
   *   - (integer) depth (default 10, max 20)
   *   - (integer) truncate (default 1000, max 100000)
   *   - (boolean) hide_scalars: hide values of scalar vars, and type of
   *     resource (also supported as value 'hide_scalars')
   *   - (boolean) hide_paths: only optional for log (mandatory for get), also
   *     supported as value 'hide_paths' (NB: document root is always hidden)
   *   - (string) name: '$var_name' or "\$var_name", must be escaped, will be
   *     truncated to 255 (use 'none' to omit)
   *   - (string) message, will be truncated to 255 (default empty)
   *   - (string) type: logging type (default inspect)
   *   - (string) category: alias of type
   *   - (integer|string) severity: default ~ 'debug'
   *   - (integer) code: default zero
   *   - (object) logger: PSR-3, default none, use Inspect's standard logger
   *   - (integer) wrappers: the (inspect) function/method is wrapped in one or
   *     more local logging functions/methods (default zero)
   *   - (string|array) filter: filter out that|those key name(s)
   *     (default empty)
   *   - (array) needles: list of additional char|string needles to be used in
   *     str_replace
   *   - (array) replacers: list of additional char|string replacers to be used
   *     in str_replace
   *   - (boolean) one_lined: create one-lined formatting (also supported as
   *     value 'one_lined')
   *   - (boolean) no_fileline: don't display file and line of call to inspect
   *     function/method (also supported as value 'no_fileline')
   *   - (boolean) no_preface: omit [Inspect... preface (also supported as value
   *     'no_preface')
   *
   *   String truncation:
   *   - truncate option less than one (hide strings) doesnt apply to strings
   *     that solely consist of digits (stringed integers)
   *   - a string bucket of an object/array keyed pass or password (lowercase)
   *     will always be hidden (no matter it's content)
   *
   *   Removes paths at the beginning of variables:
   *   - always removes document root
   *   - always removes relative and absolute paths when getting (optional,
   *     hide_paths, when logging)
   *
   *   Reports non-integer numbers:
   *   - double as float
   *   - NaN as NaN and infinite as infinite
   *
   *   Reports string lengths and path removal (path reduced to filename):
   *   - un-truncated string, and when hiding scalar values:
   *     (multibyte length|ascii length)
   *   - truncated string: (multibyte length|ascii length|truncation length)
   *   - if path removed: there will be a fourth flag (exclamation mark), if no
   *     truncation truncation length will be hyphen
   *
   *   Recursive:
   *   - detects if a bucket of an object references the object itself
   *     (marked *RECURSION*)
   *   - detects GLOBALS array's self-referencing GLOBALS bucket
   *     (marked *RECURSION*)
   *   - doesnt check identity for arrays (not possible in PHP), nor 'same-ness'
   *     (too heavy, and prone to err)
   *
   * Executes variable analysis within try-catch.
   *
   * @param mixed $var
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to suggested depth.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function log($var, $options = NULL) {
    if (!static::permit('log')) {
      return NULL;
    }
    /** @var Inspect $inspect */
    $inspect = new static('log', $options);

    try {
      $output = $inspect->nspct($var);
    }
    catch (\Exception $xc) {
      // If severity is less grave than warning: make it a warning.
      if ($inspect->severity > static::$severityToInteger['warning']) {
        $inspect->severity = static::$severityToInteger['warning'];
      }
      switch (($errorCode = (int)$xc->getCode())) {
        case Inspect::ERROR_OUTPUTLENGTH: // Output length exceeding maximum.
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          // Try again?
          $retry = FALSE;
          if ($inspect->depth > 2) {
            $retry = TRUE;
            $inspect->depth = ceil($inspect->depth / 2);
          }
          if ($inspect->truncate > 100) {
            $retry = TRUE;
            $inspect->truncate = ceil($inspect->truncate / 2);
          }
          if ($retry) {
            $output .= $inspect->newline . 'Trying again, using options depth[' . $inspect->depth . '] and truncate[' . $inspect->truncate . '].';
            $inspect->reset();
            try {
              $output .= $inspect->newline . $inspect->nspct($var);
            }
            catch (\Exception $xc1) {
              if ((int)$xc1->getCode() != Inspect::ERROR_OUTPUTLENGTH) {
                return '';
              }
              $output .= $inspect->newline . '(' . $xc1->getCode() . ') ' . $xc1->getMessage();
            }
          }
          break;
        case Inspect::ERROR_EXECTIME: // Max percent of max_execution_time passed.
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          break;
        default:
          return FALSE;
      }
    }

    // Concat.
    $output = (!$inspect->no_preface ?
        (!$inspect->message ?
          ('[Inspect - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters))
            . ':' . (++static::$logNo) . ' - depth:' . $inspect->depth . ']') :
          ('[' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo) . '] '
            . $inspect->message . ':' . $inspect->newline . '[Inspect - depth:' . $inspect->depth . ']')
        ) :
        (!$inspect->message ? '' : ($inspect->message . ':' . $inspect->newline) )
      )
      . (!$inspect->name || $inspect->name == 'none' ? '' : (' ' . $inspect->name) ) . $inspect->newline
      . ($inspect->no_fileline ? '' :
        (static::fileLine($inspect->hide_paths, $inspect->wrappers) . $inspect->newline)
      )
      . $output;

    // Truncate; is more sensitive when logging than filing or getting,
    // because may end in db query.
    $truncated = FALSE;
    // Deliberately not multibyte strlen().
    if (strlen($output) > $inspect->output_max) {
      $truncated = TRUE;
      $output = static::truncateBytes($output, $inspect->output_max - 5) . '[...]';
    }
    // Enclose in HTML tags?
    if (($u = $inspect->enclose_tag)) {
      $output = '<' . $u . ' class="module-inspect' . (!$truncated ? '-collapsible' : '') . '">'
        . $output
        . '</' . $u . '>';
    }

    if ($inspect->logger) {
      return static::logToInjected(
        $inspect->logger,
        $output,
        static::plaintext(static::mb_substr($inspect->type, 0, 64)),
        $inspect->severity,
        $inspect->code
      );
    }
    return static::logToStandard(
      $output,
      static::plaintext(static::mb_substr($inspect->type, 0, 64)),
      $inspect->severity,
      $inspect->code
    );
  }

  /**
   * Inspect variable and log to file, if permitted.
   *
   *   Array/object value of $options are like Inspect::log()'s options, except:
   *   - (boolean) by_user: log to user specific file instead of common file
   *
   * Deliberately doesn't escape strings, because the target is a file
   * (not HTTP output).
   *
   * @param mixed $var
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to suggested depth.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function file($var, $options = NULL) {
    if (!static::permit('file')) {
      return NULL;
    }
    /** @var Inspect $inspect */
    $inspect = new static('file', $options);

    try {
      $output = $inspect->nspct($var);
    }
    catch (\Exception $xc) {
      // If severity is less grave than warning, make it a warning.
      if ($inspect->severity > static::$severityToInteger['warning']) {
        $inspect->severity = static::$severityToInteger['warning'];
      }
      switch (($errorCode = (int)$xc->getCode())) {
        case Inspect::ERROR_OUTPUTLENGTH: // Output length exceeding maximum.
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          // Try again?
          $retry = FALSE;
          if ($inspect->depth > 2) {
            $retry = TRUE;
            $inspect->depth = ceil($inspect->depth / 2);
          }
          if ($inspect->truncate > 100) {
            $retry = TRUE;
            $inspect->truncate = ceil($inspect->truncate / 2);
          }
          if ($retry) {
            $output .= $inspect->newline . 'Trying again, using options depth[' . $inspect->depth . '] and truncate[' . $inspect->truncate . '].';
            $inspect->reset();
            try {
              $output .= $inspect->newline . $inspect->nspct($var);
            }
            catch (\Exception $xc1) {
              if ((int)$xc1->getCode() != Inspect::ERROR_OUTPUTLENGTH) {
                return '';
              }
              $output .= $inspect->newline . '(' . $xc1->getCode() . ') ' . $xc1->getMessage();
            }
          }
          break;
        // Max percent of max_execution_time passed.
        case Inspect::ERROR_EXECTIME:
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          break;
        default:
          return FALSE;
      }
    }
    // Option no_preface not supported.
    //@formatter:off
    return static::logToFile(
      (!$inspect->message ? '' : ($inspect->message . ':' . $inspect->newline) )
      . '[Inspect - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo)
      . ' - depth:' . $inspect->depth . ']'
      . (!$inspect->name || $inspect->name == 'none' ? '' : (' ' . $inspect->name) ) . $inspect->newline
      . ($inspect->no_fileline ? '' :
        (static::fileLine($inspect->hide_paths, $inspect->wrappers) . $inspect->newline)
      )
      . $output,
      $inspect->type,
      $inspect->severity,
      $inspect->by_user
    );
    //@formatter:on
  }

  /**
   * Inspect variable and get the output as string, if permitted.
   *
   * @param mixed $var
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to suggested depth.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return string
   *   Empty: user isnt permitted to get inspections, or on error (other error
   *   than exceeding max. output length).
   */
  public static function get($var, $options = NULL) {
    if (!static::permit('get')) {
      return '';
    }
    /** @var Inspect $inspect */
    $inspect = new static('get', $options);

    try {
      $output = $inspect->nspct($var);
    }
    catch (\Exception $xc) {
      // If severity is less grave than warning, make it a warning.
      if ($inspect->severity > static::$severityToInteger['warning']) {
        $inspect->severity = static::$severityToInteger['warning'];
      }
      switch (($errorCode = (int)$xc->getCode())) {
        case Inspect::ERROR_OUTPUTLENGTH: // Output length exceeding maximum.
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          // Try again?
          $retry = FALSE;
          if ($inspect->depth > 2) {
            $retry = TRUE;
            $inspect->depth = ceil($inspect->depth / 2);
          }
          if ($inspect->truncate > 100) {
            $retry = TRUE;
            $inspect->truncate = ceil($inspect->truncate / 2);
          }
          if ($retry) {
            $output .= $inspect->newline . 'Trying again, using options depth[' . $inspect->depth . '] and truncate[' . $inspect->truncate . '].';
            $inspect->reset();
            try {
              $output .= $inspect->newline . $inspect->nspct($var);
            }
            catch (\Exception $xc1) {
              if ((int)$xc1->getCode() != Inspect::ERROR_OUTPUTLENGTH) {
                return '';
              }
              $output .= $inspect->newline . '(' . $xc1->getCode() . ') ' . $xc1->getMessage();
            }
          }
          break;
        // Max percent of max_execution_time passed.
        case Inspect::ERROR_EXECTIME:
          $output = '(' . $errorCode . ') ' . $xc->getMessage();
          break;
        default:
          return '';
      }
    }
    $tagStart = $tagEnd = '';
    if (($u = $inspect->enclose_tag)) {
      $tagStart = '<' . $u . '  class="module-inspect-collapsible">';
      $tagEnd = '</' . $u . '>';
    }
    //@formatter:off
    return $tagStart
    . (!$inspect->no_preface ?
      (!$inspect->message ?
        ('[Inspect - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo)
          . ' - depth:' . $inspect->depth . ']') :
        ('[' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo) . '] '
          . $inspect->message . ':' . $inspect->newline . '[Inspect - depth:' . $inspect->depth . ']')
      ) :
      (!$inspect->message ? '' : (static::plaintext($inspect->message) . ':' . $inspect->newline) )
    )
    . (!$inspect->name || $inspect->name == 'none' ? '' : (' ' . static::plaintext($inspect->name)) ) . $inspect->newline
    . ($inspect->no_fileline ? '' : (static::fileLine(TRUE, $inspect->wrappers) . $inspect->newline) )
    . $output . $tagEnd . $inspect->newline;
    //@formatter:on
  }

  /**
   * Inspect and log stack trace to standard log, if permitted.
   *
   * Logging an error message and/or exception message is always allowed, if
   * severity is more severe than ~debug.
   *
   * Integer value of options evalutuates to severity.
   *
   *   Array/object value of $options are like Inspect::log()'s options, except:
   *   - (integer) limit: maximum stack frame depth (default 5, max 100)
   *   - (string) type: inspect trace
   *   - (string) pre_indent
   *   - (string) trace_spacer: spacer between frames
   *     (default hyphen dotted line, length 49)
   *
   * See inspect() for specifics of variable analysis.
   *
   * Default logging severity: ~debug.
   *
   * Executes variable analysis within try-catch.
   *
   * @see Inspect::log()
   *
   * @param \Exception|NULL $exception
   *   Default: NULL (~ create new backtrace).
   *   Exception: trace that.
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to severity.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function trace($exception = NULL, $options = NULL) {
    /** @var Inspect $inspect */
    $inspect = new static('log', $options, 'trace');

    if (!static::permit('log')) {
      // Logging is always permitted when severity is more grave than debug;
      // however only the exception message and/or $options message will be
      // logged.
      if ($inspect->severity < static::$severityToInteger['debug']) {
        $ms = !$inspect->message ? '' : (
          htmlspecialchars(
            str_replace(
              $inspect->needles,
              $inspect->replacers,
              $inspect->message
            ),
            ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
            'UTF-8',
            FALSE // No double encoding
          )
          . ':' . $inspect->newline
        );

        if ($exception && is_object($exception)) {
          if (!$inspect->code) {
            $inspect->code = $exception->getCode();
          }
          // Escape exception message; may contain unexpected characters that
          // are inappropriate for a logging implementation.
          $em = htmlspecialchars(
              addcslashes(str_replace($inspect->needles, $inspect->replacers, $exception->getMessage()), "\0..\37"),
              ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
              'UTF-8',
              FALSE // No double encoding
            )
            . $inspect->newline . '@' . basename($exception->getFile()) . ':' . $exception->getLine();
        }
        else {
          $em = static::fileLine(TRUE, $inspect->wrappers);
        }

        return static::logToStandard(
          $ms . $em,
          static::plaintext(static::mb_substr($inspect->type, 0, 64)),
          $inspect->severity
        );
      }
      return NULL;
    }

    if (!($trace = $inspect->trc($exception))) {
      return FALSE;
    }

    // Concat.
    $output = (!$inspect->message ?
        ('[Inspect trace - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo)
          . ' - limit:' . $inspect->limit . '|depth:' . $inspect->depth . ']|truncate:' . $inspect->truncate . ']') :
        ('[' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo) . '] '
          . $inspect->message . ':' . $inspect->newline . '[Inspect trace - limit:'
          . $inspect->limit . (!$inspect->limitReduced ? '' : '(reduced to ' . $inspect->limitReduced . ')')
          . '|depth:' . $inspect->depth . ']|truncate:' . $inspect->truncate . ']')
      )
      . $inspect->newline
      . $trace;

    // Truncate; is more sensitive when logging than filing or getting, because
    // may end in db query.
    // Deliberately not multibyte strlen().
    if (strlen($output) > $inspect->output_max) {
      $output = static::truncateBytes($output, $inspect->output_max - 5) . '[...]';
    }
    // Enclose in HTML tags?
    if (($u = $inspect->enclose_tag)) {
      $output = '<' . $u . ' class="module-inspect-trace">'
        . $output
        . '</' . $u . '>';
    }

    if ($inspect->logger) {
      return static::logToInjected(
        $inspect->logger,
        $output,
        static::plaintext(static::mb_substr($inspect->type, 0, 64)),
        $inspect->severity,
        $inspect->code
      );
    }
    return static::logToStandard(
      $output,
      static::plaintext(static::mb_substr($inspect->type, 0, 64)),
      $inspect->severity,
      $inspect->code
    );
  }

  /**
   * Alias of trace().
   *
   * @param \Exception|NULL $exception
   * @param mixed $options
   *
   * @return boolean|NULL
   */
  public static function traceLog($exception = NULL, $options = NULL) {
    return static::trace($exception, $options);
  }

  /**
   * Inspect and log stack trace to file, if permitted.
   *
   * Filing an error message and/or exception message is always allowed, if
   * severity is more severe than ~debug.
   *
   *   Array/object value of $options are like Inspect::trace(), except:
   *   - (boolean) by_user: log to user specific file instead of common file
   *
   * Deliberately doesn't escape strings, because the target is a file
   * (not HTTP output).
   *
   * @param \Exception|NULL $exception
   *   Default: NULL (~ create new backtrace)
   *   Exception: trace that.
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to severity.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function traceFile($exception = NULL, $options = NULL) {
    /** @var Inspect $inspect */
    $inspect = new static('file', $options, 'trace');

    if (!static::permit('file')) {
      // Logging is always permitted when severity is more grave than debug;
      // however only the exception message and/or $options message will be
      //logged.
      if ($inspect->severity < static::$severityToInteger['debug']) {
        $ms = !$inspect->message ? '' : (
          str_replace($inspect->needles, $inspect->replacers, $inspect->message)
          . ':' . $inspect->newline
        );

        if ($exception && is_object($exception)) {
          $em = str_replace($inspect->needles, $inspect->replacers, $exception->getMessage())
            . $inspect->newline . '@' . basename($exception->getFile()) . ':' . $exception->getLine();
        }
        else {
          $em = static::fileLine(TRUE, $inspect->wrappers);
        }

        return static::logToFile(
          $ms . $em,
          $inspect->type,
          $inspect->severity,
          $inspect->by_user
        );
      }
      return NULL;
    }

    if (!($trace = $inspect->trc($exception))) {
      return FALSE;
    }

    return static::logToFile(
      (!$inspect->message ? '' : ($inspect->message . ':' . $inspect->newline) )
      . '[Inspect trace - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo)
      . ' - limit:' . $inspect->limit . (!$inspect->limitReduced ? '' : '(reduced to ' . $inspect->limitReduced . ')')
      . '|depth:' . $inspect->depth . ']|truncate:' . $inspect->truncate . ']'
      . $inspect->newline
      . $trace,
      $inspect->type,
      $inspect->severity,
      $inspect->by_user
    );
  }

  /**
   * Inspect stack trace and get the output as string, if permitted.
   *
   * @param \Exception|NULL $exception
   *   Default: NULL (~ create new backtrace)
   *   Exception: trace that.
   * @param mixed $options
   *   Array|object: list of options.
   *   Integer: interprets to severity.
   *   String: interprets to message.
   *   Default: NULL.
   *
   * @return string
   *   Empty: user isnt permitted to get inspections, or tracing failed (other
   *   error than exceeding max. output length).
   */
  public static function traceGet($exception = NULL, $options = NULL) {
    if (!static::permit('get')) {
      return '';
    }
    /** @var Inspect $inspect */
    $inspect = new static('get', $options, 'trace');

    if (!($trace = $inspect->trc($exception))) {
      return '';
    }

    $tagStart = $tagEnd = '';
    if (($u = $inspect->enclose_tag)) {
      $tagStart = '<' . $u . ' class="module-inspect-trace">';
      $tagEnd = '</' . $u . '>';
    }
    return $tagStart
    . (!$inspect->message ?
      ('[Inspect trace - ' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo)
        . ' - limit:' . $inspect->limit . '|depth:' . $inspect->depth . ']|truncate:' . $inspect->truncate . ']') :
      ('[' . (static::$sessionCounters['session'] == 'na' ? 'i' : join(':', static::$sessionCounters)) . ':' . (++static::$logNo) . '] '
        . $inspect->message . ':' . $inspect->newline . '[Inspect trace - limit:'
        . $inspect->limit . (!$inspect->limitReduced ? '' : '(reduced to ' . $inspect->limitReduced . ')')
        . '|depth:' . $inspect->depth . ']|truncate:' . $inspect->truncate . ']')
    )
    . $inspect->newline
    . $trace
    . $tagEnd . $inspect->newline;
  }

  /**
   * Log a message - and file + line of the call - if permitted.
   *
   * Checks message length against maximum allowed database query length.
   *
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function logMessage($message = '', $type = 'inspect', $severity = 'debug') {
    if (!static::permit('log')) {
      return NULL;
    }
    /** @var Inspect $opts */
    $opts = new static('log');

    $tagStart = $tagEnd = '';
    if (($u = $opts->enclose_tag)) {
      $tagStart = '<' . $u . '>';
      $tagEnd = '</' . $u . '>';
    }
    return static::logToStandard(
      $tagStart
      . htmlspecialchars(
        str_replace($opts->needles, $opts->replacers, $message),
        ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
        'UTF-8',
        FALSE // No double encoding
      )
      . $opts->newline
      . static::fileLine(FALSE, 0)
      . $tagEnd,
      static::plaintext(static::mb_substr($type, 0, 64)),
      static::severity($severity)
    );
  }

  /**
   * Log to file - and file + line of the call - if permitted.
   *
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   * @param boolean $by_user
   *   Truthy: logs to user specific log file.
   *   Default: FALSE (~ logs to common log file).
   *
   * @return boolean|NULL
   *   NULL: user isnt permitted to log inspections.
   *   FALSE: on error.
   */
  public static function fileMessage($message = '', $type = 'inspect', $severity = 'debug', $by_user = FALSE) {
    if (!static::permit('file')) {
      return NULL;
    }
    /** @var Inspect $opts */
    $opts = new static('file');

    return static::logToFile(
      str_replace($opts->needles, $opts->replacers, $message)
      . $opts->newline
      . static::fileLine(FALSE, 0),
      $type,
      static::severity($severity),
      $by_user
    );
  }

  /**
   * Logs message from frontend to backend log.
   *
   *  Required POST vars:
   *  - (string) message (HTML allowed, will be escaped, max multibyte length
   *    100.000)
   *  - (string) type: will always get 'frontend' prefixed (plaintext)
   *  - (integer) severity
   *  - (integer) logNo
   *  - (string) kind: info | inspect | trace
   *  - (string) url
   *  - (string) browser
   *  - (string) fileLine
   *
   *  Optional POST vars.
   *  - (string) caption (plaintext, max multibyte length 255)
   *  - (string) pageLoadId: ~ session counters (and we just use that GET var
   *    instead)
   *
   * @param string $sessionCounter
   * @param integer $logNo
   * @param string $severity
   *   Values: info|notice|warning|error, etc.
   *
   * @return void
   *   Exits.
   *   Sends 403 header if the expected POST vars arent set or their values
   *   aren't acceptable.
   */
  public static function logFromFrontend($sessionCounter, $logNo, $severity) {
    // Optionals POST vars, which might not get initialized otherwise.
    $caption = '';

    // Permissions and retrieve/validate vars; get out on failure.
    if (!static::permit('frontend log')

      // GET vars validation.

      // Session counter pattern (cookie inspect__sc + frontend log number):
      // 'ascii-letters-and-numbers:number:number'.
      || !$sessionCounter || ($sessionCounter !== 'i' && !preg_match('/^[a-zA-Z\d]+:\d{1,5}:\d{1,5}$/', $sessionCounter))

      // Log no. must be a number of a reasonable size.
      || !$logNo || strlen('' . $logNo) > 5 || !is_numeric($logNo)

      // Severity must be a non-empty string that matches a known severity.
      || !$severity || strlen($severity) > 9 // Deliberately not multibyte strlen().
      || !array_key_exists('' . $severity, static::$severityToInteger)

      // Required POST vars validation.

      // Type must be a non-empty string, no longer than 64 chars.
      || empty($_POST['type']) || static::mb_strlen($type = static::plaintext($_POST['type'])) > 64

      // Kind must be a non-empty string, no longer than 32 chars.
      // Deliberately not multibyte strlen().
      || empty($_POST['kind']) || strlen($kind = static::plaintext($_POST['kind'])) > 32

      // Message must be a non-empty string, no longer than 102400 multibyte
      // chars (100Kb + multibyte extra encoding chars).
      || !array_key_exists('message', $_POST) || static::mb_strlen($message = '' . $_POST['message']) > 102400

      // Url (including original GET vars etc.) must be a non-empty string,
      // no longer than 1024 multibyte chars.
      || empty($_POST['url']) || static::mb_strlen($url = static::plaintext($_POST['url'])) > 1024

      // Browser (including original GET vars etc.) must be a non-empty string,
      // no longer than 512 multibyte chars.
      || empty($_POST['browser']) || static::mb_strlen($browser = static::plaintext($_POST['browser'])) > 512

      // fileLine must be a non-empty string, no longer than 512 multibyte chars.
      || empty($_POST['fileLine']) || static::mb_strlen($fileLine = static::plaintext($_POST['fileLine'])) > 512

      // Optional POST vars validation.

      // Caption must (if set) be a non-empty string, no longer than 255
      // multibyte chars.
      || (!empty($_POST['caption']) && static::mb_strlen($caption = static::plaintext($_POST['caption'])) > 255)
    ) {
      header('HTTP/1.1 403 Forbidden');
      exit;
    }

    // Convert severity to integer.
    $severity = static::$severityToInteger[$severity];

    // Escape message.
    $message = htmlspecialchars(
      $message,
      ENT_QUOTES, // PHP 5.4: ENT_QUOTES | ENT_SUBSTITUTE
      'UTF-8',
      FALSE // No double encoding
    );

    // Make sure severity isnt too severe; frontend should probably not be
    // allowed to log an 'emergency'.
    if ($severity < ($maxSeverity = static::configGet('inspect', 'fronttoback_sevmax', static::$severityToInteger['error']))) {
      $severity = $maxSeverity;
    }

    // Get Inspects default formatting options.
    /** @var Inspect $opts */
    $opts = new static('log');
    $nl = $opts->newline;

    switch ($kind) {
      case 'inspect':
      case 'dump':
        $kind = 'inspect';
        $output = !$caption ? ('[frontend Inspect - ' . $sessionCounter . ':' . $logNo . ']') :
          ('[' . $sessionCounter . ':' . $logNo . '] ' . $caption . $nl . '[frontend Inspect]');
        break;
      case 'trace':
        $output = !$caption ? ('[frontend Inspect trace - ' . $sessionCounter . ':' . $logNo . ']') :
          ('[' . $sessionCounter . ':' . $logNo . '] ' . $caption . $nl . '[frontend Inspect trace]');
        break;
      default: // info
        $output = '[' . $sessionCounter . ':' . $logNo . ']' . (!$caption ? '' : (' ' . $caption . ':'));
    }
    $output .= $nl
      . $fileLine . $nl
      . $url . $nl
      . $browser . $nl
      . $message;

    $tagEnd = '';
    if (($u = $opts->enclose_tag)) {
      $output = '<' . $u . '  class="module-inspect' . ($kind == 'inspect' ? '-collapsible inspect-frontend' : '') . '">' . $output;
      $tagEnd = '</' . $u . '>';
    }

    static::logToStandard(
      $output . $tagEnd,
      'frontend ' . $type,
      $severity
    );

    // Deliberately dont tell frontend if logging failed.
    $response = new \stdClass();
    $response->success = TRUE;
    $response->error = '';
    $response->logNo = $logNo;
    header('Content-Type: application/json; charset=utf-8');
    header('Cache-Control: private, no-store, no-cache, must-revalidate');
    header('Expires: Thu, 01 Jan 1970 00:00:01 GMT');
    // No reason to use custom json encoder, because we dont send html strings.
    echo json_encode($response);
    flush();
    exit;
  }


  // Utility methods.

  /**
   * Get maximum output length.
   *
   * @see Inspect::OUTPUT_DEFAULT
   *
   * @param boolean $check
   *   Truthy: descendant implementation may establish max. from sources like
   *   database; fall-back Inspect::OUTPUT_DEFAULT.
   *
   * @return integer
   */
  public static function outputMax($check = FALSE) {
    return static::configGet('inspect', 'output_max', static::OUTPUT_DEFAULT);
  }

  /**
   * Secures existance of filing directory; optionally subdir to that.
   *
   * Uses conf var 'file_path'; defaults to system temp dir.
   *
   * @param string $sub_dir
   *   Default: empty.
   *   No leading nor trailing slash.
   * @param integer $mode
   *   Default: 0775 (~ group write mode).
   *
   * @return string|boolean
   *   FALSE: on error.
   */
  public static function ensureDirectory($sub_dir = '', $mode = 0775) {
    static $_dir;
    if (!($dir = $_dir)) {
      if ($dir === NULL) {
        if (!($dir = static::configGet('inspect', 'file_path'))) {
          $dir = sys_get_temp_dir();
        }
      }
      else {
        return FALSE;
      }
      if (DIRECTORY_SEPARATOR != '/') {
        $dir = str_replace('\\', '/', $dir);
      }
      $le = strlen($dir);
      if ($dir{$le - 1} == '/') { // Remove trailing slash.
        $dir = substr($dir, 0, $le - 1); // Deliberately not multibyte substr().
        $le -= 1;
      }
      if ($le < 2) { // Dir cannot be shorter than 2 chars (/x).
        return ($_dir = FALSE);
      }
      if (!is_dir($dir) && !mkdir($dir, $mode, TRUE)) {
        static::logToStandard(
          'Directory [' . $dir . '] is not a dir or cannot be created',
          'inspect',
          static::severity('warning')
        );
        return ($_dir = FALSE);
      }
      $_dir = $dir;
    }
    if (!$sub_dir
      || is_dir($dir .= '/' . $sub_dir) || mkdir($dir, $mode, TRUE)
    ) {
      return $dir;
    }
    static::logToStandard(
      'Directory [' . $dir . '/' . $sub_dir . '] is not a dir or cannot be created',
      'inspect',
      static::severity('warning')
    );
    return FALSE; // Failed to ensure subdir.
  }

  /**
   * Convert number or hex string to any base through 2 and 62.
   *
   * Base 62 produces up to 35% shorter string than base 16.
   *
   *  Like native base_convert() precision fails for numbers (somewhere) larger
   *  than (tested on windows box with ini precision 14):
   *  - base 10: 9007199254740991 (> 15 digits)
   *  - base 16: 1fffffffffffff (> 14 digits)
   *
   * @param integer|float|string $num
   * @param integer $fromBase
   *   Default: 10
   *   Only supports 10 and 16; if other, returns arg $num unaltered.
   * @param integer $toBase
   *   Default (and max): 62.
   *
   * @return string
   */
  public static function baseConvert($num, $fromBase = 10, $toBase = 62) {
    static $table = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $zero = $table{0};
    $remainder = $num;
    if ($num < 1) {
      if (!$num) {
        return '' . $zero; // zero
      }
      $remainder *= -1;
    }
    if ($toBase == $fromBase) {
      return $remainder;
    }
    switch ($fromBase) {
      case 10:
        break;
      case 16:
        $remainder = base_convert($remainder, 16, 10);
        break;
      default:
        return $remainder;
    }
    // Arg $num may be string, and base_convert() returns string.
    $remainder = (float)$remainder;
    if ($toBase > 62) {
      $toBase = 62;
    }
    elseif ($toBase < 2) {
      $toBase = 2;
    }
    $res = '';
    // Find power ~ required number of digits.
    $pow = 0;
    $divisor = 1;
    // Limit:100 probably way too high, havent seen more than 10 iterations
    // (1fffffffffffff, with precision 14)
    $limit = 100;
    while ($remainder >= ($test = pow($toBase, ++$pow)) && (--$limit)) {
      $divisor = $test;
      --$limit;
    }
    --$pow;
    // Do modulo procedure.
    // Limit:100 probably way too high, havent seen more than 14 iterations
    // (1fffffffffffff, with precision 14).
    $limit = 100;
    while ($remainder > 0 && (--$limit)) {
      if ($pow) {
        if ($divisor == $remainder) {
          return $res . $table{1} . str_repeat($zero, $pow);
        }
        elseif ($divisor > $remainder) {
          $res .= $zero;
        }
        else {
          if ($remainder > $divisor) {
            $res .= $table{ (int)floor($remainder / $divisor) };
            $remainder = fmod($remainder, $divisor);
          }
          else {
            $res .= $table{ $pow };
            $remainder = fmod($remainder, $divisor);
          }
          if ($remainder < 1 && !floor($remainder)) {
            return $res . str_repeat($zero, $pow);
          }
        }
      }
      else {
        return $res . $table{ (int)$remainder };
      }
      $divisor = pow($toBase, --$pow);
    }
    return $res;
  }

  /**
   * Get file and line of outmost call to an inspect function or public method.
   *
   * @param boolean $hidePaths
   *   Default: TRUE.
   * @param integer $wrappers
   *   Default: zero.
   *
   * @return string
   */
  protected static function fileLine($hidePaths = TRUE, $wrappers = 0) {
    $trace = debug_backtrace();
    // Find first frame whose file isn't named inspect.* (case insensitive).
    $le = count($trace);
    $nFrame = -1;
    for ($i = 1; $i < $le; ++$i) {
      if (!empty($trace[$i]['file']) && strtolower(substr(basename($trace[$i]['file']), 0, 8)) !== 'inspect.') { // Deliberately not multibyte.
        $nFrame = $i;
        break;
      }
    }
    if ($nFrame > -1 && (!$wrappers || !empty($trace[$nFrame += $wrappers]['file']))) {
      //@formatter:off
      return '@' . (
      $hidePaths ? basename($trace[$nFrame]['file']) :
        ('document_root/' . str_replace('\\', '/', preg_replace(static::$paths, '', $trace[$nFrame]['file'])))
      )
      . ':' . (isset($trace[$nFrame]['line']) ? $trace[$nFrame]['line'] : '?');
      //@formatter:on
    }
    return '@unknown';
  }

  /**
   * Log to the equivalent of stderr (PHP error_log).
   *
   * Caller must guarantee to escape type and message.
   *
   * May truncate message to prevent failure; maximum length may be as short as
   * 1 kilobyte (1024 raw chars.).
   *
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   * @param integer|string $code
   *   Default: zero.
   *
   * @return boolean
   *   FALSE: on error.
   */
  protected static function logToStandard($message = '', $type = 'inspect', $severity = 'debug', $code = 0) {
    static $outputMax;
    // No instance output_max here, and in this very basic implementation of
    // the method we only consider the nature of PHP ini:error_log.
    if (!$outputMax) {
      // Syslog normally allows for 1Kb.
      // Filing isn't atomical when message length exceeds the file system's
      // max block size (typically 4Kb; ext3 and NTFS).
      $outputMax = ini_get('error_log') === 'syslog' ? 1024 : 4096;
    }

    // Get rid of enclosing html tags, and filing truncators
    // (newline, null byte).
    if ($message) {
      if ($message{0} === '<') {
        $message = strip_tags($message);
      }
      $message = str_replace(array("\n", "\0"), array('\\n', '_NUL_'), $message);
    }
    // Prefix severity and type.
    $message = '[' . static::severity($severity, TRUE) . ':' . $type . '] ' . $message;

    // Truncate.
    if (strlen($message) > $outputMax) { // Deliberately not multibyte strlen().
      $message = static::truncateBytes($message, $outputMax - 5) . '[...]';
    }

    return error_log($message) ? TRUE : FALSE;
  }

  /**
   * Log to an injected PSR-3 logger.
   *
   * Caller must guarantee to escape type and message.
   *
   * Logs to standard logger upon failure.
   *
   * @param object $logger
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   * @param integer|string $code
   *   Default: zero.
   *
   * @return boolean
   *   FALSE: on error.
   */
  protected static function logToInjected($logger, $message = '', $type = 'inspect', $severity = 'debug', $code = 0) {
    // Prefix severity and type.
    $output = '[' . static::severity($severity, TRUE) . ':' . $type . '] ' . $message;
    // Truncate.
    if (strlen($output) > static::$outputMax) {
      $output = static::truncateBytes($output, static::$outputMax - 5) . '[...]';
    }

    $context = array(
      'type' => $type,
    );
    if ($code) {
      $context['code'] = $code;
    }
    try {
      $logger->log(static::severity($severity), $output, $context);

      return TRUE;
    }
    catch (\Exception $xc) {
      // Warning: this could easily go perpetual if a global mechanism for using
      // injected logger instead of standard logger gets implemented.
      static::log(
        $logger,
        array(
          'severity' => 'critical',
          'type' => $type,
          'message' => 'Failed to log via injected logger'
        )
      );
      static::logToStandard($message, $type, $severity, $code);
    }

    return FALSE;
  }

  /**
   * Log to custom file.
   *
   * Deliberately doesn't escape message, because the target is a file
   * (not HTTP output).
   *
   * May truncate message; though maximum length is probably hundreds
   * of kilobytes.
   *
   * @param string $message
   *   Default: empty string.
   * @param string $type
   *   Default: inspect.
   * @param integer|string $severity
   *   Default: 'debug'.
   * @param boolean $by_user
   *   Default: FALSE (~ logs to common log file).
   *   Truthy: logs to user specific log file.
   *
   * @return boolean
   *   FALSE: on error.
   */
  protected static function logToFile($message = '', $type = 'inspect', $severity = 'debug', $by_user = FALSE) {
    static $_logDir;
    if (!$_logDir && !($_logDir = static::ensureDirectory('logs'))) {
      return FALSE;
    }
    $type = static::plaintext(static::mb_substr($type, 0, 64));
    $severity = static::severity($severity);

    // Truncate.
    // No instance output_max here, so we have to resort to the
    // (possibly larger) class value.
    // Deliberately not multibyte strlen().
    if (strlen($message) > static::$outputMax) {
      $message = static::truncateBytes($message, static::$outputMax - 5) . '[...]';
    }

    $uid = static::userId();
    $rtm = static::$requestTimeMilli;
    return file_put_contents(
      $_logDir . '/inspect_' . date('Ymd', (int) round($rtm / 1000))
      // For anonymous user: avoid filing concurrency by using the session
      // number.
      . (!$by_user ? '' :
        ('_user_' . $uid . ($uid ? '' : ('_' . static::$sessionCounters['session'])))
      )
      . '.log',
      join(':', static::$sessionCounters)
      . ' - ' . date('Y-m-d H:i:s', floor($rtm / 1000)) . ' - user: ' . $uid
      . ' --------------------------------------------------'
      . "\n" . $_SERVER['REQUEST_METHOD'] . ' ' . $_SERVER['REQUEST_URI']
      . "\nseverity+type: " . static::$severityToString[$severity] . ' - ' . $type
      . "\n" . $message . "\n\n",
      FILE_APPEND
    ) ? TRUE : FALSE;
  }

  /**
   * @param string $str
   *
   * @return boolean
   */
  protected static function validUtf8($str) {
    return $str === '' ? TRUE :
      // The PHP regex u modifier forces the whole subject to be evaluated
      // as UTF-8. And if any byte sequence isn't valid UTF-8 preg_match()
      // will return zero for no-match.
      // The s modifier makes dot match newline; without it a string consisting
      // of a newline solely would result in a false negative.
      preg_match('/./us', $str);
  }

  /**
   * @param string $str
   *
   * @return string
   */
  protected static function plaintext($str) {
    return htmlspecialchars(strip_tags($str), ENT_QUOTES, 'UTF-8');
  }

  /**
   * Multibyte-safe string length.
   *
   * @param string $str
   *
   * @return integer
   */
  protected static function mb_strlen($str) {
    static $mb = -1;
    if ($str === '') {
      return 0;
    }
    if ($mb == -1) {
      $mb = function_exists('mb_strlen');
    }
    if ($mb) {
      return mb_strlen($str);
    }

    $n = 0;
    $le = strlen($str);
    $leading = FALSE;
    for ($i = 0; $i < $le; $i++) {
      // ASCII.
      if (($ord = ord($str{$i})) < 128) {
        ++$n;
        $leading = FALSE;
      }
      // Continuation char.
      elseif ($ord < 192) {
        $leading = FALSE;
      }
      // Leading char.
      else {
        // A sequence of leadings only counts as a single.
        if (!$leading) {
          ++$n;
        }
        $leading = TRUE;
      }
    }
    return $n;
  }

  /**
   * Multibyte-safe sub string.
   *
   * @param string $str
   * @param integer $start
   * @param integer|NULL $length
   *    Default: NULL.
   *
   * @return string
   */
  protected static function mb_substr($str, $start, $length = NULL) {
    static $mb = -1;
    // Interprete non-NULL falsy length as zero.
    if ($str === '' || (!$length && $length !== NULL)) {
      return '';
    }

    if ($mb == -1) {
      $mb = function_exists('mb_substr');
    }
    if ($mb) {
      return !$length ? mb_substr($str, $start) : mb_substr($str, $start, $length);
    }

    // The actual algo (further down) only works when start is zero.
    if ($start > 0) {
      // Trim off chars before start.
      $str = substr($str, strlen(static::mb_substr($str, 0, $start)));
    }
    // And the algo needs a length.
    if (!$length) {
      $length = static::mb_strlen($str);
    }

    $n = 0;
    $le = strlen($str);
    $leading = FALSE;
    for ($i = 0; $i < $le; $i++) {
      // ASCII.
      if (($ord = ord($str{$i})) < 128) {
        if ((++$n) > $length) {
          return substr($str, 0, $i);
        }
        $leading = FALSE;
      }
      // Continuation char.
      elseif ($ord < 192) { // continuation char
        $leading = FALSE;
      }
      // Leading char.
      else {
        // A sequence of leadings only counts as a single.
        if (!$leading) {
          if ((++$n) > $length) {
            return substr($str, 0, $i);
          }
        }
        $leading = TRUE;
      }
    }
    return $str;
  }

  /**
   * Truncate multibyte safe until ASCII length is equal to/less than arg
   * length.
   *
   * Does not check if arg $str is valid UTF-8.
   *
   * @param string $str
   * @param integer $length
   *   Byte length (~ ASCII char length).
   *
   * @return string
   */
  protected static function truncateBytes($str, $length) {
    if (strlen($str) <= $length) {
      return $str;
    }

    // Truncate to UTF-8 char length (>= byte length).
    $str = static::mb_substr($str, 0, $length);
    // If all ASCII.
    if (($le = strlen($str)) == $length) {
      return $str;
    }

    // This algo will truncate one UTF-8 char too many,
    // if the string ends with a UTF-8 char, because it doesn't check
    // if a sequence of continuation bytes is complete.
    // Thus the check preceding this algo (actual byte length matches
    // required max length) is vital.
    do {
      --$le;
      // String not valid UTF-8, because never found an ASCII or leading UTF-8
      // byte to break before.
      if ($le < 0) {
        return '';
      }
      // An ASCII byte.
      elseif (($ord = ord($str{$le})) < 128) {
        // We can break before an ASCII byte.
        $ascii = TRUE;
        $leading = FALSE;
      }
      // A UTF-8 continuation byte.
      elseif ($ord < 192) {
        $ascii = $leading = FALSE;
      }
      // A UTF-8 leading byte.
      else {
        $ascii = FALSE;
        // We can break before a leading UTF-8 byte.
        $leading = TRUE;
      }
    } while($le > $length || (!$ascii && !$leading));

    return substr($str, 0, $le);
  }

  /**
   * Convert number to string avoiding E-notation for numbers outside system
   * precision range.
   *
   * @param float|integer $num
   *
   * @return string
   */
  protected static function numberToString($num) {
    static $precision;
    if (!$precision) {
      $precision = pow(10, (int)ini_get('precision'));
    }
    // If within system precision, just string it.
    return ($num > -$precision && $num < $precision) ? ('' . $num) : number_format($num, 0, '.', '');
  }

  /**
   * @return string
   */
  protected static function docRoot() {
    return getcwd();
  }

  /**
   * Get config var.
   *
   * This implementation attempts to get from server environment variables.
   * And it only supports the domain 'inspect'.
   *
   *  Server environment variable names used:
   *  - lib_simplecomplex_inspect_truncate
   *  - lib_simplecomplex_inspect_trace_limit
   *  - lib_simplecomplex_inspect_paths
   *  - lib_simplecomplex_inspect_exectime_percent
   *  - lib_simplecomplex_inspect_session_counters
   *  - lib_simplecomplex_inspect_fronttoback_sevmax
   *  - lib_simplecomplex_inspect_output_max
   *  - lib_simplecomplex_inspect_file_path
   *
   * @param string $domain
   * @param string $name
   * @param mixed $default
   *   Default: NULL.
   *
   * @return mixed
   */
  protected static function configGet($domain, $name, $default = NULL) {
    // Environment vars.
    return ($val = getenv('lib_simplecomplex_inspect_' . $name)) !== FALSE ? $val : $default;
  }

  /**
   * Set and save configuration variable.
   *
   * This implementation does nothing, extending class should override.
   *
   * @param string $domain
   * @param string $name
   * @param mixed $value
   */
  protected static function configSet($domain, $name, $value) {
    // Save where?
  }

  /**
   * @param string $name
   *
   * @return string|NULL
   */
  protected static function cookieGet($name) {
    return isset($_COOKIE[$name]) ? $_COOKIE[$name] : NULL;
  }

  /**
   * @param string $name
   * @param mixed $value
   * @param integer $expire
   *   Default: zero (~ session).
   * @param boolean $httponly
   *   Default: FALSE.
   */
  protected static function cookieSet($name, $value, $expire = 0, $httponly = FALSE) {
    setcookie(
      $name, '' . $value, $expire,
      '/', '', !empty($_SERVER['HTTPS']), $httponly
    );
  }

  /**
   * @return integer|string
   */
  protected static function userId() {
    return 0;
  }

}
