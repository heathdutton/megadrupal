<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  config, Konfigurationsdaten fuer netpay
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 11.07.2005
   History:       18.11.2003 - Initial version
                  22.11.2003 - Fixed a typing error
		  04.03.2004 - Complete update for eps v2 Standard
		  31.03.2004 - Bugfixing and testing complete.
		  01.04.2004 - Final netpayV2 release
		  07.04.2004 - Added $netpay_errorcode Variable, a few bugfixes
		  01.10.2004 - $homepage, $transactionokurl moved to DoV2Order call;
			       Former "Mandatory PArameters" must now be valid and
			       are used by EPS2.
		  11.07.2005 - sapPopRequestor, HaendlerPIN, BfiBicIdentifier, 
			       BeneficiaryAccountIdentifier
			       now moved to banks.inc.php as the code now supports
			       multiple banks! The parameters in here will be IGNORED!
   Update:	  Um von der Vorversion dieser Datei aufzudatieren,
		  verwenden Sie bitte diese Datei als Skelett und
		  uebertragen Sie die Dateien der alten Konfigurationsdatei
		  sorgfaeltig. 
		  Bitte beachten Sie die "MOVED" Hinweise !

   Dieses Modul ist von Ihnen anzupassen! Es beinhaltet die globalen
   Einstellungen fuer netpay! Bitte beachten Sie die Datei banks.inc.php, die
   ab sofort ebenfalls anzupassen ist.
*/

// GENERAL PARAMETERS

    // Geben Sie hier den Pfad und Dateinamen der Datei an, die Ihre XML-Signatur
    // beinhaltet, falls Sie wuenschen, diese statt der MD5-Summe zur
    // Authentifizierung zu verwenden. Sonst lassen Sie diese Variable kommentiert
    // $sigfile="my_signature.xml";

    // Geben Sie hier ihre HaendlerID an
    // $sapPopRequestor="00088_501866818";
// --> MOVED to banks.inc.php ! <--

    // Und hier bitte den PIN-code
    // $HaendlerPIN="WFuFmCnHrJ";
// --> MOVED to banks.inc.php ! <--
	
    // Rueckmelde-URL, auf die der Bankserver hinschreiben soll
    $confirmationurl="http://".$_SERVER['HTTP_HOST'].dirname($_SERVER["REQUEST_URI"]);
    if (dirname($_SERVER["REQUEST_URI"])!="/") $confirmationurl.="/";
    $confirmationurl.="backref.php";

    // Kommentieren Sie folgenderZeile aus, wenn Sie
    // debugging ausschalten moechten. Standardmaessig wird in die Datei
    // log.txt protokolliert
    $netpay_debug=1;

    // Wird das Testsystem oder bereits die Produktionsumgebung genutzt?
    // ACHTUNG: Auskommentieren bedeutet Produktionsumgebung!
    $UseTestSystem=1;
    
    // Soll im Fehlerfall die Meldung direkt ausgegeben werden (mittels print())
    // oder nicht?
    // Kommentieren Sie diese Zeile aus, wenn Sie eventuelle Fehler selbst
    // mittels der $netpay_errorcode Variable abfangen und gesondert
    // anzeigen moechten.
    // Zum Debuggen empfiehlt sich, diese Einstellung aktiviert zu lassen.
    $ShowErrorMsg=1;

    // ISO 9362 Bank Identifier Code der Bank des Beguenstigten
    // $BfiBicIdentifier="GIBAATW0";

    // -- Identifikation des Beguenstigten
    // Name und Adresse des Beguenstigten
    $BeneficiaryNameAddressText="Max Mustermann; 1230 Wien";
    // - ODER -
    // Identifikation des Beguenstigten durch BEI (Business Entity Identifier)
    // Achtung: Muss genau 11 Zeichen lang sein!
    // $BeneficiaryBeiIdentifier="XXNOTUSEDXX";
    
    // IBAN (International Bank Account Number) des Beguenstigten
    // Um die IBAN zu Ihrer Kontonummer zu erhalten, koennen Sie den IBAN-
    // Generator unter http://www.easy-web.de/iban/ibangenerator.htm
    // verwenden.
    // $BeneficiaryAccountIdentifier="AT092011100000637777";    

// GLOBAL OPTIONAL PARAMETERS

/* Entfernen Sie die Kommentarzeichen am Anfang der jeweiligen Variablen und
   fuellen Sie den gewuenschten Wert ein, wenn Sie die optionalen Parameter
   nutzen moechten
   Anmerkung: Diese Variablen koennen auch pro Bestellung unterschiedlich gesetzt
   werden, was Sie hier definieren wird der globale Standardwert bei jenen
   Aufrufen, bei denen die hier genannten Parameter nicht explizit uebergeben
   werden.
    Sie koennen natuerlich auch weitere optionale Variablen hier global
   definieren, wenn Sie dies wuenschen. Naehere Informationen finden Sie in der
   technischen Beschreibung des eps 
   http://service.stuzza.at/EPS/Release2/PFLICHTENHEFT/FINAL%20V2-1/Pflichtenheft%20eps%20e-payment%20standard%20V2-1.pdf
*/

    // Variable zur Bekanntgabe einer URL, z.B. Ihrer Heimseite (epi:Url)
    // $Url="http://eps.matschi.com/xml/v2/index.html";
    
    // Variable zur Bekanntgabe einer e-mail Adresse (epi:EmailAddressIdentifier)
    // $EmailAddressIdentifier="ludwig.ertl@softwerker.at";
    
    // Gibt an, ob eine garantierte Zahlung erwuenscht ist, oder nicht.
    // ACHTUNG: DIESES FELD WIRD IN DERZEITIGEN ZAHLUNGSSYSTEMEN NICHT UNTERSTUETZT
    // Sie koennen dieses Feld auch pro Zahlung entsprechend setzen.
    // $Realization="GAR";

?>