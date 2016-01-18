<?php
/**
* @author Sina Salek
* 
*/

/**
* 
*/
class cmfcCalendarV1Julian extends cmfcCalendarV1 implements cmfcCalendarV1Plugin {  
  
  var $_dateFormats=array(
    'rfc10'=>'Y-m-d H:i:s',
    'ArraySimple'=>''
  );
  
  var $_defaultFormat='rfc10';
  
  var $_monthsName=array(
    '1'=>'January',
    '2'=>'February',
    '3'=>'March',
    '4'=>'April',
    '5'=>'May',
    '6'=>'June',
    '7'=>'July',
    '8'=>'August',
    '9'=>'September',
    '10'=>'October',
    '11'=>'November',
    '12'=>'December'
  );
  
  var $_monthsShortName=array(
    '1'=>'Jan',
    '2'=>'Feb',
    '3'=>'Mar',
    '4'=>'Apr',
    '5'=>'May',
    '6'=>'Jun',
    '7'=>'Jul',
    '8'=>'Aug',
    '9'=>'Sep',
    '10'=>'Oct',
    '11'=>'Nov',
    '12'=>'Dec'
  );
  
  var $_weeksName=array(
    '0'=>'Sunday',//Sunday
    '1'=>'Monday',
    '2'=>'Tuesday',
    '3'=>'Wednesday',
    '4'=>'Thursday',
    '5'=>'Friday',
    '6'=>'Saturday'//Saturday
  );
  
  var $_weeksShortName=array(
    '0'=>'Sun',//Sunday
    '1'=>'Mon',
    '2'=>'Tu',
    '3'=>'We',
    '4'=>'Th',
    '5'=>'Fr',
    '6'=>'Sa'//Saturday
  );
    
  var $_meridiemsName=array(
    'am'=>'AM',
    'pm'=>'PM',
  );
  
  var $_meridiemsShortName=array(
    'am'=>'AM',
    'pm'=>'PM',
  );
  
  var $_weekDaysHoliday=array(6);
  

  
  function timestampToStr($format,$timestamp=null) {
    if (is_null($timestamp)) {
      $timestamp=$this->phpTime();
    }
    parent::timestampToStr($format,$timestamp);
  }
  
  function strToTimestamp($string) {
    parent::strToTimestamp($string);
  }
  
  function timestampToInfoArray($timestamp=null) {
    $arr=$this->phpGetDate($timestamp);
    if (is_null($timestamp)) $timestamp=$this->phpTime();

    list($arr['year'],$arr['month'],$arr['day'])=$this->fromGregorian($arr['year'],$arr['mon'],$arr['mday']);

    $arr['monthName']=$this->getMonthName($arr['month']);
    $arr['monthShortName']=$this->getMonthShortName($arr['month']);
  
    $arr['monthFirstDayWeekday']=$this->phpDate('w',$this->infoArrayToTimestamp(array('year'=>$arr['year'],'month'=>$arr['month'],'day'=>'1')))+1;
    if ($arr['monthFirstDayWeekday']>=6) {
      $arr['monthFirstDayWeekday']=0;
    }
    $arr['monthDaysNumber']=$this->date('t',$timestamp);
  
    $arr['weekday']++;
    $arr['weekday']=$arr['wday'];
    $arr['weekdayName']=$this->getWeekName($arr['weekday']);
    $arr['weekdayShortName']=$this->getWeekShortName($arr['weekday']);

    return $arr;
  }
  
  function infoArrayToTimestamp($arr) {
    list($gy,$gm,$gd)=$this->toGregorian($arr['year'],$arr['month'],$arr['day']);
    if (!isset($arr['hour'])) {
      $arr['hour']=$this->phpDate('H');
    }
    if (!isset($arr['minute'])) {
      $arr['minute']=$this->phpDate('i');
    }
    if (!isset($arr['second'])) {
      $arr['second']=$this->phpDate('s');
    }
  
    return strtotime("$gy-$gm-$gd".' '.$arr['hour'].':'.$arr['minute'].':'.$arr['second']);
  }
  
  
  /**
  * Implementation of PHP date function
  * This is the simplified versino by Sina Salek
  */
  function date($format,$maket=null)
  {
    $farsi=1;
    $type=$format;
    //set 1 if you want translate number to farsi or if you don't like set 0
    $transnumber=false;
    ///chosse your timezone
    $TZhours=0;
    $TZminute=0;
    $need="";
    $result1="";
    $result="";

    if (is_null($maket)) {
      $year=$this->phpDate("Y");
      $month=$this->phpDate("m");
      $day=$this->phpDate("d");
      $maket=parent::mktime($this->phpDate("H")+$TZhours,$this->phpDate("i")+$TZminute,$this->phpDate("s"),$this->phpDate("m"),$this->phpDate("d"),$this->phpDate("Y"));
    } else {
      $maket+=$TZhours*3600+$TZminute*60;
      $year=$this->phpDate("Y",$maket);
      $month=$this->phpDate("m",$maket);
      $day=$this->phpDate("d",$maket);
    }
    $need=$maket;
    $i=0;
    $subtype="";
    $subtypetemp="";
    list( $jyear, $jmonth, $jday ) = $this->fromGregorian($year, $month, $day);

    while($i<strlen($type))
    {
      $subtype=substr($type,$i,1);

      if($subtype=="\\")
      {
        $subtypetemp=$subtype;
        $i++;
        continue;
      } elseif($subtypetemp=="\\")
      {
        $result.=$subtype;
        $subtypetemp=$subtype;
        $i++;
        continue;
      } else
      {
        $subtypetemp=$subtype;
        $i++;
      }

      switch ($subtype)
      {
        case "A":
          $result1=$this->phpDate("a",$need);
          $result.=$this->getMeridiemName($result1);
          break;
        case "a":
          $result1=$this->phpDate("a",$need);
          $result.=$this->getMeridiemShortName($result1);
        case "d":
          if($jday<10) $result1="0".$jday;
            else $result1=$jday;
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
            else $result.=$result1;
          break;
        case "D":
          $result1=$this->phpDate("w",$need);
          $result.=$this->getWeekShortName($result1);
          break;
        case "F":
          $result.=$this->getMonthName($jmonth);
          break;
        case "g":
          $result1=$this->phpDate("g",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
            else $result.=$result1;
          break;
        case "G":
          $result1=$this->phpDate("G",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
          case "h":
          $result1=$this->phpDate("h",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "H":
          $result1=$this->phpDate("H",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "i":
          $result1=$this->phpDate("i",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "j":
          $result1=$jday;
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "l":
          $result1=$this->phpDate("w",$need);
          $result.=$this->getWeekName($result1);
          break;
        case "m":
          if($jmonth<10) $result1="0".$jmonth;
          else  $result1=$jmonth;
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "M":
          $result.=$this->getMonthShortName($jmonth);
          break;
        case "n":
          $result1=$jmonth;
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "s":
          $result1=$this->phpDate("s",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "S":
          $result.='ام';
          break;
        case "t":
          $result.=$this->monthTotalDays ($month,$day,$year);
          break;
        case "w":
          $result1=$this->phpDate("w",$need);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "y":
          $result1=substr($jyear,2,4);
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;
        case "Y":
          $result1=$jyear;
          if($transnumber==1) $result.=cmfcString::convertNumbersToFarsi($result1);
          else $result.=$result1;
          break;    
        case "O" :
          $result.=$this->phpDate('O', $need);
          break;
        case "P" :
          $result.=$this->phpDate('P', $need);
          break;
        case "T" :
          $result.=$this->phpDate('T', $need);
          break;
        case "U" :
          $result.=$this->phpDate();
          break;
        case "Z" :
          $result.=$this->yearTotalDays($jmonth,$jday,$jyear);
          break;
        case "L" :
          list( $tmp_year, $tmp_month, $tmp_day ) = $this->toGregorian(1384, 12, 1);
          /*
          echo $tmp_day;
          if(lastday($tmp_month,$tmp_day,$tmp_year)=="31")
            $result.="1";
          else 
            $result.="0";
          */
          break;
        default:
          $result.=$subtype;
      }
    }
    return $result;
  }
  
 
  /**
  * accept array,timestamp and string as input datetime in jalali 
  * or gregorian format and convert it to timestamp
  * Implementation of PHP strtotime function
  */
  function strtotime($value) {
    foreach ($this->_dateFormats as $formatKey=>$formatSample) {
    $result=call_user_func(array(&$this,'dateFormat'.$formatKey),$value);
    if ($result!==false) {
      return $result;
    }
  }
  
    return $value;
  }
  
  
  function dateFormatArraySimple($value) {
    if (is_array($value)) {
      if (isset($value[0])) {
        $y=$value['0'];
        $m=$value['1'];
        $d=$value['2'];
        $h=$value['3'];
        $i=$value['4'];
        $s=$value['5'];
      } elseif (isset($value['year'])) {
        $y=$value['year'];
        $m=$value['month'];
        $d=$value['day'];
        $h=$value['hour'];
        $i=$value['minute'];
        $s=$value['second'];
      } elseif (isset($value['y'])) {
        $y=$value['y'];
        $m=$value['m'];
        $d=$value['d'];
        $h=$value['h'];
        $i=$value['i'];
        $s=$value['s'];
      }
      
      return $this->valueToTimeStamp($y,$m,$d,$h,$i,$s);
    }
    return false;
  }
  
  function dateFormatRfc10($value) {
    if (is_string($value)) {
      if (preg_match('/^([0-9]{2,4})[-\/\\\]([0-9]{1,2})[-\/\\\]([0-9]{1,2})( +([0-9]{1,2})[\:]([0-9]{1,2})[\:]([0-9]{1,2}))?/', $value, $regs)) {
        $y=$regs['1'];
        $m=$regs['2'];
        $d=$regs['3'];
        $h=$regs['5'];
        $i=$regs['6'];
        $s=$regs['7'];

        return $this->valueToTimeStamp($y,$m,$d,$h,$i,$s);
      }

    }
    
    return false;
  }
      

  function fromGregorian ($g_y, $g_m, $g_d) 
  {
    if ($g_y<1300 or $g_m<1 or $g_d<1) return array('','','');
    $g_days_in_month = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31); 
    $j_days_in_month = array(31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29);   
    
     $gy = $g_y-1600; 
     $gm = $g_m-1; 
     $gd = $g_d-1; 

     $g_day_no = 365*$gy+div($gy+3,4)-div($gy+99,100)+div($gy+399,400); 

     for ($i=0; $i < $gm; ++$i) 
      $g_day_no += $g_days_in_month[$i]; 
     if ($gm>1 && (($gy%4==0 && $gy%100!=0) || ($gy%400==0))) 
      /* leap and after Feb */ 
      $g_day_no++; 
     $g_day_no += $gd; 

     $j_day_no = $g_day_no-79; 

     $j_np = div($j_day_no, 12053); /* 12053 = 365*33 + 32/4 */ 
     $j_day_no = $j_day_no % 12053; 

     $jy = 979+33*$j_np+4*div($j_day_no,1461); /* 1461 = 365*4 + 4/4 */ 

     $j_day_no %= 1461; 

     if ($j_day_no >= 366) { 
      $jy += div($j_day_no-1, 365); 
      $j_day_no = ($j_day_no-1)%365; 
     } 

     for ($i = 0; $i < 11 && $j_day_no >= $j_days_in_month[$i]; ++$i) 
      $j_day_no -= $j_days_in_month[$i]; 
     $jm = $i+1; 
     $jd = $j_day_no+1; 

     return array($jy, $jm, $jd); 
  } 

  function toGregorian($j_y, $j_m, $j_d) 
  {
    $j_d = (int) $j_d;
    $j_m = (int) $j_m;
    $j_y = (int) $j_y;
    
    
    $g_days_in_month = array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31); 
    $j_days_in_month = array(31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29);
    
    #--(Begin)-->By Sina
    if ($j_m>12) {
      $j_y=$j_y+(floor($j_m/12));
      $j_m=$j_m%12;
  }
    #--(End)-->By Sina
    
    
    if ($j_d < 1)
      $j_d = 1;
    //elseif ($j_d > $j_days_in_month[ $j_m - 1 ])
      //  $j_d = $j_days_in_month[ $j_m - 1 ];
    
    
    $jy = $j_y-979; 
    $jm = $j_m-1;
    $jd = 0;
    $jd = $j_d - 1; 
    
    $j_day_no = 365*$jy + div($jy, 33)*8 + div($jy%33+3, 4); 
    for ($i=0; $i < $jm; ++$i) 
      $j_day_no += $j_days_in_month[$i]; 
    
    $j_day_no += $jd; 
    
    $g_day_no = $j_day_no+79; 
    
    $gy = 1600 + 400*div($g_day_no, 146097); /* 146097 = 365*400 + 400/4 - 400/100 + 400/400 */ 
    $g_day_no = $g_day_no % 146097; 
    
    $leap = true; 
    if ($g_day_no >= 36525) /* 36525 = 365*100 + 100/4 */ 
    { 
      $g_day_no--; 
      $gy += 100*div($g_day_no,  36524); /* 36524 = 365*100 + 100/4 - 100/100 */ 
      $g_day_no = $g_day_no % 36524; 
    
      if ($g_day_no >= 365) 
       $g_day_no++; 
      else 
       $leap = false; 
    } 
    
    $gy += 4*div($g_day_no, 1461); /* 1461 = 365*4 + 4/4 */ 
    $g_day_no %= 1461; 
    
    if ($g_day_no >= 366) { 
      $leap = false; 
    
      $g_day_no--; 
      $gy += div($g_day_no, 365); 
      $g_day_no = $g_day_no % 365; 
    } 
    
    for ($i = 0; $g_day_no >= $g_days_in_month[$i] + ($i == 1 && $leap); $i++) 
      $g_day_no -= $g_days_in_month[$i] + ($i == 1 && $leap); 
    $gm = $i+1; 
    $gd = $g_day_no+1; 
    return array($gy, $gm, $gd); 
  }
  
  function valueToTimeStamp($y,$m,$d,$h,$i,$s) {
    $y=intval(strval($y));
    $m=intval(strval($m));
    $d=intval(strval($d));
    $h=intval(strval($h));
    $i=intval(strval($i));
    $s=intval(strval($s));
    
    if ($y<1900) {
      list($y,$m,$d)=$this->toGregorian($y,$m,$d);
    }
    if (!empty($h) or $h!=0) {      
      $value=strtotime("$y-$m-$d $h:$i:$s");
  } else {
      $value=strtotime("$y-$m-$d");
  }
  
  return $value;
  }
  
  /**
  * Find num of Day Begining Of Month ( 0 for Sat & 6 for Sun)
  */
  function monthStartDay($month,$day,$year)
  {
    list( $jyear, $jmonth, $jday ) = $this->fromGregorian($year, $month, $day);
    list( $year, $month, $day ) = $this->toGregorian($jyear, $jmonth, "1");
    $timestamp=parent::mktime(0,0,0,$month,$day,$year);
    return $this->phpDate("w",$timestamp);
  }
  
  /**
  * Find days in this year untile now 
  */
  function yearTotalDays($jmonth,$jday,$jyear)
  {
    $year="";
    $month="";
    $year="";
    $result="";
    if($jmonth=="01")
      return $jday;
    for ($i=1;$i<$jmonth || $i==12;$i++)
    {
      list( $year, $month, $day ) = $this->toGregorian($jyear, $i, "1");
      $result+=lastday($month,$day,$year);
    }
    return $result+$jday;
  }
  
  
  /**
  * @desc accept array,timestamp and string as input datetime in jalali or gregorian format
  */
  function smartGet($type,$value="now") {
    
    if ($value!='now') {
      $value=$this->strtotime($value);
    }
    
    if (empty($value)) return;
    
    return $this->date($type,$value);
  }
    
  
  /**
  * translate number of month to name of month
  */
  function getWeekName($weekNumber)
  {
  return $this->_weeksName[$weekNumber];
  }
  
  function getWeekShortName($weekNumber)
  {
  return $this->_weeksShortName[$weekNumber];
  }
    
  /**
  * translate number of month to name of month
  */
  function getMonthName($month)
  {
  return $this->_monthsName[$month];
  }
  
  function getMonthShortName($month)
  {
    return $this->_monthsShortName[$month];
  }
  
  function getMeridiemName($m)
  {
    return $this->_meridiemsName[$m];
  }
  
  function getMeridiemShortName($m)
  {
    return $this->_meridiemsShortName[$m];
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
  
  
  function isDateValid($month,$day,$year) {
    $j_days_in_month = array(31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29);
  $result = FALSE;
    if($month<=12 && $month>0 && $day>0)
    {
      if($j_days_in_month[$month-1]>=$day) {
        $result = TRUE;
    }
      if($this->isKabise($year) && $month==12 && $j_days_in_month[$month-1]+1==$day) {
        $result = TRUE;
    }
    }
    
    return $result;
  }
  
  
  function dateDiff($first,$second) {
    $first_date = explode("-",$first);
    $first_date = parent::mktime(0, 0, 0, $first_date[1],$first_date[2], $first_date[0]);
    //echo $first_date[1];
    $second_date = explode("-",$second);
    $second_date = parent::mktime(0, 0, 0,$second_date[1],$second_date[2], $second_date[0]);
    $totalsec=$second_date- $first_date;
    return $totalday = round(($totalsec/86400));
  }
  

  
  
  
  /**
  * Find Number Of Days In This Month
  */
  /**
  * @author 
  * Find Number Of Days In This Month
  */
  function monthTotalDays($month,$day,$year)
  {
  $jday2="";
  $jdate2 ="";
  $lastdayen=$this->phpDate("d",parent::mktime(0,0,0,$month+1,0,$year));
  list( $jyear, $jmonth, $jday ) = $this->fromGregorian($year, $month, $day);
  $lastdatep=$jday;
  $jday=$jday2;
  while($jday2!="1")
  {
    if($day<$lastdayen)
    {
    $day++;
    list( $jyear, $jmonth, $jday2 ) = $this->fromGregorian($year, $month, $day);
    if($jdate2=="1") break;
    if($jdate2!="1") $lastdatep++;
    }
    else
    { 
    $day=0;
    $month++;
    if($month==13) 
    {
      $month="1";
      $year++;
    }
    }

  }
  return $lastdatep-1;
  }
  
    
  /**
  * Find Number Of Days In This Month
  */
  function daysInMonth($monthId, $ctype = 'gregorian'){
    $daysInMonth = array(
      'jalali' => array(
        '31', 
        '31',
        '31',
        '31',
        '31',
        '31',
        '30',
        '30',
        '30',
        '30',
        '30',
        '29'
      ),
      'gregorian' => array(
        '31', 
        '28',
        '31',
        '30',
        '31',
        '30',
        '31',
        '31',
        '30',
        '31',
        '30',
        '31'
      )
    );
    return $daysInMonth[$ctype][$monthId];
  }
 


  /**
   * Get list of translatable strings
   */
  function getStrings() {
    return array();
    $stringGroups = array(
      'monthsName' => $this->_monthsName,
      'monthsShortName' => $this->_monthsShortName,
      'weeksName' => $this->_weeksName,
      'weeksShortName' => $this->_weeksShortName,
      'meridiemsName' => $this->_meridiemsName,
      'meridiemsShortName' => $this->_meridiemsShortName
    );
    return $stringGroups;
  }
}