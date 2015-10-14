<?php
/**
* @author Sina Salek
* @changes
* 	
* @todo
* 	- finding and using persian calendar parent::mktime function
* 	- implementing arabic date
* 	- checking to see if pdf function has a better algorithem than the of jdf
*/

if (!is_callable('div')) {
    function div($a,$b) {
        return (int) ($a / $b);
    }
}

define('CMF_CalendarV1_Ok',true);
define('CMF_CalendarV1_Error',2);
define('CMF_CalendarV1_Does_No_Exsists',3);

if (!class_exists('cmfcClassesCoreStandAlone'))
	trigger_error('cmfcCalendarV1:calendarV1 needs cmfcClassesCoreStandAlone packages/cmf/classesCore.class.inc.php',E_USER_ERROR);

class cmfcCalendarV1 extends cmfcClassesCoreStandAlone{
	var $_name;
	var $_language;
	var $_timeZoneName;
	var $_timeZoneInfo;
	
	var $_defaultError=CMF_CalendarV1_Error;
	var $_messagesValue=array(
        CMF_CalendarV1_Ok	=> 'no error',
        CMF_CalendarV1_Error	=> 'unkown error',
        CMF_CalendarV1_Does_No_Exsists	=> 'template "%internalName%" does not exists',
	);
	
	function __construct($options) {
		$this->setOptions($options);
	}
	
	function getPlugins() {
		$dir = dirname(__FILE__).'/calendarSystems';
	    $dh = opendir($dir);
		$plugins = array();
		while (false !== ($filename = readdir($dh))) {
			if (strpos($filename,'.class.inc.php')!==false) {
			  $plugin = str_replace('.class.inc.php','',$filename);
			  $plugins[$plugin] = array('name'=>$plugin);
			}
		}
		return $plugins;
	}

	static function factory($options) {
		if (isset($options['name'])) {
			require_once(dirname(__FILE__).'/calendarV1.plugin.class.inc.php');
			$className='cmfcCalendarV1'.ucfirst($options['name']);
			$file=dirname(__FILE__).'/calendarSystems/'.$options['name'].'.class.inc.php';
			if (!class_exists($className)) {
			  require_once($file);
			}
			return new $className($options);
		} else {
			return new cmfcCalendarV1(array());
		}
	}
	
	function setOption($name,$value,$merge=false) {
		if ($name=='timeZoneOffset') {
			$this->setTimeZoneOffset($value);
		}
		return parent::setOption( $name,$value,$merge);
	}

	function setTimeZoneOffset($offset) {
		if (!empty($offset) && is_string($offset)) {
			list($hour,$minute)=explode(':',$offset);
			$offset=(abs($hour))*60*60+$minute*60;
			if ($hour<0) {
				$offset=$offset*-1;
			}
		}
		$this->_timeZoneInfo['offset']=$offset;
	}
	
	function infoArrayToTimestamp($arr) {
	}
	
	function timestampToInfoArray($var = NULL) {
	}
	
    function gregorianStrToTimestamp($str) {
    	return strtotime($str);
	}
	
    function timestampToStr($format,$timestamp=null) {
		if (is_null($timestamp)) {
			$timestamp=$this->phpTime();
		}
		return $this->date($format,$timestamp);
	}

	function strToTimestamp($string) {
		return $this->strtotime($string);
	}

	
	/**
	* For supporting timezone , use this class date and time functions like phpDate
	*/
	function countDown($infoArray) {
		//$currentDate=$this->timestampToInfoArray();
		$toTimestamp=$this->infoArrayToTimestamp($infoArray);
		$fromTimestamp=$this->phpTime();
				
		$r=$this->dateTimeDiff($toTimestamp,$fromTimestamp);
		$r['toTimestamp']=$toTimestamp;
		$r['fromTimestamp']=$fromTimestamp;
		return $r;
	}	
	
	
	function infoArrayToInfoArray($array) {
		$ts=$this->infoArrayToTimestamp($array);
		return $this->timestampToInfoArray($ts);
	}
	
	function phpGetDate($timestamp=null) {
		if (is_null($timestamp)) {
			if (!empty($this->_timeZoneInfo)) {
				$timestamp=time()+$this->_timeZoneInfo['offset'];
				$r=array(
					'seconds' => gmdate('s', $timestamp),
					'minutes' => gmdate('i', $timestamp),
					'hours'   => gmdate('H', $timestamp),
					'mday'    => gmdate('d', $timestamp),
					'wday'    => gmdate('w', $timestamp),
					'mon'     => gmdate('m', $timestamp),
					'year'    => gmdate('Y', $timestamp),
					'yday'    => gmdate('z', $timestamp),
					'weekday' => gmdate('l', $timestamp),
					'month'   => gmdate('M', $timestamp),
					'0'       => $timestamp
				);
				
			} else {
				$timestamp=time();
				$r=getdate($timestamp);
			}

		} else {
			$r=getdate($timestamp);
		}
		return $r;
	}

    function phpDate($format, $timestamp=null) {
        if (is_null($timestamp) or empty($timestamp)) {
            if (!empty($this->_timeZoneInfo)) {
                $timestamp=time()+$this->_timeZoneInfo['offset'];
                $r=gmdate($format, $timestamp);
            } else {
                $timestamp=time();
                $r=gmdate($format, $timestamp);
            }
        } else {
            if (!empty($this->_timeZoneInfo)) {
                $r=gmdate($format, $timestamp);
            } else {
                $r=date($format, $timestamp);
            }
        }
        return $r;
    }
	
	function phpTime() {
		if (!empty($this->_timeZoneInfo)) {
			$timestamp=time()+$this->_timeZoneInfo['offset'];
		} else {
			$timestamp=time();
		}
		return $timestamp;
	}
	
	function getYmdwMonthAsNavigationalArray($options) {
			
		if (empty($options['columnsHorizontal'])) {
			$options['columnsHorizontal']=6;
		}
		if (empty($options['columnsVertical'])) {
			$options['columnsVertical']=5;
		}
		
		if (isset($options['secondaryCalendar'])) {
			$secondaryCalendar=self::factory(array(
				'name'=>$options['secondaryCalendar']
			));
		}
	    
		$table=array();
		
		$currentMonth=$this->timestampToInfoArray();
		if (is_null($options['year'])) {
			$options['year']=$currentMonth['year'];
		}
		if (is_null($options['month'])) {
			$options['month']=$currentMonth['month'];
		}
		if (is_null($options['day'])) {
			$options['day']=1;
		} else {
			$selectedDay=$options['day'];
		}
		
		$previousMonth=$this->infoArrayToInfoArray(array(
			'year'=>$options['year'],
			'month'=>$options['month']-1,
			'day'=>$options['day'],
		));

		$activeMonth=$this->infoArrayToInfoArray(array(
			'year'=>$options['year'],
			'month'=>$options['month'],
			'day'=>$options['day'],
		));

		
		$nextMonth=$this->infoArrayToInfoArray(array(
			'year'=>$options['year'],
			'month'=>$options['month']+1,
			'day'=>$options['day'],
		));
	
	    
	    $table['currentMonth']=$currentMonth;
	    $table['activeMonth']=$activeMonth;
	    $table['nextMonth']=$nextMonth;
	    $table['previousMonth']=$previousMonth;
		
		foreach ($this->_weeksName as $x=>$weekName) {
		//for ($x=1;$x<=$options['columnsHorizontal'];$x++) {
			$table['weekDays'][$x]=array(
				'number'=>$x,
				'name'=>$weekName,
				'shortName'=>''
			);
		}

		$dayNumber=1;
		$monthStarted=false;
		for ($y=1;$y<=$options['columnsVertical'];$y++) {
			for ($x=0;$x<=$options['columnsHorizontal'];$x++) {
				$value=array();
				//echo $activeMonth['monthFirstDayWeekday'].'-'.$x.'<br />';
				if (($x==$activeMonth['monthFirstDayWeekday'] or $y>1 or $monthStarted==true) and $dayNumber<=$activeMonth['monthDaysNumber']) {
					$monthStarted=true;
					$value['status']=array();
					$value['day']=$dayNumber;
					if ($dayNumber==$selectedDay) {
						$value['status'][]='selected';
					}
					if ($dayNumber==$currentMonth['day'] and $currentMonth['month']==$activeMonth['month'] and $currentMonth['year']==$activeMonth['year']) {
						$value['status'][]='today';
					}
					if (in_array($x,$this->_weekDaysHoliday)) {
						$value['status'][]='holiday';
					}
					
					if (isset($secondaryCalendar)) {
						$__activeMonth=$activeMonth;
						$__activeMonth['day']=$dayNumber;
						$value['secondaryCalendar']=$secondaryCalendar->timestampToInfoArray($this->infoArrayToTimestamp($__activeMonth));
					}
					
					if ($y==5 and $x==6 and $activeMonth['monthDaysNumber']>($dayNumber)) {
						$options['columnsVertical']++;
					}
					$dayNumber++;
				}
				$table['days'][$y][$x]=$value;
			}
		}
		
		return $table;
	}
	
    /**
    * Date    : 15-12-2003.
    *    
    * Ref: Dates go in "2003-12-31".
    * Ref: Times go in "12:59:13".
    * Ref: parent::mktime(HOUR,MIN,SEC,MONTH,DAY,YEAR).
    *   
    * Splits the dates into parts, to be reformatted for parent::mktime.
    * $first_datetime = getdate($first_datetime);
    * $second_datetime = getdate($second_datetime);
    *    
    * makes the dates and times into unix timestamps.
    * $first_unix  = parent::mktime($first_datetime['hours'], $first_time_ex[1], $first_time_ex[2], $first_date_ex[1], $first_date_ex[2], $first_date_ex[0]);
    * $second_unix  = parent::mktime($second_time_ex[0], $second_time_ex[1], $second_time_ex[2], $second_date_ex[1], $second_date_ex[2], $second_date_ex[0]);
    * Gets the difference between the two unix timestamps.
    */
    function dateTimeDiff($endTimestamp,$startTimestamp)
    {
        $timediff = $endTimestamp-$startTimestamp;
        // Works out the days, hours, mins and secs.
        $daysTotal=floor($timediff/(24*60*60));
        $remain=$timediff%(24*60*60);
        $hours=floor($remain/(60*60));
        $remain=$remain%(60*60);
        $mins=floor($remain/(60));
        $remain=$remain%(60);
        $secs=$remain;
        
        // Returns a pre-formatted string. Can be chagned to an array.
        $result['daysTotal']=$daysTotal;
        //$result['days']=$days;
        $result['hours']=$hours;
        $result['minutes']=$mins;
        $result['seconds']=$secs;
        return $result;
    }
    
    /**
     * Placeholder for smartGet function.
     */
    function smartGet($format, $timestamp = NULL) {
      if (!$timestamp) {
        $timestamp = time();
      }

      return $this->phpDate($format, $timestamp);
    }
    
    
    /**
    * convert seconds to days,hours,minuts,seconds as array
    * @param integer $seconds
    * @return array
    */
    function secondsToDays($seconds) {
        $days=intval($seconds/86400);
        $remain=$seconds%86400;
        $hours=intval($remain/3600);
        $remain=$remain%3600;
        $mins=intval($remain/60);
        $secs=$remain%60;
        $r=array(
            'days'=>$days,
            'hours'=>$hours,
            'minutes'=>$mins,
            'seconds'=>$secs
        );
        return $r;
    }
	
	function getStrings() {
		return array();
	}
	
    /**
    * $replacements['00username00']='jafar gholi';
    */
    function replaceVariables($replacements,$text) {
        foreach ($replacements as $needle=>$replacement) {
            $text=str_replace($needle,$replacement,$text);
        }
        return $text;
    }

	/**
	* For backward compatibility with PHP 4.x - 5.1
	*/
	function mktime($hour = NULL, $minute = NULL, $second = NULL, $month = NULL, $day = NULL, $year = NULL, $is_dst = -1) {
	  if (is_null($hour)) $hour = date('J');
	  if (is_null($minute)) $minute = date('i');
	  if (is_null($second)) $second = date('s');
	  if (is_null($month)) $month = date('n');
	  if (is_null($day)) $day = date('j');
	  if (is_null($year)) $year = date('Y');
	  if (class_exists('DateTime')) {
        $date = new DateTime();
        $date->setDate($year, $month, $day);
        $date->setTime($hour, $minute, $second);
		return $date->getTimestamp();
      } else {
	    return mktime($hour, $minute, $second, $month, $day, $year, $is_dst);
	  }
    }

	/**
	 * Get list of translatable strings
	 */
	function getAllStrings() {
		$plugins=$this->getPlugins();
		$strings=array();
		foreach ($plugins as $plugin) {
			$calendar=self::factory($plugin);
			$_strings=$calendar->getStrings();
	        if (isset($_strings['meridiemsName'])) {
                foreach ($_strings['meridiemsName'] as $num=>$var) {
                    $_strings['meridiemsName'][$num][0]=$_strings['meridiemsName'][$num]['am'];
                    $_strings['meridiemsName'][$num][1]=$_strings['meridiemsName'][$num]['pm'];
                    unset($_strings['meridiemsName'][$num]['am']);
                    unset($_strings['meridiemsName'][$num]['pm']);
                }
            }
            if (isset($_strings['meridiemsShortName'])) {
                foreach ($_strings['meridiemsShortName'] as $num=>$var) {
                    $_strings['meridiemsShortName'][$num][0]=$_strings['meridiemsShortName'][$num]['am'];
                    $_strings['meridiemsShortName'][$num][1]=$_strings['meridiemsShortName'][$num]['pm'];
                    unset($_strings['meridiemsShortName'][$num]['am']);
                    unset($_strings['meridiemsShortName'][$num]['pm']);
                }
            }
            $strings=array_merge_recursive($strings,$_strings);
        }
        return $strings;
    }
}