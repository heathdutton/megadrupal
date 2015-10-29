<?php

// since we are merging this here with the message file from AWSTATS, we have to
// add these internal messages to the ones from AWSTATS. Please remember NOT to
// use any numeric indexes since they are used by AWSTATS.

global $BAW_MES;
$BAW_MES += array(

    // display.inc.php
    'data_file_stats' => 'Data File Update Stats:',
    'parsed_records' => 'Parsed records:',
    'old_records' => 'Old records:',
    'new_records' => 'New records:',
    'corrupted' => 'Corrupted:',
    'dropped' => 'Dropped:',
    'ratio_pages_hits' => 'Ratio'.BR.'(Hits/Pages)',
    'files_paths' => 'Files/paths',
    'user_agent' => 'User Agent',
    'size' => 'Size',
    'by_domains' => '%s by Domains',
    'visits' => 'Visits',
    'averages' => ' (Averages)',
    'assumedscript' => 'Assumed Robots/Spiders',
    'weekday_averages' => $BAW_MES[91] . ' (Averages)',
    'hours_averages' => $BAW_MES[20] .  ' (Averages)',
    'os_versions' =>  "{$BAW_MES[59]} (+{$BAW_MES[58]})",
    'browser_versions' => "{$BAW_MES[21]} (+{$BAW_MES[58]})",
    'referer_hotlinks' => 'Hotlinks',
    'hl_by_domains ' => "Hotlinks by Domains",
    'hotlinks_proxies' => 'Hotlinks/Proxies',

    // baw_raise_error
    'baw_error' => 'BetterAWstats Error',
    'datafile' =>
        "Datafile is corrupt, data could not be read: %s;".BR."'%s' should be BEGIN_...".BR
        . "Please read the FAQ (/doc/FAQ.txt)",
    'datafileindex' =>
        "Datafile is corrupt, data-index could not be read: %s;".BR."'%s' should be BEGIN_...".BR
        . "Please read the FAQ (/doc/FAQ.txt)",
    'datafilesdir' =>
        "The Data files could not be found in this location: %s".BR
        . "Please check your config.php. The value of \$BAW_CONF['path_data'] has "
        . "to point to the directory where awstats stores its data files "
        . "(typically somewhere /cgi-bin/awstats/data). Please also make sure "
        . "that php has read-permissions on that folder.",
    'languagefile' =>
        "The language file could not be found in this location: %s".BR
        . "Please check your config.php. The value of \$BAW_CONF['path_lang'] has "
        . "to point to the directory where awstats stores its language files "
        . "(typically somewhere /cgi-bin/awstats/lang). Please also make sure "
        . "that php has read-permissions on that folder.",
    'libraryfiles' =>
        "The library files could not be found in this location: %s".BR
        . "Please check your config.php. The value of \$BAW_CONF['path_lib'] has "
        . "to point to the directory where awstats stores its library files "
        . "(typically somewhere /cgi-bin/awstats/lib). Please also make sure "
        . "that php has read-permissions on that folder.",
    'libraryeval' =>
        "There was an error parsing the awstats library file %s that is used to "
        . "parse the variable %s. Please switch on debugging in config.php and "
        . "post the PHP errors displayed to the support forum so the problem can be fixed.",
    'jpgraph_path' =>
        "The files for jpgraph could not be found at %s. Either disable the feature in "
        . "config.php or correct the path to the files.",
    'gd2notavailable' =>
        "The PHP graphics library GD2 is not available. Please disable the map function or install GD2."
        . "You can get more info at http://php.net/manual/en/ref.image.php",
    'mapnotavailable' =>
        "The graphics to create the Map (%s) is not available. Please make sure that "
        . "you have all the files from the setup and that they are readable by "
        . "the server.",
    'iconpath' =>
        "The AWSTATS icons could not be found in this location: %s".BR
        . "Please check your config.php. The value of \$BAW_CONF['icons_url'] has "
        . "to point to the URL where awstats stores its library files "
        . "(typically somewhere http://awstats.local/awstats/icon).",
    'site_path' =>
        "The path to awstats is set incorrectly to the folder: %s".BR
        . "Please check your config.php. The value of \$BAW_CONF['site_path'] has "
        . "to point to the directory where BetterAWStats is installed"
        . "(typically somewhere /home/user/public_html/baw).".BR
        . "It seems your script is installed under '%s'.",
    'site_url' =>
        'The URL to BetterAWStats is set incorretly in your config.php. It is currently '
        . 'set to %s, but it this is incorrect. It seems the URL could be %s instead.',
    'xmldata' =>
        'Your AWStats data files are stored in XML format. Unfortunately, BetterAWSTats '
        . 'does not support XML data yet. Please store your datafiles in plain text.',

    // $BAW_LIB['item_groups']
    'time' => 'Time',
    'user_information' => 'User Information',
    'user_actions' => 'User Actions',
    'user_origin' => 'User Origin',
    'other_access' => 'Other Access',

    // debug messages
    'dbg_detect_language' => "trying to detect language",
    'dbg_detected_language' => "Detected language %s",
    'dbg_test_writable' => "Checking if %s is writable",
    'dbg_test_writable_false' => "IS NOT writable: %s",
    'dbg_test_writable_true' => 'IS writable: %s',
    'dbg_found_dir' => 'found directory %s',
    'dbg_found_file' => 'Found File: %s',
    'dbg_finished_parse_dir' => "Finished parsing Directory %s",
    'dbg_start_parse_dir' => "Start parsing Directory %s",

    // config_default.inc.php
    'cfg_enable' => 'Enable',
    'cfg_disable' => 'Disable',
    'cfg_show_all' => 'Show all',
    'monday' => 'Monday',
    'sunday' => 'Sunday',
    'cfg_hide' => 'Hide',
    'cfg_show' => 'Show',
    'cfg_site_settings' => 'Site Settings',
    'cfg_path_to_aws_data' => 'Path to AWStats Data',
    'cfg_path_to_aws_data_hlp' => "Set this value to the directory where AWStats "
        . "saves its database files into. ATTENTION: If you read those files on "
        . "windows but have them created on linux or the other way round, make "
        . "sure you transfer them 'BINARY'. Otherwise they cannot be read properly. "
        . "No trailing slash",
    'cfg_path_to_aws_lib' => 'Path to AWStats Libraries',
    'cfg_path_to_aws_lib_hlp' => "Set this value to the directory where AWStats "
        . "saves its library files into. No trailing slash",
    'cfg_path_to_aws_lang' => 'Path to AWStats Language files',
    'cfg_path_to_aws_lang_hlp' => 'Set this value to the directory where AWStats '
        . 'saves its language files into. No trailing slash',
    'cfg_script_url' => 'Script URL',
    'cfg_script_url_hlp' => "The url of BetterAWstats' directory, No trailing slash",
    'cfg_script_path' => 'Script path',
    'cfg_script_path_hlp' => 'The path of BetterAWstats, No trailing slash',
    'cfg_aws_icons_url' => 'URL to AWStats Icons',
    'cfg_aws_icons_url_hlp' => 'The url to the awstats icons, should include the '
        . 'whole http://..., no trailing slash.',
    'cfg_limit_server' => 'Limit to server?',
    'cfg_limit_server_hlp' => 'Set this to a simgle server that you want to limit '
        . 'or "false" to show all. The server name should be the one used for awstats.',
    'cfg_layout_settings' => 'Layout Settings',
    'cfg_layout_type' => 'Layout Type',
    'cfg_layout_type_hlp' => 'Display page in vertical or horizontal layout?',
    'cfg_vertical' => 'Vertical',
    'cfg_horizontal' => 'Horizontal',
    'cfg_language' => 'Language',
    'cfg_language_hlp' => 'Set your language. Set to "auto" to autodetect from browser',
    'cfg_firstweekday' => 'First day of the week',
    'cfg_firstweekday_hlp' => 'Should Sunday be the first day of the week or monday?',
    'cfg_decimalpoint' => 'Decimal Point',
    'cfg_decimalpoint_hlp' => "Decimal Point Character (99.9)",
    'cfg_thous_sep' => 'Thousands separator',
    'cfg_thous_sep_hlp' => "Thousand Digit separator (1'000)",
    'cfg_date_form' => 'Date format (2007-31-12)',
    'cfg_date_form_hlp' => 'How should a date look like? For formatting, please '
        . 'consult http://php.net/manual/en/function.date.php',
    'cfg_date_time_form' => 'Date &amp; Time format (2007-31-12 23:59)',
    'cfg_date_time_form_hlp' => 'How should a date &amp; time look like? For formatting,'
        . 'please consult http://php.net/manual/en/function.date.php',
    'cfg_percent_dec' => 'Percentage decimals',
    'cfg_percent_dec_hlp' => "How many decimals for percentage value? (99.9%)",
    'cfg_field_length' => 'Field Length',
    'cfg_field_length_hlp' => 'What is the max. text length of table fields? '
        . "(Applies only to links)",
    'cfg_max_lines' => 'Max. Table Lines',
    'cfg_max_lines_hlp' => 'What is the max. no. of lines a table can have? '
        . 'Set to "false" to disable. If a table reaches this number of lines, '
        . 'The rest is summarized into one line. This also applies to the "full list" '
        . 'view of a table',
    'cfg_hide_empty' => 'Hide Empty data',
    'cfg_hide_empty_hlp' => 'Completely hide graphs with zero entries? '
        . '(The menu will also be hidden)',
    'cfg_auto_submit_form' => 'Submit dropdowns on change',
    'cfg_auto_submit_form_hlp' => 'If enabled, the site/date dropdowns do '
        . 'not have an "OK"-button. The page is refreshed as soon as you choose '
        . 'a new value. Not recommended for large sites.',
    'cfg_chart_settings' => 'Chart Settings',
    'cfg_max_scale_visitors' => 'Max scale for Visitors',
    'cfg_max_scale_visitors_hlp' => 'The maximum value of the chosen option will '
        . 'define the maximum height of the Visitors bars in the chart',
    'cfg_max_scale_visits' => 'Max scale for Visits',
    'cfg_max_scale_visits_hlp' => 'The maximum value of the chosen option will '
        . 'define the maximum height of the Visits bars in the chart',
    'cfg_max_scale_pages' => 'Max scale for Pages',
    'cfg_max_scale_pages_hlp' => 'The maximum value of the chosen option will '
        . 'define the maximum height of the Pages bars in the chart',
    'cfg_max_scale_hits' => 'Max scale for Hits',
    'cfg_max_scale_hits_hlp' => 'The maximum value of the chosen option will '
        . 'define the maximum height of the Hits bars in the chart',
    'cfg_max_chart_items' => 'Max no of chart rows',
    'cfg_max_chart_items_hlp' => 'When displaying the charts with the full lists, '
        . 'How many items can there be displayed? The rest will sum up into "'.$BAW_MES[2].'". '
        . 'This is done to prevent too wide charts',
    'cfg_chart_titles' => 'Chart Titles?',
    'cfg_chart_titles_hlp' => 'If enabled, it will show a title on top of each chart.',
    'cfg_jpgraph_settings' => 'JPGraph Settings',
    'cfg_jpgraph_enable' => 'Enable JPgraph?',
    'cfg_jpgraph_enable_hlp' => 'To use JPGraph, you have to download it from '
        . 'http://www.aditus.nu/jpgraph/jpdownload.php.',
    'cfg_jpgraph_path' => 'Path to JPGraph',
    'cfg_jpgraph_path_hlp' => 'Where is your JPGraph installation? (The folder '
        . 'where jpgraph.php is in. No trailing slash)',
    'cfg_table_settings' => 'Table Settings',
    'cfg_advanced_settings' => 'Advanced Settings',
    'cfg_version' => 'Version',
    'cfg_version_hlp' => 'Version of this software',
    'cfg_xhtml' => 'XHTML/ HTML',
    'cfg_xhtml_hlp' => 'Do you want output in HTML or XHTML?',
    'cfg_debug' => 'Debug',
    'cfg_debug_hlp' => 'Do you want to show debug-output (VERY detailed)?',
    'cfg_parser' => 'Parser Stats',
    'cfg_parser_hlp' => 'Do you want to show log file parsing data below the stats summary?',
    'cfg_module' => 'Module settings',
    'cfg_module_hlp' => 'Are you using BetterAWstats as a module for another '
        . 'software? (Currently only Drupal is supported)',
    'cfg_type_order' => 'Item Sequence',
    'cfg_type_show' => 'Show this Data?',
    'cfg_type_collapse' => 'Collapsed?',
    'cfg_type_table' => 'Show data table?',
    'cfg_type_sort' => 'Sort for which column? ',
    'cfg_possible_values'=> ' Possible values are:',
    'cfg_type_sort_dir' => "Sort direction?",
    'cfg_type_sort_dir_opts' => 'SORT_ASC=Ascending, SORT_DESC=Descending',
    'cfg_type_chart' => 'Show HTML chart?',
    'cfg_type_map' => 'Show Map Image?',
    'cfg_type_avg' => 'Show averages?',
    'cfg_type_total' => 'Show total Sum?',
    'cfg_type_top_x' => 'Show how many entries?',
    'cfg_type_assumebot' => 'Hits/pages minimum ratio to assume normal user?',
    'cfg_type_showmonths' => 'Show how many months?',
    'cfg_type_favicon' => 'Retrieve favicons for external URLs?',
    'cfg_type_domain_lvls' => 'Shorten URL to how many domain levels? (-1 to disable)',
    'cfg_dis_overview' => 'General Overview of key figures and dates',
    'cfg_dis_months' => 'Monthly data',
    'cfg_dis_days' => 'Daily data',
    'cfg_dis_weekdays' => 'Weekdays',
    'cfg_dis_hours' => 'Hours of the day',
    'cfg_dis_domains' => 'Domains of visitors',
    'cfg_dis_visitors' => 'IP addresses of visitors',
    'cfg_dis_logins' => 'Logins for username/password protected pages',
    'cfg_dis_robots' => 'Spiders, Robots of Search engines etc.',
    'cfg_dis_worms' => 'Worms searching for security holes',
    'cfg_dis_sessions' => 'How long have people been on the site?',
    'cfg_dis_filetype' => 'What filetypes are on the site',
    'cfg_dis_urls' => 'Pages on the site',
    'cfg_dis_paths_hlp' => 'Files/paths on the site',
    'cfg_dis_paths' => 'Files/paths',
    'cfg_dis_os' => 'Operating system of users',
    'cfg_dis_unknownos' => 'Unknown Operating system',
    'cfg_dis_osversions' => 'Operating system of users including versions',
    'cfg_dis_browsers' => 'User Browser Type',
    'cfg_dis_browserversions' => "User Browser Type (+{$BAW_MES[58]})",
    'cfg_dis_unknownbrowser' => 'Unknown Browsers',
    'cfg_dis_unknownbrowser_agent' => 'User Agent',
    'cfg_dis_screensizes' => 'Screensizes of users',
    'cfg_dis_se_referers' => 'Referrals from search engines',
    'cfg_dis_referers' => 'Referrals from other sites',
    'cfg_dis_referer_domains' => 'Referrals from other sites, grouped by 2-nd level domains',
    'cfg_dis_hotlinks' => 'Pages linking to images/data on your site',
    'cfg_dis_hotlink_domains' => 'Domains linking to images/data on your site',
    'cfg_dis_searchphrases' => 'Search phrases',
    'cfg_dis_searchwords' => 'Search words',
    'cfg_dis_misc' => 'User system features',
    'cfg_dis_errors' => 'Acesses to pages that returned errors',

    // display_helpers
    'created_by' => 'created by %s',
    'show' => 'Show',
    'hide' => 'Hide',
    'all_months' => 'All (Monthly)',
    'all_days' => 'All (Daily)',
    'get_help' => 'Get Help',
    'back' => 'Back',
    'version_check' => 'Check for updates',
    'version_info' => 'Version Data',
    'unknown_list' => 'Unknown Data',

    // reder_table
    'table_max_hits_exceed' => '(Above max_hits config limit of %s)',
    'records'=> '%s records', // new

    // section help
    'hlp_overview' => 'This section shows the general data for the currently '
        . 'selected month. Please note that this data does not accurately exclude '
        . 'hits caused by hotlinks and robots, hacker-scripts etc. If you set '
        . "\$BAW_CONF['show_parser_stats'] to true, you can see here statistics about"
        . 'the logfile update such as Parsed records, Old records, New records, '
        . 'Corrupted and Dropped lines.',
    'hlp_months' => 'This section shows the history of data several months into '
        . 'the past. You can change the time span in the configuration.',
    'hlp_days' => 'This section shows the daily history of data of the selected month. '
        . 'If the current month is not finished, it will show the data of the '
        . 'past month for the same amount of days that are left within the current month. ',
    'hlp_weekdays' => 'This section shows the averages of visits/pages/hits history for '
        .'each weekday. Since the each month has a different amount of weekdays, summing '
        . 'up the data as AWSTATS is doing it, would distort the data. That is why BetterAWStats '
        . 'is showing the averages only.',
    'hlp_hours' => 'This section shows the averages of visits/pages/hits history for '
        .'each hour of the day. In case the current month is not finished, summing '
        . 'up the data as AWSTATS is doing it, would distort the data. That is why BetterAWStats '
        . 'is showing the averages.',
    'hlp_domains' => 'This section shows the hits per country (if known). Please note that '
        . 'the map-image summs the domains us, mil, edu, gov and arpa into the "us" domain.',
    'hlp_visitors' => 'This section shows the statistics per user\'s IP address. '
        . 'Since almost all pages consist out of several files (images, stylesheets etc.),  '
        . 'visits with a low hits/pages ratio are most '
        . 'likely bots, scripts or other software but not by normal users. '
        . 'Those are summed up here under "Assumed Scripts &amp; Bots". To change the ratio '
        . 'that defines a visit as a script, please change the '
        . '$BAW_CONF_DIS[\'visitors\'][\'assumebot\'] value in the configuration.'
        . 'Similarly, users with only hits but no pages are hits from proxies or '
        . 'hotlinking users. Those are summed up under "Hotlinks/Proxies".' ,
    'hlp_logins' => 'This section shows the hits per authenticated user. A user '
        . 'with one hit most likely represents a unsuccessful login attempt.',
    'hlp_referer_domains' => 'This section takes the data from "Referring sites" '
        . 'and sums it up by domain. AWStats does not show this information. '
        . 'If a referer has a link to your site on each of its '
        . 'pages, the overall number of referred visitors might be high, but the '
        . 'referrer might still not show up in the top 10 of the "Refering Sites". '
        . 'This is why BetterAWStats shows the domain sum so you can judge the true '
        . 'value of a refering site.',
    'hlp_hotlinks' => 'This section shows the referrers from other sites where files '
        . 'such as graphics where requested but no pages. This indicates that the '
        . 'referring site is hotlinking to your site (using your bandwidth, storage '
        . 'and image) while giving its own users the impression the data was hosted '
        . 'at the referring site. This data is separated from the "Referring sites" '
        . 'Statistics by BetterAWStats. AWStats does not make this difference.',
    'hlp_hotlink_domains' => 'This section shows the referrers from other sites where files '
        . 'such as graphics where requested but no pages. This indicates that the '
        . 'referring site is hotlinking to your site (using your bandwidth, storage '
        . 'and image) while giving its own users the impression the data was hosted '
        . 'at the referring site. This data is separated from the "Referring sites" '
        . 'Statistics by BetterAWStats. AWStats does not make this difference. '
        . 'In addition, BetterAWStats sums in this section the data by domain to make the hits by '
        . 'domain more visible since one domain might hotlink the same data from '
        . 'multiple URLs.',
);

?>