<?php
/**
 * betterawstats, an alternative display for awstats data
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
 * Foundation, Inc., 59 Temple Place','Suite 330, Boston, MA  02111-1307, USA.
 */

/**
 * File contents:
 *
 * This file contains functions that read data from files process it and store it
 * in an array. This concerns library-data as well as stats-data. Language files
 * are NOT processed here.
 */


// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'data.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

/**
* Data function: read the data directories this function is called by
* baw_match_files() and itself recursively
*
* @param    str     $dir  directory
* @return   arr     ALL files found in that directory
*
*/
function baw_parse_dir($dir = false) {
    global $BAW_CONF;
    if ($dir == false) {
       $dir = $BAW_CONF['path_data'];
    }
    if (!file_exists($dir)) {
        baw_raise_error('datafilesdir', array($dir));
        return;
    }
    // add trailing slash if not exists
    if (substr($dir, -1) != '/') {
        $dir .= '/';
    }
    baw_debug('dbg_start_parse_dir', $dir);
    $files = array();
    if ($dh = @opendir($dir)) {
        while (($file = readdir($dh)) !== false) {
            if (!preg_match('/^\./s', $file)) {
                if (is_dir($dir.$file)) {
                    $newdir = $dir.$file.'/';
                    $files = array_merge($files, baw_parse_dir($newdir));
                    baw_debug('dbg_found_dir', $dir);
                } else {
                    $files[] = $dir.$file;
                    baw_debug('dbg_found_file', $file);
                }
            }
        }
        closedir($dh);
    }
    baw_debug('dbg_finished_parse_dir', $dir);
    return $files;
}

/*
* Data function: find all files that contain the data for the current month and
* 12 month before now to write a rolling month data chart. This function is called
* by index.php
*
* fills the gl. arrays: $BAW_CURR['years'], $BAW_CURR['months'], $BAW_DFILES
*
*/
function baw_match_files() {
    global $BAW_CURR, $BAW_CONF, $BAW_CONF_DIS, $BAW_MES, $BAW_DFILES, $BAW_SERVERS;
    // read all the files in the directory
    $dirfiles = baw_parse_dir();
    $pat_others = '/awstats(\d{6})\.((.+))\.txt$/';
    // go through all files and find matching ones
    $year_array = array();
    $month_array = array();
    foreach ($dirfiles as $file) {
        $filename = explode("/", $file);
        $filename = $filename[count($filename)-1];
        if (preg_match($pat_others, $file, $match)) {
            $month = substr($filename, 7, 2);
            $year = substr($filename, 9, 4);
            $year_array[$year] = $year;
            $month_array[$month] = $BAW_MES[$month + 59];
            // future feature where the admin can tell which sites to read/ exclude
            // if (!$BAW_CONF['filter_configs'] || in_array($site, $BAW_CONF['filter_configs'])) {
            $site = $match[2];
            $BAW_DFILES[$site][$year.$month] = array(
                'file' => $file,
                'map' => null
            );
            // add the sites to another array since we need that for the config editor
            $BAW_SERVERS[$site] = $site;
            baw_debug("site found for month $month and year $year: $file, SITE: {$site}");
        }
    }
    if (count($year_array)>0) {
        $year_array = array_unique($year_array);
        ksort($year_array);
    }
    if (count($month_array)>0) {
        ksort($month_array);
        $month_array = array_unique($month_array);
    }
    $BAW_CURR['years'] = $year_array;
    $BAW_CURR['months'] = $month_array;
    // since we added sites with each file, remove duplicates
    ksort($BAW_DFILES);
    ksort($BAW_SERVERS);
    $BAW_SERVERS = array('show_all' => $BAW_MES['cfg_show_all']) + $BAW_SERVERS;
    if (!isset($BAW_CURR['site_name'])) {
        $BAW_CURR['site_name'] = key($BAW_DFILES); // we dont have a selected, take first in line
        reset($BAW_DFILES);
    }
}

/*
* Data function: calculate the month-data by summing up days and retrieving
* single-value data. This function is called by different display-functions
*
* @param    arr     $t  TIME data array of the current file
* @param    arr     $g  GENERAL data array of the current file
* @return   arr     summed dataset for processing
*
*/
function baw_calc_monthdata($t, $g) {
    baw_debug("Calculating Month-data");
    $pages = 0;
    $hits = 0;
    $bandwidth = 0;
    $not_viewed_pages = 0;
    $not_viewed_hits = 0;
    $not_viewed_bandwidth = 0;
    $num_t = count($t);
    for ($i=0; $i<$num_t; $i++) {
        $pages += $t[$i][0];
        $hits += $t[$i][1];
        $bandwidth += $t[$i][2];
        $not_viewed_pages += $t[$i][3];
        $not_viewed_hits += $t[$i][4];
        $not_viewed_bandwidth += $t[$i][5];
    }
    $result = array(
        $g['TotalUnique'][0],
        $g['TotalVisits'][0],
        $pages,
        $hits,
        $bandwidth,
        $not_viewed_pages,
        $not_viewed_hits,
        $not_viewed_bandwidth
    );
    return $result;
}

/*
* Data Function: read data from awstats library files (perl format)
* Opens a file, reads the contents and does corrections and makes PHP out of PERL
* This function is called by the file library.inc.php
*
* @param    str        $file    path to the library file
* @param    str OR arr $data    the name(s) of the array in the file
* @return   arr        summed dataset for processing
*
*/
function baw_get_library($file, $data) {
    global $BAW_CONF, $BAW_LOGTYPE;
    $file = $BAW_CONF['path_lib'] . $file;
    $datastr = is_array($data) ? implode(",", $data) : $data;
    baw_debug("Trying to read library data ($datastr) from file: $file");
    if (!file_exists($file)) {
        baw_raise_error('libraryfiles', array($file));
        return array();
    }
    // read the whole file in one go
    $file_text = file_get_contents($file);
    // these are to attempt to convert the perl data file to PHP

    $search = array(
        "\$LogType eq 'S'",
        "= ", // remove spaces between = and (
        "@",
        "%",
        "'=>'",
        "=(",
        "=\n(",
        "=\r(",
        "=\r\n(",
        ",,",
        ' target="_blank"',
        ' [new window]',
        '<a href="',
        '&',
        '/" Searc',
        '[',
        ']'
    );
    $replace  = array(
        "\$LogType = 'S'",
        "=",
        "$",
        "$",
        "', '",
        "= array(",
        "= array(",
        "= array(",
        "= array(",
        ",",
        " ",
        " ",
        '<a class="ext_link" href="',
        '&amp;',
        '/" title="Searc',
        'array(',
        ')'
    );
    $file_text = str_replace($search, $replace, $file_text);

    $check = eval($file_text);
    // check if we have an error with the conversion
    if ($check === false) {
        baw_raise_error('libraryeval', array($file, $data));
    }
    if (is_array($data)) {
        $a = 0;
        foreach($data as $arr_name) {
            $countpairs = count($$arr_name);
            $parr = $$arr_name;
            for ($i=0; $i<$countpairs; $i=$i+2) {
                $new_arr[$a][$parr[$i]] = $parr[$i+1];
            }
            $a++;
        }
    } else {
        $countpairs = count($$data);
        $parr = $$data;
        for ($i=0; $i<$countpairs; $i=$i+2) {
            $new_arr[$parr[$i]] = $parr[$i+1];
        }
    }
    return $new_arr;
}

/*
* Data Function: read data from awstats data file. Identifies the file to read,
* locates the data-map, reads the data. This function is called by every single
* display-function on need for data. Some data might be read twice or more.
*
* @param    str     $site    which site do we read
* @param    str     $data_type    the name of the data-type to read
* @param    str     $date    YYYYMM, date of datafile
* @return   arr     data from file as assoc. array
*
*/
function baw_data($site, $data_type, $date) {
    global $BAW_CONF, $BAW_LIB, $BAW_CURR, $BAW_DFILES;
    $MAP = array();
    baw_debug("reading  $site datafile, type $data_type from $date");
    // we have to remove the linebreaks otherwise they end up in the data
    $brs_arr = array("\n", "\r");
    $brr_arr = array("", "");
    $dataset = array();
    // reduce the dataset to the necessary data
    if (!isset($BAW_DFILES[$site][$date])) {
        return false;
    } else {
        $dataset[$date] = $BAW_DFILES[$site][$date];
    }

    // iterate each data file
    $file = $dataset[$date]['file'];
    $f = fopen($file, 'r');
    $map = $dataset[$date]['map'];
    baw_debug("reading site $site datafile, date $date, file $file");

    // read the map if required
    if ($map == null) {
        baw_read_filemap($f, $site, $date);
        if (!isset($BAW_DFILES[$site][$date]['map'][$data_type])) {
            return false;
        } else {
            $offset = $BAW_DFILES[$site][$date]['map'][$data_type];
        }
    } else if (isset($map[$data_type])) {
        $offset = $map[$data_type];
        baw_debug("data map present, reading data $data_type from offset $offset");
    } else {
        baw_debug("data map present, type $data_type not existant in map!");
        return false;
    }
    $filedata = array();

    fseek($f , $offset, SEEK_SET);
    $check = 1;
    while ($check !== 0) {
        baw_debug("reading aditional line to find data...");
        $firstline = fgets($f, 20000);
        $check = strpos($firstline,"BEGIN_$data_type");
    }

    if ($check !== 0) {
        $err_data = array($file, $firstline);
        baw_raise_error('datafile', $err_data);
        return;
    }
    $index = explode(' ', $firstline);
    $lines_count = $index[1];
    baw_debug("Data $data_type found at offset ". ftell($f) . " instead of $offset (" . (ftell($f) - $offset) . " diff), is $lines_count lines long, reading now:");
    for ($i=0; $i<$lines_count;$i++) {
        baw_debug("reading Line $data_type $i");
        $str = fgets($f, 20000);
        if (substr($str,0,4) == 'END_') { // this is a security check since sometimes we are off
            continue;
        }
        // remove linebreaks from string
        $str = str_replace($brs_arr,$brr_arr, $str);
        $line_arr = explode(' ', $str);
        // shift first element as array name
        $first_element = array_shift($line_arr);
        // check if one dataset has occurred twice and would
        // overwrite another. SIDER and PAGEREF had this issue for sure.
        if (!isset($filedata[$first_element])) {
            $filedata[$first_element] = $line_arr;
        } else {
            foreach ($line_arr as $no => $line_item) {
                if (is_numeric($line_item)) {
                    // this could cause trouble in case this was a date, percentage etc.
                    $filedata[$first_element][$no] += $line_item;
                }
            }
        }
    }
    fclose($f);
    baw_debug("data read, file closed");
    return $filedata;
}

/*
* Data Function: read the filemap at the begginning of a data file. This function
* is called only by baw_data if the data was not read before.
*
* @param    obj     &$f    file-handler to read
* @param    str     $site    the concerned site
* @param    str     $date    YYYYMM, date of datafile
* adds the data to gl. array $BAW_DFILES
*
*/
function baw_read_filemap(&$f, $site, $date) {
    global $BAW_DFILES;
    // $f = fopen($file, 'r');
    $str ='';
    $check = 1;
    // read this file until we hit the offsets
    while ($check !== 0) {
        $str = fgets($f, 20000);
        // check for XML Data
        if (strstr($str, '<xml') !== false) { // we have XML
            baw_raise_error('xmldata');
            return;
        }
        $check = strpos($str, 'BEGIN_MAP');
    }
    $check = explode(' ', $str);
    $lines_count = $check[1]; // line length of the map
    baw_debug("found data map for file, has $lines_count lines");
    // now read x more lines
    for ($i=0; $i<$lines_count;$i++) { // read the complete offset map
        $str = fgets($f, 512);
        // split the info in string - byte offset
        $check = explode(' ', $str);
        $type = substr($check[0], 4);
        $offset = $check[1];
        if ($offset > 1) {
            baw_debug("data type $type, starts at offset $offset");
            $BAW_DFILES[$site][$date]['map'][$type] = $offset;
        } else {
            baw_raise_error('datafileindex', $err_data);
            return;
        }
    }
    baw_debug("map read");
}
?>