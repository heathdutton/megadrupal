<?php

// since we are merging this here with the message file from AWSTATS, we have to
// add these internal messages to the ones from AWSTATS. Please remember NOT to
// use any numeric indexes since they are used by AWSTATS.

// this language files is kindly translated into Dutch by Edwin de Ruijter

global $BAW_MES;
$BAW_MES += array(

    // display.inc.php
    'data_file_stats' => 'Gegevens bestand bijwerken statistieken:',
    'parsed_records' => 'Geanalyseerde records:',
    'old_records' => 'Oude records:',
    'new_records' => 'Nieuwe records:',
    'corrupted' => 'Onbetrouwbaar:',
    'dropped' => 'Niet meegenomen:',
    'ratio_pages_hits' => 'Verhouding'.BR.'(Hits/Pagina\'s)',
    'files_paths' => 'Bestanden/directories',
    'user_agent' => 'User Agent',
    'size' => 'Grootte',
    'by_domains' => '%s per domeinen',
    'visits' => 'Bezoeken',
    'averages' => ' (Gemiddelden)',
    'assumedscript' => 'Verondersteld Robots/Spiders',
    'weekday_averages' => $BAW_MES[91] . ' (Gemiddelden)',
    'hours_averages' => $BAW_MES[20] .  ' (Gemiddelden)',
    'os_versions' =>  "{$BAW_MES[59]} (+{$BAW_MES[58]})",
    'browser_versions' => "{$BAW_MES[21]} (+{$BAW_MES[58]})",
    'referer_hotlinks' => 'Hotlinks',
    'hl_by_domains ' => "Hotlinks per domeinen",
    'hotlinks_proxies' => 'Hotlinks/Proxies',

    // baw_raise_error
    'baw_error' => 'BetterAWstats Fout',
    'datafile' =>
        "Gegevens bestand is onbetrouwbaar, gegevens konden niet worden gelezen: %s;<br>'%s' moet zijn BEGIN_...".BR
        . "Lees a.u.b. de FAQ (/doc/FAQ.txt)",
    'datafileindex' =>
        "Gegevens bestand is onbetrouwbaar, gegevens-index kon niet worden gelezen: %s;<br>'%s' moet zijn BEGIN_...".BR
        . "Lees a.u.b. de FAQ (/doc/FAQ.txt)",
    'datafilesdir' =>
        "De gegevens bestanden konden niet gevonden worden in deze lokatie: %s".BR
        . "Controleer a.u.b. uw config.php. De waarde van \$BAW_CONF['path_data'] moet "
        . "verwijzen naar de directory waar awstats zijn gegevens bestanden bewaart "
        . "(typisch iets als /cgi-bin/awstats/data). Controleer a.u.b. ook "
        . "dat php lees-rechten heeft op deze directory.",
    'languagefile' =>
        "Het taal bestand kon niet gevonden worden in deze lokatie: %s".BR
        . "Controleer a.u.b. uw config.php. De waarde van \$BAW_CONF['path_lang'] moet "
        . "verwijzen naar de directory waar awstats zijn taal bestand bewaart "
        . "(typisch iets als /cgi-bin/awstats/lang). Controleer a.u.b. ook "
        . "dat php lees-rechten heeft op deze directory.",
    'libraryfiles' =>
        "De library bestanden konden niet gevonden worden in deze lokatie: %s".BR
        . "Controleer a.u.b. uw config.php. De wwarde van \$BAW_CONF['path_lib'] moet "
        . "verwijzen naar de directory waar awstats zijn library bestanden bewaart "
        . "(typisch iets als /cgi-bin/awstats/lib). Controleer a.u.b. ook "
        . "dat php lees-rechten heeft op deze directory.",
    'libraryeval' =>
        "Een fout is opgetreden bij het analyseren van het awstats library bestand %s dat wordt gebruikt bij "
        . "het ontleden van de variabele %s. Schakel a.u.b. debugging aan in config.php en "
        . "zend de getoonde PHP foutboodschappen naar het support forum zodat het probleem verholpen kan worden.",
    'jpgraph_path' =>
        "De bestanden voor jpgraph konden niet gevonden worden in %s. Schakel dit onderdeel uit in "
        . "config.php of corrigeer het pad naar de bestanden.",
    'gd2notavailable' =>
        "De PHP graphics library GD2 is niet beschikbaar. Schakel a.u.b. de map functie uit of installeer GD2."
        . "U kunt meer informatie verkrijgen op http://php.net/manual/en/ref.image.php",
    'mapnotavailable' =>
        "De afbeeldingen om de Map (%s) te maken zijn niet beschikbaar. Controleer a.u.b. dat u "
        . "alle bestanden van de setup heeft en dat deze leesbaar zijn door "
        . "de server.",
    'iconpath' =>
        "De AWSTATS icons konden niet gevonden worden in deze locatie: %s".BR
        . "Controlleer a.u.b. uw config.php. De waarde van \$BAW_CONF['icons_url'] moet "
        . "wijzen naar de URL waar awstats zijn library bestanden bewaart "
        . "(typisch iets als http://awstats.local/awstats/icon).",
    'site_path' =>
        "Het pad naar awstats is niet goed gezet naar directory: %s".BR
        . "Controlleer a.u.b. uw config.php. De waarde van \$BAW_CONF['site_path'] moet "
        . "wijzen naar de directory waar BetterAWStats is ge-installeerd "
        . "(typisch iets als /home/user/public_html/baw).".BR
        . "Het schijnt dat uw script is ge-installeerd in '%s'.",
    'site_url' =>
        'De URL naar BetterAWStats is niet goed gezet in uw config.php. Hij staat momenteel '
        . 'op %s, maar dat is niet goed. Het zou kunnen dat de URL %s moet zijn.',
    'xmldata' =>
        'Your AWStats data files are stored in XML format. Unfortunately, BetterAWSTats '
        . 'does not support XML data yet. Please store your datafiles in plain text.',

    // $BAW_LIB['item_groups']
    'time' => 'Tijd',
    'user_information' => 'Gebruiker informatie',
    'user_actions' => 'Gebruiker akties',
    'user_origin' => 'Gebruiker herkomst',
    'other_access' => 'Andere toegang',

    // debug messages
    'dbg_detect_language' => "Bezig de taal te vinden",
    'dbg_detected_language' => "Gevonden taal %s",
    'dbg_test_writable' => "Bezig te controleren of %s overschrijfbaar is",
    'dbg_test_writable_false' => "IS NIET overschrijfbaar: %s",
    'dbg_test_writable_true' => 'IS overschrijfbaar: %s',
    'dbg_found_dir' => 'Gevonden directory %s',
    'dbg_found_file' => 'Gevonden bestand: %s',
    'dbg_finished_parse_dir' => "Klaar met analyseren directory %s",
    'dbg_start_parse_dir' => "Start analyseren directory %s",

    // config_default.inc.php
    'cfg_enable' => 'Schakel in',
    'cfg_disable' => 'Schakel uit',
    'cfg_show_all' => 'Toon alle',
    'monday' => 'Maandag',
    'sunday' => 'Zondag',
    'cfg_hide' => 'Verberg',
    'cfg_show' => 'Toon',
    'cfg_site_settings' => 'Site instellingen',
    'cfg_path_to_aws_data' => 'Pad naar AWStats gegevens',
    'cfg_path_to_aws_data_hlp' => "Zet deze waarde op de directory waar AWStats "
        . "zijn gegevens bestanden in bewaart. ATTENTIE: Als u deze bestanden met "
        . "windows leest, maar u heeft ze met Linux aangemaakt of vice versa, let er dan op "
        . "dat u ze 'BINARY' overzet. Anders kunnen ze niet goed gelezen worden. "
        . "Niet afsluiten met een slash.",
    'cfg_path_to_aws_lib' => 'Pad naar AWStats libraries',
    'cfg_path_to_aws_lib_hlp' => "Zet deze waarde op de directory waar AWStats "
        . "zijn library bestanden in bewaart. Niet afsluiten met een slash.",
    'cfg_path_to_aws_lang' => 'Pad naar AWStats taal bestanden',
    'cfg_path_to_aws_lang_hlp' => 'Zet deze waarde op de directory waar AWStats '
        . 'zijn language bestanden in bewaart. Niet afsluiten met een slash.',
    'cfg_script_url' => 'Script URL',
    'cfg_script_url_hlp' => "De url van BetterAWstats' directory, Niet afsluiten met een slash.",
    'cfg_script_path' => 'Script pad',
    'cfg_script_path_hlp' => 'Het pad van BetterAWstats, Niet afsluiten met een slash.',
    'cfg_aws_icons_url' => 'URL naar AWStats icons',
    'cfg_aws_icons_url_hlp' => 'De url naar de awstats icons, moet bevatten de '
        . 'gehele http://..., niet afsluiten met een slash.',
    'cfg_limit_server' => 'Beperken tot server?',
    'cfg_limit_server_hlp' => 'Zet dit naar een enkele server waartoe u zich wilt beperken '
        . 'of "false" om allen te tonen. De server naam moet degene zijn die gebruikt wordt voor awstats.',
    'cfg_layout_settings' => 'Opmaak instellingen',
    'cfg_layout_type' => 'Opmaak type',
    'cfg_layout_type_hlp' => 'Toon page in verticale of horizontale opmaak?',
    'cfg_vertical' => 'Verticaal',
    'cfg_horizontal' => 'Horizontaal',
    'cfg_language' => 'Taal',
    'cfg_language_hlp' => 'Zet uw taal. Zet op "auto" om de browserwaarde te nemen.',
    'cfg_firstweekday' => 'Eerste dag van de week',
    'cfg_firstweekday_hlp' => 'Moet Zondag de eerste dag van de weekzijn of maandag?',
    'cfg_decimalpoint' => 'Decimale punt',
    'cfg_decimalpoint_hlp' => "Decimale punt teken (99.9)",
    'cfg_thous_sep' => 'Duizendtal scheidingsteken',
    'cfg_thous_sep_hlp' => "Duizendtal scheidingsteken (1'000)",
    'cfg_date_form' => 'Datum formaat (2007-31-12)',
    'cfg_date_form_hlp' => 'Hoe moet een datum er uit zien? Voor formattering, raadpleeg '
        . 'a.u.b. http://php.net/manual/en/function.date.php',
    'cfg_date_time_form' => 'Datum &amp; tijd formaat (2007-31-12 23:59)',
    'cfg_date_time_form_hlp' => 'Hoe moet een datum &amp; tijd er uit zien? Voor formattering, raadpleeg '
        . 'a.u.b http://php.net/manual/en/function.date.php',
    'cfg_percent_dec' => 'Percentage decimalen',
    'cfg_percent_dec_hlp' => "Hoeveel decimalen voor percentages? (99.9%)",
    'cfg_field_length' => 'Veld lengte',
    'cfg_field_length_hlp' => 'Wat is de maximale tekstlengte van tabel velden? '
        . "(Alleen van toepassing op links)",
    'cfg_max_lines' => 'Maximaal aantal tabel rijen',
    'cfg_max_lines_hlp' => 'Wat is het maximale aantal rijen dat een tabel kan hebben? '
        . 'Zet op "false" om uit te schakelen. Als een tabel dit aantal rijen bereikt, '
        . 'wordt de rest geaggregeerd in 1 regel. Dit gaat ook op voor de "full list view"'
        . ' van een tabel.',
    'cfg_hide_empty' => 'Verberg lege gegevens',
    'cfg_hide_empty_hlp' => 'Volledig verbergen van grafieken met nul-entries? '
        . '(Het menu wordt ook verborgen)',
    'cfg_auto_submit_form' => 'Voer dropdown-wijzigingen meteen uit',
    'cfg_auto_submit_form_hlp' => 'Als dit is ingeschakeld, zullen de site/datum dropdowns geen '
        . '"OK"-button hebben. De pagina wordt aangeroepen zodra u een nieuwe waarde '
        . 'kiest. Niet aanbevolen voor grote sites.',
    'cfg_chart_settings' => 'Grafiek instellingen',
    'cfg_max_scale_visitors' => 'Maximale schaal voor Bezoekers',
    'cfg_max_scale_visitors_hlp' => 'De maximum waarde van de gekozen optie zal '
        . 'de maximum hoogte van de Bezoekers staven in de grafiek bepalen.',
    'cfg_max_scale_visits' => 'Maximale schaal voor Bezoeken',
    'cfg_max_scale_visits_hlp' => 'De maximum waarde van de gekozen optie zal '
        . 'de maximum hoogte van de Bezoeken staven in de grafiek bepalen.',
    'cfg_max_scale_pages' => 'Maximale schaal voor Paginas',
    'cfg_max_scale_pages_hlp' => 'De maximum waarde van de gekozen optie zal '
        . 'de maximum hoogte van de Paginas staven in de grafiek bepalen.',
    'cfg_max_scale_hits' => 'Maximale schaal voor Hits',
    'cfg_max_scale_hits_hlp' => 'De maximum waarde van de gekozen optie zal '
        . 'de maximum hoogte van de Hits staven in de grafiek bepalen.',
    'cfg_max_chart_items' => 'Maximaal aantal grafiek rijen',
    'cfg_max_chart_items_hlp' => 'Wanneer grafieken getoond worden met de volledige lijsten, '
        . 'hoeveel items kunnen dan getoond worden? De rest wordt gesommeerd in "'.$BAW_MES[2].'". '
        . 'Dit wordt zo gedaan om te brede grafieken te voorkomen.',
    'cfg_chart_titles' => 'Grafiek Titels?',
    'cfg_chart_titles_hlp' => 'Indien ingeschakeld, zal er een titel boven iedere grafiek getoond worden.',
    'cfg_jpgraph_settings' => 'JPGraph instellingen',
    'cfg_jpgraph_enable' => 'Schakel JPgraph in?',
    'cfg_jpgraph_enable_hlp' => 'Om JPGraph te gebruiken, zult u dit moeten downloaden van '
        . 'http://www.aditus.nu/jpgraph/jpdownload.php.',
    'cfg_jpgraph_path' => 'Pad naar JPGraph',
    'cfg_jpgraph_path_hlp' => 'Waar staat uw JPGraph installatie? (De directory '
        . 'waar jpgraph.php in staat. Niet afsluiten met een slash.)',
    'cfg_table_settings' => 'Tabel instellingen',
    'cfg_advanced_settings' => 'Geavanceerde instellingen',
    'cfg_version' => 'Versie',
    'cfg_version_hlp' => 'Versie van deze software',
    'cfg_xhtml' => 'XHTML/ HTML',
    'cfg_xhtml_hlp' => 'Wilt u uw output in HTML or XHTML?',
    'cfg_debug' => 'Debug',
    'cfg_debug_hlp' => 'Wilt u debug-output tonen (ZEER gedetaileerd)?',
    'cfg_parser' => 'Analyse statistieken',
    'cfg_parser_hlp' => 'Wilt u log file analyse gegevens tonen onder de statistiek samenvatting?',
    'cfg_module' => 'Module instellingen',
    'cfg_module_hlp' => 'Gebruikt u BetterAWstats als module voor andere '
        . 'software? (Momenteel is alleen Drupal ondersteund)',
    'cfg_type_order' => 'Sorteer volgorde',
    'cfg_type_show' => 'Toon deze gegevens?',
    'cfg_type_collapse' => 'Ingeklapt?',
    'cfg_type_table' => 'Toon gegevens tabel?',
    'cfg_type_sort' => 'Gesorteerd op welke kolom?',
    'cfg_possible_values'=> ' Mogelijke waarden zijn:',
    'cfg_type_sort_dir' => 'Sorteer richting?',
    'cfg_type_sort_dir_opts' => 'SORT_ASC=Oplopend, SORT_DESC=Aflopend',
    'cfg_type_chart' => 'Toon HTML grafiek?',
    'cfg_type_map' => 'Toon landkaart afbeelding?',
    'cfg_type_avg' => 'Toon geniddelden?',
    'cfg_type_total' => 'Toon totalen som?',
    'cfg_type_top_x' => 'Toon hoeveel entries?',
    'cfg_type_assumebot' => 'Verberg visitors waar Hits = Paginas?',
    'cfg_type_showmonths' => 'Toon hoeveel maanden?',
    'cfg_type_favicon' => 'Favicons ophalen voor externe URLs?',
    'cfg_type_domain_lvls' => 'Verkort URL naar hoeveel domein levels? (-1 om uit te schakelen)',
    'cfg_dis_overview' => 'Algemeen overzicht van kentallen en datums',
    'cfg_dis_months' => 'Maandelijkse gegevens',
    'cfg_dis_days' => 'Dagelijkse gegevens',
    'cfg_dis_weekdays' => 'Weekdagen',
    'cfg_dis_hours' => 'Uren van de dag',
    'cfg_dis_domains' => 'Domeinen van bezoekers',
    'cfg_dis_visitors' => 'IP addressen van bezoekers',
    'cfg_dis_logins' => 'Logins voor gebruikersnaam/wachtwoord beschermde paginas',
    'cfg_dis_robots' => 'Spiders, Robots van Zoek machines etc.',
    'cfg_dis_worms' => 'Worms die beveiligingslekken zoeken',
    'cfg_dis_sessions' => 'Hoe lang is men op de site geweest?',
    'cfg_dis_filetype' => 'Welke bestandstypen zijn op de site',
    'cfg_dis_urls' => 'Paginas op de site',
    'cfg_dis_paths_hlp' => 'Bestanden/paden op de site',
    'cfg_dis_paths' => 'Bestanden/paden',
    'cfg_dis_os' => 'Besturingssysteem van gebruikers',
    'cfg_dis_unknownos' => 'Onbekend besturingssysteem',
    'cfg_dis_osversions' => 'Besturingssysteem van gebruikers inklusief versies',
    'cfg_dis_browsers' => 'Gebruikers browser type',
    'cfg_dis_browserversions' => "Gebruikers browser type (+{$BAW_MES[58]})",
    'cfg_dis_unknownbrowser' => 'Onbekende browsers',
    'cfg_dis_unknownbrowser_agent' => 'User agent',
    'cfg_dis_screensizes' => 'Schermafmetingen van gebruikers',
    'cfg_dis_se_referers' => 'Referrals van zoek machines',
    'cfg_dis_referers' => 'Referrals van andere sites',
    'cfg_dis_referer_domains' => 'Referrals van andere sites, gegroepeerd per 2e level domeinen',
    'cfg_dis_hotlinks' => 'Paginas die verwijzen naar afbeeldingen/gegevens op uw site',
    'cfg_dis_hotlink_domains' => 'Domeinen die verwijzen naar afbeeldingen/gegevens op uw site',
    'cfg_dis_searchphrases' => 'Zoek zinnen',
    'cfg_dis_searchwords' => 'Zoek woorden',
    'cfg_dis_misc' => 'Gebruikers systeem kenmerken',
    'cfg_dis_errors' => 'Toegang tot pagina\'s die foutmeldingen afgaven',

    // display_helpers
    'created_by' => 'gemaakt door %s',
    'show' => 'Toon',
    'hide' => 'Verberg',
    'all_months' => 'Alle (Maandelijks)',
    'all_days' => 'Alle (Dagelijks)',
    'get_help' => 'Vraag hulp',
    'back' => 'Terug',
    'version_check' => 'Kijk of er updates zijn',
    'version_info' => 'Versie Gegevens',
    'unknown_list' => 'Onbekende Gegevens',

    // reder_table
    'table_max_hits_exceed' => '(Overstijgende max_hits config beperken tot %s)',
    'records'=> '%s records',

    // section help
    'hlp_overview' => 'Deze sectie toont de algemene gegevens van de huidig '
        . 'geselecteerde maand. Let a.u.b. op dat de hits, veroorzaakt door hotlinks en '
        . 'robots, hacker-scripts etc., op een niet accurate wijze zijn weggelaten. Als u '
        . "\$BAW_CONF['show_parser_stats'] op true zet, kunt u hier statistieken zien over"
        . 'de logfile update zoals geanalyseerde records, oude records, nieuwe records, '
        . 'onbetrouwbare en weggelaten regels.',
    'hlp_months' => 'Deze sectie toont de historie van gegevens enkele maanden in het '
        . 'verleden. U kunt de tijdspanne veranderen in de configuratie.',
    'hlp_days' => 'Deze sectie toont de dagelijkse historie van gegevens van de geselecteerde maand. '
        . 'Als de huidige maand nog gaande is, zullen hier de gegevens getoond worden van de '
        . 'verleden maand voor hezelfde aantal dagen dat nog te gaan is in de huidige maand. ',
    'hlp_weekdays' => 'Deze sectie toont de gemiddelden van bezoeken/pagina\'s/hits historie voor '
        .'elke weekdag. Omdat iedere maand een verschillend aantal weekdagen heeft, zou sommering '
        . 'van de gegevens, zoals AWSTATS dat doet, de gegevens vervormen. Daarom toont BetterAWStats '
        . 'alleen de gemiddelden.',
    'hlp_hours' => 'Deze sectie toont de gemiddelden van bezoeken/pagina\'s/hits historie voor '
        .'elk uur van de dag. Als de huidige maand nog gaande is, zou sommering '
        . 'van de gegevens, zoals AWSTATS dat doet, de gegevens vervormen. Daarom toont BetterAWStats '
        . 'alleen de gemiddelden.',
    'hlp_domains' => 'Deze sectie toont de hits per land (indien bekend). Let a.u.b. op dat '
        . 'in de wereldkaartafbeelding de domeinen us, mil, edu, gov en arpa gesommeerd zijn in het "us" domein.',
    'hlp_visitors' => 'Deze sectie toont de statistieken per gebruiker\'s IP adres. '
        . 'Omdat haast alle pagina\'s bestaan uit verschillende bestanden (afbeeldingen, stylesheets etc.),  '
        . 'zijn bezoeken met met een lage hits/pagina\'s ratio zeer '
        . 'waarschijnlijk gecreeerd door bots, scripts of andere software maar niet door reguliere gebruikers. '
        . 'Deze zijn hier gesommeerd onder "Veronderstelde Scripts &amp; Bots". Om de ratio te wijzigen '
        . 'die een gebruiker als een script definieert, verandert u de '
        . '$BAW_CONF_DIS[\'visitors\'][\'assumebot\'] waarde in de configuratie.'
        . 'Evenzo, gebruikers met alleen hits maar geen pagina\'s zijn hits van proxies of '
        . 'hotlinking gebruikers. Deze zijn gesommeerd onder "Hotlinks/Proxies".' ,
    'hlp_logins' => 'Deze sectie toont de hits per ge-autoriseerde gebruiker. Een gebruiker '
        . 'met maar 1 hit is zeer waarschijnlijk het resultaat van een mislukte login poging.',
    'hlp_referer_domains' => 'Deze sectie neemt de gegevens van "Referring sites" '
        . 'en sommeerd dit per domein. AWStats toont deze informatie niet. '
        . 'Als een referer een link naar uw site heeft op al zijn '
        . 'pagina\'s, dan zou het totaal aantal van refer bezoekers zeer hoog zijn, maar de '
        . 'referrer zou nog steeds niet getoond worden in de top 10 van de "Refering Sites". '
        . 'Dit is de reden dat BetterAWStats het domein totaal toont, zodat u de juiste waarde aan '
        . 'refering site kunt hechten.',
    'hlp_hotlinks' => 'Deze sectie toont de referrers van andere sites die bestanden '
        . 'zoals afbeeldingen aanvragen maar geen pagina\'s. Dit houdt in dat de '
        . 'referring site hotlinks naar uw site heeft (en dus gebruik maakt van uw bandbreedte, opslag '
        . 'en afbeelding(en)) terwijl zijn gebruikers het idee gegeven wordt dat de gegevens gehost '
        . 'zijn op de referring site. Deze gegevens zijn gescheiden van "Referring sites" '
        . 'Statistieken door BetterAWStats. AWStats maakt dit onderscheid niet. ',
    'hlp_hotlink_domains' => 'Deze sectie toont de referrers van andere sites die bestanden '
        . 'zoals afbeeldingen aanvragen maar geen pagina\'s. Dit houdt in dat de '
        . 'referring site hotlinks naar uw site heeft (en dus gebruik maakt van uw bandbreedte, opslag '
        . 'en afbeelding(en)) terwijl zijn gebruikers het idee gegeven wordt dat de gegevens gehost '
        . 'zijn op de referring site. Deze gegevens zijn gescheiden van "Referring sites" '
        . 'Statistieken door BetterAWStats. AWStats maakt dit onderscheid niet '
        . 'Bovendien, BetterAWStats sommeert in deze sectie de gegevens per domain om de hits per '
        . 'domein meer inzichtelijk te maken, omdat een domain wellicht dezelfde gegevens '
        . 'van meerdere URLs hotlinkt.',
);

?>