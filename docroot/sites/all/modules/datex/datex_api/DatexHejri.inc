<?php

/**
* @author Sina Salek
* 
* Original persian to gregorian convertor by Milad Rastian, Roozbeh Pournader
* Mohammad Toossi.
*/
 
class DatexHejri extends DatexObject {
	protected static $islamicEpoch = 1948439.5;
  protected static $numbers;
  protected static $monthDays;
  protected static $names = array(
    'months' => array(
      1 => 'محرم',
      2 => 'صفر',
      3 => 'ربيع الأول',
      4 => 'ربيع الثاني',
      5 => 'جمادى الأول',
      6 => 'جمادى الآخر',
      7 => 'رجب',
      8 => 'شعبان',
      9 => 'رمضان',
      10 => 'شوال',
      11 => 'ذو القعدة',
      12 => 'ذو الحجة',
    ),
    'ampm' => array(
    	'am'=>'صباحاً',
    	'pm'=>'مساءاً',
    ),
    'day_abbr' => array( 'ش', 'ی', 'د', 'س', 'چ', 'پ', 'ج'),

    'day_abbr_short' => array(
      'الأحد',
      'الاثنين',
      'الثلاثاء',
      'الأربعاء',
      'الخميس',
      'الجمعة',
      'السبت',
    ),

    'day' => array(
      'الأحد',
      'الاثنين',
      'الثلاثاء',
      'الأربعاء',
      'الخميس',
      'الجمعة',
      'السبت',
    ),
    'order' => array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
      18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31),
  );
	
  public static function isLeap($year) {}
  public static function dayOfWeek($year, $dayOfYear = 0) {}
  public static function convert($timestamp_from = NULL) {}
  public static function toGregorian($jalali_year = 0, $jalali_month = 0, $jalali_day = 0) {}
  public function calendarFormat() {}
  public function getInfo($info) {}
  public function getIntlArgs() {}
  public function fixGranularities(array $granuls) {}

	/**
	 * Islamic Calendar
	 * @return Array          Hegri date [int Year, int Month, int Day] (Islamic calendar)
	 * @param  Integer Year   Gregorian year
	 *          Integer Month  Gregorian month
	 *         Integer Day    Gregorian day
	 * @desc   hj_convert will convert given Gregorian date into Hegri date (Islamic calendar)
	 * @author Khaled Al-Shamaa
	 */
	function hj_convert($Y, $M, $D){
		$jd = GregorianToJD($M, $D, $Y);

		list($year, $month, $day) = $this->jd_to_islamic($jd);

		return array($year, $month, $day);
	}

	/**
	 * Islamic Calendar 
	 * @return Array        Hegri date [int Year, int Month, int Day] (Islamic calendar)
	 * @param  Integer jd   Julian day
	 * @desc   jd_to_islamic will convert given Julian day into Hegri date (Islamic calendar)
	 * @author Khaled Al-Shamaa
	*/
	function jd_to_islamic($jd){
		$jd = floor($jd) + 0.5;
		$year = floor(((30 * ($jd - $this->_islamicEpoch)) + 10646) / 10631);
		$month = min(12,ceil(($jd - (29 + $this->islamic_to_jd($year, 1, 1))) / 29.5) + 1);
		$day = ($jd - $this->islamic_to_jd($year, $month, 1)) + 1;

		return array($year, $month, $day);
	}

	/**
	 * Islamic Calendar 
	 * @return Integer        Julian day
	 * @param  Integer Year   Hegri year
	 *         Integer Month  Hegri month
	 *         Integer Day    Hegri day
	 * @desc   islamic_to_jd will convert given Hegri date (Islamic calendar) into Julian day
	 * @author Khaled Al-Shamaa
	 */
	function islamic_to_jd($year, $month, $day){
		return ($day +
			ceil(29.5 * ($month - 1)) +
			($year - 1) * 354 +
			floor((3 + (11 * $year)) / 30) +
			$this->_islamicEpoch) - 1;
	}
            

	/**
	 * Islamic Calendar
	 * @param $g_y
	 * @param $g_m
	 * @param $g_d
	 * @return unknown_type
	 */
    function fromGregorian ($g_y, $g_m, $g_d) 
    {
		$y = $g_y;   
		$m = $g_m;
		$d = $g_d;
		if (( $y > 1582 ) || (( $y == 1582 ) && ( $m > 10 )) || (( $y == 1582 ) && ( $m == 10 ) && ( $d > 14 ))) 
		{
			$jd = (int)(( 1461 * ( $y + 4800 + (int)(( $m - 14 ) / 12 )))/ 4) + (int)(( 367 * ( $m - 2 - 12 * ((int)(( $m - 14 ) / 12)))) / 12) - (int)(( 3 * ((int)(( $y + 4900+ (int)(( $m - 14) / 12) ) / 100))) / 4)+ $d - 32075;
		} else {
			$jd = 367 * $y - (int)(( 7 * ( $y + 5001 + (int)(( $m - 9 ) / 7))) / 4) + (int)(( 275 * $m) / 9) + $d + 1729777;
		}
		$julianday = $jd;
		$l = $jd - 1948440 + 10632;
		$n = (int)(( $l - 1 ) / 10631);
		$l = $l - 10631 * $n + 354;
		$j = ( (int)(( 10985 - $l ) / 5316)) * ( (int)(( 50 * $l) / 17719)) + ( (int)( $l / 5670 )) * ( (int)(( 43 * $l ) / 15238 ));
		$l = $l - ( (int)(( 30 - $j ) / 15 )) * ( (int)(( 17719 * $j ) / 50)) - ( (int)( $j / 16 )) * ( (int)(( 15238 * $j ) / 43 )) + 29;
		$m = (int)(( 24 * $l ) / 709 );
		$d = $l - (int)(( 709 * $m ) / 24);
		$y = 30 * $n + $j - 30;
		

		
		return array($y, $m, $d);
    } 

    /**
     * Islamic Calendar 
     * @param $i_y
     * @param $i_m
     * @param $i_d
     * @return unknown_type
     */
    function _toGregorian($i_y, $i_m, $i_d) 
    {
    	//$jd=$this->islamic_to_jd($i_y, $i_m, $i_d);
    	//list($gy, $gm, $gd)=JDToGregorian($jd);
        $y = $i_y;  
        $m = $i_m;
        $d = $i_d;
       
        $jd = (int)((11*$y+3)/30)+354*$y+30*$m-(int)(($m-1)/2)+$d+1948440-385;
        //$this->julianday = $jd;
        if ($jd> 2299160 )
        {
            $l=$jd+68569;
            $n=(int)((4*$l)/146097);
            $l=$l-(int)((146097*$n+3)/4);
            $i=(int)((4000*($l+1))/1461001);
            $l=$l-(int)((1461*$i)/4)+31;
            $j=(int)((80*$l)/2447);
            $d=$l-(int)((2447*$j)/80);
            $l=(int)($j/11);
            $m=$j+2-12*$l;
            $y=100*($n-49)+$i+$l;
        } else {
            $j=$jd+1402;
            $k=(int)(($j-1)/1461);
            $l=$j-1461*$k;
            $n=(int)(($l-1)/365)-(int)($l/1461);
            $i=$l-365*$n+30;
            $j=(int)((80*$i)/2447);
            $d=$i-(int)((2447*$j)/80);
            $i=(int)($j/11);
            $m=$j+2-12*$i;
            $y=4*$k+$n+$i-4716;
        }
    	
    	
		return array($y, $m, $d); 
    }
    
    /**
     * 
     */
    function valueToTimeStamp($y,$m,$d,$h,$i,$s) {
	    $y=intval(strval($y));
	    $m=intval(strval($m));
	    $d=intval(strval($d));
	    $h=intval(strval($h));
	    $i=intval(strval($i));
	    $s=intval(strval($s));
	    
	    if ($y < 1900) {
	      list($y,$m,$d)=$this->toGregorian($y,$m,$d);
	    }
      
      if (!empty($h) or $h != 0) {
	        $value = strtotime("$y-$m-$d $h:$i:$s");
		  } 
      else {
	      $value = strtotime("$y-$m-$d");
      }
      
      return $value;
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
    
    function isLeap($year) {
      if($year %4 == 0 && $year % 100 !=0) {
        return true;
      }
      return false;
    }
    
    
	/**
	* @author 
	* Find Number Of Days In This Month
	*/
	function monthTotalDays($month,$day,$year) {
		$jday2="";
		$jdate2 ="";
		$lastdayen=$this->phpDate("d",mktime(0,0,0,$month+1,0,$year));
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
}

