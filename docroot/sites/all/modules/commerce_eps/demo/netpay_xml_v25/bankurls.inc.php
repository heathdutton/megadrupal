<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  bankurls, Definition der bankspezifischen Logon-URLs
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 01.06.2009
   History:	  30.09.2008 - Komplette Ueberarbeitung, anpassung an neue
			       Banken und Reihenfolge. Neue funktion ListBanks()
		  25.11.2011 - Anpassung an Scheme Operator. EPS1 entfernt.
		  01.04.2012 - Ab sofort nur noch eine SO URL
                  15.08.2014 - Anpassung an EPS 2.5, ERSTE-URL eliminiert.
*/

/*
 * LOGIN-URLs der einzelnen Rechenzentren
 */
$urls=array(
    "EPSSO"				=> $UseTestSystem?
					   "https://routing.eps.or.at/appl/epsSO-test/transinit/eps/v2_5":
					   "https://routing.eps.or.at/appl/epsSO/transinit/eps/v2_5",
);

/*
 * Zur Auswahl stehende Banken
 */
$banks=array(
    // Standardmaessig kann man den SO die Bankenauswahl treffen lassen
    "SO" 				// Name der Bank
    => array (
	"name"				=> "Testhaendler - SO mit Bankenauswahl",
	"group"				=> 2,
	"account"			=> $logins['ARZ_SO'],	// Siehe banks.inc.php fuer HTTPS-Specials...
	"Login"				=> $urls['EPSSO']
    ), 

    // Hier 2 weitere Beispiele, wie man stattdessen die Bank direkt waehlen kann:
/*
    "SPARDAT_SO" 				// Name der Bank
    => array (
	"name"				=> "Testhaendler - Erste Bank und Sparkassen via SO",
	"group"				=> 2,
	"account"			=> $logins['ARZ_SO'],
	"Login"				=> $urls['EPSSO'].'/3fdc41fc-3d3d-4ee3-a1fe-cd79cfd58ea3'
    ), 

    "SPARDAT_SOBIC" 				// Name der Bank
    => array (
	"name"				=> "Testhaendler - Erste Bank und Sparkassen via SO mit BIC selector",	// Siehe order.php fuer Setzen des BIC
	"group"				=> 2,
	"account"			=> $logins['ARZ_SO'],
	"Login"				=> $urls['EPSSO']
    )
*/
);

?>
