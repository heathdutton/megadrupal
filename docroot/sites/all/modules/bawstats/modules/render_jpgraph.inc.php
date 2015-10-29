<?php
/**
 * betterawstats - an alternative display for awstats data
 *
 * @author      Oliver Spiesshofer, support at betterawstats dot com
 * @copyright   2007 Oliver Spiesshofer
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


// this file can't be used on its own
if (isset($_GET['getgraph'])) {
    baw_render_jpgraph_img();
} else if (strpos ($_SERVER['PHP_SELF'], 'reder_map.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

// $out .= baw_render_jpgraph ($set['name'], $newval, $format, 'pie', false, $set['top_x']);

function baw_render_jpgraph ($name, $chart, $format, $type='pie', $get_avg=false, $top_x=false, $sort=0) {
    global $BAW_CONF, $BAW_MES;
    $dir = $BAW_CONF['jpgraph_path'] . '/jpgraph.php';

    if (!file_exists($dir)) {
      baw_raise_error('jpgraph_path', array($dir));
      return;
    }

    $jp_data = array();
    // $jp_legend = array();
    $entries = 0;
    if (count($chart) == 0) {
        return "";
    }
    // calc others
    $f = '';
    $d = '';
    $others = 0;
    $other_arr = array();

    foreach ($chart as $id => $data) {
        $count = count($data);
        if ($entries < $top_x) {
            foreach ($format as $fid => $fdata) {
                $d .= "&amp;d[$fid][]={$data[$fid]}";
            }
        } else { // 'others'
            foreach ($format as $fid => $fdata) {
                if ($fdata['format'] !== 'layout_text' &&
                    $fdata['format'] !== 'layout_date') {
                    @$other_arr[$fid] += $data[$fid];
                } else {
                    if ($fid == 0) {
                        $other_arr[$fid] = $BAW_MES[2];
                    } else {
                        $other_arr[$fid] = '';
                    }
                }
            }
        }
        $entries ++;
    }
    foreach ($other_arr as $oid => $odata) {
        $d .= "&amp;d[$oid][$top_x]=$odata";
    }
    foreach ($format as $fid => $fdata) {
        $f .= "&amp;f[]= {$fdata['title']}";
    }

    // $count = count($jp_data);

    $img_url = $BAW_CONF['site_url'] . "/modules/render_jpgraph.inc.php?getgraph=true&amp;type=$type$d$f";
    $out = "<div class=\"aligncenter\"><img width=\"575\" height=\"286\" alt=\"chart: $name\" src=\"$img_url\"></div>";
    return $out;
}

function baw_render_jpgraph_img() {
    include_once("../config.php");

    if (isset($_GET['type'])) {
        $type = $_GET['type'];
    } else {
        return;
    }

    $d = $_GET['d'];
    $f = $_GET['f'];

    $count = count($d);
    $dir = $BAW_CONF['jpgraph_path'] . '/jpgraph.php';
    include_once ($dir);
    switch($type){
        case 'bar':
            include_once ($BAW_CONF['jpgraph_path'] . '/jpgraph_bar.php');
        break;
        case 'pie':
            include_once ($BAW_CONF['jpgraph_path'] . "/jpgraph_pie.php");
            $size = 0.17;
            $graph = new PieGraph(600,250,"auto");
            for ($i=0; $i<1; $i++) {
                $p1 = new PiePlot($d[$i]);
                if ($i == 0) {
                    $p1->SetLegends($d[0]);
                }
                $p1->title->Set($f[1]);
                $p1->SetSize($size);
                $vert = 0.15 + (($size + 0.1) * $i);
                $p1->SetCenter($vert,0.3);
                $p1->SetStartAngle(0);
                $p1->SetTheme("sand");
                $graph->Add($p1);
            }
            $graph->Stroke();
            break;
        case 'line':
            include_once ($BAW_CONF['jpgraph_path'] . "/jpgraph_line.php");
        break;
    }
    return $out;
}


?>