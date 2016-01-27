<?php
/**
 * @file
 * Jalali calendar for datex.
 */

/**
 * Jalali calendar for datex.
 *
 * Original conversion alogirthm by: Amin Saeedi. 
 * Forked from project: Shamsi-Date v4.0 (GPL).
 * On github: github.com/amsa
 * mail: amin.w3dev@gmail.com
 */
class DatexJalali extends DatexObject implements DatexCalendarIterface {
  // _________________________________ DATEX Variables.
  /**
   * number of days in a each Jalali month.
   */
  protected static $monthDays = 
    array(31, 31, 31, 31, 31, 31,30, 30, 30, 30, 30, 29);

  /**
   * Persian equivalanat of english numbers.
   */
  protected static $numbers = 
    array('۰', '۱', '۲', '۳', '۴', '۵', '۶', '۷', '۸', '۹');

  /*
   * Translated names of days, month, ...
   */
  protected static $names = array(
    'months' => array(
      0 => 'Farvardin',
      1 => 'Ordibehesht',
      2 => 'Khordad',
      3 => 'Tir',
      4 => 'Mordad',
      5 => 'Shahrivar',
      6 => 'Mehr',
      7 => 'Aban',
      8 => 'Azar',
      9 => 'Dei',
      10 => 'Bahman',
      11 => 'Esfand',
    ),
    'ampm' => array(
      'am' => 'Ghablazohr',
      'pm' => 'Badazohr',
    ),
    'day_abbr' => array( 'sh', 'y', 'd', 's', 'ch', 'p', 'j'),

    'day_abbr_short' => array( 'sh', 'y', 'd', 's', 'ch', 'p', 'j'),

    'day' => array(
      'Shanbe',
      'Yekshanbe',
      'Doshanbe',
      'Seshanbe',
      'Cheharshanbe',
      'Panjshanbe',
      'Jome',
    ),
    'order' => array(
      'Yekom',
      'Dovom',
      'Sevom',
      'Cheharom',
      'Panjom',
      'Sheshom',
      'Haftom',
      'Hashtom',
      'Nohom',
      'Dahom',
      'Yazdahom',
      'Davazdahom',
      'Sizdahom',
      'Chehardahom',
      'Panzdahom',
      'Shanzdahom',
      'Hefdahom',
      'Hejdahom',
      'Noozdahom',
      'Bistom',
      'Bisto yekom',
      'Bisto dovom',
      'Bisto sevom',
      'Bisto cheharom',
      'Bisto panjom',
      'Bisto sheshom',
      'Bisto haftom',
      'Bisto hashtom',
      'Bisto nohom',
      'Siom',
      'Sio yekom',
      'Sio dovom',
    ),
  );

  // __________________________________ Jalali calculation variables.
  /**
   * Constant used in calculating year.
   *
   * Length of a year Calculated by Khayam is 365.2422 days (approx.); but as
   * the years are getting shorter the new value (valid from year 1380 
   * Per./2000 A.D.) is used instead.
   */
  protected static $khayamYear = 365.24218956;

  /** 
   * Correction to khayami constant.
   *
   * Recent calculations has introduced a correcting factor, which Khayam could
   * not reach. This is used to better adjust length of each year in seconds.
   */
  protected static $khayamYearCorrection = 0.00000006152;

  /**
   * Reference table made by Khayam for leap years.
   */
  protected static $khayamii = array(
    5, 9, 13, 17, 21, 25, 29,
    34, 38, 42, 46, 50, 54, 58, 62,
    67, 71, 75, 79, 83, 87, 91, 95,
    100, 104, 108, 112, 116, 120, 124, 0
  );

  /**
   * Count of days at the end of each Persian month.
   */
  protected static $mCounter = array(
    0, 31, 62, 93, 124, 155,
    186, 216, 246, 276, 306, 336
  );

  // _______________________________________________________ METHODS.
  /**
   * Function to return various info, Used by parent class.
   */
  public function getInfo($info) {
    switch ($info) {
      case 'numbers':
        return self::$numbers;

      case 'names':
        return self::$names;

      case 'month_days':
        return self::$monthDays;

      default:
        throw new Exception('Unknown info.');
    }
  }

  /**
   * Calculate format values which parent won't handle.
   */
  public function calendarFormat() {
    $day_of_week = self::dayOfWeek($this->year, $this->dayOfYear);
    return array(
      'w' => $day_of_week,
      'N' => $day_of_week + 1, 
    );
  }

  /**
   * A parser for Jalali date strings.
   *
   *@TODO implement.
   */
  public static function parser($date, $format) {
    if (!(is_string($date) && is_string($format))) {
      throw new Exception('Datex parser: date or format not valid.');
    }

    $granularities = array();
    $j = $i = 0;
    $format_len = strlen($format);
    do {
      while (!ctype_alpha($format[$i]) && $i < $format_len) {
        $i++;
      }

      while (!ctype_digit($date[$j])) {
        $j++;
      }

      switch ($format[$i]) {
        case 'd':
          $granularities['day'] = $date[$j] . $date[++$j];
          break;

        case 'j':
          if (ctype_digit($date[$j+1])) {
            $granularities['day'] = $date[$j] . $date[++$j];
          }
          else {
            $granularities['day'] = $date[$j];
          }
          break;

        case 'm':
          $granularities['month'] = $date[$j] . $date[++$j];
          break;

        case 'n':
          if (ctype_digit($date[$j+1])) {
            $granularities['month'] = $date[$j] . $date[++$j];
          }
          else {
            $granularities['month'] = $date[$j];
          }
          break;

        case 'y':
        case 'Y':
          $granularities['year'] = $date[$j] . $date[++$j] . $date[++$j] . $date[++$j];

        case 'h':
          $granularities['hour'] = $date[$j] . $date[++$j];
          break;

        case 'g':
          if (ctype_digit($date[$j+1])) {
            $granularities['hour'] = $date[$j] . $date[++$j];
          }
          else {
            $granularities['hour'] = $date[$j];
          }
          break;

        case 'H':
          $granularities['hour'] = $date[$j] . $date[++$j];
          break;

        case 'G':
          if (ctype_digit($date[$j+1])) {
            $granularities['hour'] = $date[$j] . $date[++$j];
          }
          else {
            $granularities['hour'] = $date[$j];
          }
          break;

        case 'i':
          $granularities['minute'] = $date[$j] . $date[++$j];
          break;

        case 's':
          $granularities['second'] = $date[$j] . $date[++$j];
          break;
      }
      $j++;
    } while (++$i < $format_len);

    return $granularities;
  }

  /**
   * Converts a Gregorian date to Jalali.
   */
  public static function convert($timestamp, $offset = 0) {
    $timestamp = $timestamp + $offset;
    // DateTime will handle time.
    $ts = floor($timestamp % 60);
    $tm = floor(( $timestamp % 3600 ) / 60);
    $th = floor(( $timestamp % 86400 ) / 3600);

    $d = floor($timestamp / 86400) + 287;

    $y = floor(
      ($d/ self::$khayamYear) - ($d * self::$khayamYearCorrection)
    );

    $day_of_year = $d - round($y * self::$khayamYear, 0);
    if ($day_of_year == 0) {
      $day_of_year = 366;
    }

    $y += 1348;
    
    $m = 0;
    while ($m < 12 && $day_of_year > self::$mCounter[$m]) {
      $m++;
    }

    $d = $day_of_year - self::$mCounter[$m - 1];

    $day_of_week = self::dayOfWeek($y, $day_of_year);
    return array($y, $m, $d, $th, $tm, $ts, $day_of_year, $day_of_week);
  }

  /**
   * Converts a Jalali date to Gregorian.
   *
   * @TODO replcae with khayam calcualtion method.
   */
  public static function toGregorian($jalali_year = 0, $jalali_month = 0, $jalali_day = 0) {
    $now = self::convert(time());

    $jalali_year = ($jalali_year ? $jalali_year : $now[0]) - 979;
    $jalali_month = ($jalali_month ? $jalali_month : $now[1]) - 1;
    $jalali_day = ($jalali_day ? $jalali_day : $now[2]) - 1;

    $jalali_day_no = 365 * $jalali_year + intval($jalali_year / 33) * 8 + intval((($jalali_year % 33) + 3) / 4);
    for ($i = 0; $i < $jalali_month; ++$i) {
      $jalali_day_no += self::$monthDays[$i];
    }
    $jalali_day_no += $jalali_day;
    $gregorian_day_no = $jalali_day_no + 79;

    $g_year = 1600 + 400 * intval($gregorian_day_no / 146097);
    $gregorian_day_no = $gregorian_day_no % 146097;

    $leap = TRUE;
    if ($gregorian_day_no >= 36525) {
      $gregorian_day_no--;
      $g_year += 100 * intval($gregorian_day_no / 36524);
      $gregorian_day_no = $gregorian_day_no % 36524;

      if ($gregorian_day_no >= 365) {
        $gregorian_day_no++;
      }
      else {
        $leap = FALSE;
      }
    }

    $g_year += 4 * intval($gregorian_day_no / 1461);
    $gregorian_day_no %= 1461;

    if ($gregorian_day_no >= 366) {
      $leap = FALSE;

      $gregorian_day_no--;
      $g_year += intval($gregorian_day_no / 365);
      $gregorian_day_no = $gregorian_day_no % 365;
    }

    for ($i = 0; $gregorian_day_no >= self::$daysInGregorianMonth[$i] + ($i == 1 && $leap); $i++) {
      $gregorian_day_no -= self::$daysInGregorianMonth[$i] + ($i == 1 && $leap);
    }
    $g_month = $i + 1;
    $g_day = $gregorian_day_no + 1;

    return array($g_year, $g_month, $g_day);
  }

  /**
   * Returns non zero if given year is a leap year.
   */
  public static function isLeap($year) {
    $rasad = self::observationYear($year);
    $year = ($rasad % 2820) % 128;
    $is_leap = array_search($year, self::$khayamii);

    return $is_leap;
  }

  /**
   * returns weekday of the specified day of the year. 1 to 7.
   */
  public static function dayOfWeek($year, $dayOfYear = 0) {   
    $rasad = self::observationYear($year);

    $count2820 = floor($rasad / 2820);
    $mod2820 = $rasad % 2820;
    $count128 = floor($mod2820 / 128);
    $mod128 = $mod2820 % 128;

    $leapCount = 0;
    while ($mod128 > self::$khayamii[$leapCount]) {
      $leapCount++;
    }

    $yearStartDay = ( $count2820 + 1 ) * 3 + 
      $count128 * 5 + 
      $mod128 + 
      $leapCount;

    if ($dayOfYear > 0) {
      $dayOfYear--;
    }

    return ($yearStartDay + $dayOfYear) % 7;
  }

  /**
   * calculates the years from reference Observation year.
   */
  public static function observationYear($year) {
    return $year + 2346;
  }

  /**
   * Provides default arguments for INTL Formatter for this locale.
   */
  public function getIntlArgs() {
    static $default_intl_args = array(
      'fa_IR@calendar=persian',
      IntlDateFormatter::FULL,
      IntlDateFormatter::FULL,
      IntlDateFormatter::TRADITIONAL,
    );

    return $default_intl_args;
  }

  /**
   * Make up for missing granularities.
   *
   * Neccessary to get proper result while converting a date with a missing
   * granularity, Look at DatexCalendarInterface::fixGranularities for more
   * info.
   */
  public function fixMissingGranularities(array $granuls) {
    $offset = 0;
    if (!($granuls['hour'] || $granuls['minute'] || $granuls['second'])) {
      // 12 hours.
      $offset = 43200;
    }
    else {
      $offset = 0;
    }
   
    if (!($granuls['month'] || $granuls['day'])) {
      // 6 months.
      $offset += 16070400;
    }
    elseif (!$granuls['day']) {
      // 31 days.
      $offset += 2678400;
    }

    return $offset;
  }
}

