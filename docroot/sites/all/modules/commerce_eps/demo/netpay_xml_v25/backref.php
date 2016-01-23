<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  backref, Auswertung der Netpay-Rueckgabedaten
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 17.04.2004
   History:       18.11.2003 - Initial version
                  07.03.2004 - Complete update for eps v2 Standard
		  23.03.2004 - Bugfixing and testing complete.
		  17.04.2004 - Bugfix for PaymentReferenceIdentifier
		  11.02.2005 - Bugfix for '?' characters in SessID
		  02.01.2006 - Bugfix: In backref_characterdata nicht behandelte Parameter, wie z.B. REMITTANCEIDENTIFIER
			       wurden nicht global abgelegt und daher nicht gespeichert. -> Wandern nun in $GLOBALS array.
		  08.05.2007 - PHP 5.2.2 bugfix hinzugefuegt.
		  30.11.2011 - Nun EPSHeader() Funktion fuer gemeinsamen XML-Kopf

   Dieses Modul wird vom Netpay-server aufgerufen, welcher die Ueberweisung
   damit bestaetigt
   Das Modul ist von Ihnen entsprechend anzupassen (siehe unten)
*/

include("netpay_functions.inc.php");

// PHP 5.2.2 bugfix
if (!isset($HTTP_RAW_POST_DATA)) $HTTP_RAW_POST_DATA = file_get_contents('php://input');

$xmltext = $GLOBALS["HTTP_RAW_POST_DATA"]; 	// String vom Server
if (!$xmltext)  // wenn raw nichts existiert, dann einen normalen Post einlesen
{
  while(list($key, $val) = each($HTTP_POST_VARS))
  {
   $key = stripslashes($key);
   $val = stripslashes($val);
   $key = rawurldecode($key);

 // bloederweise wird bei HTTP_POST_VARS aus <?xml version  xml_version weil Leerzeichen konvertiert werden => leerzeichen wiederherstellen
   if (($i=strpos($key, 'xml_',0))> 0) $key=str_replace('xml_', 'xml ', $key);
   $val = rawurldecode($val);
   $xmltext.= "$key=$val";
  }
}

$xmltext=utf8_decode($xmltext);
PutLog("Antwort des Bankrechners:\n$xmltext");

// Parser initialisieren zum Parsen, handler initialisieren
unset($currentTag);
$xml_parser = xml_parser_create();
xml_set_element_handler($xml_parser, "backref_startElement", "backref_endElement");
xml_set_character_data_handler($xml_parser, "backref_characterData");
$antwort=EPSHeader()."<epsp:ShopResponseDetails>";

// Parsen
if (!xml_parse($xml_parser, $xmltext)) {
    $netpay_errorcode="012";
    PutLog(sprintf("ERR:XML Fehler: %s in Zeile %d",xml_error_string(xml_get_error_code($xml_parser)),xml_get_current_line_number($xml_parser)));
}
if (isset($vitality)) $antwort=$xmltext; else $antwort.="</eps:ShopConfirmationDetails></epsp:ShopResponseDetails></epsp:EpsProtocolDetails>";

PutLog("Antwort an den Bankserver:\n$antwort");
echo utf8_encode($antwort);
xml_parser_free($xml_parser);

// Parse-Funktionen



/* Funktion: backref_characterData
   Zweck:    Parsed die Texte der XML-Elemente und wertet diese entsprechend aus
   Anmerkung:Sie koennen auch die anderen in der Nachricht uebermittelten Parameter zur
	     Identifikation des Geschaeftsvorgangs heranziehen. Alle nicht behandelten
	     Parameter werden im Array der globalen Variablen gross geschrieben
	     abgelegt. Der Zugriff daraif erfolgt ueber die $GLOBALS Variable.
	     Standardmaessig verwendet man aber $GLOBALS['REMITTANCEIDENTIFIER']
*/
function backref_characterData($parser, $data) {
    global $currentTag, $antwort, $prefid;

    switch (strtoupper($currentTag)) {
	case "SESSIONID": $antwort.=$data; break;
	case "STATUSCODE":
	    PutLog("Es wurde eine Confirmation zum Geschaeftsvorgang ".$GLOBALS['REMITTANCEIDENTIFIER']." geschickt:");
	    switch (strtoupper($data)) {
		case 'OK':	// --- es war eine Confirmation fuer eine positiv abgeschlossene Transaktion 
    		    PutLog("BEZAHLUNG OK!");

    		    // Alles OK, hier Code einfuegen, um dies entsprechend zu behandeln
		    // z.B. in Datenbank als Bezahlt eintragen oder was auch immer
		    
		    break;
		case 'VOK': 	// --- es war eine Confirmation mit vorlaeufig OK
		    PutLog("BEZAHLUNG VORLAEUFIG OK, Order ueberprueft!");

		    // Vorlaeufig OK, hier Code einfuegen, um dies entsprechend zu behandeln
		    // z.B. in der eigenen Datenbank die Bestellung speichern, nochmals mit
		    // HAENDLERTOOL checken, Auslieferung kann erst dann beginnen 

		    break;
		case 'ERROR':	// --- Technischer Fehler
		    PutLog("ERR: Technischer Fehler aufgetreten!");
		    
		    // Code einfuegen, der bei einem technischen Fehler ausgefuehrt werden soll.
		    // Falls dies derselbe ist wie bei NOK, dann kommentieren Sie bitte einfach
		    // das folgende break; aus
		    
		    break;
	        case 'NOK':	// --- es war eine confirmation mit Nicht OK
		    PutLog("NICHT OK, Bezahlung der Order fehlgeschlagen!");

		    // Nicht OK, hier Code einfuegen, um dies entsprechend zu behandeln
		    // z.B. in der eigenen Datenbank die Bestellung als ABGEBROCHEN vermerken
		    // KEINE Auslieferung !

		    break;
		default:	// --- Unbekannt
		    PutLog("ERR: Unbekannter StatusCode! Setzen Sie Sich ggf. mit Ihrer Bank zwecks eines Updates in Verbindung");
	    }
	    $antwort.="<eps:StatusCode>$data</eps:StatusCode>$prefid";
	    break;
	case "PAYMENTREFERENCEIDENTIFIER":
	    // Sinnigerweise muss man StatusCode und PaymentReferenceIdentifier genau
	    // in der entgegengesetzten Reihenfolge schicken, als in welcher man sie
	    // erhaelt. Daher ist die extra-Variable hier notwendig.
	    $prefid.=$data;
	    break;
	default:
	    $GLOBALS[$currentTag]=$data;
    }
}

/* Funktion: backref_startElement
   Zweck:    Parsed die XML-Startelemente und wertet diese entsprechend aus
*/
function backref_startElement($parser, $name, $attrs) { 
    global $currentTag, $vitality, $prefid, $antwort;

    list($prefix, $currentTag)=explode(":", $name, 2);
    if (!$currentTag) $currentTag=$name;
    switch (strtoupper($currentTag)) {
	case "SESSIONID": $antwort.="<epsp:SessionId>"; break;
	case "VITALITYCHECKDETAILS": $vitality=1; break;
	case "PAYMENTREFERENCEIDENTIFIER": $prefid="<eps:PaymentReferenceIdentifier>"; break;
    }
}

/* Funktion: backref_endElement
   Zweck:    Parsed die XML-Endelemente und wertet diese entsprechend aus
*/
function backref_endElement($parser, $name) { 
    global $prefid, $antwort, $currentTag;
    
    switch (strtoupper($currentTag)) {
	case "PAYMENTREFERENCEIDENTIFIER": $prefid.="</eps:PaymentReferenceIdentifier>"; break;
	case "SESSIONID": $antwort.="</epsp:SessionId><eps:ShopConfirmationDetails>";
    }
    $currentTag="";
}

?>