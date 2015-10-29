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

// DO NOT EDIT THESE 3 LINES
if (strpos ($_SERVER['PHP_SELF'], 'extras.inc.php') !== false) {
    die ('This file can not be used on its own!');
}

// EDIT BELOW HERE
// Please make sure to read /docs/install.txt to see all places where you have to
// edit items to see your extras
// In order to add new sections of extras to the site, please make copies of the
// section below. change all occurences of extra_1 to extra_2 etc and all EXTRA_1
// to EXTRA_2 etc. Also change the texts as indicated.

// copy section starts below here
function baw_display_extra_1($set) {  // <- change extra_1 to extra_2, 3, 4 etc
    global $BAW_MES, $BAW_CURR, $BAW_D, $BAW_CONF, $BAW_LIB, $BAW_CONF_DIS_DEF;
    $format = array( // change the 'Screen colors' to other titles below here
        0 => array ('title'=> 'Screen colors', 'format' => 'layout_text'),
        1 => array ('percent' => true, 'title'=> $BAW_MES[57], 'format' => 'layout_hits')
    );
    $newval = array(); // change the two following lines from EXTRA_1 to EXTRA_2, 3, 4 etc
    if ($val = baw_data($BAW_CURR['site_name'], 'EXTRA_1', $BAW_CURR['yearmonth'])) {
        foreach ($val as $type => $data) {
            $newval[$type] = array($type, $data[1]) ;
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
// copy section ends above here

// insert copies here

// Please make sure to read /docs/install.txt to see all places where you have to
// edit items to see your extras
// to add more extras-sections, please copy the following section as often as
// you need and edit the extras_1 to extras_2 etc. Also change the text to
// the desired descriptions.

// copy section start below here
$BAW_CONF_DIS_DEF['extra_1'] = array ( // please make sure this is unique
    'help' => 'Screen colors of users', // help for config
    'name' => "Color depth",  // display name
    'sorting' => array( // what fields can it be sorted by?
        0 => 'Color depth in bits',
        1 => $BAW_MES[57]
    )
);
// copy section end above here

    // insert copies here

?>