<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  banks, Definition der bankspezifischen Parameter
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 03.06.2007
   History:       11.07.2005 - Initial version
		  03.06.2007 - ARZ Banken separat hinzugefuegt
		  30.09.2008 - Komplette Ueberarbeitung, anpassung an neue
			       Banken und Reihenfolge. Neue funktion ListBanks()
		  01.04.2012 - Ab sofort nur noch ein SO User

   Dieses Modul ist von Ihnen anzupassen! Es beinhaltet die bankspezifischen
   Parameter, die eingestellt werden muessen!

   Hinweise
   --------
   Um die IBAN zu Ihrer Kontonummer zu erhalten, koennen Sie den IBAN-
   Generator unter http://www.easy-web.de/iban/ibangenerator.htm
   verwenden.
*/

include("netpay_config.inc.php");

/* 
 * Zugangsdaten fuer Banken
 */
$logins=array (
    "ARZ_SO"
    => array (
        "enabled"			=> true,		  // Wird die Bank von Ihrem Shop unterstuetzt?
        "sapPopRequestor"		=> $UseTestSystem?
					   "GIBAATWWXXX_103533":	//   - Testsystem
					   "",			  	//   - Produktion
        "HaendlerPIN"			=> $UseTestSystem?
					   "AA9191D80773B41E":		//   - Testsystem
        				   "",				//   - Produktion
        "BfiBicIdentifier"	   	=> $UseTestSystem?	  // ISO 9362 Bank Identifier Code der Bank:
					   "GIBAATW0":			//   - Testsystem \
					   "",			  	//   - Produktion (leer=globale Einst. verw.)
        "BeneficiaryAccountIdentifier" 	=> $UseTestSystem?	  // IBAN (International Bank Account Number):
					   "AT212099900000637777":			//   - Testsystem \
					   ""			  	//   - Produktion (leer=globale Einst. verw.)
    )
);

include ('bankurls.inc.php');

?>
