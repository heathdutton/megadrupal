<?php

// since we are merging this here with the message file from AWSTATS, we have to
// add these internal messages to the ones from AWSTATS. Please remember NOT to
// use any numeric indexes since they are used by AWSTATS.

global $BAW_MES;
$BAW_MES += array(

    // display.inc.php
    'data_file_stats' => 'Data File Update Stats:',
    'parsed_records' => 'Verarb. Einträge:',
    'old_records' => 'Alte Einträge:',
    'new_records' => 'Neue Einträge:',
    'corrupted' => 'Ungültig:',
    'dropped' => 'Übersprungen:',
    'ratio_pages_hits' =>  'Ratio'.BR.'(Zugriffe/Seiten)',
    'files_paths' => 'Dateien/Verzeichnisse',
    'user_agent' => 'User Agent',
    'size' => 'Grösse',
    'by_domains' => '%s pro Domäne',
    'visits' => 'Besuche',
    'averages' => ' (Averages)',
    'assumedscript' => 'Vermutlich Roboter/Spider',
    'weekday_averages' => $BAW_MES[91] . ' (Durschn.)',
    'hours_averages' => $BAW_MES[20] .  ' (Durschn.)',
    'os_versions' =>  "{$BAW_MES[59]} (+{$BAW_MES[58]})",
    'browser_versions' => "{$BAW_MES[21]} (+{$BAW_MES[58]})",
    'referer_hotlinks' => 'Hotlinks',
    'hl_by_domains ' => "Hotlinks pro Domäne",
    'hotlinks_proxies' => 'Hotlinks/Proxies',

    // baw_raise_error
    'baw_error' => 'BetterAWstats Fehler',
    'datafile' =>
        "Daten-Datei ist fehlerhaft, Daten konnten nicht gelesen werden: %s;<br>'%s' sollte BEGIN_... sein.".BR
        . "Bitte lesen sie die FAQ (/docs/FAQ.txt)",
    'datafileindex' =>
        "Daten-Datei ist fehlerhaft, Daten-Index konnte nicht gelesen werden: %s;<br>'%s' sollte BEGIN_... sein.".BR
        . "Bitte lesen sie die FAQ (/docs/FAQ.txt)",
    'datafilesdir' =>
        "Die Daten-Dateien konnten nicht gefunden werden: %s".BR
        . "Bitte überprüfen Sie die config.php. Der Wert von \$BAW_CONF['path_data'] muss "
        . "auf das Verzeichnis der AWStats-Daten zeigen, "
        . "typischerweise /cgi-bin/awstats/data. Bitte stellen Sie auch sicher "
        . "dass PHP diesen Ordner lesen kann.",
    'languagefile' =>
        "Die Sprach-Dateien konnten nicht gefunden werden: %s".BR
        . "Bitte überprüfen Sie die config.php. Der Wert von \$BAW_CONF['path_lang'] muss "
        . "auf das Verzeichnis der AWStats-Sprach-Dateien zeigen, "
        . "typischerweise /cgi-bin/awstats/lang. Bitte stellen Sie auch sicher "
        . "dass PHP diesen Ordner lesen kann.",
    'libraryfiles' =>
        "Die Library-Dateien konnten nicht gefunden werden: %s".BR
        . "Bitte überprüfen Sie die config.php. Der Wert von \$BAW_CONF['path_lib'] muss "
        . "auf das Verzeichnis der AWStats-Library-Dateien zeigen, "
        . "typischerweise /cgi-bin/awstats/lib. Bitte stellen Sie auch sicher "
        . "dass PHP diesen Ordner lesen kann.",
    'libraryeval' =>
        "Es gab einen Fehler beim Lesen der awstats library datei %s um die Variable"
        . "%s zu erhalten. Bitte verwenden Sie den Debug-Modus und wenden Sie sich an "
        . "den Support.",
    'jpgraph_path' =>
        "Die JPGraph-Dateien konnten nicht unter %s gefunden werden. Bitte schalten"
        . "Sie entweder JPGraph ab oder installieren Sie die Dateien richtig.",
    'gd2notavailable' =>
        "Die PHP GD2-Graphik-Bibliothek konnte nicht gefunden werden. Bitte installieren Sie diese."
        . "oder schalten Sie die Karten-funktion ab. Weitere info zu GD2 unter http://php.net/manual/en/ref.image.php",
    'mapnotavailable' =>
        "Die Graphik zur erstellung der Karte (%s) konnte nicht gefunden werden. "
        . "Bitte stellen Sie sicher dass sie BetterAWStats richtig installiert haben"
        . "und dass alle Dateien von PHP lesbar sind.",
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
    'time' => 'Zeit',
    'user_information' => 'Besucher-Information',
    'user_actions' => 'Besucher-Handlungen',
    'user_origin' => 'Besucher-Herkunft',
    'other_access' => 'Andere Zugriffe',

    // debug messages
    'dbg_detect_language' => "Versuche Sprache zu erkennen.",
    'dbg_detected_language' => "Erkannte Sprache: %s",
    'dbg_test_writable' => "überprüfe ob %s schreibbar ist.",
    'dbg_test_writable_false' => "NICHT schreibbar: %s",
    'dbg_test_writable_true' => 'IST schreibbar: %s',
    'dbg_found_dir' => 'Verzichnis gefunden: %s',
    'dbg_found_file' => 'Datei gefunden: %s',
    'dbg_finished_parse_dir' => "Verzeichnis lesen beendet: %s",
    'dbg_start_parse_dir' => "Verszeichnis lesen startet: %s",

    // config_default.inc.php
    'cfg_enable' => 'Einschalten',
    'cfg_disable' => 'Ausschalten',
    'cfg_show_all' => 'Alle zeigen',
    'monday' => 'Montag',
    'sunday' => 'Sonntag',
    'cfg_hide' => 'Verstecken',
    'cfg_show' => 'Zeigen',
    'cfg_site_settings' => 'Site Einstellungen',
    'cfg_path_to_aws_data' => 'Pfad zu AWStats Daten',
    'cfg_path_to_aws_data_hlp' => "Dieser Pfad muss auf die von AWSTATS "
        . "generierten Daten zeigen. ACHTUNG: Wenn diese Daten von einem Windows-"
        . "System auf ein Linux-System übertragen werden (oder umgekehrt), ist es "
        . "wichtig, diese als BINÄR-Daten zu übertragen. Ansonsten können sie nicht "
        . "gelesen werden. Kein '/' am Ende.",
    'cfg_path_to_aws_lib' => 'Pfad zu den AWStats Libraries',
    'cfg_path_to_aws_lib_hlp' => "Dieser Pfad muss auf den Ordner zeigen, in dem "
        . "AWSTATS die Library-Dateien speichert. Kein '/' am Ende.",
    'cfg_path_to_aws_lang' => 'Pfad zu den AWStats Sprachdateien',
    'cfg_path_to_aws_lang_hlp' => "Dieser Pfad muss auf den Ordner zeigen, in dem "
        . "AWSTATS die Sprachdateien speichert. Kein '/' am Ende.",
    'cfg_script_url' => 'Programm-URL',
    'cfg_script_url_hlp' => "Die URL zu BetterAWStats. Kein '/' am Ende.",
    'cfg_script_path' => 'Programm-Pfad',
    'cfg_script_path_hlp' => "Der Pfad zu BetterAWstats. Kein '/' am Ende.",
    'cfg_aws_icons_url' => 'AWStats Icons URL',
    'cfg_aws_icons_url_hlp' => 'Die URL zu den AWStats Icons. Inclusive http://... '
        . "Kein '/' am Ende.",
    'cfg_limit_server' => 'Nur einen Server verwenden?',
    'cfg_limit_server_hlp' => 'Setzen Sie dieses gleich einem Servernamen oder '
        . '"false" um alle zu zeigen. Der Servername sollte der gleiche sein wie in AWStats.',
    'cfg_layout_settings' => 'Layout Einstellungen',
    'cfg_layout_type' => 'Layout Typ',
    'cfg_layout_type_hlp' => 'Sollen die einzelnen Statistiken horizontal oder vertikal angeordnet werden?',
    'cfg_vertical' => 'Vertikal',
    'cfg_horizontal' => 'Horizontal',
    'cfg_language' => 'Sprache',
    'cfg_language_hlp' => "Gewünschte Interface-sprache auswählen. 'Auto' für automatisches erkennen.",
    'cfg_firstweekday' => 'Erster Tag der Woche',
    'cfg_firstweekday_hlp' => 'Sollte Montag oder Sonntag als erster Tag der Woche angezeigt werden?',
    'cfg_decimalpoint' => 'Dezimal-Trennzeichen',
    'cfg_decimalpoint_hlp' => "Welches Zeichen soll als Dezimal-zeichen verwendet werden (99.9)?",
    'cfg_thous_sep' => 'Tausender-Trennzeichen',
    'cfg_thous_sep_hlp' => "Welches Zeichen soll als Tausender-Trennzeichen verwendet werden (1'0000)?",
    'cfg_date_form' => 'Datums-Format (2007-31-12)',
    'cfg_date_form_hlp' => 'Wie soll das Datum aussehen? Für Hilfe bitte unter '
        . 'http://php.net/manual/de/function.date.php nachsehen.',
    'cfg_date_time_form' => 'Datums &amp; Zeit Format (2007-31-12 23:59)',
    'cfg_date_time_form_hlp' => 'Wie soll Datum mit Zeit dargestellt werden? Für Hilfe bitte unter '
        . 'http://php.net/manual/de/function.date.php nachsehen.',
    'cfg_percent_dec' => 'Prozent-Dezimalstellen',
    'cfg_percent_dec_hlp' => "Wieviele dezimalstellen sollen Prozentwerte haben? (99.9%)",
    'cfg_field_length' => 'Link-länge',
    'cfg_field_length_hlp' => 'Wieviele Zeichen soll ein Tabllenfeld maximal lang sein?'
        . "(Betrifft nur Links)",
    'cfg_max_lines' => 'Max. Anzahl von Zeilen einer Tabelle',
    'cfg_max_lines_hlp' => 'Wievele Zeilen darf eine Tabelle max. lang sein? '
        . 'Auf "false" setzen um ohne limit zu arbeiten. Wenn die max. Anzahl erreicht ist, '
        . 'wird der rest der tabelle in einer Zeile zusammengefasst. Das gilt auch fuer die'
        . '"Gesamte Liste"-Ansicht einer Tabelle.',
    'cfg_hide_empty' => 'Verstecke leere Statistiken',
    'cfg_hide_empty_hlp' => 'Sollen Statistiken mit Null Zeilen versteckt werden? '
        . '(Inklusive des Menu-Eintrags)',
    'cfg_auto_submit_form' => 'Dropdowns nach Änderung ausführen',
    'cfg_auto_submit_form_hlp' => 'Wenn aktiviert haben die Site/datums-dropdowns '
        . 'keinen "OK"-Button. Die Seite wird neu angezeigt sobald sie einen neuen '
        . 'Wert auswählen. Nicht empfohlen für grosse Sites.',
    'cfg_chart_settings' => 'Chart Einstellungen',
    'cfg_max_scale_visitors' => 'Massstab Besucher',
    'cfg_max_scale_visitors_hlp' => 'Der höchste wert des gewählten Datentyps '
        . 'bestimmt den Massstab für die Besucher-Balken im Chart. ',
    'cfg_max_scale_visits' => 'Massstab Besuche',
    'cfg_max_scale_visits_hlp' => 'Der höchste wert des gewählten Datentyps '
        . 'bestimmt den Massstab für die Besuche-Balken im Chart. ',
    'cfg_max_scale_pages' => 'Massstab Seiten',
    'cfg_max_scale_pages_hlp' => 'Der höchste wert des gewählten Datentyps '
        . 'bestimmt den Massstab für die Seiten-Balken im Chart. ',
    'cfg_max_scale_hits' => 'Massstab Zugriffe',
    'cfg_max_scale_hits_hlp' => 'Der höchste wert des gewählten Datentyps '
        . 'bestimmt den Massstab für die Zugriffs-Balken im Chart. ',
    'cfg_max_chart_items' => 'Max Anz. Chart-Reihen',
    'cfg_max_chart_items_hlp' => 'Wenn Charts mit kompletten Listen angezeigt werden, '
        . 'wieviele Reihen sollen angezeigt sein? Der Rest wird in "'.$BAW_MES[2].'" summiert. '
        . 'Dies hilft, überbreite Charts zu vermeiden.',
    'cfg_chart_titles' => 'Chart Titel?',
    'cfg_chart_titles_hlp' => 'Sollen Titel über den Charts angzeigt werden?',
    'cfg_jpgraph_settings' => 'JPGraph Einstellungen',
    'cfg_jpgraph_enable' => 'JPgraph verwenden?',
    'cfg_jpgraph_enable_hlp' => 'Um JPGraph zu verwenden, muss es von'
        . 'http://www.aditus.nu/jpgraph/jpdownload.php heruntergeladne werden',
    'cfg_jpgraph_path' => 'Pfad zu JPGraph',
    'cfg_jpgraph_path_hlp' => 'Wo ist JPGraph installiert? (Der Ordner '
        . "in dem sich jpgraph.php befindet. Kein '/' am Ende.",
    'cfg_table_settings' => 'Tabellen Einstellungen',
    'cfg_advanced_settings' => 'Fortgeschrittene Einstellungen',
    'cfg_version' => 'Version',
    'cfg_version_hlp' => 'Version des Programms',
    'cfg_xhtml' => 'XHTML/ HTML',
    'cfg_xhtml_hlp' => 'Soll HTML oder XHTML angezeigt werden?',
    'cfg_debug' => 'Problemsuche',
    'cfg_debug_hlp' => 'Sollen Status-meldungen zur Fehlersuche engezeigt werden (SEHR detailiert)?',
    'cfg_parser' => 'Daten-Verarbeitungs-Statistiken',
    'cfg_parser_hlp' => 'Wollen Sie eine Zusammenfassung der Logfile-auswertung anzeigen?',
    'cfg_module' => 'Modul-Einstellungen',
    'cfg_module_hlp' => 'Wird BetterAWStats als Modul innerhalb einer anderen Anwendung verwendet? '
        . '(Derzeit nur Drupal)',
    'cfg_type_order' => 'Sortierung',
    'cfg_type_show' => 'Daten anzeigen?',
    'cfg_type_collapse' => 'Minimiert?',
    'cfg_type_table' => 'Tabelle anzeigen?',
    'cfg_type_sort' => 'Sortieren nach...?',
    'cfg_possible_values'=> ' Mögliche Werte sind:',
    'cfg_type_sort_dir' => 'Sortier-richtung?',
    'cfg_type_sort_dir_opts' => 'SORT_ASC=Aufsteigend, SORT_DESC=Absteigend',
    'cfg_type_chart' => 'HTML Chart anzeigen?',
    'cfg_type_map' => 'Landkarte anzeigen?',
    'cfg_type_avg' => 'Durchnitt anzeigen?',
    'cfg_type_total' => 'Summe anzeigen?',
    'cfg_type_top_x' => 'Anzahl Zeilen?',
    'cfg_type_assumebot' => 'Verstecke Besucher wenn Zugriffe = Seiten?',
    'cfg_type_showmonths' => 'Wieviele Monate?',
    'cfg_type_favicon' => 'Favicons für externe URLs anzeigen?',
    'cfg_type_domain_lvls' => 'URLs auf wieviele Domain level kürzen? (-1 = nicht kürzen)',
    'cfg_dis_overview' => 'Übersicht Daten und Zahlen',
    'cfg_dis_months' => 'Monatsdaten',
    'cfg_dis_days' => 'Tagesdaten',
    'cfg_dis_weekdays' => 'Wochentage',
    'cfg_dis_hours' => 'Stunden',
    'cfg_dis_domains' => 'Besucher-Domains',
    'cfg_dis_visitors' => 'Besucher IP-Addressen',
    'cfg_dis_logins' => 'Login-Usernamen',
    'cfg_dis_robots' => 'Suchmaschinen, Spiders, Robots etc.',
    'cfg_dis_worms' => 'Würmer',
    'cfg_dis_sessions' => 'Wie lange waren besucher auf der Seite',
    'cfg_dis_filetype' => 'Auf welche Datei-Typen wurde zugegriffen',
    'cfg_dis_urls' => 'Seiten auf dem Server',
    'cfg_dis_paths_hlp' => 'Daten/Pfade auf dem Server',
    'cfg_dis_paths' => 'Daten/Pfade',
    'cfg_dis_os' => 'Betriebssysteme der Benutzer',
    'cfg_dis_unknownos' => 'Unbekannte Betriebssysteme',
    'cfg_dis_osversions' => 'Betriebssystem inkl. Versionen',
    'cfg_dis_browsers' => 'Browser-Typen',
    'cfg_dis_browserversions' => "Browser-Typen (+{$BAW_MES[58]})",
    'cfg_dis_unknownbrowser' => 'Unbekannte Browser',
    'cfg_dis_unknownbrowser_agent' => 'User Agent',
    'cfg_dis_screensizes' => 'Bildschirmgrössen',
    'cfg_dis_se_referers' => 'Eingehende Links von Suchergebnissen',
    'cfg_dis_referers' => 'Eingehende Links von anderen Seiten',
    'cfg_dis_referer_domains' => 'Eingehende Links, nach zweit-Level Domain sortiert',
    'cfg_dis_hotlinks' => 'Externe Seiten, welche direkt auf Bilder/Daten linken',
    'cfg_dis_hotlink_domains' => 'Domains externer Seiten, welche direkt auf Bilder/Daten linken',
    'cfg_dis_searchphrases' => 'Suchausdrücke',
    'cfg_dis_searchwords' => 'Suchwörter',
    'cfg_dis_misc' => 'Details der Besucher-Systeme',
    'cfg_dis_errors' => 'Fehler-Meldungen von aufgerufenen Seiten',

    // display_helpers
    'created_by' => 'erstellt von %s',
    'show' => 'Anzeigen',
    'hide' => 'Verstecken',
    'all_months' => 'Alle (Monatlich)',
    'all_days' => 'Alle (Täglich)',
    'get_help' => 'Hilfe',
    'back' => 'Zurück',
    'version_check' => 'Suche nach Updates',
    'version_info' => 'Versions-Daten',
    'unknown_list' => 'Unbekannte Daten',

    // reder_table
    'table_max_hits_exceed' => '(Über dem max_hits config Limit von %s)',
    'records'=> '%s records', // new

    // section help
    'hlp_overview' => 'Dieser Abschnitt zeigt generelle Daten des aktuell ausgewählten '
        . 'Monats. Diese Daten schliessen leider nicht exakt alle zugriffe durch '
        . 'Hotlinks, Roboter und Hacker-scripts etc. aus. Wenn man '
        . "\$BAW_CONF['show_parser_stats'] == true setzt, sieht man hier zusätzliche "
        . 'Daten zum logfile Update wie zum Beispiel Parsed records, Old records, New records, '
        . 'Corrupted und Dropped lines.',
    'hlp_months' => 'Dieser Abschnitt zeigt die Monatlichen Daten. Wie weit diese '
        . 'für vergangene Monate angezeigt werden, lässt sich in der Konfiguration einstellen.',
    'hlp_days' => 'Dieser Abschnitt zeigt die täglichen Daten des aktuell ausgewählten Monats. '
        . 'Wenn der gewählte Monat noch nicht zu Ende ist, sieht man hier die Daten des letzten Monats, '
        . 'für die gleiche Anzahl von Tagen wie in diesem Monat noch Daten fehlen.',
    'hlp_weekdays' => 'Dieser Abschnitt zeigt Durschnittswerte für jeden Wochentag. '
        .'Da jeder Monat eine andere Anzahl jedes Wochentags hat, erzeugt eine einfache Summe dieser '
        . 'Daten, so wie es AWStats darstellt, eine Verzerrung der Daten. Darum stellt '
        . 'BetterAWStats die Durschnitte dar.',
    'hlp_hours' => 'Dieser Abschnitt zeigt die Durschnittswerte für jede Stunde des Tages. '
        .'Wenn der aktuell gewählte Monat noch nicht zu Ende ist, verzerrt eine Summe der Zugriffe '
        . 'So wie es von AWStats dargestellt wird, die Daten. Darum zeigt BetterAWStats die '
        . 'Durschnitte.',
    'hlp_domains' => 'Dieser Abschnitt zeigt die Zugriffe pro Land (sofern bekannt). '
        . 'Die Landkarte summiert die Domains mil, edu, gov and arpa zur "us"-Domain dazu.',
    'hlp_visitors' => 'Dieser Abschnitt zeigt die Daten pro User-IP Addresse. '
        . 'Da praktisch alle Webseiten aus mehreren Dateien bestehen (Bilder, CSS-file etc), '
        . 'sind zugriffe mit einem sehr tiefen "Zugriff pro Seiten"-Verhältnis höchstwahrscheinlich'
        . 'von Robotern, Scripten oder anderen Programmen aber nicht von normalen Benutzern '
        . 'erzeugt. Diese Zugriffe sind hier unter "Vermuteten Scripts & Bots zusammengefasst. '
        . 'Um das Verhältnis "Zugriff pro Seiten" welches eine Script definiert zu ändern, '
        . 'justieret man den Wert von $BAW_CONF_DIS[\'visitors\'][\'assumebot\'] in der Konfiguration.'
        . 'Ähnlich sind Zugriffe ohne Seitenaufrufe von Proxies or '
        . 'Hotlinks. Diese sind unter "Hotlinks/Proxies" summiert.' ,
    'hlp_logins' => 'Dieser Abschnitt zeigt die Passwort-geschützten zugriffe pro Username. Ein Username'
        . 'mit nur einem Zugriff stellt höchstwahrscheinlich einen Zugriffsversuch ohne Passwort dar.',
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