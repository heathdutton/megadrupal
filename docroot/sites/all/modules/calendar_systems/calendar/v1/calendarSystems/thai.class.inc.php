<?php

/**
 * 
 */
class cmfcCalendarV1Thai extends cmfcCalendarV1 implements cmfcCalendarV1Plugin {
  var $_monthsName = array(
    '1' => 'มกราคม',
    '2' => 'กุมภาพันธ์',
    '3' => 'มีนาคม',
    '4' => 'เมษายน',
    '5' => 'พฤษภาคม',
    '6' => 'มิถุนายน',
    '7' => 'กรกฎาคม',
    '8' => 'สิงหาคม',
    '9' => 'กันยายน',
    '10' => 'ตุลาคม',
    '11' => 'พฤศจิกายน',
    '12' => 'ธันวาคม',
  );
  
  var $_monthsShortName = array(
    '1' => 'ม.ค.',
    '2' => 'ก.พ.',
    '3' => 'มี.ค.',
    '4' => 'เม.ย.',
    '5' => 'พ.ค.',
    '6' => 'มิ.ย.',
    '7' => 'ก.ค.',
    '8' => 'ส.ค.',
    '9' => 'ก.ย.',
    '10' => 'ต.ค.',
    '11' => 'พ.ย.',
    '12' => 'ธ.ค.',
  );
  
  var $_weeksName = array(
    '0' => 'อาทิตย์',
    '1' => 'จันทร์',
    '2' => 'อังคาร',
    '3' => 'พุธ',
    '4' => 'พฤหัสบดี',
    '5' => 'ศุกร์',
    '6' => 'เสาร์',
  );
  
  var $_weeksShortName = array(
    '0' => 'อา',
    '1' => 'จ',
    '2' => 'อ',
    '3' => 'พ',
    '4' => 'พฤ',
    '5' => 'ศ',
    '6' => 'ส',
  );
  
  var $_weekDaysHoliday = array(6, 0);

  function timestampToStr($format, $timestamp = NULL) {
    if (is_null($timestamp)) {
      $timestamp = time(); //$this->phpTime();
    }
    return $this->date($format, $timestamp);
  }
  
  function strToTimestamp($string) {
    $date = explode(' ', $string);
    $date_parts = explode('-', $date[0]);
    $date_parts[0] = $date_parts[0] - 543;
    $date[0] = implode('-', $date_parts);
    $date = implode(' ', $date);
    return strtotime($date);
  }
  
  function timestampToInfoArray($timestamp = NULL) {
    if (is_null($timestamp)) $timestamp = $this->phpTime();
    $info = $this->phpGetDate($timestamp);
    
    $info['month'] = $info['mon'];
    $info['day'] = $info['mday'];
    
    $info['monthName'] = $this->getMonthName($info['month']);
    $info['monthShortName'] = $this->getMonthShortName($info['month']);
    
    $info_timestamp = $this->infoArrayToTimestamp(array(
      'year' => $info['year'],
      'month' => $info['month'],
      'day' => 1,
    ));
    $info['monthFirstDayWeekday'] = $this->phpDate('w', $info_timestamp) + 1;
    $info['monthDaysNumber'] = $this->phpDate('t', $timestamp);
    
    $info['weekday'] = $info['wday'];
    $info['weekdayName'] = $this->getWeekName($info['weekday']);
    $info['weekdayShortName'] = $this->getWeekShortName($info['weekday']);
    
    return $info;
  }
  
  function infoArrayToTimestamp($info) {
    return parent::mktime(0, 0, 0, $info['month'], $info['day'], $info['year']);
  }
  
  function dateDiff($first, $second) {
    $first_date = explode('-', $first);
    $first_date = parent::mktime(0, 0, 0, $first_date[1], $first_date[2], $first_date[0]);
    
    $second_date = explode('-', $second);
    $second_date = parent::mktime(0, 0, 0, $second_date[1], $second_date[2], $second_date[0]);
    
    $totalasec = $second_date - $first_date;
    return $totalday = round($totalasec/86400);
  }
  
  function date($format, $timestamp = NULL) {
    if (is_null($timestamp) || $timestamp == '') $timestamp = $this->phpTime();
    $value = $this->phpDate($format, $timestamp);

    $letters = preg_split('//', $format);
    array_shift($letters);
    array_pop($letters);

    $year = $this->phpDate('Y', $timestamp) + 543;
    $output = '';
	$prvLetter = '';
    foreach ($letters as $letter) {
      if ($letter == "\\") {
        $prvLetter = $letter;
        continue;
      } elseif ($prvLetter == "\\") {
        $output .= $letter;
        $prvLetter = $letter;
        continue;
      }
      $prvLetter = $letter;
      switch ($letter) {
        case 'D': $output .= $this->getWeekShortName($this->phpDate('w', $timestamp)); break;
        case 'l': $output .= $this->getWeekName($this->phpDate('w', $timestamp)); break;
        case 'S': $output .= ''; /* In Thai has no suffix.*/ break;
        case 'F': $output .= $this->getMonthName($this->phpDate('n', $timestamp)); break;
        case 'M': $output .= $this->getMonthShortName($this->phpDate('n', $timestamp)); break;
        case 'Y': $output .= $year; break;
      case 'y': 
          $output .= substr((string)$year, 2);
      break;
        case 'U': $output .= $this->phpTime(); break;
        case ' ': $output .= ' '; break;
        default: $output .= $this->phpDate($letter, $timestamp); break;
      }
    }
    return $output;
  }
  
  function smartGet($format, $timestamp = NULL) {
    return $this->date($format, $timestamp);
  }

  function fromGregorian($year, $month, $day) {
    return NULL;
  }

  function toGregorian($year, $month, $day) {
    return array(((int)$year) - 543, $month, $day);
  }

  /**
  * accept array,timestamp and string as input datetime in jalali
  * or gregorian format and convert it to timestamp
  * Implementation of PHP strtotime function
  */
  function strtotime($value) {
    return NULL;
  }

  function isDateValid($month, $day, $year) {
    $month = (int) $month;
    $day = (int) $day;
    $year = ((int) $year) - 543;
    $timestamp = parent::mktime(10, 10, 10, $month, $day, $year);
    if ($month < 1 || $month > 12) {
      return FALSE;
    }
    if ($year < 1970 || $year > date('Y', $timestamp)) {
      return FALSE;
    }
    if ($day < 1 || $day > date('t', $timestamp)) {
      return FALSE;
    }
    return TRUE;
  }

  function makeTime($hour="",$minute="",$second="",$jmonth="",$jday="",$jyear="")
  {
	if(!$hour && !$minute && !$second && !$jmonth && !$jmonth && !$jday && !$jyear) {
		return $this->phpTime();;
	}
	list( $year, $month, $day ) = $this->toGregorian($jyear, $jmonth, $jday);
	$i=parent::mktime($hour,$minute,$second,$month,$day,$year);
	return $i;
  }

  /**
   * translate number of month to name of month
   */
  function getWeekName($weekNumber) {
    return $this->_weeksName[$weekNumber];
  }
      
  function getWeekShortName($weekNumber){   
    return $this->_weeksShortName[$weekNumber];
  }
      
  /**     
   * translate number of month to name of month
   */
  function getMonthName($month) {   
    return $this->_monthsName[$month];
  }

  function getMonthShortName($month){   
    return $this->_monthsShortName[$month];
  }
  
  /**
  * Get list of translatable strings
  */
  function getStrings() {
    $stringGroups = array(
      'monthsName' => array('th'=>$this->_monthsName),
      'monthsShortName' => array('th'=>$this->_monthsShortName),
      'weeksName' => array('th'=>$this->_weeksName),
      'weeksShortName' => array('th'=>$this->_weeksShortName),
    );
    return $stringGroups;
  }
}