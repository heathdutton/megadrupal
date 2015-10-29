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

function hideElement(element) {
    document.getElementById('box_' + element).style.display = 'none';
    document.getElementById('button_show_' + element).style.display = '';
    document.getElementById('button_hide_' + element).style.display = 'none';
}

function showElement(element) {
    document.getElementById('box_' + element).style.display = '';
    document.getElementById('button_hide_' + element).style.display = '';
    document.getElementById('button_show_' + element).style.display = 'none';
}

function toggleBox(element, group, count, layout) {
    for (i=0; i<count; i++) {
        if (element != i) {
            document.getElementById('box_' + group + '_' + i).className = layout +  "_section_inactive";
            document.getElementById('button_' + group + '_' + i).className = layout + "_button_inactive";
        } else {
            document.getElementById('box_' + group + '_' + i).className =  layout + "_section_active";
            document.getElementById('button_' + group + '_' + i).className =  layout + "_button_active";
        }
    }
}
