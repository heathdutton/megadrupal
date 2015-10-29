<?php
/**
 * betterawstats - an alternative display for awstats data
 *
 * @author      Oliver Spiesshofer, support at betterawstats dot com
 * @copyright   2008 Oliver Spiesshofer
 * @version     1.0
 * @link        http://betterawstats.com

 * Based on the initial BAWstats drupal module map
 * copyright 2007  Andrew Gillies (anaru at equivocation dot org)
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
  *
  * @param $data
  *   Page and hit data for each country
  * @param $top_x
  *   The number of domains to display. If false, display all.
  *
  * @return
  *   A string containing the HTML to include an image of the map.
  */
function baw_render_map($data, $top_x = false) {
    global $BAW_CONF;
    // if we don't have GD2 functions, we can't generate the image
    baw_debug("rendering map");
    if (!function_exists('imagecreatetruecolor')) {
        baw_raise_error('gd2notavailable', array());
        return;
    }

    $im = $BAW_CONF['site_path'] .'/icons/wmap.png';
    if (!file_exists($im)) {
        baw_raise_error('mapnotavailable', array($im));
    }

    // fix data for geography
    $us_domains =array('us', 'mil', 'edu', 'gov', 'arpa');
    foreach ($us_domains as $us_domain) {
        if (isset($data[$us_domain][3])) {
            if (!isset($data['us'])) {
                $data['us'] = array(0,0,0,0,0);
            }
            $data['us'][3] += $data[$us_domain][3];
        }
        if (isset($data[$us_domain][4])) {
            if (!isset($data['us'])) {
                $data['us'] = array(0,0,0,0,0);
            }
            $data['us'][4] += $data[$us_domain][4];
        }
    }
    $doms = 0;
    $query = array('c' => array());
    foreach ($data as $country => $vars) {
        if (($doms >= $top_x) && ($top_x !== false)) {
            break;
        }
        $query['c'][$country] = array("{$vars[3]}", "{$vars[4]}");
        $doms ++;
    }

    $img_url = url("admin/reports/bawstats/modules/render_map", array('query' => $query));
    $out = "<div class=\"aligncenter\"><img width=\"574\" height=\"286\" alt=\"map of domains\" src=\"$img_url\"" . XHTML . "></div>";
    baw_debug("rendering map finished");
    return $out;
}


/**
 *
 * Produce the image of the map. The data is passed via the c query arguments.
 *
 */
function bawstats_render_map_image() {
  $data = $_GET['c'];

  // Set headers
  drupal_add_http_header('Expires','Mon, 01 Jan 1997 05:00:00 GMT');
  drupal_add_http_header('Cache-Control','no-store, no-cache, must-revalidate');
  drupal_add_http_header('Cache-Control','post-check=0, pre-check=0');
  drupal_add_http_header('Pragma','no-cache');
  drupal_add_http_header('Content-type','image/png');

  $site_path = drupal_get_path('module', 'bawstats');

  $im = bawstats_create_map_image(
    $site_path . '/icons/wmap.png',
    $site_path . '/icons/circ-blue.png',
    $site_path . '/icons/circ-green.png',
    $data);

  //output image to browser
  imagepng($im);
  imagedestroy($im);
}

/**
 * Create an world map image with statistics
 *
 * @param $map_file
 *   Path to file of world map
 * @param $crcblue_file
 *   Path to file containing blue circle
 * @param $crcgreen_file
 *   Path to file containing green circle
 * @param $data
 *   Array of data to be plotted
 * @return
 *   PNG image that can be displayed using imagepng
 */
function bawstats_create_map_image($map_file, $crcblue_file, $crcgreen_file, $data) {
    $im = $map_file;
    if (!file_exists($im)) {
      drupal_set_message(t('Image file does not exist.'), 'error');
    }
    $im = imagecreatefrompng($im);
    imagealphablending($im, true);

    $mapX = imagesx($im);
    $mapY = imagesy($im);

    $dommap = array(
        'uk' => array(266,88), 'au' => array(485,224), 'nz' => array(545,251),
        'be' => array(276,95), 'es' => array(262,116), 'pt' => array(256,117),
        'ad' => array(271,111), 'it' => array(289,111), 'fr' => array(272,103),
        'ie' => array(257,90), 'nl' => array(277,92), 'de' => array(284,94),
        'pl' => array(292,91), 'lu' => array(276,97), 'ch' => array(283,102),
        'at' => array(287,102), 'gr' => array(304,118), 'al' => array(300,114),
        'cs' => array(299,109), 'ba' => array(296,108), 'mk' => array(303,114),
        'bg' => array(307,110), 'ro' => array(309,105), 'hu' => array(300,102),
        'sk' => array(300,99), 'cz' => array(292,97), 'pl' => array(296,91),
        'dk' => array(283,83), 'va' => array(288,112), 'tr' => array(321,117),
        'ua' => array(320,97), 'by' => array(312,88), 'lt' => array(304,86),
        'lv' => array(305,81), 'ee' => array(308,77), 'fi' => array(310,65),
        'se' => array(296,64), 'no' => array(281,72), 'is' => array(241,62),
        'cy' => array(321,125), 'sy' => array(329,125), 'il' => array(325,129),
        'ps' => array(325,130), 'jo' => array(326,132), 'sa' => array(339,142),
        'iq' => array(340,130), 'ir' => array(358,130), 'kw' => array(344,135),
        'bh' => array(348,140), 'qa' => array(350,141), 'ae' => array(355,144),
        'om' => array(358,148), 'ye' => array(345,157), 'eg' => array(316,140),
        'pk' => array(372,135), 'af' => array(368,125), 'tm' => array(364,119),
        'uz' => array(365,111), 'kz' => array(371,97), 'kg' => array(379,112),
        'ti' => array(378,116), 'in' => array(391,152), 'np' => array(402,137),
        'bd' => array(411,143), 'lk' => array(396,171), 'bt' => array(412,137),
        'mm' => array(419,146), 'la' => array(431,151), 'vn' => array(435,149),
        'kh' => array(435,162), 'th' => array(431,159), 'my' => array(430,177),
        'sg' => array(433,180), 'ph' => array(462,162), 'id' => array(459,185),
        'ti' => array(468,197), 'tw' => array(460,144), 'cn' => array(433,120),
        'mm' => array(411,92), 'ru' => array(399,64), 'kr' => array(471,121),
        'kp' => array(469,115), 'jp' => array(490,120), //TODO polynesia
        'gl' => array(204,27), 'ca' => array(103,76), 'us' => array(107,116),
        'bm' => array(160,132), 'mx' => array(108,145), 'cu' => array(145,149),
        'gt' => array(126,156), 'bz' => array(129,155), 'hn' => array(134,160),
        'sv' => array(128,161), 'ni' => array(136,163), 'cr' => array(138,169),
        'pa' => array(145,170), 'co' => array(152,176), 've' => array(165,170),
        'gy' => array(175,174), 'sr' => array(182,177), 'gf' => array(186,177),
        'pe' => array(151,196), 'ec' => array(147,183), 'br' => array(189,197),
        'bo' => array(170,212), 'py' => array(176,218), 'uy' => array(182,235),
        'cl' => array(158,231), 'ar' => array(168,235), 'fk' => array(177,274),
        'ly' => array(293,135), 'tn' => array(284,124), 'dz' => array(174,128),
        'ma' => array(258,130), 'eh' => array(249,139), 'mr' => array(252,152),
        'ml' => array(263,154), 'ne' => array(282,155), 'td' => array(304,158),
        'sd' => array(316,154), 'et' => array(331,165), 'er' => array(330,156),
        'dj' => array(336,162), 'sn' => array(246,159), 'gm' => array(244,160),
        'gw' => array(246,163), 'gn' => array(248,163), 'sl' => array(250,168),
        'lr' => array(255,172), 'ci' => array(260,170), 'gh' => array(267,169),
        'bf' => array(266,163), 'bj' => array(273,176), 'ng' => array(276,167),
        'tg' => array(174,167), 'cm' => array(288,174), 'cf' => array(299,170),
        'so' => array(343,173), 'ke' => array(325,183), 'cd' => array(309,186),
        'ug' => array(319,183), 'rw' => array(319,188), 'bi' => array(319,190),
        'tz' => array(325,194), 'ao' => array(298,200), 'zm' => array(313,204),
        'mz' => array(329,202), 'mw' => array(323,204), 'zw' => array(315,212),
        'bw' => array(305,221), 'na' => array(296,216), 'za' => array(306,230),
        'sz' => array(317,227), 'mg' => array(343,212),  'com' => array(40,158),
        'net' => array(40,188), 'org' => array(40,221), 'info' => array(94,222),
        'ip' => array(94,189), 'mobi'=>  array(40,253), 'mobi'=>  array(94,253)
    );

    $maxpages = 0;
    $maxhits = 0;
    $doms = array();
    //var_dump($data);
    foreach ($data as $domain => $set) {
        $pages  = $set[0];
        $hits  = $set[1];
        // we use hits here since they are always bigger than pages
        if (isset($dommap[$domain]) && ($hits>0)) {
            $doms[$domain] = array('pages' => $pages, 'hits' => $hits);
            if ($hits > $maxhits) {
                $maxhits = $hits;
            }
            if ($pages > $maxpages) {
                $maxpages = $pages;
            }
        }
    }

    $crcblue = @imagecreatefrompng($crcblue_file);
    imagealphablending($crcblue, true);
    $crcgreen = @imagecreatefrompng($crcgreen_file);
    imagealphablending($crcgreen, true);
    $crcX = imagesx($crcgreen);
    $crcY = imagesy($crcgreen);

    $maxcirc = 80;
    $mincirc = 2;

    foreach ($doms as $dom => $val) {
        $hits = ($val['hits'] / $maxhits);
        $scz = $mincirc + ($hits*($maxcirc-$mincirc));
        imagecopyresampled(
            $im,
            $crcgreen,
            $dommap[$dom][0]-1,
            $dommap[$dom][1]-($scz/2),
            0,
            0,
            $scz/2,
            $scz,
            $crcX,
            $crcY
        );
        $pages = ($val['pages'] / $maxpages);
        $scz = $mincirc + ($pages*($maxcirc-$mincirc));
        imagecopyresampled(
            $im,
            $crcblue,
            $dommap[$dom][0]-($scz/2),
            $dommap[$dom][1]-($scz/2),
            0,
            0,
            $scz/2,
            $scz,
            $crcX,
            $crcY
        );
    }
    return($im);
}