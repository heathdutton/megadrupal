<html><head>
  <title>Testshop</title>
  <!-- EPS 2.4 funktioniert nicht, wenn Fenster einen Namen haben -->
  <script language="Javascript">if (window.name) window.name='';</script>
</head><body>
<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  order, Demonstration fuer die Nutzung der netpay-Funktionen
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 11.07.2005
   History:       31.03.2004 - Update auf neue DoV2Order Funktion, Ausgliederung der
			       Zahlungsfunktion in order.php, um dem Wunsch der Zahlung
			       in einem separaten Fenster gerecht zu werden.
		  30.08.2004 - Bugfix, sodass Monat, Tag und Jahr auch bei
			       register_globals = off richtig geholt werden.
		  01.10.2004 - Adaptierung auf neue DoV2Order-Funktion
		  11.07.2005 - Unterstuetzung fuer andere Banken implementiert
		  01.04.2012 - Anpassung an Scheme Operator, Demo fuer BIC selector

   Simpler 0815-Webshop, der einfach die Moeglichkeiten unserer netpay-Funktion
   demonstrieren soll. Hier wird ein Beispielartikel definiert
   (normalerweise kommen die natuerlich aus der Datenbank) und dann zum Verkauf
   angeboten.
   Dies soll nur die Funktionsweise des Webshops demonstrieren. 
   In der Praxis ist der Code natuerlich je nach Webshop um einiges komplexer.
*/

  include("netpay_functions.inc.php");
  // Anhängen von optionalen Parametern, die dann in der backref.php ausgewertet werden koennen
  // wurde wie folgt funktionieren:
  // Wollen Sie weitere PArameter anhaengen, muessen Sie das & als &amp; codieren
  // $confirmationurl.="?bestellnr=".$_REQUEST['bestell_nr'];
  
  $basepath="http://".$_SERVER['HTTP_HOST'].dirname($_SERVER["REQUEST_URI"]);
  if (dirname($_SERVER["REQUEST_URI"])!="/") $basepath.="/";
  $params=array();
//  $params=array("DigSig"=>"SIG");
  if ($_REQUEST['preis']<=0.20) cry("099", "ERR: Der Preis ist zu niedrig, Mindestpreis ist 21 cent.");
  if ($_REQUEST['Tag'] && $_REQUEST['Monat'] && $_REQUEST['Jahr']) {
    $Tag=sprintf("%02u", $_REQUEST['Tag']);
    $Monat=sprintf("%02u", $_REQUEST['Monat']);
    $Jahr=$_REQUEST['Jahr'];
    PutLog("Verwende Durchfuehrungsdatum $Jahr-$Monat-$Tag");
    if ($Tag>31 || $Monat>12 || $Jahr<2004) cry("011", "ERR: Ungueltiges Datum!");
    else $params=array("OptionDate"=>"$Jahr-$Monat-$Tag", "DateSpecificationCode"=>"DBD");
  } else PutLog("INFO: Kein Durchfuehrungsdatum angegeben");
  if ($_REQUEST['Bank']) $params=array_merge($params, array("Bank" => $_REQUEST['Bank']));
  if ($_REQUEST['targetwnd']) $params=array_merge($params, array("TargetWindow" => $_REQUEST['targetwnd']));
  $items = array(array($_REQUEST['zweck'], 1, $_REQUEST['preis']));
  if ($_REQUEST['gutschrift']) array_push($items, array('Gutschrift', 1, $_REQUEST['gutschrift']));
  $endsumme=0;
  foreach ($items as $i) $endsumme+=$i[2];
  $endsumme = sprintf ("%.2f", $endsumme);
  $params = array_merge($params, array('PaymentInstructionIdentifier' => '12345'));

  /* Hier ein Beispiel, wie man die Bank ueber den BIC selector waehlen koennte (siehe auch  bankurls.inc.php) */
  if ($_REQUEST['Bank']=='SPARDAT_SOBIC')
    $params['OrderingCustomerOfiIdentifier'] = 'GIBAATWWXXX';
  DoV2Order("", "000008150815".($_REQUEST['Zweck']?$_REQUEST['Zweck']:"Ein Verwendungszweck"), $endsumme, $basepath."thanks.html", $basepath."shop_error.php", $items, $params);
  die($netpay_errorcode);
?> 
</body></html>
