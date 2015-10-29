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

// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'render_htmlchart.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

function baw_render_htmlchart($chart, $format, $get_avg=false, $top_x=false, $dataformat=false) {
    baw_debug("rendering chart");
    global $BAW_CONF, $BAW_LIB, $BAW_MES;

    // FORMATS -----------------------------------------------------------------
    $format_arr = array();
    $itemcount = count($chart);

    // we have time data, $top_x has to work from the back instead of the front
    $time_data = false;
    if ($top_x < 0) {
        $time_data = true;
        $top_x = abs($top_x);
    }
    if ($get_avg) {
        $top_x++;
    }

    $top_x = min($BAW_CONF['max_chart_items'], $top_x);
    $top_x_count = $top_x;
    // if its not set, use count of items (day, month etc)
    if (!$top_x_count) {
        $top_x_count = $itemcount;
    }
    // find the smaller one to know the resulting lines
    if (min($top_x_count, $itemcount) > 12) {
        $width = 4;
    } else {
        $width = 6;
    }
    // iterate formats for header
    $text_fields = 0;
    foreach ($format as $cell => $attr) {
        // iterate one formats attributes
        $format_arr[] = $attr['format'];
        // we need the number of textfields so we can prepend them to the averages array.
        // otherwise the fields for avg and others are not in the right columns
        if ($attr['format'] == 'layout_text') {
            $text_fields++;
        }
        // $title_arr[] = $attr['title'];
    }
    // Max & averages calculation ----------------------------------------------
    // the max has to be calculated before so the bar height can be calculated
    // the sum has to be known for averages
    // iterate all the table and count the sums of all numeric data
    $max_arr = array();
    $sum_arr = array();
    $avg_arr = array();
    $others = array();

    $filled_lines = $itemcount; // get the value for substraction
    $row_no = 0;
    $othercount = 0;
    $hasothers = false;
    foreach ($chart as $lineid => $row) {
        $cell_no = 0;
        $rowsum = 0;
        $hasothers = false;
        foreach ($row as $cell) {
            if ($format_arr[$cell_no] != 'layout_text') { // we dont include the text
                // max calculation
                @$max_arr[$cell_no] = max($max_arr[$cell_no], $cell);
                // sum calculation
                @$sum_arr[$cell_no] += $cell;
                // average calculation
                if (is_numeric($cell) && $get_avg) {
                    $rowsum += $cell;
                    if ($row_no == ($itemcount-1) && $filled_lines >= 1) { // last line, assume sums are done
                        if ($rowsum == 0 && $cell_no == 0) { // do once again for last line, dont substract further after first cell
                            $filled_lines--;
                        }
                        $avg_arr[$cell_no] = $sum_arr[$cell_no] / $filled_lines;
                    }
                } else if ($get_avg) {
                    $avg_arr[$cell_no] = '';
                }
                // others calculation
                if ((!$time_data && $top_x && ($row_no >= $top_x))) {
                    // max calculation
                    @$max_arr[$cell_no] = max($max_arr[$cell_no], $others[$cell_no]);
                    $hasothers = true;
                    @$others[$cell_no] += $cell;
                } else if ($time_data && $top_x && (($itemcount - $row_no) >= $top_x)){
                    $hasothers = true;
                    // for now, we do not accumulate old data for months/days -- optional?
                    // risk is that 'others' is so big that it does not make sense
                    //@$others[$cell_no] += $cell;
                    //@$max_arr[$cell_no] = max($max_arr[$cell_no], $others[$cell_no]);
                }
            }
            $cell_no ++;
        }
        // we processes the line, if we are already in the 'others' remove the line
        if ($hasothers) {
            $othercount ++;
            unset($chart[$lineid]);
        }
        // remove one line from avg-count if data empty
        if ($rowsum == 0) {
            $filled_lines--;
        }
        $row_no++; // count to find out if we are ready to do averages
    }
    // make the array longer to fit, first index is 1 since it comes after the array title
    if ($text_fields >= 1) {
        $empty_arr = array_pad(array(), $text_fields - 1, '');
    } else {
        $empty_arr = array();
    }
    // add others to the table
    if ($hasothers && !$time_data) { // we got others, add them to the end
        $row_no++;
        $others = $empty_arr + array($text_fields - 1 => $BAW_MES[2]) + $others;
        $chart += array('layout_others' => $others);
    } else if ($hasothers && $time_data) {
        $row_no++;
        $others = $empty_arr + array($text_fields - 1 => $BAW_MES[2]) +  $others;
        //$temp = array('layout_others' => $others);
        //$chart = $temp + $empty_arr + $chart;
    }
    // add averages to the table
    if ($get_avg) {
        $row_no++;
        $avg_arr = $empty_arr + array($text_fields - 1 => $BAW_MES[96]) + $avg_arr;
        $chart += array('layout_avg' => $avg_arr);
    }

    $out = "\n<table class=\"charttable\">\n";
    $out .= "    <tr>\n";
    // create the title

    if (isset($format[0]['title']) && $BAW_CONF['chart_titles']) {
        $count_str = sprintf($BAW_MES['records'], baw_num_format($itemcount));
        $out .="    <tr>\n        <th class=\"header_wrap\" colspan=\"$row_no\">{$format[0]['title']} ($count_str)</th>\n    </tr>\n";
    }

    $l = 0;
    $fieldcount = 0;
    $legend = array();
    foreach ($chart as $lineid => $row) {
        $class = '';
        if (isset($dataformat[$lineid])) {
            $class = " {$dataformat[$lineid]}";
        } else if ($lineid === 'layout_others' || $lineid === 'layout_avg') {
            $class .= " $lineid";
        }
        $out .= "        <td class=\"chartcell$class\">\n";
        $cell_no = 0;
        foreach ($row as $cell) {
            // only take the numeric values
            if ($format_arr[$cell_no] != 'layout_text') {
                $tags = '';
                $function = $BAW_LIB['formats'][$format_arr[$cell_no]]['frm'];
                $txt =  $BAW_LIB['formats'][$format_arr[$cell_no]]['txt'];
                $img =  $BAW_LIB['formats'][$format_arr[$cell_no]]['img'];
                $alt = "{$BAW_MES[$txt]}: ". exec_function($function, $cell);
                $max_string = str_replace('layout_', '', $format[$cell_no]['format']);
                // scale after what?
                $max_key = array_search($BAW_CONF["max_$max_string"], $format_arr);
                if ((isset($max_arr[$max_key])) && ($max_arr[$max_key]>0)) {
                    $height = $cell / ($max_arr[$max_key] / 100);
                } else {
                    $height = 1;
                }
                if ($height < 1) {
                    $height = 1;
                }
                $attr = array(
                    'imgwidth'=> $width,
                    'imgheight'=> $height,
                    'height'=> $height,
                    'width' => $width,
                    'alt' => $alt,
                    'title' => $alt,
                    'class' => 'chartimg'
                );
                $out .= baw_create_image($BAW_CONF['icons_url'] . "/other/{$img}", $attr);
            } else {
                // write legend
                $fieldcount = max($cell_no, $fieldcount);
                @$legend[$l][$cell_no] ="        <td$tags colspan=\"1\">$cell</td>\n";
                if (isset($legend[$l-1][$cell_no]) && isset($cell)) {
                    if (preg_match('#colspan=\\"(\d+)\\">'. $cell.'</td>#', $legend[$l-1][$cell_no], $number)) {
                        $number[1]++;
                        @$legend[$l][$cell_no] ="        <td$tags colspan=\"{$number[1]}\">$cell</td>\n";
                        $legend[$l-1][$cell_no] ="";
                    }
                }
            }
            $cell_no ++;
        }
        $out .= "\n        </td>\n";
        $f = 1;
        $l ++;
    }

    $out .= "    </tr>\n";
    // Display legend
    $linecount = $l;
    // we inverse the order so the ones standing closer to the data in the table
    // are on top here
    for ($f=$fieldcount; $f>=0; $f--) {
        $out .= "    <tr class=\"chartlegend\">\n";
        for ($l=0; $l<$linecount; $l++) {
            $out .= $legend[$l][$f];
        }
        $out .= "    </tr>\n";
    }
    $out .= "</table>\n";
    baw_debug("rendering chart finished");
    return $out;
}

?>