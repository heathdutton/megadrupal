<html>
<head>
<title>Oops.. Something went wrong...</title>
</head>
<body>
<?
/* Netpay-Democode fuer XML-Interface
   Modul:         shop_error, Demonstration fuer die Fehlerauswertung
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 11.07.2005
   History:       01.10.2004 - Initial Version
		  11.07.2005 - BasketUrl now shop.php, not shop.html

   Simpler 0815-Webshop, der einfach die Moeglichkeiten unserer netpay-Funktion
   demonstrieren soll. Hier wird ein Beispielartikel definiert
   (normalerweise kommen die natuerlich aus der Datenbank) und dann zum Verkauf
   angeboten.
   Dies soll nur die Funktionsweise des Webshops demonstrieren.
   In der Praxis ist der Code natuerlich je nach Webshop um einiges komplexer.


 Folgende Werte wurden frueher der DoV2Order - Funktion direkt uebergeben.
 Nun sind diese Werte hierher verschoben worden

 BasketUrl:          Die URL des Einkaufswagens beim Haendler, um dem  
                     Benutzer die Moeglichkeit zu geben, nach dem      
                     Abbruch der Zahlung weitere Produkte zu kaufen.   
                                                          [xs:anyURI]  
 PaymentModeUrl:     Die URL jener Seite des Webshops, auf welcher     
                     der Benutzer nach Abbruch der Zahlung wieder      
                     zwischen den verschiedenen Zahlungsmoeglichkeiten 
                     waehlen kann,                        [xs:anyURI]  
*/

$basepath="http://".$_SERVER['HTTP_HOST'].dirname($_SERVER["REQUEST_URI"]);
if (dirname($_SERVER["REQUEST_URI"])!="/") $basepath.="/";
$BasketUrl=$basepath."shop.php";
$PaymentModeUrl=$BasketUrl;

print "Ihre Bestellung konnte leider nicht durchgefuehrt werden.<br>\n";

switch (strtoupper($_REQUEST['epserrorcode'])) {
    case "ERROR1": 
	print "Der H&auml;ndlerserver konnte bei der &Uuml;bermittlung der Zahlungsbest&auml;tigung nicht erreicht werden.<br>";
	print "Versuchen Sie es evtl. etwas sp&auml;ter nochmals.";
	break;
    case "ERROR2":
	print "Der XML-Strom war ung&uuml;ltig oder die Signatur defekt. Bitte setzen Sie den H&auml;ndler davon in Kenntnis.";
	break;
    case "ERROR3":
	print "Sie haben die Zahlung abgebrochen. Ihre Bestellung wurde nicht durchgef&uuml;hrt!";
	print "<p><a href=\"index.html\">Zur Homepage>></a></p>";
	break;
    case "ERROR4":
	print "<A HREF=\"$BasketUrl\">Weiter zum Einkaufswagen</A><META HTTP-EQUIV=Refresh CONTENT=\"0; URL=$BasketUrl\">";
	break;
    case "ERROR5":
	print "<A HREF=\"$PaymentModeUrl\">Weiter zur Auswahl der Zahlungsm&ouml;glichkeiten</A><META HTTP-EQUIV=Refresh CONTENT=\"0; URL=$PaymentModeUrl\">";
	break;
    default:
	print "Unbekannter Fehler. M&ouml;glicherweise gibt es einen neuen Fehlercode, der in dieser Version des Codes noch nicht behandelt wird.<br>";
	print "Setzen Sie sich mit dem H&auml;ndler in verbindung, eventuell steht eine neue Democode-Version unter ";
	print "<A HREF=\"http://netpay.0wnz.at\">http://eps.matschi.com</A> zur Verf&uuml;gung.<br>Der Fehlercode war: ".$_REQUEST['epserrorcode'];
}
?>
</body></html>