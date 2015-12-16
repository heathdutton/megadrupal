<?php
/**
 * @file
 * Provides an API to work with dates.
 */

/**
 * Default behaviour of datex
 */
define('DATEX_USE_INTL', FALSE);

function _datex_autoloader($class) {
  if (substr($class, 0, 5) == 'Datex') {
    require_once $class . '.inc';
  }
}

if (!defined('DATEX_AUTOLOAD')) {
  spl_autoload_register('_datex_autoloader');
  define('DATEX_AUTOLOAD', TRUE);
}

/**
 * Default translator is set to t() for Drupal.
 *
 * Should return name of function responsible for translating names.
 */
function _datex_get_translator() {
  return '_datex_t';
}

/**
 * Drupal translator wrapper.
 *
 * For registering new translator, You should leave this function untouched
 * and change _datex_get_translator().
 */
function _datex_t($string) {
  return t($string, array(), array('context' => 'datex'));
}

/**
 * A calendar class extending DatexObject should implement this interface
 * to function correctly. An example implemention is DatexJalali.
 */
interface DatexCalendarIterface {
  /**
   * Methos responsible for converting Gregorian date to localized date.
   *
   * @return array
   *   Array containing converted date, Array should be indexed from 0 to 7 in
   *   this order: year, month, day, hour, minute, month, day of year (number 
   *   of days passed since day 1 of that year), day of week. Day of week MUST
   *   be from 1 to 7.
   */
  public static function convert($timestamp, $offset = 0);

  /**
   * Opposite of DatexCalendarIterface::convert(). 
   */
  public static function toGregorian($year = 0, $month = 0, $day = 0);

  /**
   * Will calculate whether the localized year is leap year or not. 
   */
  public static function isLeap($year);

  /**
   * Returns various information used by DatexObject.
   *
   * For an example look at DatexJalali.
   *
   * Required information are:
   *   intl_args: array of arguments passed to IntlDateFormatter.
   *   First, second, third and fifth arguments of IntlDateFormatter 
   *   constructor should be put in array in order.
   *
   *   numbers: Associative array of localized numbers in calendar's language. 
   *   from 0 to 9 in array.
   *
   *   names: Name of dates.
   *
   *   month_days: Number of days in each month of year, NOT counting leap 
   *   years.
   */
  public function getInfo($name);

  /**
   * To override format charachters DatexObject can't or shouldn't handle.
   *
   * Shoud return an associative array keyed by date format charachter and it's
   * value as formatted date (the date is available from the onject instance).
   */
  function calendarFormat();

  /**
   * Day of week calculated from given year and day of year.
   */
  public static function dayOfWeek($year, $dayOfYear = 0);

  /**
   * Provides an array containing default arguments for INTL formatter.
   */
  function getIntlArgs();

  /**
   * Helper function to make up for missing granularities.
   *
   * While converting dates, If some granularities are missing, It's needed to
   * add some amount to timestamp, To have some final conversion correct.
   * You see, Date module with only year, will store time as 2013-1-1, But it's
   * not obvious user meant 1392 or 1391?
   * Why? half of 2013 is 1392 (from march to december) and half is 1391, (from
   * january to april). But this month is lost after it's converted to 2013-1-1.
   * A fixed amount of time, (equal to choped off) amount of time, needs to be
   * added each time. I hope I could explain it well! I guess not.
   *
   * Special handling is needed for when day is present and month is not and is
   * not supported by this method.
   */
  function fixMissingGranularities(array $granuls);
}

/**
 * Base class for localized DateTime.
 */
abstract class DatexObject extends DateTime {
  /**
   * Number of days in each Gregorian month. Leap year is not counted.
   */
  protected static $daysInGregorianMonth = array(31, 28, 31, 30, 31, 30,
    31, 31, 30, 31, 30, 31);

  /**
   * Default format string.
   */
  protected $formatString;

  /**
   * IntlDateFormatter instance used by object instance.
   *
   * @TODO share with objects.
   */
  protected $intlFormatter;

  /**
   * Whether to use PHP-Intl or not.
   */
  protected $useINTL = DATEX_USE_INTL;

  /**
   * Result of formats handled by extended class.
   */
  protected $calendarFormats = array();

  /**
   * Localized year value.
   */
  protected $year;

  /**
   * Localized month value.
   */
  protected $month;

  /**
   * Localized day value.
   */
  protected $day;

  /**
   * Constructor for DatexObject.
   *
   * @param mixed $datetime
   *   For list of accepted values see objectFromDate().
   * @param mixed $tz
   *   DateTimeZone to use, Can be DateTimeZone object or name string of it.
   * @param string $format
   *   Default format string.
   * @param bool $use_intl
   *   Whether to use PHP-Intl or not.
   */
  public function __construct($datetime = NULL, $tz = NULL, $format = '', $use_intl = DATEX_USE_INTL) {
    parent::__construct();

    $this->setUseINTL($use_intl);
    $this->setDatetime($datetime, $tz);
    $this->setFormat($format);

    return $this;
  }

  /**
   * Magic Function toString.
   */
  public function __toString() {
    return $this->format();
  }

  /**
   * Determines wether PECL package PHP-Intl is available or not.
   */
  public static function hasINTL() {
    return class_exists('IntlDateFormatter');
  }

  /**
   * Set whether to use PHP-Intl or not.
   *
   * Checks and makes sure PHP-Intl is available.
   */
  public function setUseINTL($use = NULL) {
    if (!$this->hasINTL()) {
      $this->useINTL = FALSE;
      return;
    }

    if ($use || ($use === NULL && DATEX_USE_INTL)) {
      $this->setIntlArgs();
      $this->useINTL = TRUE;
      $this->createINTLFormatter($this->getTimezone());
    }
    else {
      $this->useINTL = FALSE;
    }

    return $this->useINTL;
  }

  /**
   * Makes sure a tz is object, Not string ID of it.
   */
  public static function getTzObject($tz = NULL) {
    return is_string($tz) ? new DateTimeZone($tz) : $tz;
  }

  /**
   * Makes sure $tz is string, Not tz object.
   */
  public static function getTzString($tz) {
    return ($tz instanceof DateTimeZone) ? $tz->getName() : $tz;
  }

  /**
   * Similar to DateTime::format().
   *
   * If format is not given internal format string set by constructor will be
   * used.
   */
  public function format($format = NULL) {
    if ($format === NULL) {
      $format = $this->formatString;
    }

    return ($this->useINTL) ? $this->formatINTL($format) : $this->formatPHP($format);
  }

  /**
   * Set date-time of object instance by extracting timestamp from given date.
   *
   * Also calls conversion methods so internal values are set.
   */
  public function setDatetime($datetime, $tz = NULL) {
    $this->checkSetTimezone($tz);

    $date = $this->objectFromDate($datetime);
    $this->setTimestamp($date->getTimestamp());

    return $this;
  }

  /**
   * Same as DateTime::setTimestamp but also resets the object.
   */
  public function setTimestamp($timestamp) {
    parent::setTimestamp($timestamp);
    $this->reset();
  }

  /**
   * Set date-time of object instance by extracting timestamp from given date.
   *
   * Treat datetime as a Gregorian date.
   */
  public function xsetDatetime($datetime, $tz = NULL) {
    $this->checkSetTimezone($tz);

    $date = $this->xobjectFromDate($datetime);
    $this->setTimestamp($date->getTimestamp());

    return $this;
  }

  /**
   * Same as DateTime::setDate().
   */
  public function setDate($year = NULL, $month = NULL, $day = NULL) {
    list($year, $month, $day) = $this->toGregorian($year, $month, $day);
    $this->xsetDate($year, $month, $day);

    return $this;
  }

  /**
   * Set default format string of object.
   */
  public function setFormat($format) {
    $this->formatString = $format;
    return $this;
  }

  /**
   * Returns format string set by setFormat.
   */
  public function getFormat() {
    return $this->formatString;
  }

  /**
   * Sets Time Zone of internal date object.
   *
   * Accepts a DateTimeZone Object or an string representing a timezone.
   */
  public function setTimezone($timezone) {
    $timezone = $this->getTzObject($timezone);
    parent::setTimezone($timezone);
    $this->reset();

    return $this;
  }

  /**
   * Check to see if given timezone is not NULL, Then if it's so, set it.
   */
  protected function checkSetTimezone($tz) {
    if ($tz) {
      $this->setTimezone($tz);
    }

    return $this;
  }

  /**
   * Same as DateTime::format().
   */
  public function xformat($format = NULL) {
    if ($format === NULL) {
      $format = $this->formatString;
    }

    return parent::format($format);
  }

  /**
   * Same as DateTime::setDate().
   */
  public function xsetDate($year, $month, $day) {
    return parent::setDate($year, $month, $day);
    $this->reset();

    return $this;
  }

  /**
   * When new date or time is set on object, This method must be called.
   *
   * Gives a chance to extended classes to recalculate converted date from new 
   * date.
   */
  public function reset($reset_datetime = NULL) {
    // If no timestamp, Then just recalculate date and set timezone.
    if ($reset_datetime !== NULL) {
      $this->setTimestamp(time());
    }

    $this->tzOffset = $this->xformat('Z');
    $this->setConvert();
    $this->setIsLeap();

    return $this;
  }

  /**
   * Returns an object containing first day of month.
   */
  public function monthFirstDay() {
    $date = clone $this;
    $date->setDate(NULL, NULL, 1);

    return $date;
  }

  /**
   * Returns an object containing last day of month.
   */
  public function monthLastDay() {
    $date = clone $this;
    $date->setDate(NULL, NULL, $date->format('t'));
    return $date;
  }

  /**
   * Returns this date object granularities, put in an array.
   */
  public function toArray() {
    return array(
      'year' => $this->format('Y'),
      'month' => $this->format('n'),
      'day' => $this->format('j'),
      'hour' => $this->format('H'),
      'minute' => $this->format('i'),
      'second' => $this->format('s'),
      'timezone' => $this->format('e'),
    );
  }

  /**
   * Returns amount of time difference to another date object
   *
   * @TODO properly implement.
   */
  public function difference(DatexObject $date2_in, $measure = 'seconds', $absolute = TRUE) {
    // Create cloned objects or original dates will be impacted by the
    // date_modify() operations done in this code.
    $date1 = clone $this;
    $date2 = clone $date2_in;

    $diff = $date2->format('U') - $date1->format('U');
    if ($diff == 0) {
      return 0;
    }
    elseif ($diff < 0 && $absolute) {
      // Make sure $date1 is the smaller date.
      $temp = $date2;
      $date2 = $date1;
      $date1 = $temp;
      $diff = $date2->format('U') - $date1->format('U');
    }

    $year_diff = intval($date2->format('Y') - $date1->format('Y'));

    switch ($measure) {
      case 'seconds':
        return $diff;

      case 'minutes':
        return $diff / 60;

      case 'hours':
        return $diff / 3600;

      case 'years':
        return $year_diff;

      case 'months':
        $item1 = $date1->format('n');
        $item2 = $date2->format('n');
        if ($year_diff == 0) {
          return intval($item2 - $item1);
        }
        else {
          $item_diff = 12 - $item1;
          $item_diff += intval(($year_diff - 1) * 12);
          return $item_diff + $item2;
        }
        break;

      case 'days':
        break;
        $item1 = $date1->format('z');
        $item2 = $date2->format('z');
        if ($year_diff == 0) {
          return intval($item2 - $item1);
        }
        else {
          $item_diff = date_days_in_year($date1) - $item1;
          for ($i = 1; $i < $year_diff; $i++) {
            date_modify($date1, '+1 year');
            $item_diff += date_days_in_year($date1);
          }
          return $item_diff + $item2;
        }
        break;

      case 'weeks':
      default:
        break;
    }

    return NULL;
  }

  /**
   * Same as DatexObject toArray but in Gregorian format.
   */
  public function xtoArray() {
    return array(
      'year' => $this->xformat('Y'),
      'month' => $this->xformat('n'),
      'day' => $this->xformat('j'),
      'hour' => $this->xformat('H'),
      'minute' => $this->xformat('i'),
      'second' => $this->xformat('s'),
      'timezone' => $this->xformat('e'),
    );
  }

  /**
   * Converts date format string (like 'Y-m-d') to it's PHP-Intl equivilant.
   */
  public static function phpToIntl($format) {
    static $format_map = array(
      'd' => 'dd',
      'D' => 'EEE',
      'j' => 'd',
      'l' => 'EEEE',
      'N' => 'e',
      'S' => 'LLLL',
      'w' => '',
      'z' => 'D',
      'W' => 'w',
      'm' => 'MM',
      'M' => 'MMM',
      'F' => 'MMMM',
      'n' => 'M',
      't' => '',
      'L' => '',
      'o' => 'yyyy',
      'y' => 'yy',
      'Y' => 'YYYY',
      'a' => 'a',
      'A' => 'a',
      'B' => '',
      'g' => 'h',
      'G' => 'H',
      'h' => 'hh',
      'H' => 'HH',
      'i' => 'mm',
      's' => 'ss',
      'u' => 'SSSSSS',
      'e' => 'z',
      'I' => '',
      'O' => 'Z',
      'P' => 'ZZZZ',
      'T' => 'v',
      'Z' => '',
      'c' => '',
      'r' => '',
      'U' => '',
      ' ' => ' ',
      '-' => '-',
      '.' => '.',
      '-' => '-',
      ':' => ':',
    );

    $replace_pattern = '/[^ \:\-\/\.\\\\dDjlNSwzWmMFntLoyYaABgGhHisueIOPTZcrU]/';
    return strtr(preg_replace($replace_pattern, '', $format), $format_map);
  }

  /**
   * Format datetime using PHP-Intl.
   */
  protected function formatINTL($format) {
    $this->intlFormatter->setPattern($this->phpToIntl($format));
    $date = $this->intlFormatter->format($this->getTimestamp());

    // Replace localized number charachters with english equivalant.
    // @TODO check to see if Intl has support of this by itself?
    return self::decor($date, FALSE);
  }

  /**
   * Creates a DateTime object containing date extracted from $date.
   */
  public function xobjectFromDate($date = NULL, $tz = NULL) {
    if (is_object($date)) {
      $date = clone $date;
    }
    else {
      if (is_numeric($date)) {
        $date = '@' . $date;
      }
      elseif (is_array($date)) {
        $now = getdate();
        $year = isset($date['year']) ? intval($date['year']) : $now['year'];
        $month = isset($date['month']) ? intval($date['month']) : $now['mon'];
        $day = isset($date['day']) ? intval($date['day']) : $now['mday'];
        $hour = isset($date['hour']) ? intval($date['hour']) : $now['hours'];
        $minute = isset($date['minute']) ? intval($date['minute']) : $now['minutes'];
        $second = isset($date['second']) ? intval($date['second']) : $now['seconds'];

        $date = '@' . mktime($hour, $minute, $second, $month, $day, $year);
      }
      elseif ($date == NULL) {
        $date = 'now';
      }

      $date = new DateTime($date);
    }

    // For anyone reading this comment: Passing a DateTimeZone to datetime
    // constructor has no effect on it! You MUST use setTimezone to set a
    // tz on the stupid object.
    // Tested on PHP 5.4.15 (built: May 12 2013 13:11:23) Archlinux.
    if (isset($tz)) {
      $tz = $this->getTzObject($tz);
      $date->setTimeZone($tz);
    }

    return $date;
  }

  /**
   * Created datex object from a localized date.
   *
   * The only way to create an object from localized date is using an array,
   * Other type of dates have no difference in localized format such as 
   * timestamp. String are not supported yet till parser() is fully 
   * implemented.
   */
  public function objectFromDate($date, $tz = NULL) {
    if (is_array($date)) {
      foreach (array('year', 'month', 'day') as $name) {
        if (!isset($date[$name])) {
          $date[$name] = 0;
        }
      }
      list($date['year'], $date['month'], $date['day']) 
        = $this->toGregorian($date['year'], $date['month'], $date['day']);
    }

    return $this->xobjectFromDate($date, $tz);
  }

  /**
   * Create an IntlDateFormatter fot this object.
   */
  protected function createINTLFormatter($tz = NULL) {
    $args = $this->intlArgs;
    if (!$tz) {
      $tz = date_default_timezone_get();
    }
    
   $this->intlFormatter = new IntlDateFormatter(
     $args[0],
     $args[1],
     $args[2],
     $this->getTzString($tz),
     $args[3]
   );
  }

  /**
   * Use php to format date
   */
  protected function formatPHP($format) {
    static $t = FALSE;
    if (!$t) {
      $t = _datex_get_translator();
    }

    $names = $this->getInfo('names');
    $monthDays = $this->getInfo('month_days');
    $this->setConvert();
    $this->setIsLeap();

    // Should contain N, w, W, z, S, M,
    $this->setCalendarFormats();

    $ampm = $this->xformat('a');

    // A series of calls to str replace can not be used since format may
    // contain \ charachter which should not be replaced.
    $formatted = '';
    for ($i = 0; $i < strlen($format); $i++) {
      $f = $format[$i];

      // If extended class wants to override some formats, It should put it
      // in here.
      if (isset($this->calendarFormats[$f])) {
        $formatted .= $this->calendarFormats[$f];
      }
      else {
        switch ($f) {
          case '\\':
            $date .= $format[++$i];
          case 'd':
            $formatted .= sprintf('%02d', $this->day);
            break;

          case 'q':
            $formatted .= $t($names['day_abbr'][$this->dayOfWeek]);
            break;

          case 'D':
            $formatted .= $t($names['day_abbr_short'][$this->dayOfWeek]);
            break;

          case 'j':
            $formatted .= intval($this->day);
            break;

          case 'l':
            $formatted .= $t($names['day'][$this->dayOfWeek]);
            break;

          case 'S':
            $formatted .= $t($names['order'][$this->day - 1]);
            break;

          case 'W':
            $value_w = strval(ceil($this->dayOfYear / 7));
            $formatted = $formatted . $value_w;
            break;

          case 'z':
            $formatted = $this->dayOfYear;
            break;

          case 'M':
          case 'F':
            $formatted .= $t($names['months'][$this->month - 1]);
            break;

          case 'm':
            $formatted .= sprintf('%02d', $this->month);
            break;

          case 'n':
            $formatted .= intval($this->month);
            break;

          case 't':
            $formatted .= ($this->isLeap && $this->month == 12) ? 30 : $month_days[$this->month - 1];
            break;

          case 'L':
            $formatted .= $this->isLeap ? 1 : 0;
            break;

          case 'Y':
            $formatted .= $this->year;
            break;

          case 'y':
            $formatted .= substr($this->year, 2, 4);
            break;

          case 'o':
            $formatted .= $this->year;
            break;

          case 'a':
            $formatted .= $t($names['ampm'][$ampm]);
            break;

          case 'A':
            $formatted .= $t($names['ampm'][$ampm]);
            break;

          case 'c':
            $formatted .= "$this->year - $this->month - {$this->day}T";
            $formatted .= $this->xformat('H:i:sP');
            break;

          case 'r':
            $formatted .= $t($names['day_abbr'][$fw]) . ', ' . $cd . ' ' .
              $t($names['months'][$cm]) . ' ' . $cy . $this->xformat('H:i:s P');
            break;

          default:
            // Any format charachter not handled by Datex or extended class,
            // Will be handled by DateTime.
            $formatted .= ctype_alpha($format[$i]) ? $this->xformat($format[$i]) : $format[$i];
            break;
        }
      }
    }

    return $formatted;
  }

  /**
   * Call calendarFormat() on extended classes but hide storage details.
   *
   * Get extended classes opportunity to format date formats which Datex should
   * not handle.
   */
  protected function setCalendarFormats() {
    $this->calendarFormats = $this->calendarFormat();
  }

  /**
   * Transliterate English numbers to localized numbers and vice versa.
   */
  public function decor($str, $decorate = FALSE) {
    static $en = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    $localized = $this->getInfo('numbers');

    return $decorate ? str_replace($en, $localized, $str) : str_replace($localized, $en, $str);
  }

  /**
   * Call extended class convert and set converted date on object instance,
   */
  protected function setConvert() {
    list(
      $this->year,
      $this->month,
      $this->day, 
      $this->hour,
      $this->minute,
      $this->second,
      $this->dayOfYear,
      $this->dayOfWeek
    ) = $this->convert($this->xformat('U'), $this->tzOffset);

    return $this;
  }

  /**
   * Call extended class isLeap and set isLeap on object instance,
   */
  protected function setIsLeap() {
    $this->isLeap = $this->isLeap($this->year);
    return $this;
  }

  /**
   * A bad method which set's day directly.
   *
   * A must have for when dealing with dates which have been stored as 
   * timestamp according to a format without month but with year and
   * day.
   */
  public function setDayBad($day) {
    $this->day = $day;
    return $this;
  }

  /**
   * Generate arguments passed to INTL formatter.
   */
  public function setIntlArgs($a1 = NULL, $a2 = NULL, $a3 = NULL, $a4 = NULL) {
    $default_intl_args = $this->getIntlArgs();

    $this->intlArgs = array();
    foreach (func_get_args() as $index => $arg) {
      if ($arg === NULL) {
        $this->intlArgs[$index] = $default_intl_args[$index];
      }
      else {
        $this->intlArgs[$index] = $arg;
      }
    }

    $this->intlArgs = $default_intl_args;
  }

  /**
   * Names of date granularities.
   */
  public static function granularityNames() {
    return array('year', 'month', 'day', 'hour', 'minute', 'second');
  }

  /**
   * Helper method to make up for time difference if some granularites are 
   * missing.
   */
  public function fixByGranularities (array $granularities) {
    $this->setTimestamp($this->getTimestamp() + $this->fixMissingGranularities($granularities));
  }
}

/**
 * Factory for datex object.
 */
function datex_factory($locale, $dt = NULL, $tz = NULL, $fmt = NULL, $intl = NULL) {
  switch (strtolower($locale)) {
    case 'fa':
    case 'farsi':
    case 'parsi':
    case 'persian':
    case 'jalali':
    case 'shamsi':
    case 'iran':
    case 'iranian':
      $locale = 'Jalali';
      break;

    default:
      throw new Exception('Datex: Unknown calendar.');
  }

  $datex = 'Datex' . $locale;
  return new $datex($dt, $tz, $fmt, $intl);
}

/**
 * Similar to php date, localized version.
 */
function datex_format($locale, $dt = NULL, $fmt = NULL, $tz = NULL, $intl = NULL) {
  return datex_factory($locale, $dt, $tz, $fmt, $intl)->format();
}

