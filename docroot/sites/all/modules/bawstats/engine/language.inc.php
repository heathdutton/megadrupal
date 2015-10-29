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
 * This file coordinates the complete language-file processing.
 */


// this is the main function that should coordinate reading all language ressources
function baw_language() {
// find out what our language is
baw_detect_language();
// include the AWSTATS language file
baw_set_language();
// include the BAW language file
baw_select_internal_language();
// read the AWSTATS tooltips
// baw_get_tooltips();

}

function baw_set_language() {
    global $BAW_CONF, $BAW_MES;

    $path = $BAW_CONF['path_lang']."/awstats-{$BAW_CONF['lang']}.txt";
    if (!file_exists($path)) {
        // lets get an error-free error message
        $BAW_MES[58] = '';
        $BAW_MES['e'] = 'UTF-8';
        include_once($BAW_CONF['site_path'].'/language/lang_en.inc.php');
        echo baw_raise_error('languagefile', array($path));
        return;
    } else {
        $alternative = $BAW_CONF['path_lang']."/awstats-en.txt";
        baw_read_lng_data($path, $alternative);
    }
}

// this is detecting the required language if not set to auto by config.
// this is called only by baw_set_language()
function baw_detect_language() {
    global $BAW_CONF;
    $lang = '';
    if ($BAW_CONF['lang_setting'] == 'auto') {
        baw_debug('dbg_detect_language');
        $acc_lang = explode(',', @$_SERVER['HTTP_ACCEPT_LANGUAGE']);
        foreach ($acc_lang as $lang) {
            $lang = strtolower(substr($lang, 0, 2));
            if (file_exists($BAW_CONF['path_lang'] . "/awstats-$lang.txt")) {
                break;
            } else {
                $lang = '';
            }
        }
        if (!$lang) {
            $lang = 'en';
        }
    } else {
        $lang = $BAW_CONF['lang_setting'];
    }
    $BAW_CONF['lang'] = $lang;
    baw_debug('dbg_detected_language', $lang);
}

// we assume that awstats has more translations than betterawstats
// so we check if the one selected is available, otherwise include
/// english.
function baw_select_internal_language() {
    global $BAW_CONF;
    $lang = $BAW_CONF['lang'];
    $path = "{$BAW_CONF['site_path']}/language/lang_$lang.inc.php";
    if (file_exists($path)) {
        include_once($path);
    } else {
        $path = "{$BAW_CONF['site_path']}/language/lang_en.inc.php";
        include_once($path);
    }
}

// this function gets the file and returns the array
// that is then added to the language file.
function baw_read_lng_data($file, $alternative) {
    global $BAW_MES;
    $r = array();
    baw_debug("reading language data: $file");
    $lines = file($file);
    $encode_str = false;
    foreach ($lines as $line) {
        if (substr($line, 0, 7) == 'message') {
            $match = explode('=', $line);
            $match[0] = substr($match[0], 7);
            $r[$match[0]] = trim($match[1]);
        } else if (substr($line, 0, 8) == 'PageCode') {
            $encode_str = substr($line, 9);
            $r['e'] = $encode_str;
        }
    }
    // check that we have the whole file
    if (count($r) < 173) {
        baw_debug(
            "The language file is incomplete, only " . count($r)
            . "entries. Filling the rest with english"
        );
        $lines = file($alternative);
        foreach ($lines as $line) {
            if (substr($line, 0, 7) == 'message') {
                $match = explode('=', $line);
                $match[0] = substr($match[0], 7);
                if (!isset($r[$match[0]])) {
                    $r[$match[0]] = trim($match[1]);
                }
            }
        }
    }
    baw_debug("finished reading language data");
    $BAW_MES += $r;
}

function baw_get_tooltips() {
    global $BAW_CONF, $BAW_MES;
    $path = $BAW_CONF['path_lang']."/tooltips_w/awstats-tt-{$BAW_CONF['lang']}.txt";
    if (!file_exists($path)) {
        echo baw_raise_error('languagefile', array($path));
        $file_text = array();
    } else {
        $file_text = file_get_contents($path);
    }
    $search = array(
        "\n",
        "\r",
        "</div>",
        "<br />"
    );
    $replace = array(
        "",
        "",
        "",
        ""
    );
    $file_text = str_replace($search, $replace,$file_text);
    $file_text = explode('<div class="CTooltip" ', $file_text);
    array_shift($file_text);
    $pattern = '/id=\"tt(\d{1,3})\">(.*)/';
    foreach ($file_text as $line) {
        preg_match($pattern, $line, $match);
        $BAW_MES['tooltip'][$match[1]] = "<span>{$match[2]}</span>";
    }
}
?>