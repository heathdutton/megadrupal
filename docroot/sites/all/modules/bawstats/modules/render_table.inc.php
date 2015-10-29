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
if (strpos ($_SERVER['PHP_SELF'], 'reder_table.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

// this function generates ratio-calculations for a whole line
// it requires that the format of the data contains
// 'format' => 'layout_ratio', 'ratio' => '2/1'
// it would take the second field and divide it by the first
function baw_calc_ratio($data_set, $ratio_set) {
    foreach ($ratio_set as $field => $formula) {
        $frm = explode("/", $formula);
        $data_set[$field] = $data_set[$frm[0]] / $data_set[$frm[1]];
    }
    return $data_set;
}

// this function does std. processing that this needed in 2 places
function baw_process_cell($cell, $format, $target = null) {
    if ($target == null) {
        $target = 0;
    }
    if (is_numeric($cell)) {
        if ($format == 'layout_date') {
            $target = max($target, $cell);
        } else if ($format == 'layout_ratio') {
            $target = 0;
        } else {
            $target += $cell;
        }
    } else {
        $target = '';
    }
    return $target;
}


function baw_render_table($section_name, $table, $format, $get_avg = false, $get_sum = false,
                      $top_x = false, $dataformat = false) {
    global $BAW_CONF, $BAW_LIB, $BAW_MES, $BAW_CONF_DIS;
    baw_debug("rendering table");
    $out = '';
    $theader = "\n<table class=\"datatable\">\n";
    // HEADER -----------------------------------------------------------------
    $out .= "    <tr>\n";
    $format_arr = array();
    $column = 0;
    $itemcount = count($table);
    $ratio_set = array();
    // iterate formats for header
    foreach ($format as $cell => $attr) {
        $class = '';
        $tags ='';
        $percent_header = '';
        // iterate one formats attributes
        foreach ($attr as $name => $value){
            if ($name == 'title') {
                $data = $value;
            } else if ($name == 'format') {
                $format_arr[] = $value;
                $class_arr[] = $value;
                $class = " $value";
            } else if ($name == 'ratio') {
                $ratio_set[$cell] = $value;
            // percentage header insert ... this could be set per column in the config?
            } else if ($name == 'percent' && $value){
                $percent_header = "        <th class=\"layout_percent\">{$BAW_MES[15]}</th>\n";
            } else if (strlen($value)>0) {
                $tags .= " $name=\"$value\"";
            }
        }
        // find out how many items in whole table
        if ($column == 0) { // add only to first column
            $data = sprintf($BAW_MES['records'], baw_num_format($itemcount));
        }
        if (isset($attr['title'])) {
            $out .="        <th class=\"header_wrap$class\"$tags>$data</th>\n$percent_header";
            $column ++;
        }
    }
    $out .= "    </tr>\n";

    // SUM & averages calculation ----------------------------------------------
    // the sum has to be calculated before so the percentages can be calculated
    // iterate all the table and count the sums of all numeric data
    $sum_arr = array(0 => $BAW_MES[102]); // we set the text here to make sure its in the right position
    $avg_arr = array(0 => $BAW_MES[96]);
    $others = array();
    $filled_lines = $itemcount; // get the value for substraction
    $row_no = 0;
    $othercount = 0;
    $hasothers = false;
    foreach ($table as $lineid => $row) {
        $cell_no = 0;
        $rowsum = 0;
        foreach ($row as $cell) {
            // sum calculation
            $sum_arr[$cell_no] = baw_process_cell($cell, @$format_arr[$cell_no] , @$sum_arr[$cell_no]);
            // average calculation
            if (is_numeric($cell) && $get_avg) {
                $rowsum += $cell;
                if ($row_no == ($itemcount-1) && $filled_lines >= 1) { // last line, assume sums are done
                    if ($rowsum == 0 && $cell_no == 0) { // do once again for last line, dont substract further after first cell
                        $filled_lines--;
                    }
                    if ($format_arr[$cell_no] == 'layout_date') {
                        $avg_arr[$cell_no] = false; // this will prevent formatting, averages on date not possible?
                    } else {
                        $avg_arr[$cell_no] = $sum_arr[$cell_no] / $filled_lines;
                    }
                }
            } else if ($get_avg) {
                $avg_arr[$cell_no] = '';
            }
            // others calculation
            if ($top_x && ($row_no >= $top_x)) {
                $hasothers = true;
                $others[$cell_no] = baw_process_cell($cell, @$format_arr[$cell_no] , @$others[$cell_no]);
            }
            $cell_no ++;
        }
        // we processes the line, if we are already in the 'others' remove the line
        if ($hasothers) {
            $othercount ++;
            unset($table[$lineid]);
        }
        // remove one line from avg-count if data empty
        if ($rowsum == 0) {
            $filled_lines--;
        }
        $row_no++; // count to find out if we are ready to do averages
    }
    // add others to the table
    if ($hasothers) { // we got others, add them to the end
        if ($top_x < $BAW_CONF['maxlines']) {
            $text = baw_display_list_link($BAW_MES[2], $BAW_MES[2], $section_name, false, 'full_list')
                . " (". baw_num_format($othercount) . ")";
        } else {
            $text = $BAW_MES[2]
                . " (". baw_num_format($othercount) . ")<br>"
                . sprintf($BAW_MES['table_max_hits_exceed'], baw_num_format($BAW_CONF['maxlines']));
        }
        $others = baw_calc_ratio($others, $ratio_set);
        $others[0] = $text;
        $table += array('layout_others'=> $others);
    }
    // add averages to the table
    if ($get_avg) {
        // re-set the value here since it might have been overwritten
        $avg_arr[0] = $BAW_MES[96];
        $avg_arr = baw_calc_ratio($avg_arr, $ratio_set);
        $table += array('layout_avg' => $avg_arr);
    }
    // add the sum to the table
    if ($get_sum) {
        $sum_arr[0] = $BAW_MES[102];
        $sum_arr = baw_calc_ratio($sum_arr, $ratio_set);
        $table += array('layout_sum' => $sum_arr);
    }

    // these are set later to make sure they are not overwritten in the func before
    // this saves one more check

    // DATA --------------------------------------------------------------------
    $lastlineid = '';
    $rowspan = array();
    foreach ($table as $lineid => $row) {
        $cell_no = 0;
        $class = '';
        // get the class for the whole row
        if (isset($dataformat[$lineid])) {
            $class = " class=\"{$dataformat[$lineid]}\"";
            // for some _STRANGE_ reason, the 403-error lineid is equal to the string here, so use ===
        } else if ($lineid === 'layout_others' || $lineid === 'layout_avg' || $lineid === 'layout_sum') {
            $class = " class=\"$lineid\"";
        }
        $out .= "    <tr$class>\n";
        $iteration = 0;
        foreach ($row as $cell) {
            $class = ''; // get the class for the column (set by field)
             // for some _STRANGE_ reason, the 403-error lineid is equal to the string here, so use ===
            if ($lineid === 'layout_others' || $lineid === 'layout_avg' || $lineid === 'layout_sum') {
                $span = 0;
                $attr = '';
                if ($cell_no > $iteration) {
                    $iteration ++;
                    continue;
                }
                if (isset($format[$cell_no]['colspan'])) {
                    $span = $format[$cell_no]['colspan'];
                    $class .= " colspan=\"$span\"";
                    $span --;
                    $cell_no += $span;
                }
                $iteration ++;
            } else {
                $class = '';
            }
            // check if the row has data, otherwise remove 1 line for average count
            $function = $BAW_LIB['formats'][$format_arr[$cell_no]]['frm'];
            $cell_str = exec_function($function, $cell);
            if (isset($class_arr[$cell_no]) && strlen($class_arr[$cell_no]) > 0) {
                $class .= " class=\"{$class_arr[$cell_no]}\"";
            }
            // do rowspan
            $doline = true;
            $rowspan_str = '';
            if (($format_arr[$cell_no] === 'layout_text') &&
                ($lineid !== 'layout_others' && $lineid !== 'layout_avg' && $lineid !== 'layout_sum')) {
                if (isset($table[$lastlineid][$cell_no]) && $cell === $table[$lastlineid][$cell_no]) {
                    $rowspan[$lineid][$cell_no] = $rowspan[$lastlineid][$cell_no] + 1;
                    $search = " rowspan=\"{$rowspan[$lastlineid][$cell_no]}\">$cell_str</td>";
                    $replace = " rowspan=\"{$rowspan[$lineid][$cell_no]}\">$cell_str</td>";
                    // we search the last time we had this data and replace it
                    $pos = strrpos($out, $search);
                    $out = substr_replace($out, $replace, $pos, strlen($search));
                    $doline = false;
                } else { // no match, restart with 1
                    $rowspan_str = ' rowspan="1"'; // do =1 always so we can replace later
                    $rowspan[$lineid][$cell_no] = 1;
                }
            }
            if ($doline) {
                $out .= "        <td$class$rowspan_str>$cell_str</td>\n";
            }
            if (isset($format[$cell_no]['percent']) &&
                    $format[$cell_no]['percent'] == true &&
                    $sum_arr[$cell_no] !== 0) {
                $percent = $cell / ($sum_arr[$cell_no] / 100);
                $out .="        <td$class>" . @baw_percent_format($percent) . "</td>\n";
            }
            $cell_no ++;
        }
        $lastlineid = $lineid;
        $out .= "    </tr>\n";
    }
    $out .= "</table>\n";
    baw_debug("rendering table finished");
    return $theader . $out;
}

?>