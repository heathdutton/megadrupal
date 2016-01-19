<html><head><title>Testshop</title></head><body>

<!--
   Netpay-Democode fuer XML-Interface
   Modul:	  shop, Demonstration fuer die Nutzung der netpay-Funktionen
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 11.07.2005
   History:       18.11.2003 - Initial version
                  22.11.2003 - Fixed a typing error		
		  31.03.2004 - Update auf neue DoV2Order Funktion, Ausgliederung der
			       Zahlungsfunktion in order.php, um dem Wunsch der Zahlung
			       in einem separaten Fenster gerecht zu werden.
		  11.07.2005 - Nun shop.php, statt shop.html, da auch eine Funktion
			       zur Auswahl der Bank implementiert wurde.
		  28.02.2006 - Der submit-Button oeffnet nun zusaetzlich ueber Javascript
		               das Zielfenster, sodass es keine Probleme mit der Fenster-
			       Verwaltung mehr geben sollte.

   Simpler 0815-Webshop, der einfach die Moeglichkeiten unserer netpay-Funktion
   demonstrieren soll. Hier wird ein Beispielartikel definiert
   (normalerweise kommen die natuerlich aus der Datenbank) und dann zum Verkauf
   angeboten.
   Dies soll nur die Funktionsweise des Webshops demonstrieren.

Warenkorb:
-->
<?
    include("netpay_functions.inc.php");
    
    $preis="0.90";
    $gutschrift="-0.10";
    $bezeichnung="Pseudoartikel";
    
?>    
<!-- <form action="order.php" target="pay"> 
Kein Targetwindow mehr verwenden bei SOE
-->
<form action="order.php"> 
 <table>
  <tr><td style="background-color:lightgrey;">Bezeichnung</td><td style="background-color:lightyellow; font-weight:bold;"><?=$bezeichnung?></td></tr>
  <tr><td style="background-color:lightgrey;">Preis</td><td style="background-color:lightyellow;"><input type="text" name="preis" size=6 value="<?=$preis?>"></td></tr>
<!--  <tr><td style="background-color:lightgrey;">Gutschrift</td><td style="background-color:lightyellow;"><input type="text" name="gutschrift" size=6 value="<?=$gutschrift?>"></td></tr> -->
  <tr><td style="background-color:lightgrey;">Zusatz</td><td style="background-color:lightyellow;">Das perfekte Produkt</td></tr>
  <tr><td style="background-color:lightgrey;">Durchfuehrungsdatum:</td><td style="background-color:lightyellow;"><input type=text size=2 maxlength=2 name="Tag">.<input type=text size=2 maxlength=2 name="Monat">.<input type=text size=4 maxlength=4 name="Jahr" value="<?=date("Y")?>"></td></tr>
  <tr><td style="background-color:lightgrey;">Verwendungszweck:</td><td style="background-color:lightyellow;"><input type=text size=23 maxlength=23 name="Zweck" value="<?=date('Y-m-d H:i:s')?>"></td></tr>
  <tr><td style="background-color:lightgrey;">Targetwindow testen:</td><td style="background-color:lightyellow;"><input type=checkbox name="targetwnd" value="pay1"></td></tr>
  <tr><td style="background-color:lightgrey;">Bank</td><td style="background-color:lightyellow;">
    <select name="Bank">
    <?ListBanks();?>
    </select>
  </td></tr>
 </table>
 <input type=hidden name="zweck" value="<?=$bezeichnung?>">
 <input type=hidden name="bestell_nr" value="12353429432">
 <input type=submit value="Kaufen!">
</form>
</body></html>