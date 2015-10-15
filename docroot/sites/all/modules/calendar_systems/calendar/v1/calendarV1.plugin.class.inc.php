<?php
interface cmfcCalendarV1Plugin {
	public function timestampToInfoArray($timestamp=null);

	public function infoArrayToTimestamp($arr);

    /**
    * Implementation of PHP date function
    * This is the simplified versino by Sina Salek
    */
    public function date($format,$maket=null);

    /**
    * Implementation of PHP strtotime function
    */
    public function strtotime($value);

	public function fromGregorian($g_y, $g_m, $g_d);
	public function toGregorian($j_y, $j_m, $j_d);

    /**
    * @desc accept array,timestamp and string as input datetime in jalali or gregorian format
    */
    public function smartGet($type,$value="now");

    public function makeTime($hour="",$minute="",$second="",$jmonth="",$jday="",$jyear="");

    public function isDateValid($month,$day,$year);

    public function dateDiff($first,$second);

    /**
     * Get list of translatable strings
     */
    public function getStrings();
}