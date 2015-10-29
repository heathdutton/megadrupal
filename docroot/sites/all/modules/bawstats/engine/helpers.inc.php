<?php
/**
 * betterawstats - an alternative display for awstats data
 *
 * @author      Oliver Spiesshofer, support at betterawstats dot com
 * @copyright   2008 Oliver Spiesshofer
 * @version     1.0
 * @link        http://betterawstats.com

 * Based on the GPL AWStats Totals script by:
 * Jeroen de Jong <jeroen@telartis.nl>
 * copyright   2004-2006 Telartis
 * version 1.13 (http://www.telartis.nl/xcms/awstats)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * File contents:
 *
 * This file contains functions that are used to make checks, format data and do
 * other side-functions.
 */

// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'helpers.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

/*
* Helper Function: executes a function that is stored in the $BAW_LIB['formats']
* called by final output functions in /modules/
*
* @param    str     $function_str    string of format 'baw_num_format(%s);'
* @param    str     $var    data to be filled in
* @return   result of the function
*
*/
function exec_function($function_str=false, $var=false) {
    // check if function exists
    $out = $var;
    if ($function_str && $var !== false) {
        $txt = explode('(',$function_str);
        $func_name = $txt[0];
        if (function_exists($func_name)) {
            $out = eval("return " . sprintf($function_str, $var));
        } else {
            $out = "error finding function $func_name ($function_str, $var)";
        }
    }
    return $out;
}

/*
* Helper Function: check for various cfg settings to find if they are correct or not
* called by index.php
*
*/
function baw_check_config() {
    global $BAW_CONF, $BAW_MES;

    // lets check a non-std file, config.php might exist elsewhere
    $path = $BAW_CONF['site_path'] . "/modules/render_table.inc.php";
    if (!file_exists($path)) {
        // lets get an error-free error message
        $BAW_MES[58] = '';
        $BAW_MES['e'] = 'UTF-8';
        include_once('./language/lang_en.inc.php');
        $check = substr($_SERVER["SCRIPT_FILENAME"], 0, -10);
        baw_raise_error('site_path', array($BAW_CONF['site_path'], $check));
        return;
    }
}

/*
* Helper Function: debug output with time stamp and memory meter;
* called by all functions
*
* @param    str     $msg_code   the message to be shown either directly or from
*                               language file
* @param    str     $data       data to be filled in
* @param    bol     $always     show always or only when debug more is set?
* @return   debug message
*
*/
function baw_debug($msg_code, $data = false, $always = false) {
    global $BAW_CONF, $BAW_MES;
    $out = '';
    if ($BAW_CONF['debug'] || $always) {
        if (isset($BAW_MES[$msg_code])) {
            if ($data) {
                $out .= sprintf($BAW_MES[$msg_code], $data);
            } else {
                $out = $BAW_MES[$msg_code];
            }
        } else {
            $out = $msg_code;
        }
        $memory = baw_byte_format(memory_get_usage(),4);
        $time = baw_ptime();
        echo "<div style=\"text-align:right\">$out | Memory used: $memory | Time: $time</div>\n";
    }
}

/*
* Helper Function: returns the time between the start of the script until now
* called by baw_debug() and baw_display_index()
*
* @return   time in seconds
*
*/
function baw_ptime() {
    $now = baw_mtime();
    $overall = $now - START_TIME;
    $time_str = baw_num_format($overall, 3). " sec";
    return $time_str;
}

/*
* Helper Function: sets the current time to START_TIME for calculation
* of time spent for script execution.
* called by index.php, baw_ptime()
*
*/
function baw_mtime() {
    $arr = explode(" ", microtime());
    $out = $arr[0] + $arr[1];
    return $out;
}

/*
* Helper Function: format numeber of bytes
*
* @param    int     $number     number of bytes (1024-base)
* @param    int     $decimals   number of decimals to display
* @return   formatted according to config
*
*/
function baw_byte_format($number, $decimals = 2) {
    global $BAW_CONF;
    // kilo, mega, giga, tera, peta, exa, zetta, yotta
    $prefix_arr = array('','k','M','G','T','P','E','Z','Y');
    $i = 0;
    $value = round($number, $decimals);
    while ($value > 1024) {
        $value /= 1024;
        $i++;
    }
    $result = baw_num_format($value, $decimals);
    $result .= ' '.$prefix_arr[$i].'B';
    return $result;
}

/*
* Helper Function: format numbers according to config
*
* @param    int     $number     number
* @param    int     $decimals   number of decimals to display
* @return   formatted according to config
*
*/
function baw_num_format($number, $decimals = 0) {
    if ($number == 0 || empty($number) || !isset($number)) {
        $number = 0;
    }
    if (module_exists('format_number')) {
      $result = format_number($number, $decimals);
    } else {
      $result = number_format($number, $decimals);
    }
    return $result;
}

/*
* Helper Function: format dates according to config
*
* @param    int     $str     date in the format YYYYMMDDHHMMSS
* @return   formatted date according to config
*
*/
function baw_getdate_format($str) {
    global $BAW_CONF;
    $out = date($BAW_CONF['date_format'], strtotime(sprintf("%.0f",$str)));
    return $out;
}

/*
* Helper Function: format time according to config
*
* @param    int     $str     date in the format YYYYMMDDHHMMSS
* @return   formatted time only according to config
*
*/
function baw_time_format($str) {
    global $BAW_CONF, $BAW_MES;
    if ($str == 0) {
        return $BAW_MES[0];
    }
    $out = date($BAW_CONF['date_time_format'], strtotime(sprintf("%.0f",$str)));
    return $out;
}

/*
* Helper Function: deliver output according to odd/even numbers
*
* @param    int     $num     number to check if odd/even
* @param    str     $even    string to be returned in case $num is even
* @param    str     $odd     string to be returned in case $num is odd
* @return   $odd or $even
*
*/
function baw_even($num, $even, $odd) {
    if ($num%2 == 0) {
        return $even;
    } else {
        return $odd;
    }
}

/*
* Helper Function: format a number as a percentage
*
* @param    int     $num     number to format
* @return   str     formatted number
*
*/
function baw_percent_format($number) {
    global $BAW_CONF;
    $result = baw_num_format($number, $BAW_CONF['percent_decimals']) . " %";
    return $result;
}

/*
* Helper Function: Slice an array but keep numeric keys
* called only by baw_display_days()
*
* @param    arr     $array     array to be sliced
* @param    int     $day       $day from where to slice
* @return   arr     sliced array
*
*/
function baw_cut_date_array($array, $day) {
    $output_array = array();
    foreach ($array as $key => $value) {
        if ($key > $day) {
            $output_array[$key] = $value;
        }
    }
    return $output_array;
}

/*
* Helper Function: sort a multi-dimensional array by key or index
* called by almost all functions in display.inc.php. For performance reasons,
* the key is the preferred method.
*
* @param    arr     $data     array to be sorted
* @param    str     $column   column to be the sorting reference
* @param    con     $order    SORT_DESC or SORT_ASC
* @return   arr     sorted array
*
*/
function baw_array_sorting($data, $column, $order = SORT_DESC) {
    if (count($data) ==0) {
        return $data;
    } else if ($column == 'key') {
        baw_debug("sorting array by key");
        if ($order == SORT_DESC) {
            krsort($data);
        } else {
            ksort($data);
        }
    } else {
        baw_debug("creating sorting array using column $column $order");
        foreach ($data as $description => $numbers) {
            $sort_col[$description] = @$numbers[$column];
        }
        baw_debug("sorting array by column $column $order");
        array_multisort($sort_col, $order, $data);
    }
    baw_debug("array sorted");
    return $data;
}

/*
* Helper Function: raise an error for custom events
*
* @param    str     $type     index for error message
* @param    str     $data     error-specific data to fill into the message
* @return   str     error message
*
*/
function baw_raise_error($type, $data = array()) {
    global $BAW_MES, $BAW_CONF;

    $cd = count($data);
    if ($cd == 0) {
        $out = $BAW_MES[$type];
    } else if ($cd == 1) {
        $out = sprintf($BAW_MES[$type], $data[0]);
    } else if ($cd == 2) {
        $out = sprintf($BAW_MES[$type], $data[0], $data[1]);
    } else if ($cd == 3) {
        $out = sprintf($BAW_MES[$type], $data[0], $data[1], $data[2]);
    }
    // this is here in case the error comes so early that display helpers was not included yet.
    drupal_set_message(t('The Betterawstats engine generated the error below. If it is a configuration error concerning AWStats locations (i.e. the AWstats data directory, library directory, or language direction), these must be set using the') . ' <a href="skins/default/admin/settings/bawstats">' . t('BAW Statistics') . '</a> ' . t('administration pages.'),'error');
    drupal_set_message($out,'warning');
}

/*
* Helper Function: check if a file is writable
* called by itself and other occasions where config links are displayed
*
* @param    str     $path     path to the file
* @return   bol     true if writable, otherwise false
*
*/
function baw_is_writable($path) {
    //will work in despite of Windows ACLs bug
    //NOTE: use a trailing slash for folders!!!
    //see http://bugs.php.net/bug.php?id=27609
    //see http://bugs.php.net/bug.php?id=30931
    baw_debug('dbg_test_writable', $path);
    if ($path{strlen($path)-1}=='/') {// recursively return a temporary file path
        return baw_is_writable($path.uniqid(mt_rand()).'.tmp');
    } else if (is_dir($path)) {
        return baw_is_writable($path.'/'.uniqid(mt_rand()).'.tmp');
    }
    // check tmp file for read/write capabilities
    $rm = file_exists($path);
    $f = @fopen($path, 'a');
    if ($f===false) {
        @fclose($f);
        baw_debug('dbg_test_writable_false', $path);
        return false;
    }
    if (!$rm) {
        unlink($path);
    }
    fclose($f);
    baw_debug('dbg_test_writable_true', $path);
    return true;
}

/*
* Helper Function: debug helper that gives var_dump as a return instead of a screendump
*
* @param    misc     $var     variable to be dumped
* @return   str      formatted, dumped variable contents
*
*/
function baw_var_dump($var) {
    ob_start();
    var_dump($var);
    $dump = ob_get_contents();
    ob_end_clean();
    return "<pre>$dump</pre>";
}
?>