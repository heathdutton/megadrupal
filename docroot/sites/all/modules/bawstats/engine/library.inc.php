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
 * This file sets default/addon data for libraries and reads the aditional data
 * from the library files.
 */


// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'library.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

$BAW_LIB['formats'] = array(
    // class img text format
    'layout_visitors' => array(
        'img' => 'vu.png',
        'txt' => 11,
        'frm' => 'baw_num_format(%s);',
        'max'=> 'layout_visits'),
    'layout_visits' => array(
        'img' => 'vv.png',
        'txt' => 10,
        'frm' => 'baw_num_format(%s);',
        'max'=> 'layout_visits'),
    'layout_pages' => array(
        'img' => 'vp.png',
        'txt' => 56,
        'frm' => 'baw_num_format(%s);',
        'max'=> 'layout_pages'),
    'layout_hits' => array(
        'img' => 'vh.png',
        'txt' => 57,
        'frm' => 'baw_num_format(%s);',
        'max'=> 'layout_hits'),
    'layout_bytes' => array(
        'img' => 'vk.png',
        'txt' => 75,
        'frm' => 'baw_byte_format(%s);',
        'max'=> 'layout_bytes'),
    'layout_percent' => array(
        'img' => '',
        'txt' => 15,
        'frm' => 'baw_percent_format(%s);',
        'max'=> ''),
    'layout_date' => array(
        'img' => '',
        'txt' => '',
        'frm' => 'baw_getdate_format(%s);',
        'max'=> ''),
    'layout_text' => array(
        'img' => '',
        'txt' => '',
        'frm' => false,
        'max'=> ''),
    'layout_ratio' => array(
        'img' => '',
        'txt' => '',
        'frm' => 'baw_num_format(%s,2);',
        'max'=> '')
);

// this is defined here to indicate the order of the array, otherwise the information is in the datafile
$BAW_LIB['sessions'] = array(
    '0s-30s', '30s-2mn', '2mn-5mn', '5mn-15mn', '15mn-30mn', '30mn-1h', '1h+'
);

$BAW_LIB['os']['families'] = array(
    'win'  => array('win', '<b>Windows</b>',0),
    'mac'  => array('mac', '<b>Macintosh</b>',0),
    'linux'=> array('linux', '<b>Linux</b>',0),
    'bsd'  => array('bsd', '<b>BSD</b>',0)
);

$BAW_LIB['os']['codes'] = array('win', 'mac', 'linux', 'bsd');
$BAW_LIB['os']['list'] = baw_get_library("/operating_systems.pm", 'OSHashLib');
$BAW_LIB['domains'] = baw_get_library("/domains.pm", 'DomainsHashIDLib');
$BAW_LIB['robots'] = baw_get_library("/robots.pm", 'RobotsHashIDLib');
list (
    $BAW_LIB['worms']['names'],
    $BAW_LIB['worms']['targets']) = baw_get_library(
        "/worms.pm",
        array('WormsHashLib', 'WormsHashTarget')
);
list (
    $mime_hash_lib,
    $mime_hash_family) = baw_get_library (
        "/mime.pm",
        array('MimeHashLib', 'MimeHashFamily')
);
// In the 7.x version of AWStats the meanings of the mime arrays is
// different from the 6.x versions. We can check for this by seeing if the
// first element of $mime_hash_lib has two elements.
$mime_hash_lib_values = array_values($mime_hash_lib);
if (count($mime_hash_lib_values[0]) == 2) {
    // This is the 7.x version of AWStats
    $BAW_LIB['files']['types'] = $mime_hash_family;
    foreach ($mime_hash_lib as $ext => $family_type) {
        $BAW_LIB['files']['family'][$ext] = $family_type[0];
    }
    $BAW_LIB['files']['icons'] = $BAW_LIB['files']['family'];
}
else {
    // This is the 6.x version of AWStats
    $mime_hash_icon = baw_get_library("/mime.pm", 'MimeHashIcon');
    $BAW_LIB['files']['types'] = $mime_hash_lib;
    $BAW_LIB['files']['icons'] = $mime_hash_icon;
    $BAW_LIB['files']['family'] = $mime_hash_family;
}
$BAW_LIB['browser']['familes'] = array('msie'=>1,'firefox'=>2,'netscape'=>3,'svn'=>4);
list (
    $BAW_LIB['browser']['names'],
    $BAW_LIB['browser']['grabbers'],
    $BAW_LIB['browser']['icons']) = baw_get_library(
        "/browsers.pm",
        array('BrowsersHashIDLib','BrowsersHereAreGrabbers','BrowsersHashIcon')
);
$BAW_LIB['searchengines']['names']= baw_get_library("/search_engines.pm", 'SearchEnginesHashLib');
$BAW_LIB['misc'] = array(
    // 'TotalMisc' => 0,
    // 'AddToFavourites' => 137,
    'JavascriptDisabled' => 168,
    'JavaEnabled' => 140,
    'DirectorSupport'=> 141,
    'FlashSupport' => 142,
    'RealPlayerSupport'=> 143,
    'QuickTimeSupport' => 144,
    'WindowsMediaPlayerSupport' => 145,
    'PDFSupport' => 146
);
$BAW_LIB['http_status'] = baw_get_library("/status_http.pm", 'httpcodelib');

$BAW_LIB['item_groups'] = array(
    'time' => array(
        'title' => $BAW_MES['time'],
        'members' => array('overview', 'months', 'days', 'weekdays', 'hours')
    ),
    'userinfo' => array(
        'title' => $BAW_MES['user_information'],
        'members' => array('domains', 'visitors', 'os', 'osversions', 'browsers','browserversions', 'misc', 'screensizes')
    ),
    'actions' => array(
        'title' => $BAW_MES['user_actions'],
        'members' => array('logins', 'sessions', 'filetype', 'urls')
    ),
    'origin' => array(
        'title' => $BAW_MES['user_origin'],
        'members' => array('referers', 'referer_domains', 'se_referers','searchphrases', 'searchwords')
    ),
    'other' => array(
        'title' => $BAW_MES['other_access'],
        'members' => array('robots', 'worms', 'unknownos', 'unknownbrowser', 'hotlinks', 'hotlink_domains', 'errors', 'errors404')
    )
);

/*
  this is obsolete but kept here for ev. future usage

$BAW_LIB['data']['full'] = array(
    'GENERAL' => array('Misc ID', 'Pages', 'Hits', 'Bandwidth'),
    'TIME' => array('Hour','Pages','Hits','Bandwidth','Not viewed Pages','Not viewed Hits','Not viewed Bandwidth'),
    'VISITOR' => array('Host','Pages','Hits','Bandwidth','Last visit date','[Start date of last visit]','[Last page of last visit]'),
    'DAY' => array('Date','Pages','Hits','Bandwidth','Visits'),
    'DOMAIN' => array('Domain','Pages','Hits','Bandwidth'),
    'LOGIN' => array('Login','Pages','Hits','Bandwidth','Last visit'),
    'ROBOT' => array('Robot ID','Hits','Bandwidth','Last visit','Hits on robots.txt'),
    'WORMS' => array('Worm ID','Hits','Bandwidth','Last visit'),
    'SESSION' => array('Session range','Number of visits'),
    'SIDER' => array('URL','Pages','Bandwidth','Entry','Exit'),
    'FILETYPES' => array('Files type','Hits','Bandwidth','Bandwidth without compression','Bandwidth after compression'),
    'OS' => array('OS ID','Hits'),
    'BROWSER' => array('Browser ID','Hits'),
    'SCREENSIZE' => array('Screen size','Hits'),
    'UNKNOWNREFERER' => array('Unknown referer OS','Last visit date'),
    'UNKNOWNREFERERBROWSER' => array('Unknown referer Browser','Last visit date'),
    'ORIGIN' => array('Origin','Pages','Hits '),
    'SEREFERRALS' => array('Search engine referers ID','Pages','Hits'),
    'PAGEREFS' => array('External page referers','Pages','Hits'),
    'SEARCHWORDS' => array('Search keyphrases','Number of search'),
    'KEYWORDS' => array('Search keywords','Number of search'),
    'MISC' => array('Misc ID','Pages','Hits','Bandwidth'),
    'ERRORS' => array('Errors','Hits','Bandwidth'),
    'CLUSTER' => array('Cluster ID','Pages','Hits','Bandwidth'),
    'SIDER_404' => array('URL with 404 errors','Hits','Last URL referer'),
    'EXTRA_1' => array('Extra key','Pages','Hits','Bandwidth','Last access')
);
*/

?>