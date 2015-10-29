<?php
/**
 * betterawstats - an alternative display for awstats data
 *
 * @author      Oliver Spiesshofer, support at betterawstats dot com
 * @copyright   2008 Oliver Spiesshofer
 * @version     1.0
 * @link        http://betterawstats.com
 *
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
if (strpos ($_SERVER['PHP_SELF'], 'config_default.php') !== false) {
    die ('This file can not be used on its own!');
}

$BAW_CONF_DEF = array(
    $BAW_MES['cfg_site_settings'] => array (
        'site_url' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_script_url'],
            'default' => 'http://awstats.local',
            'help' => $BAW_MES['cfg_script_url_hlp'],
        ),
        'site_path' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_script_path'],
            'default' => '/path/to/betterawstats/',
            'help' => $BAW_MES['cfg_script_path_hlp'],
        ),
        'path_data' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_path_to_aws_data'],
            'default' => '/path/to/betterawstats/awstats/data',
            'help' => $BAW_MES['cfg_path_to_aws_data_hlp']
        ),
        'path_lib' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_path_to_aws_lib'],
            'default' => '/path/to/betterawstats/awstats/lib',
            'help' => $BAW_MES['cfg_path_to_aws_lib_hlp']
        ),
        'path_lang' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_path_to_aws_lang'],
            'default' => '/path/to/betterawstats/awstats/lang',
            'help' => $BAW_MES['cfg_path_to_aws_lang_hlp'],
        ),
        'icons_url' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_aws_icons_url'],
            'default' => "http://awstats.local/awstats/icon",
            'help' => $BAW_MES['cfg_aws_icons_url_hlp'],
        ),
        'limit_server' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_limit_server'],
            'default' => 'show_all',
            'help' => $BAW_MES['cfg_limit_server_hlp'],
            'values' => $BAW_SERVERS,
        ),
    ),
    $BAW_MES['cfg_layout_settings'] => array (
        'layout_type'=> array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_layout_type'],
            'default' => 'vertical',
            'help' => $BAW_MES['cfg_layout_type_hlp'],
            'values' => array(
                'vertical' => $BAW_MES['cfg_vertical'],
                'horizontal' => $BAW_MES['cfg_horizontal']
            )
        ),
        'lang_setting' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_language'],
            'default' => "auto",
            'help' => $BAW_MES['cfg_language_hlp'],
            'values' => array(
                'auto'=>'Automatic',
                'al'=>'Albanian','ba'=>'Bosnian','bg'=>'Bulgarian','ca'=>'Catalan',
                'tw'=>'Chinese (Taiwan)','cn'=>'Chinese (Simplified)','cz'=>'Czech',
                'dk'=>'Danish','nl'=>'Dutch','en'=>'English','et'=>'Estonian',
                'eu'=>'Euskara','fi'=>'Finnish','fr'=>'Francais','gl'=>'Galician',
                'de'=>'Deutsch','gr'=>'Greek','he'=>'Hebrew','hu'=>'Hungarian',
                'is'=>'Icelandic','id'=>'Indonesian','it'=>'Italiano','jp'=>'Japanese',
                'kr'=>'Korean','lv'=>'Latvian','nn'=>'Norwegian (Nynorsk)',
                'nb'=>'Norwegian (Bokmal)','pl'=>'Polish','pt'=>'Portuguese',
                'br'=>'Portuguese (Brazilian)','ro'=>'Romanian','ru'=>'Russian',
                'sr'=>'Serbian','sk'=>'Slovak','es'=>'Spanish','se'=>'Swedish',
                'tr'=>'Turkish','ua'=>'Ukrainian','wlk'=>'Welsh'
            )
        ),
        'dec_point' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_decimalpoint'],
            'default' => ".",
            'help' => $BAW_MES['cfg_decimalpoint_hlp']
        ),
        'tho_point' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_thous_sep'],
            'default' => "'",
            'help' => $BAW_MES['cfg_thous_sep_hlp']
        ),
        'date_format' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_date_form'],
            'default' => "Y-M-d",
            'help' => $BAW_MES['cfg_date_form_hlp'],
        ),
        'date_time_format' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_date_time_form'],
            'default' => "Y-M-d H:i",
            'help' => $BAW_MES['cfg_date_time_form_hlp']
        ),
        'percent_decimals' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_percent_dec'],
            'default' => 1,
            'help' => $BAW_MES['cfg_percent_dec_hlp'],
        ),
        'hideempty' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_hide_empty'],
            'default' => 'true',
            'help' => $BAW_MES['cfg_hide_empty_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_hide'],
                'false' => $BAW_MES['cfg_show']
            )
        ),
        'auto_submit_form' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_auto_submit_form'],
            'default' => 'true',
            'help' => $BAW_MES['cfg_auto_submit_form_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_enable'],
                'false' => $BAW_MES['cfg_disable']
            )
        ),
    ),
    $BAW_MES['cfg_table_settings'] => array(
        'firstdayofweek' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_firstweekday'],
            'default' => 1,
            'help' => $BAW_MES['cfg_firstweekday_hlp'],
            'values' => array(
                1 => $BAW_MES['monday'],
                2 => $BAW_MES['sunday']
            )
        ),
        'field_length' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_field_length'],
            'default' => 65,
            'help' => $BAW_MES['cfg_field_length_hlp'],
        ),
        'maxlines' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_max_lines'],
            'default' => 10000,
            'help' => $BAW_MES['cfg_max_lines_hlp'],
        ),
    ),
    $BAW_MES['cfg_chart_settings'] => array(
        'max_visitors' => array(
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_max_scale_visitors'],
            'default' => "layout_visits",
            'help' => $BAW_MES['cfg_max_scale_visitors_hlp'],
            'values' => array(
                'layout_visitos' => $BAW_MES[18],
                'layout_visits' => $BAW_MES['visits'],
                'layout_pages' => $BAW_MES[56],
                'layout_hits' => $BAW_MES[57],
                'layout_bytes' => $BAW_MES[75],
            )
        ),
        'max_visits' => array(
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_max_scale_visits'],
            'default' => "layout_visits",
            'help' => $BAW_MES['cfg_max_scale_visits_hlp'],
            'values' => array(
                'layout_visits' => $BAW_MES['visits'],
                'layout_pages' => $BAW_MES[56],
                'layout_hits' => $BAW_MES[57],
                'layout_bytes' => $BAW_MES[75],
            )
        ),
        'max_pages' => array(
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_max_scale_pages'],
            'default' => "layout_pages",
            'help' => $BAW_MES['cfg_max_scale_pages_hlp'],
            'values' => array(
                'layout_pages' => $BAW_MES[56],
                'layout_hits' => $BAW_MES[57],
                'layout_bytes' => $BAW_MES[75],
            )
        ),
        'max_hits' => array(
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_max_scale_hits'],
            'default' => "layout_hits",
            'help' => $BAW_MES['cfg_max_scale_hits_hlp'],
            'values' => array(
                'layout_hits' => $BAW_MES[57],
                'layout_bytes' => $BAW_MES[75],
            )
        ),
        'max_chart_items' => array(
            'type' => 'string',
            'name' => $BAW_MES['cfg_max_chart_items'],
            'default' => "50",
            'help' => $BAW_MES['cfg_max_chart_items_hlp'],
        ),
        'chart_titles' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_chart_titles'],
            'default' => 'false',
            'help' => $BAW_MES['cfg_chart_titles_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_enable'],
                'false' => $BAW_MES['cfg_disable']
            ),
        ),
    ),
    $BAW_MES['cfg_jpgraph_settings'] => array(
        'use_jpgraph' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_jpgraph_enable'],
            'default' => 'false',
            'help' => $BAW_MES['cfg_jpgraph_enable_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_enable'],
                'false' => $BAW_MES['cfg_disable']
            )
        ),
        'jpgraph_path' => array (
            'type' => 'string',
            'name' => $BAW_MES['cfg_jpgraph_path'],
            'default' => '/path/to/betterawstats/jpgraph/src',
            'help' => $BAW_MES['cfg_jpgraph_path_hlp'],
        ),
    ),
    $BAW_MES['cfg_advanced_settings'] => array (
        'version' => array (
            'type' => 'fixed',
            'name' => $BAW_MES['cfg_version'],
            'default' => "1.0",
            'help' => $BAW_MES['cfg_version_hlp'],
        ),
        'xhtml' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_xhtml'],
            'default' => 'false',
            'help' => $BAW_MES['cfg_xhtml_hlp'],
            'values' => array(
                'false' => 'HTML',
                'true' => 'XHTML'
            )
        ),
        'debug' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_debug'],
            'default' => 'false',
            'help' => $BAW_MES['cfg_debug_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_show'],
                'false' => $BAW_MES['cfg_hide']
            )
        ),
        'show_parser_stats' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_parser'],
            'default' => 'false',
            'help' => $BAW_MES['cfg_parser_hlp'],
            'values' => array(
                'true' => $BAW_MES['cfg_show'],
                'false' => $BAW_MES['cfg_hide']
            )
        ),
        'module' => array (
            'type' => 'dropdown',
            'name' => $BAW_MES['cfg_module'],
            'default' => 'none',
            'help' => $BAW_MES['cfg_module_hlp'],
            'values' => array('none'=>$BAW_MES['cfg_disable'], 'drupal'=>'Drupal')
        )
    )
);

$BAW_CONF_DIS_TYP = array(
    'order' => array($BAW_MES['cfg_type_order'], 'string'),
    'show' => array($BAW_MES['cfg_type_show'], 'bool'),
    'collapse' => array($BAW_MES['cfg_type_collapse'], 'bool'),
    'table' => array($BAW_MES['cfg_type_table'], 'bool'),
    'sort' => array($BAW_MES['cfg_type_sort'], 'sorting1'),
    'sort_dir' => array($BAW_MES['cfg_type_sort_dir'], 'sorting2'),
    'chart' => array($BAW_MES['cfg_type_chart'], 'bool'),
    'map' =>  array($BAW_MES['cfg_type_map'], 'bool'),
    'avg' => array($BAW_MES['cfg_type_avg'], 'bool'),
    'total' => array($BAW_MES['cfg_type_total'], 'bool'),
    'top_x' => array($BAW_MES['cfg_type_top_x'], 'string'),
    'assumebot' => array($BAW_MES['cfg_type_assumebot'], 'string'),
    'favicon' => array($BAW_MES['cfg_type_favicon'], 'bool'),
    'domain_lvls' => array($BAW_MES['cfg_type_domain_lvls'], 'string')
);

$BAW_CONF_DIS_DEF = array (
    'overview' => array (
        'help' => $BAW_MES['cfg_dis_overview'],
        'name' => $BAW_MES[128],
    ),
    'months' => array (
        'help' => $BAW_MES['cfg_dis_months'],
        'name' => $BAW_MES[162],
        /* 'sorting' => array(
            0 => $BAW_MES[5],
            1 => $BAW_MES[11],
            2 => $BAW_MES[10],
            3 => $BAW_MES[56],
            4 => $BAW_MES[57],
            5 => $BAW_MES[75]
        )*/
    ),
    'days' => array (
        'help' => $BAW_MES['cfg_dis_days'],
        'name' => $BAW_MES[138],
        /* 'sorting' => array(
            0 => $BAW_MES[4],
            1 => $BAW_MES[10],
            2 => $BAW_MES[56],
            3 => $BAW_MES[57],
            4 => $BAW_MES[75]
        )*/
    ),
    'weekdays' => array (
        'help' => $BAW_MES['cfg_dis_weekdays'],
        'name' => $BAW_MES['weekday_averages'],
        /* 'sorting' => array(
            0 => $BAW_MES[4],
            1 => $BAW_MES[10],
            2 => $BAW_MES[56],
            3 => $BAW_MES[57],
            4 => $BAW_MES[75]
        )*/
    ),
    'hours' => array (
        'help' => $BAW_MES['cfg_dis_hours'],
        'name' => $BAW_MES['hours_averages'],
        'sorting' => array(
            1 => $BAW_MES[20],
            2 => $BAW_MES[56],
            3 => $BAW_MES[57],
            4 => $BAW_MES[75]
        )
    ),
    'domains' => array (
        'help' => $BAW_MES['cfg_dis_domains'],
        'name' => $BAW_MES[25],
        'sorting' => array(
            'key' => $BAW_MES[25],
            0 => $BAW_MES[56],
            1 => $BAW_MES[57],
            2 => $BAW_MES[75]
        )
    ),
    'visitors' => array (
        'help' => $BAW_MES['cfg_dis_visitors'],
        'name' => $BAW_MES[81],
        'sorting' => array(
            0 => $BAW_MES[81],
            // 1 => $BAW_MES[114],
            1 => $BAW_MES[56],
            2 => $BAW_MES[57],
            3 => $BAW_MES[75],
            4 => $BAW_MES['ratio_pages_hits'],
            5 => $BAW_MES[9]
        ),
        'assumebot' => '1.0'
    ),
    'logins' => array (
        'help' => $BAW_MES['cfg_dis_logins'],
        'name' => $BAW_MES[94],
        'sorting' => array(
            'key' => $BAW_MES[94],
            0 => $BAW_MES[56],
            1 => $BAW_MES[57],
            2 => $BAW_MES[75],
            3 => $BAW_MES[9]
        )
    ),
    'robots' => array (
        'help' => $BAW_MES['cfg_dis_robots'],
        'name' => $BAW_MES[53],
        'sorting' => array(
            0 => $BAW_MES[53],
            1 => $BAW_MES[57],
            2 => $BAW_MES[57]. " (robots.txt)",
            3 => $BAW_MES[75],
            4 => $BAW_MES[9]
        )
    ),
    'worms' => array (
        'help' => $BAW_MES['cfg_dis_worms'],
        'name' => $BAW_MES[163],
        'sorting' => array(
            0 => $BAW_MES[163],
            1 => $BAW_MES[167],
            2 => $BAW_MES[57],
            3 => $BAW_MES[75],
            4 => $BAW_MES[9]
        )
    ),
    'sessions' => array (
        'help' => $BAW_MES['cfg_dis_sessions'],
        'name' => $BAW_MES[117],
    ),
    'filetype' => array (
        'help' => $BAW_MES['cfg_dis_filetype'],
        'name' => $BAW_MES[73],
        'sorting' => array(
            'key' => $BAW_MES[73],
            0 => $BAW_MES[57],
            1 => $BAW_MES[75],
            2 => $BAW_MES[100],
            3 => $BAW_MES[101]
        )
    ),
    'urls' => array (
        'help' => $BAW_MES['cfg_dis_urls'],
        'name' => $BAW_MES[19],
        'sorting' => array(
            0 => $BAW_MES[19],
            1 => $BAW_MES[29],
            2 => $BAW_MES[106],
            3 => $BAW_MES[104],
            4 => $BAW_MES[116]
        )
    ),
    'paths' => array (
        'help' => $BAW_MES['cfg_dis_paths_hlp'],
        'name' => $BAW_MES['cfg_dis_paths'],
        'sorting' => array(
            0 => $BAW_MES['files_paths'],
            1 => $BAW_MES[29]
        )
    ),
    'os' => array (
        'help' => $BAW_MES['cfg_dis_os'],
        'name' => $BAW_MES[59],
        'sorting' => array(
            1 => $BAW_MES[59],
            2 => $BAW_MES[57]
        ),
        'hasversions' => 'osversions',
        'hasunknown' => 'unknownos'
    ),
    'unknownos' => array (
        'help' => $BAW_MES['cfg_dis_unknownos'],
        'name' => $BAW_MES[46],
        'sorting' => array(
            1 => $BAW_MES['user_agent'],
            2 => $BAW_MES[9]
        )
    ),
    'osversions' => array (
        'help' => $BAW_MES['cfg_dis_osversions'],
        'name' => $BAW_MES['os_versions'],
        'sorting' => array(
            1 => $BAW_MES[59],
            2 => $BAW_MES[57]
        )
    ),
    'browsers' => array (
        'help' => $BAW_MES['cfg_dis_browsers'],
        'name' => $BAW_MES[21],
        'sorting' => array(
            1 => $BAW_MES[21],
            2 => $BAW_MES[111],
            3 => $BAW_MES[57]
        ),
        'hasversions' => 'browserversions',
        'hasunknown' => 'unknownbrowser'
    ),
    'browserversions' => array (
        'help' => $BAW_MES['cfg_dis_browserversions'],
        'name' => $BAW_MES['browser_versions'],
        'sorting' => array(
            2 => $BAW_MES[21],
            3 => $BAW_MES[111],
            4 => $BAW_MES[57]
        ),
    ),
    'unknownbrowser' => array (
        'help' => $BAW_MES['cfg_dis_unknownbrowser'],
        'name' => $BAW_MES[50],
        'sorting' => array(
            1 => $BAW_MES['cfg_dis_unknownbrowser_agent'],
            2 => $BAW_MES[9]
        )
    ),
    'screensizes' => array (
        'help' => $BAW_MES['cfg_dis_screensizes'],
        'name' => $BAW_MES[135],
        'sorting' => array(
            1 => $BAW_MES[135],
            2 => $BAW_MES[57],
        )
    ),
    'se_referers' => array (
        'help' => $BAW_MES['cfg_dis_se_referers'],
        'name' => $BAW_MES[126],
        'sorting' => array(
            0 => $BAW_MES[126],
            1 => $BAW_MES[56],
            2 => $BAW_MES[57]
        ),
    ),
    'referers' => array (
        'help' => $BAW_MES['cfg_dis_referers'],
        'name' => $BAW_MES[127],
        'sorting' => array(
            0 => $BAW_MES[127],
            1 => $BAW_MES[56],
            2 => $BAW_MES[57]
        ),
        'hasdetail' => 'referer_domains'
    ),
    'referer_domains' => array (
        'help' => $BAW_MES['cfg_dis_referer_domains'],
        'name' => sprintf($BAW_MES['by_domains'], $BAW_MES[127]),
        'sorting' => array(
            0 => $BAW_MES[127],
            1 => $BAW_MES[56],
            2 => $BAW_MES[57]
        ),
        'hascompact' => 'referers'
    ),
    'hotlinks' => array (
        'help' => $BAW_MES['cfg_dis_hotlinks'],
        'name' => $BAW_MES['referer_hotlinks'],
        'sorting' => array(
            0 => $BAW_MES[127],
            // 1 => $BAW_MES[56],
            1 => $BAW_MES[57]
        ),
        'hasdetail' => 'hotlink_domains'
    ),
    'hotlink_domains' => array (
        'help' => $BAW_MES['cfg_dis_hotlink_domains'],
        'name' => $BAW_MES['hl_by_domains '],
        'sorting' => array(
            0 => $BAW_MES[127],
            // 1 => $BAW_MES[56],
            1 => $BAW_MES[57]
        ),
        'hascompact' => 'hotlinks'
    ),
    'searchphrases' => array (
        'help' => $BAW_MES['cfg_dis_searchphrases'],
        'name' => $BAW_MES[120],
        'sorting' => array(
            0 => $BAW_MES[103],
            1 => $BAW_MES[57]
        )
    ),
    'searchwords' => array (
        'help' => $BAW_MES['cfg_dis_searchwords'],
        'name' => $BAW_MES[121],
        'sorting' => array(
            0 => $BAW_MES[13],
            1 => $BAW_MES[57]
        )
    ),
    'misc' => array (
        'help' => $BAW_MES['cfg_dis_misc'],
        'name' => $BAW_MES[139],
    ),
    'errors' => array (
        'help' => $BAW_MES['cfg_dis_errors'],
        'name' => $BAW_MES[32],
        'sorting' => array(
            0 => $BAW_MES[32],
            2 => $BAW_MES[57],
            3 => $BAW_MES[106]
        )
    ),
    'errors404' => array (
        'help' => $BAW_MES[47],
        'name' => $BAW_MES[49],
        'sorting' => array(
            0 => $BAW_MES[49],
            1 => $BAW_MES[57],
            2 => $BAW_MES[57]
        )
    ),
);
?>