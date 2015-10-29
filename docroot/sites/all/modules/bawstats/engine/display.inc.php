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
 * This file contains one function for each displayed section. The functions
 * call the data from the datafile, process & sort it and output the charts/tables
 * according to the settings in config.php
 */


// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'display.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

function baw_display_overview($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF;
    $m = $BAW_MES;
    $m_name = $m[59+$BAW_CURR['month']];

    if ($g = baw_data($BAW_CURR['site_name'], 'GENERAL', $BAW_CURR['yearmonth'])) {
        // we assumethat time exists when general exist
        $t = baw_data($BAW_CURR['site_name'], 'TIME', $BAW_CURR['yearmonth']);

        $o = baw_calc_monthdata($t,$g);
        $l = $g['LastUpdate'];
        $time = baw_time_format($l[0]);
        $BAW_CURR['thismonth']['hits'] = $o[1];
        $out = "<table class=\"datatable\">\n"
            . "    <tr>\n        <td><b>{$m[133]}</b></td><td colspan=\"5\">$m_name {$BAW_CURR['year']}</td>\n    </tr>\n"
            . "    <tr class=\"layout_index\">\n        <td><b>{$m[8]}</b></td><td colspan=\"5\">".baw_time_format($g['FirstTime'][0])."</td>\n    </tr>\n"
            . "    <tr class=\"layout_index\">\n        <td><b>{$m[9]}</b></td><td colspan=\"5\">".baw_time_format($g['LastTime'][0])."</td>\n    </tr>\n"
            . "    <tr>\n        <td>&nbsp;</td>\n"
            ."        <th class=\"layout_visitors\">{$m[11]}</th>\n"
            ."        <th class=\"layout_visits\">{$m[10]}</th>\n"
            ."        <th class=\"layout_pages\">{$m[56]}</th>\n"
            ."        <th class=\"layout_hits\">{$m[57]}</th>\n"
            ."        <th class=\"layout_bytes\">{$m[75]}</th>\n    </tr>\n"

            . "    <tr style=\"white-space:nowrap;\">\n        <td>{$m[160]}&nbsp;*</td>\n"
            ."        <td><b>".baw_num_format($o[0])."</b>".BR."&nbsp;</td>\n"
            ."        <td><b>".baw_num_format($o[1])."</b>".BR."(".@baw_num_format($o[1] / $o[0],2)."&nbsp;{$m[52]})</td>\n"
            ."        <td><b>".baw_num_format($o[2])."</b>".BR."(".@baw_num_format($o[2] / $o[1],2)."&nbsp;{$m[27]}/{$m[12]})</td>\n"
            ."        <td><b>".baw_num_format($o[3])."</b>".BR."(".@baw_num_format($o[3] / $o[1],2)."&nbsp;{$m[57]}/{$m[12]})</td>\n"
            ."        <td><b>".baw_byte_format($o[4])."</b>".BR."(".@baw_byte_format($o[4] / $o[1],2)."/{$m[12]})</td>\n    </tr>\n"
            . "    <tr>\n        <td>{$m[161]}&nbsp;*</td>\n"
            ."        <td colspan=\"2\">&nbsp;</td>\n"
            ."        <td><b>".baw_num_format($o[5])."</b></td>\n"
            ."        <td><b>".baw_num_format($o[6])."</b></td>\n"
            ."        <td><b>".baw_byte_format($o[7])."</b></td></tr>\n"
            . "    <tr>\n        <td colspan=\"6\">* {$m[159]}</td>\n    </tr>\n";
        if ($BAW_CONF['show_parser_stats']) {
            $out .= "    <tr>\n        <td colspan=\"6\"><b>{$m['data_file_stats']}</b></td>\n    </tr>\n"
            . "    <tr>\n"
            ."        <td>{$m[35]}:".BR."$time</td>\n"
            ."        <td>{$m['parsed_records']}".BR . baw_num_format($l[1]) . "</td>\n"
            ."        <td>{$m['old_records']}".BR . baw_num_format($l[2]) . "</td>\n"
            ."        <td>{$m['new_records']}".BR . baw_num_format($l[3]) . "</td>\n"
            ."        <td>{$m['corrupted']}".BR . baw_num_format($l[4]) . "</td>\n"
            ."        <td>{$m['dropped']}".BR . baw_num_format($l[5]) . "</td>\n    </tr>\n";
        }
        $out .= "</table>\n\n";
    } else {
        $out = "$m_name-{$BAW_CURR['year']} has no data";
    }
    return $out;
}

function baw_display_months($set) {
    global $BAW_MES, $BAW_CONF, $BAW_CURR, $BAW_DFILES;
    $format = array(
        0 => array ('format' => 'layout_text', 'title'=> $BAW_MES[5], 'colspan'=>'2'), // first cell in first line
        1 => array ('format' => 'layout_text'), // first cell in first line
        2 => array ('format' => 'layout_visitors', 'title'=> $BAW_MES[11]),
        3 => array ('format' => 'layout_visits', 'title'=> $BAW_MES[10]),
        4 => array ('format' => 'layout_pages', 'title'=> $BAW_MES[56]),
        5 => array ('format' => 'layout_hits', 'title'=> $BAW_MES[57]),
        6 => array ('format' => 'layout_bytes', 'title'=> $BAW_MES[75])
    );
    $allsites = false;
    if ($BAW_CURR['site_name'] !== 'all_months') {
        $sites = array($BAW_CURR['site_name']);
    } else {
        $sites = array_keys($BAW_DFILES);
        $allsites = true;
    }

    $out = '';
    // for each month, get the four data sets
    $frm_arr = array('layout_visitors', 'layout_visits', 'layout_pages', 'layout_hits', 'layout_bytes');
    foreach ($sites as $site) {
        $data_format = array();
        $x = 0;
        $val = array();
        $monthback = $set['top_x'] - 1;
        $chart = array();
        $val_chart = array();
        $lastyear = 0;
        for ($i=$monthback; $i>=0; $i--){
            // get the month & Year x month ago
            $tyear = date("Y", mktime(0, 0, 0, $BAW_CURR['month']-$i, 1, $BAW_CURR['year']));
            $tmonth = date("m", mktime(0, 0, 0, $BAW_CURR['month']-$i, 1, $BAW_CURR['year']));
            // get the data for that month
            if (isset($BAW_DFILES[$site][$tyear .$tmonth])) {
                $g = baw_data($site, 'GENERAL', $tyear . $tmonth);
                $t = baw_data($site, 'TIME', $tyear . $tmonth);
                $data = baw_calc_monthdata($t, $g);
                $x++;
                $month_text = $tmonth + 59;
                $class = '';
                $date = date("Ym", mktime(0,0,0,$tmonth,1,$tyear));
                $today = date("Ym");
                if ($date == $today) {
                    $class = "currentday";
                }
                for ($j=0; $j<5; $j++) {
                    $chart["{$BAW_MES[$month_text]}|$tyear"][] = array('data' => "$data[$j]", 'class' => "$class", 'format' => $frm_arr[$j]);
                }
                $class .= baw_even($tyear, ' evenyear', ' oddyear');

                $val[$tmonth.$tyear] = array(
                    $tyear,
                    $BAW_MES[$month_text],
                    $data[0],
                    $data[1],
                    $data[2],
                    $data[3],
                    $data[4],
                );
                if ($class !== '') {
                    $data_format[$tmonth.$tyear] = $class;
                }
            }
        }
        if ($set['chart']) {
            if ($allsites) {
                // make a link to the actual statistics from the overview
                $full_url = "{$BAW_CONF['site_url']}/index.php?site=$site&amp;month={$BAW_CURR['month']}&amp;year={$BAW_CURR['year']}";
                $out .= "<h3 id=\"site_$site\"><a href=\"$full_url\">$site</a></h3>";
            }
            $out .= baw_render_htmlchart($val, $format, $set['avg'], -$set['top_x'], $data_format);
        }
        if ($set['table']) {
            $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], false, $data_format);
        }
    }
    return $out;
}

function baw_display_days($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_DFILES;
    $sum = array(0,0,0,0,0); // summ all overall data
    // this 3 lines is only for collecting data for weekdays
    $wdsum_arr = array(); // summ all weekday data
    $wdcount_arr = array(0,0,0,0,0,0,0); // count the number of sat, sun etc
    $wdavg_arr = array(); // values of avg
    $daysoflastmonth = 30;

    // assemble date
    $daysofmonth = date("t", strtotime("{$BAW_CURR['year']}{$BAW_CURR['month']}01000000"));
    //$thisyear = date("Y", strtotime("-1 month"));
    //$lastyear = date("Y", strtotime("-1 month -1 year"));
    $current_month = date('m');
    $current_day = date('d');
    $daydiff = 0;
    $data_format = array();
    // find how may days we have to go back
    if ($BAW_CURR['month'] == $current_month) { // if we have the current month, get some data from the last month
        $daydiff = abs($current_day - $daysofmonth); // calculate how many days are missing
    }

    $format = array(
        0 => array ('format'=> 'layout_text', 'title' => $BAW_MES[4], 'colspan'=>3),
        1 => array ('format'=> 'layout_text'),
        2 => array ('format'=> 'layout_text'),
        3 => array ('format'=> 'layout_visits', 'title' => $BAW_MES[10]),
        4 => array ('format'=> 'layout_pages', 'title' => $BAW_MES[56]),
        5 => array ('format'=> 'layout_hits', 'title' => $BAW_MES[57]),
        6 => array ('format'=> 'layout_bytes', 'title' => $BAW_MES[75])
    );
    // check if we do all sites or just one
    $allsites = false;
    if ($BAW_CURR['site_name'] !== 'all_days') {
        $sites = array($BAW_CURR['site_name']);
    } else {
        $sites = array_keys($BAW_DFILES);
        $allsites = true;
    }
    $out = '';
    // iterate all sites, normally only one
    foreach ($sites as $site) {
        // we have to add the days of the previous month
        // this has to be inside the loop so we get the variables again for next site
        if ($daydiff > 0) {
            if ($BAW_CURR['month'] !== '01') {
                $prevyear = $BAW_CURR['year'];
                $prevmonth = sprintf('%02d', $BAW_CURR['month']-1); // date('m', strtotime("-1 month"));
                $daysoflastmonth = date("t", strtotime("{$BAW_CURR['year']}{$prevmonth}01000000"));
            } else {
                $prevyear = $BAW_CURR['year'] - 1;
                $prevmonth = '12';
            }
        }
        $val = array();
        // get the data for this site if available
        if ($val = baw_data($site, 'DAY', $BAW_CURR['yearmonth'])) {
            // in case we need last months data, get it too
            if ($daydiff > 0) {
                if ($prevdate_data = baw_data($site, 'DAY', $prevyear . $prevmonth)) {
                    $date_cut = $prevyear . $prevmonth . $prevmonth =  sprintf('%02d', $daysoflastmonth - $daydiff);
                    $val_prev = baw_cut_date_array($prevdate_data, $date_cut);
                    $val = $val_prev + $val;
                }
            }
            $newval = array();
            $lastmonth = 0;
            for ($j=0; $j<$daysofmonth; $j++) {
                $class = '';
                // foreach ($val as $date => $data) {
                $date = date("Ymd", mktime(0,0,0,$BAW_CURR['month'],1+ $j - $daydiff,$BAW_CURR['year']));
                $month = date("m", mktime(0,0,0,$BAW_CURR['month'],1+ $j - $daydiff,$BAW_CURR['year']));
                $today = date("Ymd");
                if ($date == $today) {
                    $class = "currentday";
                }
                if (!isset($val[$date])) {
                    $val[$date] = array(0,0,0,0,0);
                }
                $data = $val[$date];
                $xdate = $date . "000000";
                $tstamp = strtotime($xdate);
                $wday = date("w", $tstamp);

                if ($wday == 0 or $wday == 6) {
                    $class = "$class weekend";
                    $class = trim($class);
                }
                // these lines is only for collecting data for weekdays
                $wdcount_arr[$wday]++;  // count no of one weekday for avg division
                for ($i=0; $i<=3; $i++) { // iterate data types
                    if (!isset($wdsum_arr[$wday][$i])) {
                        $wdsum_arr[$wday][$i] = 0;
                    }
                    $wdsum_arr[$wday][$i] += $data[$i];
                }

                if (!isset($from)) {
                    $from = $xdate;
                }
                $BAW_CURR['wdays']['count'] = $wdcount_arr;
                $BAW_CURR['wdays']['avg'] = $wdsum_arr;
                $BAW_CURR['wdays']['from'] = $from;
                $BAW_CURR['wdays']['to'] = $xdate;
                if (($j !== $daysofmonth)) {
                    $newval[$xdate] = array(
                        date("Y", strtotime($xdate)),
                        date("M", strtotime($xdate)),
                        date("d", strtotime($xdate)),
                        $data[3],
                        $data[0],
                        $data[1],
                        $data[2]
                    );
                }

                $class .= baw_even($month, ' evenmonth', ' oddmonth');
                if ($class !== '') {
                    $data_format[$xdate] = $class;
                }
            }
            if ($set['chart']) {
                if ($allsites) {
                    // make a link to the actual statistics from the overview
                    $full_url = "{$BAW_CONF['site_url']}/index.php?site=$site&amp;month={$BAW_CURR['month']}&amp;year={$BAW_CURR['year']}";
                    $out .= "<h3 id=\"site_$site\"><a href=\"$full_url\">$site</a></h3>";
                    // $html .= baw_display_updatestats($site);
                }
                $out .= baw_render_htmlchart($newval, $format, $set['avg'], false, $data_format);
            }
            if ($set['table']) {
                $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], false, $data_format);
            }
        }
    }
    return $out;
}

// get all the averages of one weekday each
function baw_display_weekdays($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF;

    if (isset($BAW_CURR['wdays']['count'])) {
        $format = array(
            0 => array ('title' => $BAW_MES[4], 'format' => 'layout_text'),
            1 => array ('percent' => true, 'title' => $BAW_MES[10], 'format' => 'layout_visits'),
            2 => array ('title' => $BAW_MES[56], 'format' => 'layout_pages'),
            3 => array ('title' => $BAW_MES[57], 'format' => 'layout_hits'),
            4 => array ('title' => $BAW_MES[75], 'format' => 'layout_bytes')
        );
        $newval = array();
        $data_format = array();
        $wdcount_arr = $BAW_CURR['wdays']['count'];
        $wdsum_arr = $BAW_CURR['wdays']['avg'];
        for ($w=0; $w<7; $w++) {
            // switch for Mo or Su start
            $x = $w;
            if ($BAW_CONF['firstdayofweek'] == 1) {
                $x = $w + 1;
                if ($x == 7) {
                    $x = 0;
                }
            }
            $class = '';
            if ($x == 0 or $x == 6) {
                $class = "weekend";
            }
            if ($wdcount_arr[$x] == 0) {
                $wdcount_arr[$x] = 1;
            }
            for ($i=0; $i<=3; $i++) {
                $wavg[$x][$i] = '1';
                if (!isset($wdsum_arr[$x][$i])) {
                    $wdsum_arr[$x][$i] = 0;
                }
                $wavg[$x][$i] = $wdsum_arr[$x][$i] / $wdcount_arr[$x];
            }
            $newval[84+$x] = array(
                $BAW_MES[84+$x],
                $wavg[$x][3],
                $wavg[$x][0],
                $wavg[$x][1],
                $wavg[$x][2]
            );
            if ($class !== '') {
                $data_format[84+$x] = $class;
            }
        }
        $out = '';
        if ($set['chart']) {
            $out .= baw_render_htmlchart($newval, $format, $set['avg'], false, $data_format);
        }
        if ($set['table']) {
            $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], false, $data_format);
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    return $out;
}

function baw_display_hours($set) {
    // we have to calculate the average of each hour depending on how many days have
    // data at all in this month
    // half days will create slightly inaccuarate data.
    global $BAW_MES, $BAW_CURR, $BAW_CONF;
    $chart = array();
    $format = array(
        0 => array ('title'=> $BAW_MES[20], 'format' => 'layout_text', 'colspan'=> 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
        3 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        4 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes')
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], "TIME", $BAW_CURR['yearmonth'])) {
        $days = baw_data($BAW_CURR['site_name'], "DAY", $BAW_CURR['yearmonth']);
        $count_days = count($days);
	if ($count_days==0) $count_days=1;
        foreach ($val as $hour => $data) {
            $ihour = $hour + 1;
            if ($ihour >= 13) {
                $ihour = ($hour - 11);
            }
            $avgdata = array();
            $icon = baw_create_image($BAW_CONF['icons_url'] . "/clock/hr$ihour.png" , array("alt"=>$hour, "title"=>$hour));
            $avgdata[0] = $data[0] / $count_days;
            $avgdata[1] = $data[1] / $count_days;
            $avgdata[2] = $data[2] / $count_days;
            $lhour  = sprintf('%02d',$hour);
            $newval_table[$hour] = array(
                $icon,
                "$lhour:00 - $lhour:59",
                $avgdata[0],
                $avgdata[1],
                $avgdata[2],
            );
            $newval_chart[$hour] = array(
                $icon,
                $hour,
                $avgdata[0],
                $avgdata[1],
                $avgdata[2],
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }

    $out = '';
    if ($set['chart']) {
        $newval_chart = baw_array_sorting($newval_chart, $set['sort'], $set['sort_dir']);
        $out .= baw_render_htmlchart($newval_chart, $format, $set['avg'], false);
    }
    if ($set['table']) {
        $newval_table = baw_array_sorting($newval_table, $set['sort'], $set['sort_dir']);
        $out .= baw_render_table($set['name'], $newval_table, $format, $set['avg'], $set['total']);
    }
    return $out;
}

/**
 * Produce ouput about domains
 *
 * @param $set
 *   Array of flags indicating which sections to display
 *
 * @return
 *   HTML output that includes the sections and links to the map
 */
function baw_display_domains($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $chart = array();
    $format_table = array(
        0 => array ('title'=> $BAW_MES[25] , 'format' => 'layout_text','colspan' => 3),
        1 => array ('format' => 'layout_text'),
        2 => array ('format' => 'layout_text'),
        3 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
        4 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        5 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[25] , 'format' => 'layout_text','colspan' => 3),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
        3 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        4 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes')
    );
    // $format_jp = array($BAW_MES[56], $BAW_MES[57], $BAW_MES[75]);
    $newval = array();
    // $jpval = array();

    // Get the domain data usage data
    if ($val = baw_data($BAW_CURR['site_name'], "DOMAIN", $BAW_CURR['yearmonth'])) {
        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        foreach ($val as $domain => $data) {
            if (isset($BAW_LIB['domains'][$domain])) {
                $file = $domain;
                $desc = $BAW_LIB['domains'][$domain];
            } else {
                $file = 'unknown';
                $desc = $file;
            }
            $icon = baw_create_image($BAW_CONF['icons_url'] . "/flags/$file.png" , array("alt" => $domain, "title" => $domain));
            $newval_table[$domain] = array(
                $icon,
                $desc,
                $domain,
                $data[0],
                $data[1],
                $data[2]
            );
            $newval_chart[$domain] = array(
                $icon,
                $domain,
                $data[0],
                $data[1],
                $data[2]
            );
        }
        $out = '';

        if ($set['chart']) {
          $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
        }
        if ($set['map']) {
          $out .= baw_render_map($newval_table, $set['top_x']);
        }
        if ($set['table']) {
          $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x']);
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    return $out;
}

function baw_display_visitors($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_CONF_DIS;
    $format = array( // first line
        0 => array ('title'=> $BAW_MES[81], 'format' => 'layout_text'),
        1 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
        2 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        3 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
        4 => array ('title'=> $BAW_MES['ratio_pages_hits'], 'format' => 'layout_ratio', 'ratio' => '2/1'),
        5 => array ('title'=> $BAW_MES[9], 'format' => 'layout_date'),
    );
    $h_data = array(0,0,0,'latest_date'=> 0);
    $s_data = array(0,0,0,'latest_date'=> 0);
    $i = 0;
    $hashotlinks = false;
    $hasbots = false;
    if ($val = baw_data($BAW_CURR['site_name'], "VISITOR", $BAW_CURR['yearmonth'])) {
        baw_debug("data loaded");
        foreach ($val as $dns => $data) {
            // $infolink = baw_create_link("?", "javascript:neww('$dns','{$dns}XXX')");
            // $line[] = array ('data'=> $infolink, 'class' => "aligncenter", 'format' => 'layout_index');
            //if (!$set['hidebots'] or $data[1] !== $data[2]) {
            $ratio = 0;
            if ($data[0] > 0) {
                $ratio = $data[1] / $data[0];
            }
            // hotlinks & Bots
            if ($data[3] =='') {
                $data[3] = 0;
            }
            // $data[3] = settype($data[3], 'int');
            if ($data[0] == 0 && $data[1] !== 0) {
                $h_data['latest_date'] = max($data[3], $h_data['latest_date']);
                $h_data[0] += $data[0];
                $h_data[1] += $data[1];
                $h_data[2] += $data[2];
                $hashotlinks = true;
            } else if ($ratio <= $BAW_CONF_DIS['visitors']['assumebot']) {
                $s_data['latest_date'] = max($data[3], $s_data['latest_date']);
                $s_data[0] += $data[0];
                $s_data[1] += $data[1];
                $s_data[2] += $data[2];
                $hasbots = true;
            } else {
                $val[$i] = array(
                    $dns,
                    $data[0],
                    $data[1],
                    $data[2],
                    baw_num_format($ratio, 2),
                    $data[3]
                );
            }
            // try extra to regain the memory here since this is the largest function
            $val[$dns] = NULL;
            unset($val[$dns]);
            $i++;
        }
        if ($hasbots) {
            $val['script'] = array(
                $BAW_MES['assumedscript'],
                $s_data[0],
                $s_data[1],
                $s_data[2],
                $s_data[1] / $s_data[0],
                $s_data['latest_date']
            );
        }
        if ($hashotlinks) {
            $val['hotlinks'] = array(
                $BAW_MES['hotlinks_proxies'],
                $h_data[0],
                $h_data[1],
                $h_data[2],
                0,
                $h_data['latest_date']
            );
        }

        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        baw_debug("data sorted");
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], $set['top_x']);
    }
    return $out;
}

function baw_display_logins($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF;
    $format = array(
        0 => array ('title'=> $BAW_MES[94], 'format' => 'layout_text'),
        1 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
        2 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        3 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
        4 => array ('title'=> $BAW_MES[9], 'format' => 'layout_date'),
    );

    $val = array();
    if ($val = baw_data($BAW_CURR['site_name'], "LOGIN", $BAW_CURR['yearmonth'])) {
        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        foreach ($val as $id => $data) {
            $val[$id] = array(
                $id,
                $data[0],
                $data[1],
                $data[2],
                $data[3]
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_robots($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[53], 'format' => 'layout_text'),
        1 => array ('percent' => false, 'title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        2 => array ('title'=> $BAW_MES[57] . " (robots.txt)", 'format' => 'layout_hits'),
        3 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
        4 => array ('title'=> $BAW_MES[9], 'format' => 'layout_date'),
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], "ROBOT", $BAW_CURR['yearmonth'])) {
        foreach ($val as $robot => $data) {
            if (isset($BAW_LIB['robots'][$robot])) {
                $robot_txt = $BAW_LIB['robots'][$robot];
            } else {
                $robot_txt = $robot;
            }
            $val[$robot] = array(
                $robot_txt,
                $data[0]-$data[3],
                $data[3],
                $data[1],
                $data[2],
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], $set['top_x']);
    }
    return $out;
}

function baw_display_worms($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[163], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[167], 'format' => 'layout_text'),
        2 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        3 => array ('title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
        4 => array ('title'=> $BAW_MES[9], 'format' => 'layout_date'),
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], "WORMS", $BAW_CURR['yearmonth'])) {
        foreach ($val as $worm => $data) {
            $val[$worm] = array(
                $BAW_LIB['worms']['names'][$worm],
                $BAW_LIB['worms']['targets'][$worm],
                $data[0],
                $data[1],
                $data[2]
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], $set['top_x']);
    }
    return $out;
}

function baw_display_sessions($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[117], 'format' => 'layout_text'),
        1 => array ('percent' => true, 'title'=> $BAW_MES[10], 'format' => 'layout_visits')
    );

    $newval = array();
    $sum = 0; // sum for "unknown"
    if ($val = baw_data($BAW_CURR['site_name'], "SESSION", $BAW_CURR['yearmonth'])) {
        foreach ($BAW_LIB['sessions'] as $timespan) {
            $newval[$timespan] = array($timespan, 0);
            if (isset($val[$timespan][0])) {
                $newval[$timespan][1] = $val[$timespan][0];
                $sum += $val[$timespan][0];
            }
        }
        $unknown = $BAW_CURR['thismonth']['hits'] - $sum;
        $newval['Unknown'] = array('Unknown', $unknown);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    // $newval = baw_array_sorting($newval, 1, SORT_DESC);
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], false);
    }
    return $out;
}

function baw_display_filetype($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format_table = array(
        0 => array ('title'=> $BAW_MES[73], 'format' => 'layout_text', 'colspan' => 3),
        1 => array ('format' => 'layout_text'),
        2 => array ('format' => 'layout_text'),
        3 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        4 => array ('percent' => true, 'title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
        5 => array ('title'=> $BAW_MES[100], 'format' => 'layout_bytes'),
        6 => array ('title'=> $BAW_MES[101], 'format' => 'layout_bytes'),
        7 => array ('title'=> $BAW_MES[99], 'format' => 'layout_bytes'),
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[73], 'format' => 'layout_text', 'colspan' => 3),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        3 => array ('percent' => true, 'title'=> $BAW_MES[75], 'format' => 'layout_bytes'),
    );

    $newval_table = array();
    $newval_chart = array();
    if ($val = baw_data($BAW_CURR['site_name'], "FILETYPES", $BAW_CURR['yearmonth'])) {
        // Files type - Hits - Bandwidth - Bandwidth without compression - Bandwidth after compression
        $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
        foreach ($val as $type => $data) {
            $icon = '';
            if (isset($BAW_LIB['files']['icons'][$type])) {
                $iconname = $BAW_LIB['files']['icons'][$type];
                $icon = baw_create_image($BAW_CONF['icons_url'] . "/mime/$iconname.png" , array("alt" => $iconname, "title" => $iconname));
            }
            if ($type == 'Unknown') {
                $type_text = "?";
                $desc = $type;
            } else {
                $type_text = $type;
                $desc = @$BAW_LIB['files']['types'][$BAW_LIB['files']['family'][$type]];
            }
            $newval_table[$type] = array(
                $icon,
                $type_text,
                $desc,
                $data[0],
                $data[1],
                $data[2],
                $data[3],
                $data[2] - $data[3]
            );
            $newval_chart[$type] = array(
                $icon,
                $type_text,
                $data[0],
                $data[1],
            );
       }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }

    // $chart = baw_array_sorting($chart, $set['sort'], $set['sort_dir']);
    // $pieval = baw_array_sorting($pieval, 1, $set['sort_dir']);
    $out = '';
    if ($set['chart']) {
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_urls($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF;
    $format = array(
        0 => array ('title'=> $BAW_MES[19], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[29], 'format' => 'layout_pages'),
        2 => array ('title'=> $BAW_MES[106], 'format' => 'layout_bytes'),
        3 => array ('title'=> $BAW_MES[104], 'format' => 'layout_hits'),
        4 => array ('title'=> $BAW_MES[116], 'format' => 'layout_hits')
    );
    $val = array();
    if ($val = baw_data($BAW_CURR['site_name'], "SIDER", $BAW_CURR['yearmonth'])) {
        foreach ($val as $url => $data) {
            $link = baw_create_link($url, "http://{$BAW_CURR['site_name']}$url", array(), true);
            // URL - Pages - Bandwidth - Entry - Exit
            $val[$url] = array(
                $link,
                $data[0],
                $data[1]/$data[0], // average size per page instead of sum
                $data[2],
                $data[3]
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $val = baw_array_sorting($val, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $val, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_paths($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF;
    $format = array(
        0 => array ('title'=> $BAW_MES['files_paths'], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[29], 'format' => 'layout_pages'),
    );
    $val = array();
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], "SIDER", $BAW_CURR['yearmonth'])) {
        foreach ($val as $url => $data) {
            $full_url = "http://{$BAW_CURR['site_name']}$url";
            $url_array = parse_url($full_url);
            $link_url = "http://{$BAW_CURR['site_name']}" . $url_array['path'];
            $link = baw_create_link($url_array['path'], $link_url, array(), true);
            if (!isset($newval[$url_array['path']])) {
                $newval[$url_array['path']] = array(
                    0 => $link,
                    1 => $data[0]
                );
            } else {
                $newval[$url_array['path']][1] += $data[0];
            }
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_os($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format_table = array(
        0 => array ('title'=> $BAW_MES[59], 'format' => 'layout_text', 'colspan' => 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[59], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $newval_table = array();
    $newval_chart = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'OS', $BAW_CURR['yearmonth'])) {
        $newval_table = $BAW_LIB['os']['families']; // create an array where the main are already
        if ($BAW_CONF['module'] == 'drupal') {
            $url = "/{$BAW_CONF['drupal_base']}/details/unknownos/{$BAW_CURR['month']}/{$BAW_CURR['year']}";
        } else {
            $url = "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}&action=get_fulltable&what=unknownos";
        }
        // iterate all the data
        foreach ($val as $os => $data) {
            $isnew = true; // this os has not been added to newval yet (not families)
            // iterate all the std. OS types and add the date to the curret one if matches
            foreach ($BAW_LIB['os']['codes'] as $code) {
                if (stristr($os, $code) !== false) {
                    $newval_table[$code][2] += $data[0]; // found it, add stats
                    $iconid = $code;
                    $isnew = false; // we have the stats now
                }
            }
            if ($isnew) { // this is an additional, non-family os
                $iconid = $os;
                $os_link = $BAW_LIB['os']['list'][$os];
                $newval_table[$os] = array('', $os_link, $data[0]);
            }
            if ($os == 'Unknown') {
                $newval_table[$os][1] = baw_create_link($BAW_MES[0], $url);
            }
            $icon = baw_create_image(
                $BAW_CONF['icons_url'] . "/os/" . str_replace('/', '',
                strtolower($iconid)) . '.png',
                array('title' => strip_tags($newval_table[$iconid][1]))
            );
            $newval_table[$iconid][0] = $icon;
        }
        $newval_table = baw_array_sorting($newval_table, $set['sort'], $set['sort_dir']);
        // clean out data with 0 hits
        foreach ($newval_table as $id => $data) {
            if ($data[2] == 0) {
                unset ($newval_table[$id]);
            } else { // copy valid points into chart array
                $newval_chart[$id] = array(
                    $data[0],
                    $data[2]
                );
            }
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }

    $out = '';
    if ($set['chart']) {
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_osversions($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format_table = array(
        0 => array ('title'=> $BAW_MES[59], 'format' => 'layout_text', 'colspan' => 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[59], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    if ($val = baw_data($BAW_CURR['site_name'], 'OS', $BAW_CURR['yearmonth'])) {
        if ($BAW_CONF['module'] == 'drupal') {
            $url = "/{$BAW_CONF['drupal_base']}/details/unknownos/{$BAW_CURR['month']}/{$BAW_CURR['year']}";
        } else {
            $url = "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}&action=get_fulltable&what=unknownos";
        }
        $newval_table = array();
        foreach ($val as $os => $data) {
            $iconid = $os;
            $os_link = $BAW_LIB['os']['list'][$os];
            $icon = baw_create_image(
                $BAW_CONF['icons_url'] . "/os/" . str_replace('/', '',
                strtolower($iconid)) . '.png',
                array('title' => strip_tags($os_link))
            );
            $newval_table[$os] = array($icon, $os_link, $data[0]);
            if ($os == 'Unknown') {
                $newval_table[$os][1] = baw_create_link($BAW_MES[0], $url);
            }
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval_table = baw_array_sorting($newval_table, 2, $set['sort_dir']);
    $out = '';
    if ($set['chart']) {
        $newval_chart = array();
        foreach ($newval_table as $os => $data) {
            $newval_chart[$os] = array($data[0], $data[2]);
        }
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_unknownos($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[46], 'format' => 'layout_text'),
        1 => array ('percent' => false, 'title'=> $BAW_MES[9], 'format' => 'layout_date')
    );
    $val = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'UNKNOWNREFERER', $BAW_CURR['yearmonth'])) {
        $newval = array();
        foreach ($val as $os_ver => $data) {
            $os_ver = strip_tags($os_ver);
            $newval[] = array($os_ver, $data[0]);
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_browsers($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format_table = array(
        0 => array ('title'=> $BAW_MES[21], 'format' => 'layout_text', 'colspan' => 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('title'=> $BAW_MES[111], 'format' => 'layout_text'),
        3 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[21], 'format' => 'layout_text',),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $search = array(
        '\\s',
        '\\-'
    );
    $replace  = array(
        "_",
        "-"
    );
    $newval_table = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'BROWSER', $BAW_CURR['yearmonth'])) {
        $lib = $BAW_LIB['browser']['names'];
        if ($BAW_CONF['module'] == 'drupal') {
            $url = "/{$BAW_CONF['drupal_base']}/details/unknownbrowser/{$BAW_CURR['month']}/{$BAW_CURR['year']}";
        } else {
            $url = "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}&action=get_fulltable&what=unknownbrowser";
        }
        // iterate all browsers in the library
        foreach ($lib as $browser => $description) {
            // get the icon
            $icon = '';
            if (isset($BAW_LIB['browser']['icons'][$browser])) {
                $icon = $BAW_LIB['browser']['icons'][$browser];
                $icon = str_replace($search, $replace, $icon);
                $icon = baw_create_image(
                    $BAW_CONF['icons_url'] . "/browser/$icon.png",
                    array('title' => strip_tags($description))
                );
            } else {
                // we have to create an icon since the chart is indexed by the icon
                // html. they have to be different each.
                $icon = baw_create_image(
                    $BAW_CONF['site_path'] . "/icons/empty.png",
                    array('title' => strip_tags($description))
                );
            }
            // grabber ?
            $grabber = $BAW_MES[113];
            if (isset($BAW_LIB['browser']['grabbers'][$browser])) {
                $grabber = $BAW_MES[112];
            }
            // create table line for this browser since we dont need versions
            $newval_table[$browser] = array($icon, $description, $grabber, 0);
            if ($browser == 'Unknown') {
                $newval_table[$browser][1] = baw_create_link($BAW_MES[0], $url);
                $newval_table[$browser][2] = '?';
            }
            // summ all occurences of this browser
            foreach ($val as $browser_ver => $hits) {
                // only add if hits and if the browser matches
                if (($hits > 0) and stristr($browser_ver, $browser) !== false) {
                    @$newval_table[$browser][3] += $hits[0];
                }
            }
            // kill zero-value browsers
            if ($newval_table[$browser][3] == 0) {
                unset($newval_table[$browser]);
            }
        }
        $newval_table = baw_array_sorting($newval_table, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }

    $out = '';
    if ($set['chart']) {
        foreach ($newval_table as $browser => $data) {
            $newval_chart[$browser] = array($data[0], $data[3]);
        }
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_browserversions($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format_table = array(
        0 => array ('title'=> $BAW_MES['browser_versions'], 'format' => 'layout_text', 'colspan' => 3),
        1 => array ('format' => 'layout_text'),
        2 => array ('format' => 'layout_text'),
        3 => array ('title'=> $BAW_MES[111], 'format' => 'layout_text'),
        4 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES['browser_versions'], 'format' => 'layout_text'),
        1 => array ('format' => 'layout_text'),
        2 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $search = array(
        '\\s',
        '\\-'
    );
    $replace  = array(
        "_",
        "-"
    );
    $newval_table = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'BROWSER', $BAW_CURR['yearmonth'])) {
        if ($BAW_CONF['module'] == 'drupal') {
            $url = "/{$BAW_CONF['drupal_base']}/details/unknownbrowser/{$BAW_CURR['month']}/{$BAW_CURR['year']}";
        } else {
            $url = "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}&action=get_fulltable&what=unknownbrowser";
        }
        $lib = $BAW_LIB['browser']['names'];
        // iterate all browsers in the library
        foreach ($val as $browser_ver => $hits) {
            // separate text from numbers/points
            preg_match('/([a-zA-Z\W]+)((.?([0-9]))*)/', $browser_ver, $matches);
            $browser = $matches[1];
            $version = $matches[2];

            $icon = '';
            $grabber = $BAW_MES[113];
            if (isset($BAW_LIB['browser']['names'][$browser])) {
                $description = $lib[$browser];
                if (isset($BAW_LIB['browser']['icons'][$browser])) {
                    $icon = $BAW_LIB['browser']['icons'][$browser];
                    $icon = str_replace($search, $replace, $icon);
                    $icon = baw_create_image(
                        $BAW_CONF['icons_url'] . "/browser/$icon.png",
                        array('title' => strip_tags($description) . " " . $version)
                    );
                } else {
                    $icon = baw_create_image(
                        $BAW_CONF['site_path'] . "/icons/empty.png",
                        array('title' => strip_tags($description) . " " . $version)
                    );
                }
            } else {
                $description = $browser_ver;
                $grabber = '?';
            }
            // grabber ?
            if (isset($BAW_LIB['browser']['grabbers'][$browser])) {
                $grabber = $BAW_MES[112];
            }
            @$newval_table[$browser_ver] = array($icon, $description , $version, $grabber, $hits[0]);
            if ($browser == 'Unknown') {
                $newval_table[$browser][1] = baw_create_link($BAW_MES[0], $url);
                $newval_table[$browser][2] = '?';
            }
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval_table = baw_array_sorting($newval_table, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['chart']) {
        $newval_chart = array();
        foreach ($newval_table as $browser => $data) {
            $newval_chart[$browser] = array($data[0], $data[2], $data[4]);
        }
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval_table, $format_table, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_unknownbrowser($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[50], 'format' => 'layout_text'),
        1 => array ('percent' => false, 'title'=> $BAW_MES[9], 'format' => 'layout_date')
    );
    $val = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'UNKNOWNREFERERBROWSER', $BAW_CURR['yearmonth'])) {
        $newval = array();
        foreach ($val as $browser_ver => $data) {
            $browser_ver = strip_tags($browser_ver);
            $newval[] = array($browser_ver, $data[0]);
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_screensizes($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[135], 'format' => 'layout_text', 'colspan' => 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $format_chart = array(
        0 => array ('title'=> $BAW_MES[135], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $val = array();
    $newval = array();
    $url = $BAW_CONF['site_url'] . "/icons/screen.png";
    if ($val = baw_data($BAW_CURR['site_name'], 'SCREENSIZE', $BAW_CURR['yearmonth'])) {
        foreach ($val as $size => $data) {
            $size_arr = explode("x", $size);
            $sizex = $size_arr[0] / 80;
            $sizey = $size_arr[1] / 80;
            $img_arr = array(
                'height'=> $sizey,
                'width' => $sizex,
                'title' => $size,
                'class' => 'screensize'
            );
            $newval[$size] = array(
                baw_create_image($url, $img_arr),
                $size,
                $data[0],
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['chart']) {
        $newval_chart = array();
        foreach ($newval as $size => $data) {
            $newval_chart[$size] = array($data[0], $data[2]);
        }
        $out .= baw_render_htmlchart($newval_chart, $format_chart, $set['avg'], $set['top_x']);
    }
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_se_referers($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    if ($set['favicon']) {
        $format = array(
            0 => array ('title'=> $BAW_MES[126], 'format' => 'layout_text', 'colspan' => 2),
            1 => array ('format' => 'layout_text'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            3 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
        $set['sort']++;
    } else {
        $format = array(
            0 => array ('title'=> $BAW_MES[126], 'format' => 'layout_text'),
            1 => array ('percent' => true, 'title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
    }
    $val = array();
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'SEREFERRALS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            $sename = @$BAW_LIB['searchengines']['names'][$id];
            if ($set['favicon']) {
                $image = baw_get_favicon($sename);
                $newval[$id] = array(
                    $image,
                    $sename,
                    $data[0],
                    $data[1]
                );
            } else {
                $newval[$id] = array(
                    $sename,
                    $data[0],
                    $data[1]
                );
            }

        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_referers($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    if ($set['favicon']) {
        $format = array(
            0 => array ('title'=> $BAW_MES[127], 'format' => 'layout_text', 'colspan' => 2),
            1 => array ('format' => 'layout_text'),
            2 => array ('title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            3 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
        $set['sort']++;
    } else {
        $format = array(
            0 => array ('title'=> $BAW_MES[127], 'format' => 'layout_text'),
            1 => array ('title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
    }
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'PAGEREFS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            if ($data[0] > 0) {
                $link = baw_create_link($id, $id, array(), true, true);
                if ($set['favicon']) {
                    $image = baw_get_favicon($id);
                    $newval[$id] = array($image,$link,$data[0],$data[1]);

                } else {
                    $newval[$id] = array($link,$data[0],$data[1]);
                }
            }
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_referer_domains($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB, $BAW_CONF_DIS;
    if ($set['favicon']) {
        $format = array(
            0 => array ('title'=> sprintf($BAW_MES['by_domains'], $BAW_MES[127]) , 'format' => 'layout_text', 'colspan' => 2),
            1 => array ('format' => 'layout_text'),
            2 => array ('title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            3 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
        $set['sort']++;
    } else {
        $format = array(
            0 => array ('title'=> sprintf($BAW_MES['by_domains'], $BAW_MES[127]) , 'format' => 'layout_text'),
            1 => array ('title'=> $BAW_MES[56], 'format' => 'layout_pages'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
    }
    $newval = array();
    $length = $BAW_CONF_DIS['referer_domains']['domain_lvls'];
    if ($val = baw_data($BAW_CURR['site_name'], 'PAGEREFS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            if ($data[0] > 0) {
                $tld = @parse_url($id);
                // we create & flip the array
                if ($length !== -1) {
                    $domain_array = explode(".", $tld['host']);
                    // we add the x number of elements to the new array
                    if (!is_numeric(implode("",$domain_array))) {

                        $domain_array = array_slice($domain_array, -$length);
                    }
                    $domain = implode(".", $domain_array);
                } else {
                    $domain = $tld['host'];
                }
                $tld = $tld['scheme'] ."://". $domain;
                // $tld = str_replace("http://www.", "http://", $tld);
                if ($set['favicon']) {
                    if (!isset($newval[$tld])) {
                        $image = baw_get_favicon($tld);
                        $link = baw_create_link($tld, $tld);
                        $newval[$tld] = array($image, $link, $data[0],$data[1]);
                    } else {
                        $newval[$tld][2] += $data[0];
                        $newval[$tld][3] += $data[1];
                    }
                } else {
                    if (!isset($newval[$tld])) {
                        $link = baw_create_link($tld, $tld);
                        $newval[$tld] = array($link, $data[0],$data[1]);
                    } else {
                        $newval[$tld][1] += $data[0];
                        $newval[$tld][2] += $data[1];
                    }
                }
            }
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}


function baw_display_hotlinks($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    if ($set['favicon']) {
        $format = array(
            0 => array ('title'=> $BAW_MES['referer_hotlinks'], 'format' => 'layout_text', 'colspan' => 2),
            1 => array ('format' => 'layout_text'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
        $set['sort']++;
    } else {
        $format = array(
            0 => array ('title'=> $BAW_MES['referer_hotlinks'], 'format' => 'layout_text'),
            1 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
    }
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'PAGEREFS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            if ($data[0] == 0) {
                $link = baw_create_link($id, $id, array(), true);
                if ($set['favicon']) {
                    $image = baw_get_favicon($id);
                    $newval[$id] = array($image, $link, $data[1]);
                } else {
                    $newval[$id] = array($link, $data[1]);
                }
            }
        }
        if ($BAW_CONF['hideempty'] && count($newval) == 0 ) {
            return '';
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_hotlink_domains($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB, $BAW_CONF_DIS;
    if ($set['favicon']) {
        $format = array(
            0 => array ('title'=> sprintf($BAW_MES['hl_by_domains '], $BAW_MES[127]), 'format' => 'layout_text', 'colspan' => 2),
            1 => array ('format' => 'layout_text'),
            2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
        $set['sort']++;
    } else {
        $format = array(
            0 => array ('title'=> sprintf($BAW_MES['hl_by_domains '], $BAW_MES[127]), 'format' => 'layout_text'),
            1 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
        );
    }
    $length = $BAW_CONF_DIS['hotlink_domains']['domain_lvls'];
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'PAGEREFS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            if ($data[0] == 0) {
                $tld = @parse_url($id);
                if ($length !== -1) {
                    $domain_array = explode(".", $tld['host']);
                    // we add the x number of elements to the new array
                    if (!is_numeric(implode("",$domain_array))) {

                        $domain_array = array_slice($domain_array, -$length);
                    }
                    $domain = implode(".", $domain_array);
                } else {
                    $domain = $tld['host'];
                }
                $tld = $tld['scheme'] ."://". $domain;
                if ($set['favicon']) {
                    if (!isset($newval[$tld])) {
                        $link = baw_create_link($tld, $tld);
                        $image = baw_get_favicon($tld);
                        $newval[$tld] = array($image, $link, $data[1]);
                    } else {
                        // $newval[$tld][1] += $data[1];
                        $newval[$tld][2] += $data[1];
                    }
                } else {
                    if (!isset($newval[$tld])) {
                        $link = baw_create_link($tld, $tld);
                        $newval[$tld] = array($link, $data[1]);
                    } else {
                        // $newval[$tld][1] += $data[1];
                        $newval[$tld][1] += $data[1];
                    }
                }
            }
        }
        // check again since none of the domains might be hotlinking
        if ($BAW_CONF['hideempty'] && count($newval) == 0 ) {
            return '';
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}


function baw_display_searchphrases($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[103], 'format' => 'layout_text'),
        1 => array ('percent'=> true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $val = array();
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'SEARCHWORDS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            $newval[$id] = array(
                urldecode($id),
                $data[0]
            );
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_searchwords($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[13], 'format' => 'layout_text'),
        1 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $val = array();
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'KEYWORDS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $id => $data) {
            $newval[$id] = array(
                urldecode($id),
                $data[0],
            );
        }
        $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, $set['avg'], $set['total'], $set['top_x'], false);
    }
    return $out;
}

function baw_display_misc($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[139], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        2 => array ('title'=> $BAW_MES[15], 'format' => 'layout_percent')
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'MISC', $BAW_CURR['yearmonth'])) {
        foreach ($BAW_LIB['misc'] as $type => $msg) {
            @$percent = $val[$type][1] / ($val['TotalMisc'][1]  / 100);
            $newval[$type] = array($BAW_MES[$msg], @$val[$type][1], $percent);
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format);
    }
    return $out;
}

function baw_display_errors($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[32], 'format' => 'layout_text', 'colspan'=> 2),
        1 => array ('format' => 'layout_text'),
        2 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        3 => array ('title'=> $BAW_MES[106], 'format' => 'layout_bytes')
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'ERRORS', $BAW_CURR['yearmonth'])) {
        foreach ($val as $type => $data) {
            $ntype = $type;
            if ($type == '404') {
                if ($BAW_CONF['module'] == 'drupal') {
                    $ntype = baw_create_link('404', "/{$BAW_CONF['drupal_base']}/details/errors404/{$BAW_CURR['month']}/{$BAW_CURR['year']}");
                } else {
                    $ntype = baw_create_link('404', "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}&action=get_fulltable&what=errors404");
                }
            }
            $newval[$type] = array(
                $ntype, $BAW_LIB['http_status'][$type], $data[0], $data[1]
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format);
    }
    return $out;
}

function baw_display_errors404($set) {
    global $BAW_MES, $BAW_CURR, $BAW_CONF, $BAW_LIB;
    $format = array(
        0 => array ('title'=> $BAW_MES[49], 'format' => 'layout_text'),
        1 => array ('title'=> $BAW_MES[57], 'format' => 'layout_hits'),
        2 => array ('title'=> $BAW_MES[127], 'format' => 'layout_text')
    );
    $newval = array();
    if ($val = baw_data($BAW_CURR['site_name'], 'SIDER_404', $BAW_CURR['yearmonth'])) {
        foreach ($val as $type => $data) {
            $newval[$type] = array(
                baw_create_link($type, "http://{$BAW_CURR['site_name']}", array(), true),
                $data[0],
                baw_create_link($data[1],$data[1], array(), true)
            );
        }
    } else if ($BAW_CONF['hideempty']){
        return '';
    }
    $newval = baw_array_sorting($newval, $set['sort'], $set['sort_dir']);
    $out = '';
    if ($set['table']) {
        $out .= baw_render_table($set['name'], $newval, $format, false, $set['total'], $set['top_x']);
    }
    return $out;
}
/*

  From0 6240 13402
From1 3 11 // unknown
From2 5471 8003 // search engine
From3 1956 3723 // external page
From4 10544 208546 // bookmarks/direct
From5 0 0 // newsgroups

*/
?>
