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
 * This file contains various functions which generate output to the browser.
 * Mainly it contains the index-function that builds the whole site. Other functions
 * create menus, links, images etc.
 */

// this file can't be used on its own
if (strpos ($_SERVER['PHP_SELF'], 'display_helpers.php') !== false) {
    die ('This file can not be used on its own!');
}

/*
* Display Helper Function: run the menus and render the whole page
* called by index.php and baw_action_get_fulltable() as well as config manager
*
* @param    arr     $settings   the 'set'-array from the config for a display section
* @return   str     HTML
*
*/
function baw_display_index($settings = array()) {
    global $BAW_CONF, $BAW_CONF_DIS, $BAW_CONF_DIS_DEF, $BAW_MES, $BAW_LIB;
    global $BAW_DFILES, $BAW_CURR, $BAW_D;

    // this comes second since site_name is unknown otherwise
    echo baw_site_header($BAW_CURR['site_name']);
    $content = "<div class=\"contentbox\">\n";
    $tmp = array(); // temp array where we record which section has data to show/hide menu options

    // sort the charts according to order in config
    $BAW_CONF_DIS = baw_array_sorting($BAW_CONF_DIS, 'order', SORT_ASC);
    // ---------- CONTENT BOX --------------------------------------------------
    // this should be removed once we move to beta.

    if ($BAW_CURR['site_name'] == 'all_months') {
        $settings['section'] = "months";
        $section = $settings['section'];
        $function = "baw_display_$section";
        $temp_conf = $BAW_CONF_DIS[$section];
        $temp_conf['chart'] = true;
        $temp_conf['table'] = false;
        $temp_conf['name'] = $BAW_CONF_DIS_DEF[$section]['name'];
        baw_debug("Printing Stats for: " . $BAW_CONF_DIS_DEF[$section]['name']);
        $content .= $function($temp_conf);
    } else if ($BAW_CURR['site_name'] == 'all_days') {
        $settings['section'] = "days";
        $section = $settings['section'];
        $function = "baw_display_$section";
        $temp_conf = $BAW_CONF_DIS[$section];
        $temp_conf['chart'] = true;
        $temp_conf['table'] = false;
        $temp_conf['name'] = $BAW_CONF_DIS_DEF[$section]['name'];
        baw_debug("Printing Stats for: " . $BAW_CONF_DIS_DEF[$section]['name']);
        $content .= $function($temp_conf);
    } else if (isset($settings['section'])) {
        $section = $settings['section'];
        $function = "baw_display_$section";
        $temp_conf = $BAW_CONF_DIS[$section];
        $temp_conf['top_x'] = $BAW_CONF['maxlines'];
        $temp_conf['name'] = $BAW_CONF_DIS_DEF[$section]['name'];
        baw_debug("Printing Stats for: " . $BAW_CONF_DIS_DEF[$section]['name']);
        $content .= baw_section_header($section, $temp_conf['name'], false, 'menu_section')
            . $function($temp_conf)
            .  baw_section_footer();
    } else {
        $content .= "\n<!-- CONTENT START ++++++++++++++++++++++++++++++++++++++ -->\n";
        if ($BAW_CONF['layout_type'] == 'horizontal') {
            foreach ($BAW_LIB['item_groups'] as $group => $group_details) {
                $group_items = $group_details['members'];
                $menubuttons = "<div class=\"clearfix\">";
                $stats_section = '';
                $item_id = 0;
                $count_members = count($group_details['members']);
                foreach ($group_details['members'] as $item) {
                    $sect_data = $BAW_CONF_DIS[$item];
                    $sect_data['name'] = $item;
                    $function = "baw_display_$item";
                    if (function_exists($function)) {
                        baw_debug("Printing Stats for: $function");
                        $stats_html = $function($sect_data);
                        if ($sect_data['show'] == true &&
                            (strlen($stats_html)> 1 || !$BAW_CONF['hideempty'])) {
                            $stats_section .= "\n<!-- ITEM {$BAW_CONF_DIS_DEF[$item]['name']} START ++++++++++++++++++++++++++++++++++++++ -->\n";
                            $button_title = $BAW_CONF_DIS_DEF[$item]['name'];
                            if ($item_id == 0) {
                                $button_class = "menu_button_active";
                            } else {
                                $button_class = "menu_button_inactive";
                            }
                            $menubuttons .= "<div class=\"$button_class\" id=\"button_{$group}_$item_id\" onClick=\"toggleBox($item_id, '$group', $count_members, 'menu');\">$button_title</div>\n";
                            if ($item == 'weekdays' && !isset($BAW_CURR['wdays']['count'])) {
                                $setarr = array('collapse' => false, 'chart' => false,'table' => false);
                                baw_display_weekdays($setarr);
                            }
                            $title = $BAW_CONF_DIS_DEF[$item]['name'];
                            $collapse = $sect_data['collapse'];
                            $name = "{$group}_$item_id";
                            $collapse = false;
                            if ($item_id == 0) {
                                $class = 'menu_section_active';
                            } else {
                                $class = 'menu_section_inactive';
                            }
                            $stats_section .= baw_section_header($name, $title, $collapse, $class)
                                . $stats_html
                                . baw_section_footer();
                            $tmp[$item] = true;
                            $stats_section .= "\n<!-- ITEM $item END ++++++++++++++++++++++++++++++++++++++ -->\n";
                            $item_id++;
                        } else {
                            $tmp[$item] = false;
                        }
                    }
                }
                if (strlen($stats_section) > 1) {
                    $content .= "\n<!-- GROUP $group START ++++++++++++++++++++++++++++++++++++++ -->\n";
                    $content .= baw_section_header($group, $group_details['title'], false, 'group_title');
                    $menubuttons .= "</div>";
                    $content .= $menubuttons . "\n$stats_section\n";
                    $content .= baw_section_footer();
                    $content .= "\n<!-- GROUP $group END ++++++++++++++++++++++++++++++++++++++ -->\n";
                    $tmp[$group] = true;
                } else {
                    $tmp[$group] = false;
                }
            }
        } else {
            foreach ($BAW_CONF_DIS as $section => $sect_data) {
                $stats_html = '';
                $content .= "\n<!-- ITEM $section START ++++++++++++++++++++++++++++++++++++++ -->\n";
                $title = $BAW_CONF_DIS_DEF[$section]['name'];
                $collapse = $sect_data['collapse'];
                $name = $section;
                $sect_data['name'] = $section;
                $function = "baw_display_$section";
                if ($sect_data['show'] == true && function_exists($function)) {
                    baw_debug("Printing Stats for: baw_display_$section");
                    $stats_html = $function($sect_data);
                    if (strlen($stats_html)> 1) {
                        $content .= baw_section_header($name, $title, $collapse, 'menu_section')
                            . $stats_html
                            . baw_section_footer();
                        $tmp[$section] = true;
                    } else {
                        $tmp[$section] = false;
                    }
                }
                $content .= "\n<!-- ITEM $section END ++++++++++++++++++++++++++++++++++++++ -->\n";
            }
        }
        $content .= "\n<!-- CONTENT END ++++++++++++++++++++++++++++++++++++++ -->\n";
    }

    $content .= "</div>";
    baw_debug("Starting to build menu");
    // ---- MENU BOX -----------------------------------------------------------
    $menubox = "\n\n<!-- MENU START ++++++++++++++++++++++++++++++++++++++ -->\n"
        . "<div class=\"menubox\">\n"
        . baw_sites_dropdown();
    $qstrng = '';
    if (strlen($_SERVER['QUERY_STRING']) > 0) {
        $qstrng = '?' .$_SERVER['QUERY_STRING'];
    }
    $url = "{$BAW_CONF['site_url']}/index.php$qstrng";
    $menubox .= "    <ul class=\"menu\" >\n"
        . "        <li class=\"menu_even\">\n"
        . "            "
        . baw_create_link($BAW_MES[77], "$url#", array('class' => 'leftlink')) . "\n "
        . "        </li>\n";
    $c = 1; // top link was '0'
    if ($BAW_CURR['site_name'] == 'all_months' or $BAW_CURR['site_name'] == 'all_days') {
        $sites = array_keys($BAW_DFILES);
        foreach ($sites as $site) {
            $class = baw_even($c, 'menu_even', 'menu_odd');
            $menubox .= "        <li class=\"$class\">\n"
                . "            " . baw_create_link($site, "$url#site_$site") . "\n"
                . "        </li>\n";
            $c++;
        }
    } else {
        if ($BAW_CONF['layout_type'] == 'horizontal') {
            foreach ($BAW_LIB['item_groups'] as $group => $group_details) {
                if ($tmp[$group]) {
                    $class = baw_even($c, 'menu_even', 'menu_odd');
                    $name = $group_details['title'];
                    $menubox .= "        <li class=\"$class\">\n"
                        . "            " . baw_create_link($name, "$url#h2_$group") . "\n"
                        . "        </li>\n";
                    $c++;
                }
            }
        } else if (isset($settings['section'])) {
            // we are doing a full list
            $url = str_replace('&action=get_fulltable&what='. $settings['section'], '', $url);
            $menubox .= "        <li class=\"menu_odd\">\n"
                .  baw_create_link($BAW_MES['back'], $url) . "\n"
                . "        </li>\n";
        } else {
            $count_sections = count($BAW_CONF_DIS);
            foreach ($BAW_CONF_DIS as $section => $sect_data) {
                if ($sect_data['show'] &&
                    ($tmp[$section] || !$BAW_CONF['hideempty'])) {
                    $class = baw_even($c, 'menu_even', 'menu_odd');
                    $name = $BAW_CONF_DIS_DEF[$section]['name'];
                    $menubox .= "        <li class=\"$class\">\n"
                        . "            " . baw_create_link($name, "$url#h2_$section", array('class' => 'leftlink')) . "\n"
                        . baw_display_list_icons ($section,  array('info'), false) . "\n"
                        . "        </li>\n";
                    $c++;
                }
            }
        }
    }

    $menubox .= "    </ul>\n";
    $menubox_end = "\n</div>\n<!-- MENU END - SITE/DATE MENU START ++++++++++++++++++++++++++++++++++++++ -->\n";
    baw_debug("Menu finished");
    $finaltime = baw_ptime();
    $out = $menubox
        . baw_display_version("<br" . XHTML . "> in $finaltime, max menory: " . baw_byte_format(memory_get_peak_usage(true)))
        . $menubox_end . $content
        . baw_site_footer();
    return $out;
}

/*
* Display Helper Function: display a text including the version and other info
* called by baw_display_index
*
* @param    str     $text   text to be included in the message
* @return   str     HTML
*
*/
function baw_display_version($text = '') {
    global $BAW_CONF_DEF, $BAW_MES;

    $link = baw_create_link('BetterAWstats', 'http://betterawstats.com');
    $out = "    <div class=\"version\">\n        "
        . sprintf($BAW_MES['created_by'], $link)
        . " {$BAW_CONF_DEF[$BAW_MES['cfg_advanced_settings']]['version']['default']}\n"
        . "        $text\n"
        . "    </div>";
    return $out;
}

/*
* Display Helper Function: display links for left-hand menu block
* called by baw_display_index
*
* @param    str     $section    which section is it? (for link)
* @param    str     $text       text to use as alt of icon or instead of icon
* @param    bol     $useicon    shall we display the icon?
* @param    bol     $getonlyone shall we get only one or all three?
* @param    bol     $btn        if true, we use alternative image with button layout
* @return   str     HTML
*
*/
function baw_display_list_icons ($section, $exclude, $btn) {
    global $BAW_CONF_DIS, $BAW_CONF_DIS_DEF, $BAW_CONF, $BAW_MES, $BAW_CURR, $BAW_MES;

    if ($btn) {
        $btn_str = '_btn';
    } else {
        $btn_str = '';
    }
    $links_arr = array(
        'info' => array(
            'check' => @$BAW_MES["hlp_$section"],
            'image' => $BAW_CONF['site_url'] . "/icons/help.png",
            'msg' => htmlentities(@$BAW_MES["hlp_$section"]),
            'what' => 'span',
        ),
        'full_list' => array(
            'check' => @$BAW_CONF_DIS[$section]['top_x'],
            'image' => $BAW_CONF['site_url'] ."/icons/fulllist{$btn_str}.png",
            'msg' => $BAW_MES[80],
            'what' => $section,
        ),
        'versions' => array(
            'check' => @$BAW_CONF_DIS_DEF[$section]['hasversions'],
            'image' => $BAW_CONF['site_url'] ."/icons/versions{$btn_str}.png",
            'msg' => $BAW_MES['version_info'],
            'what' => @$BAW_CONF_DIS_DEF[$section]['hasversions'],
        ),
        'unknown' => array(
            'check' => @$BAW_CONF_DIS_DEF[$section]['hasunknown'],
            'image' => $BAW_CONF['site_url'] ."/icons/unknown{$btn_str}.png",
            'msg' => $BAW_MES['unknown_list'],
            'what' => @$BAW_CONF_DIS_DEF[$section]['hasunknown'],
        ),
    );

    $out = '';
    $i = 1;
    foreach ($links_arr as $link => $settings) {
        if ((!in_array($link, $exclude)) && isset($settings['check'])) {
            $icon = baw_create_image($settings['image'], array('title'=> $settings['msg'],'alt'=> $settings['msg'])) . " ";
            $class = "icon_indent_$i"; // we indent more for each icon
            if ($link == 'info') { // we have a hover-text
                $out .= "    <span class=\"section_help $class\">$icon<span>{$settings['msg']}</span></span>\n";
                $i++;
            } else { // we have a link
                $out .= "            "
                    . baw_display_list_link($icon, $settings['msg'], $settings['what'], $class)
                    . "\n";
                $i++;
            }
        }
    }
    return $out;
}

function baw_display_list_link($text, $title, $section, $class) {
    global $BAW_CONF, $BAW_CURR;
    $out = '';
    if ($BAW_CONF['module'] == 'drupal') {
        $url = "/{$BAW_CONF['drupal_base']}/details/$section/{$BAW_CURR['month']}/{$BAW_CURR['year']}";
    } else {
        $url = "{$BAW_CONF['site_url']}/index.php?site={$BAW_CURR['site_name']}&month={$BAW_CURR['month']}&year={$BAW_CURR['year']}"
            . "&action=get_fulltable&what=$section";
    }
    $out .= baw_create_link($text, $url, array('title'=>$title, 'class'=>$class), false, false);
    return $out;
}

/*
* Display Helper Function: display only and the full table of one section
* called by index.php
*
* @return   str     HTML
*
*/
function baw_action_get_fulltable() {
    global $BAW_CONF;
    $settings = array();
    $out = '';
    if (isset($_GET['what'])) {
        $what = $_GET['what'];
        $settings['section'] = $what;
    } else {
        $out = baw_display_index();
    }
    if (isset($_GET['sort'])) {
        $sort = $_GET['sort'];
        $settings['sort'] = $sort;
    };
    $out .= baw_display_index($settings);
    return $out;
}

/*
* Display Helper Function: create a HTML link from parameters
*
* @param    str     $content    the text to be linked
* @param    str     $url        the url to link to
* @param    arr     $attr       var attributes to include in the links
* @param    bol     $codehtml   do we use htmlspecialchars() on the content?
* @param    bol     $shorten    do we shorten the content according to config?
* @return   str     HTML
*
*/
function baw_create_link ($content, $url, $attr=array(), $codehtml = false, $shorten = true) {
    global $BAW_CONF;
    if ($url == '-') {
        return $url;
    }
    $attr_str = '';
    if (strlen($content) > $BAW_CONF['field_length'] && $shorten) {
        $content = substr($content, 0, $BAW_CONF['field_length']-3) . "...";
    }
    $url = htmlspecialchars($url);
    if ($codehtml) {
        $content = htmlspecialchars($content);
    }
    if (!isset($attr['title'])) {
        $attr['title'] = $content;
    }
    $check = strpos($url, $BAW_CONF['site_url']);
    if ($check !== 0) {
        if (isset($attr['class'])) {
            $attr['class'] .= " ext_link";
        } else {
            $attr['class'] = "ext_link";
        }
    }
    foreach ($attr as $key => $value) {
        $attr_str .= " $key=\"$value\"";
    }
    $out = "<a$attr_str href=\"$url\">$content</a>";

    return $out;
}

/*
* Display Helper Function: create a HTML image from parameters
*
* @param    str     $url        the url of the image to link to
* @param    arr     $attr       var attributes to include in the links
* @return   str     HTML
*
*/
function baw_create_image($url, $attr=array()) {
    global $BAW_CONF;

    $hasalt = false;
    $attr_str = '';
    if (!isset($attr['alt'])) {
        $attr['alt'] = ' ';
    }
    foreach ($attr as $key => $value) {
        $attr_str .= " $key=\"$value\"";
    }
    if ($BAW_CONF['module'] !== 'drupal') {
        if (strpos($url, 'http://') !== 0 && strpos($url, 'https://') !== 0) {
            $url = $BAW_CONF['icons_url'] . $url;
        }
    }
    $out = "<img src=\"$url\"$attr_str" . XHTML . ">";
    return $out;
}

/*
* Display Helper Function: create the HTML for a favicon of a given URL
* Parses the URL, retrieves the domain and links to the favicon
*
* @param    str     $url        the url of the image to link to
* @return   str     HTML
*
*/
function baw_get_favicon($url) {
    if ($url !== strip_tags($url)) { // we have a HTML link
        $pattern = '/<a .* href=\"(.*)\"/';
        $image = '';
        preg_match($pattern, $url, $match);
        if (isset($match[1])) {
            $url = $match[1];
        } else {
            return '';
        }
    }
    $image = '';
    $code = '';
    $url_arr = @parse_url($url);
    if (isset($url_arr['scheme']) && isset($url_arr['host'])) {
        $image = baw_create_image(
            $url_arr['scheme'] . "://". $url_arr['host']. '/favicon.ico',
            array('height' => 16, 'width'=> 16)
        );
    }
    return $image . $code;
}

/*
* Display Helper Function: create the HTML of the site header
* switches between HTML/XHTML, displays title, charset etc.
*
* @param    str     $title        title of the page to be used
* @return   str     HTML
*
*/
function baw_site_header($title = '') {
    global $BAW_CONF_DEF, $BAW_CONF, $BAW_MES;
    if ($BAW_CONF['xhtml']) {
        $out = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">";
        $root_dec = " xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\"";
    } else {
        $out = "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n";
        $root_dec = '';
    }
    $out .= "<html$root_dec>\n"
        ."    <head>\n"
        ."        <title>$title Better AWstats  v. {$BAW_CONF_DEF[$BAW_MES['cfg_advanced_settings']]['version']['default']}</title>\n"
        ."        <meta http-equiv=\"Content-Type\" content=\"text/html;charset={$BAW_MES['e']}\"" . XHTML . ">\n"
        ."        <link rel=\"stylesheet\" type=\"text/css\" href=\"{$BAW_CONF['site_url']}/style.css\"" . XHTML . ">\n"
        ."        <link rel=\"stylesheet\" type=\"text/css\" href=\"{$BAW_CONF['site_url']}/{$BAW_CONF['layout_type']}.css\"" . XHTML . ">\n"
        ."    </head>\n"
        ."    <body>\n"
        ."    <script type=\"text/javascript\" src=\"{$BAW_CONF['site_url']}/scripts.js\"></script>\n";
    return $out;
}

/*
* Display Helper Function: create the HTML of the site footer
*
* @return   str     HTML
*
*/
function baw_site_footer() {
    $out ="</body></html>";
    return $out;
}

/*
* Display Helper Function: create the HTML of the section header
*
* @param    str     $name        name of the section for the HTML ID
* @param    str     $title       title to be displayed
* @param    bol     $collapse    is it collapsed or not?
* @param    str     $class       which CSS class do we use?
* @return   str     HTML
*
*/
function baw_section_header($name, $title, $collapse, $class = 'menu_section') {
    global $BAW_CONF;
    $buttons = baw_show_expand_collapse($name, $collapse). "\n"
        . baw_display_list_icons($name, array(), true);
    if ($collapse) {
        $show = " style=\"display:none\"";
    } else {
        $show = "";
    }
    $out =  "<h2 class=\"$class\" id=\"h2_$name\">\n    $title\n    $buttons\n</h2>\n"
        . "<div class=\"$class\" id=\"box_$name\"$show>\n";
    return $out;
}

/*
* Display Helper Function: create the HTML of the expand/collapse button
*
* @param    str     $name        name of the section for JS to identify it
* @return   str     HTML
*
*/
function baw_show_expand_collapse($name, $collapse) {
    global $BAW_CONF, $BAW_MES;

    $url = $BAW_CONF['site_url'] . "/icons/expand.gif";
    $arr_show = array(
        'class' => "showhidebutton",
        'onclick' => "showElement('{$name}');",
        'title' => $BAW_MES['show'],
        'id' => "button_show_{$name}",
    );
    $arr_hide = array(
        'class' => "showhidebutton",
        'onclick' => "hideElement('{$name}');",
        'id' => "button_hide_{$name}",
        'title' => $BAW_MES['hide'],
    );
    if ($collapse) {
        $arr_hide += array('style' => "display:none");
    } else {
        $arr_show += array('style' => "display:none");
    }
    $url = $BAW_CONF['site_url'] . "/icons/expand.gif";
    $out = baw_create_image($url, $arr_show);
    $url = $BAW_CONF['site_url'] . "/icons/collapse.gif";
    $out .= baw_create_image($url, $arr_hide);
    return $out;
}

/*
* Display Helper Function: create the HTML of footer of a section
*
* @return   str     HTML
*
*/
function baw_section_footer() {
    $out = "</div>\n";
    return $out;
}

/*
* Display Helper Function: create a generic dropdown for HTML forms
*
* @param    str     $name        name of the dropdown for the form identification
* @param    arr     $list        associative array of items to list (ID => NAME)
* @param    str     $selected    entry which should be preselected
* @param    str     $attr        any strng attribute that is added to the <select>
* @return   str     HTML
*
*/
function baw_generic_dropdown($name, $list, $selected='', $attr='') {
    global $BAW_CONF;
    $js = '';
    if ($BAW_CONF['auto_submit_form' ]) {
        $js = " onchange=\"this.form.submit()\"";
    }
    $out = "<select $attr name=\"$name\"$js>\n";
    foreach ($list as $key => $value) {
        if (is_bool($selected)) {
            if ($selected) {
                $selected = 'true';
            } else {
                $selected = 'false';
            }
        }
        if ($key == $selected) {
            $sel = ' selected="selected"';
        } else {
            $sel = '';
        }
        $out .= "    <option value=\"$key\"$sel>$value</option>\n";
    }
    $out .= "</select>\n";
    return $out;
}

/*
* Display Helper Function: create a year/date dropdown from the current list of
* available datafile dates
*
* @return   str     HTML
*
*/
function baw_date_dropdown() {
    global $BAW_CURR;
    $out = '';
    $out .= baw_generic_dropdown('month', $BAW_CURR['months'], $BAW_CURR['month']);
    $out .= baw_generic_dropdown('year', $BAW_CURR['years'], $BAW_CURR['year']);
    return $out;
}

/*
* Display Helper Function: create a dropdown of all sites available from the
* datafiles
*
* @param    str     $selected        preselected item if available
* @param    str     $fld_name        field name of <select>
*
* @return   str     HTML
*
*/
function baw_sites_dropdown() {
    global $BAW_CURR, $BAW_MES, $BAW_DFILES, $BAW_CONF;;
    $sites_temp = $BAW_DFILES;
    $n1 = $n2 = $js = '';
    if ($BAW_CONF['auto_submit_form' ]) {
        $n1 = "<noscript><div>";
        $n2 = "</div></noscript>";
        $js = " onchange=\"this.form.submit()\"";
    }
    $out = '';
    if (count($BAW_DFILES) > 1 && $BAW_CONF['limit_server'] == 'show_all') {
        $out .= "    <form action=\"{$_SERVER['SCRIPT_NAME']}\" method=\"get\">\n"
        . "        <div class=\"headline\">"; // the div is needed for validation
    } else {
        $out = "    <div class=\"headline\">";
        if ($BAW_CONF['limit_server'] == 'show_all' && count($BAW_DFILES) !== 1) {
            $out .= "<span>".$BAW_CONF['limit_server']."</span>";
        } else {
            $singlesite = key($BAW_DFILES);
            $out .= "<span>".$singlesite."</span>";
        }
        $out .= "</div>\n";
        return $out;
    }
    $sites_temp += array(
        'all_months' => $BAW_MES['all_months'],
        'all_days' => $BAW_MES['all_days']
    );
    $out .="\n<select name=\"site\"$js>\n";
    foreach ($sites_temp as $site => $filedata) {
        $sel = '';
        if ($BAW_CURR['site_name'] == $site) {
            $sel = ' selected="selected"';
        }
        $site_txt = $site;
        if ($site == 'all_months' || $site == 'all_days') {
            $site_txt = $BAW_MES[$site];
        }
        $out .= "    <option value=\"$site\"$sel>$site_txt</option>\n";
    }
    $out .= "</select>\n"
        . "$n1<input type=\"submit\" value=\"{$BAW_MES[115]}\"" . XHTML . ">$n2\n"
        . "<input type=\"hidden\" name=\"month\" value=\"{$BAW_CURR['month']}\"" . XHTML . ">\n"
        . "<input type=\"hidden\" name=\"year\" value=\"{$BAW_CURR['year']}\"" . XHTML . ">\n";
    if (isset($_GET['action'])) {
        $out .= "         <input type=\"hidden\" name=\"action\" value=\"{$_GET['action']}\"" . XHTML . ">\n";
    }
    if (isset($_GET['what'])) {
        $out .= "         <input type=\"hidden\" name=\"what\" value=\"{$_GET['what']}\"" . XHTML . ">\n";
    }
        $out .= "</div>\n"
        . "</form>\n";
    return $out;
}
?>