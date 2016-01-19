<?
/* Netpay-Democode fuer XML-Interface
   Modul:	  functions, beinhaltet gemeinsam genutzte Funktionen
   Author:        Ludwig Ertl  (ludwig.ertl@softwerker.at)
   Last modified: 23.10.2008
   History:       18.11.2003 - Initial version
                  22.11.2003 - Fixed a few typing errors, added new URLs
		  04.03.2004 - Complete update for eps v2 Standard
                  31.03.2004 - Bugfixing and testing complete.
                  01.04.2004 - Final netpayV2 release
		  07.04.2004 - Added $netpay_errorcode Variable, a few bugfixes
		  30.06.2004 - XML-Header fix
		  01.10.2004 - Several FUNDAMENTAL changes, please be sure to read
			       UPDATE_eps2
		  25.01.2005 - Fixed a bug with redirect-URLs containing &amp;
		  02.03.2005 - Added support for DigSig parameter (digital signature)		  
		  03.05.2005 - Hash is now calculated from UTF8-ENCODED strings.
		  11.07.2005 - Optional parameter "Bank" added for Multibank-compatibility
			       Added a redirect-URL fix for RAIKA (; in Redirect-URL in IE = bad)
		  02.01.2006 - Fixed documentation for ReferenceIdentifier.
		  08.06.2007 - Optionaler Parameter TargetWindow hinzugefuegt.
		  23.10.2008 - Anpassung an neue Bankdefinitionsdatei banks.inc.php
		  30.11.2011 - Anpassung Namespaces an EPS 2.4

   Dieses Modul braucht im Regelfall nicht angepasst zu werden. Sehen Sie
   bitte weiter unten fuer den genauen Funktionsaufruf der DoV2Order Funktion,
   welche Sie dann in Ihren webshop integrieren koennen.
   Zur Nutzung der netpay-Funktionen ist dieses Modul via include in Ihren
   bestehenden Webshop einzubinden.
*/


// Variablen initialisieren

    $EPS_SYS='EPS2';
    include("banks.inc.php");

    $logfile="log.txt";	// Wenn debugging aktiviert ist wird hier rein gelogged

// Pruefen der Variablen

    if (isset($homepage) || isset($transactionokurl)) cry("110", "ACHTUNG: Sie haben Ihren Democode geupdatet, aber die Konfigurationsdateien nicht entsprecehnd angepasst. Bitte lesen Sie UPDATE_eps2!");
    // Compatibility hacks
    if (isset($sapPopRequestor)) $banks['SPARDAT']['account']['sapPopRequestor']=$sapPopRequestor;
    if (isset($HaendlerPIN)) $banks['SPARDAT']['account']['HaendlerPIN']=$HaendlerPIN;
			       
// Funktionen

    /* Funktion: DoV2Order
       Zweck:	 Sendet eine Bestellung an den Netpay Server gemaess neuem eps-Protokoll
       Anmerkung:Keine Fehlerpruefung, ob die uebergebenen Parameter gueltig sind.
    		 Gueltigkeit siehe Datentypen in eckigen Klammern unten, bzw. eps Beschr.
       Parameter:ReferenceIdentifier:	Referenz der Zahlungsaufforderungsnachricht, z.B.
    					fuer Nachforschungszwecke beim Haendler [an..35]
					Dieser Wert wird derzeit nicht beruecksichtigt
					und nur dem Kunden als Auftragsnummer im EPS2
					angezeigt.
					Sie können hier daher auch einen Leerwert einsetzen.
		 RemittanceIdentifier:	Verwendungszweck. Eindeutige Referenz des Haendlers
					zu einem Geschaeftsfall. 		[an..35]
					Die ersten 12 Stellen muessen numerisch sein und
					stehen im Kundendatenfeld, Rest des Feldes 
					[an..23] Verwendungszweck
                    Wenn die Laenge 35 Zeichen ueberschreitet, dann wird dieser
                    als UnstructuredRemittanceIdentifier geschickt:
                    [an..140]
		 InstructedAmount:	Gesamtsumme der vom Kunden gekauften Waren [dec]
					Zur Trennung von Nachkommastellen wird ein Punkt
					verwendet, kein Komma.
		 TransacionOkUrl	Wurde die Transaktion erfolgreich durchgefuehrt   
		 			und wurde auch eine Bestaetigung gesendet, wird   
		 			der Kunde an diese URL weitergeleitet.		  
		 TransactionNokUrl	Sollte Ihre Transaktion nicht erfolgreich 	  
		 			durchgefuehrt worden sein, wird der Kunde nach 	  
		 			Erhalt der jeweiligen Fehlermeldung an diese URL  
    		 			weitergeleitet (ehemals: $homepage)		  
		 			Bitte beachten Sie die Neuerungen zu den Fehler-  
		 			codes im EPS-Pflichtenheft Seite 52:		  
		 			Der TransactionNokUrl wird ein Parameter 	  
		 			epserrorcode uebergeben, der verschiedene	  
		 			Fehlergruende identifiziert.			  
		 articles:		ein Array von Artikeln, die wiederum aus einem
					Array bestehen, das sich genau aus den 3
		    			Komponenten Name, Anzahl und Preis zusammensetzt
		        		z.B.: array(array('Netzwerkkarte', 1, '10.90'),
					array('Thinkgeek T-Shirt', 2, '15.10'))
					Zur Trennung von Nachkommastellen wird ein Punkt
					verwendet, kein Komma.				
		 opt:			Optionale Parameter in einem assoziativen Array.
					Moegliche Optionale Parameter:
					 Url, EmailAddressIdentifier, OrderInfoText, 
					 OrderingCustomerOfiIdentifier, 
					 OrderingCustomerIdentifier, 
					 OrderingCustomerNameAddressText
					 PaymentInstructionIdentifier,
					 OptionDate, DateSpecificationCode
					Derzeit nicht unterstuetzte optionale Parameter:
					 InstructionCode, OptionTime
					 Realization, PaymentDescription,
					 TransactionTypeCode, InstructionCode,
					 TargetWindow, DigSig, ExpirationTime
					Fuer eine Beschreibung dieser Parameter sei auf
					die technische Beschreibung des eps-Standards
					verwiesen. (service.stuzza.at)
					NEU ab 11.07.2005: Parameter "Bank" setzt
					 die fuer die Ueberweisung zu verwendende Bank,
					 beispiesweise "BAWAG" (Name siehe banks.inc.php)
					z.B.: array("Bank"=>"Volksbank", "Url"=>"http://netpay.0wnz.at/shop/", "EmailAddressIdentifier"=>"ludwig.ertl@softwerker.at")
       Rueckgabe:Die Funktion gibt die Antwort des Servers zurueck
       Fehlerbehandlung: Die globale Variable $netpay_errorcode enthaelt im Fehlerfall einen Fehlercode:
    			 000     -> Kein Fehler
                         001-011 -> EPS2-Fehlercodes lt. Dokumentation
			 110     -> Konfigurationsfehler (config-Dateien pruefen)
			 111     -> Fehler bei den uebergebenen optionalen Parametern
			 112     -> Fehler im erhaltenen XML-Stream
                         113     -> Ungueltiges OptionDate uebergeben
			 114	 -> Fehler bei den uebergebenen fix-Parametern
			 Die genauen Fehlerbeschreibungen stehen bei aktiviertem Logging im log
			 
       Beispiel: DoV2Order("123", "AT1234XYZ", "26.00", "http://my.shop.at/thanks.php?id=0815", "http://my.shop.at/npy_error.php?id=0815", array(array('Netzwerkkarte', 1, '10.90'), array('Thinkgeek T-Shirt', 2, '15.10')));
    */
// Obsolet ab Spezifikation vom 19072004
//    function DoV2Order($ReferenceIdentifier, $RemittanceIdentifier, $InstructedAmount, $BasketUrl, $PaymentModeUrl, $articles, $opt=array()) {
    function DoV2Order($ReferenceIdentifier, $RemittanceIdentifier, $InstructedAmount, $transactionokurl, $TransactionNokUrl, $articles, $opt=array()) {

	// -- Zwingend erforderliche globale Variablen:
	global $banks, $confirmationurl, $BfiBicIdentifier, $BeneficiaryAccountIdentifier;
// Verlegt ab Spezifikation vom 19072004
// global $homepage, $transactionokurl;
// $TransactionNokUrl=$homepage;
	global $sigfile, $BeneficiaryNameAddressText, $BeneficiaryBeiIdentifier, $netpay_errorcode;

	// -- Konstanten
	$sapUgawwhg="EUR";  // Waehrung ist EURO
	$ChargeCode="SHA";  // ChargeCode SHAred
	$netpay_errorcode="000";
	$bank=$opt['Bank']?$opt['Bank']:"SPARDAT";
	PutLog ($bank);
	
	
	// Validation
	if (!$ReferenceIdentifier) $ReferenceIdentifier="NOTUSED";
	if (!$banks[$bank]['account']['enabled']) cry("111", "ERR: Die gewaehlte Bank wird laut Konfiguration nicht unterstuetzt (siehe banks.inc.php)");
	if (!$banks[$bank]['account']['BfiBicIdentifier']) $banks[$bank]['account']['BfiBicIdentifier']=$BfiBicIdentifier;
	if (!$banks[$bank]['account']['BeneficiaryAccountIdentifier']) $banks[$bank]['account']['BeneficiaryAccountIdentifier']=$BeneficiaryAccountIdentifier;
	if (!$banks[$bank]['account']['BfiBicIdentifier']) cry("110", "ERR: epi:BfiBicIdentifier muss gesetzt sein (siehe banks.inc.php, netpay_config.inc.php)");
	if (!$banks[$bank]['account']['BeneficiaryAccountIdentifier']) cry("110", "ERR: epi:BeneficiaryAccountIdentifier muss gesetzt sein (siehe banks.inc.php, netpay_config.inc.php)");
	if (!$banks[$bank]['Login']) cry("110", "ERR: Die angegebene Bank stellt kein Testsystem zur Verfuegung. Es ist daher nur ein Produktionseinsatz moeglich. (UseTestSystem daher nicht setzbar -> siehe config)");
        if (strtolower(substr($TransactionNokUrl,0,4))!="http") cry("114", "ERR: Eine gueltige TransactionNokUrl als redirect-Ziel im Fehlerfall muss angegeben werden!");
        if (strtolower(substr($transactionokurl,0,4))!="http")  cry("114", "ERR: Eine gueltige TransactionOkUrl als redirect-Ziel im Erfolgsfall muss angegeben werden!");

	// XML-Header
	$xmldata=EPSHeader();
	$logstring="DoV2Order called with parameters $ReferenceIdentifier, $RemittanceIdentifier, $InstructedAmount, $transactionokurl, $TransactionNokUrl , ARTICLES: [";
	foreach($articles as $a) $logstring.="($a[0], $a[1], $a[2])";  
	$logstring.="], OPTIONS: [";

	// -- Optionale gloable Variablen
	$optionals=array(
	  array("Url", "EmailAddressIdentifier", "OrderInfoText", "OrderingCustomerOfiIdentifier", "OrderingCustomerIdentifier", "OrderingCustomerNameAddressText"),
	  array("PaymentInstructionIdentifier", "TransactionTypeCode", "InstructionCode"),
	  array("OptionDate", "OptionTime", "DateSpecificationCode"),
	  array("Realization", "PaymentDescription", "DigSig", "ExpirationTime"));
	foreach($optionals as $cat1) foreach($cat1 as $value) {
	    global $$value;	// Wenn kein optionaler Parameter des akt. Namens uebergeben wurde,
				// aber dieser global im Config definiert wurde, dann globale def.
				// hernehmen.
	    if (isset($opt[$value])) {
	      $$value=$opt[$value];	// sonst uebergebenen Wert nehmen
	      $logstring.="($value=".$$value.")";
	    }
	}
	PutLog($logstring."]");

	// -- Main
	// Daten-Korrekturen
	if (is_numeric($RemittanceIdentifier)) {
	    if (strlen($RemittanceIdentifier)<12 ) $RemittanceIdentifier=sprintf("%012u", $RemittanceIdentifier);
	    if (strlen($RemittanceIdentifier)==12) $RemittanceIdentifier.="Keiner";	// RAIKA: Verwendungszweck muss existieren
	}
        if (isset($OptionDate)) {
          if (!isset($DateSpecificationCode)) cry("111", "ERR: epi:OptionDate wurde angegeben, ohne einen DateSpecificationCode anzugeben. Dieser muss definiert werden, um epi:OptionDate verwenden zu koennen.");
          list($y, $m, $d)=split("-", $OptionDate);
          if (ChkDate($d, $m, $y)==false) cry("113", "ERR: Uebergebenes Durchfuehrungsdatum liegt ausserhalb des zulaessigen Bereichs (zw -1 und +28 Tagen)");
        }
	// XML-Paket bilden
	$xmldata.="<epsp:TransferInitiatorDetails><eps:PaymentInitiatorDetails><epi:EpiDetails><epi:IdentificationDetails>";
	$xmldata.="<epi:Date>".date("Y-m-d")."</epi:Date><epi:ReferenceIdentifier>$ReferenceIdentifier</epi:ReferenceIdentifier>";
	 foreach($optionals[0] as $value) if (isset($$value)) $xmldata.="<epi:$value>".$$value."</epi:$value>";
	$xmldata.="</epi:IdentificationDetails><epi:PartyDetails><epi:BfiPartyDetails><epi:BfiBicIdentifier>".$banks[$bank]['account']['BfiBicIdentifier']."</epi:BfiBicIdentifier></epi:BfiPartyDetails><epi:BeneficiaryPartyDetails>";
	 if (isset($BeneficiaryNameAddressText)) $xmldata.="<epi:BeneficiaryNameAddressText>$BeneficiaryNameAddressText</epi:BeneficiaryNameAddressText>";
	 else if (isset($BeneficiaryBeiIdentifier)) $xmldata.="<epi:BeneficiaryBeiIdentifier>$BeneficiaryBeiIdentifier</epi:BeneficiaryBeiIdentifier>";
	 else cry("110", "ERR: Keine Identifikation des Beguenstigten angegeben. epi:BeneficiaryNameAddressText ODER epi:BeneficiaryBeiIdentifier MUSS angegeben werden. Pruefen Sie die Einstellungen in der Konfigurationsdatei!");
	$xmldata.="<epi:BeneficiaryAccountIdentifier>".$banks[$bank]['account']['BeneficiaryAccountIdentifier']."</epi:BeneficiaryAccountIdentifier></epi:BeneficiaryPartyDetails></epi:PartyDetails>";
	$xmldata.="<epi:PaymentInstructionDetails>";
	 foreach($optionals[1] as $value) if (isset($$value)) $xmldata.="<epi:$value>".$$value."</epi:$value>";
    $k=strlen($RemittanceIdentifier)>35?"UnstructuredRemittanceIdentifier":"RemittanceIdentifier";
	$xmldata.="<epi:$k>$RemittanceIdentifier</epi:$k>";
    $xmldata.="<epi:InstructedAmount AmountCurrencyIdentifier=\"$sapUgawwhg\">$InstructedAmount</epi:InstructedAmount>";
	$xmldata.="<epi:ChargeCode>$ChargeCode</epi:ChargeCode>";
	if (isset($DateSpecificationCode))
	 if ($DateSpecificationCode=="DBD" || $DateSpecificationCode=="CRD") {
	  $xmldata.="<epi:DateOptionDetails DateSpecificationCode=\"$DateSpecificationCode\">";
	  if (isset($OptionDate)) {
	    $xmldata.="<epi:OptionDate>$OptionDate</epi:OptionDate>";
	    if (isset($OptionTime)) $xmldata.="<epi:OptionTime>$OptionTime</epi:OptionTime>";
	  }
	  $xmldata.="</epi:DateOptionDetails>";
	 } else cry("111", "ERR: Der epi:DateOptionDetails DateSpecificationCode ist fehlerhaft. Gueltige Werte sind CRD und DBD.");
	$xmldata.="</epi:PaymentInstructionDetails></epi:EpiDetails>";
	if (isset($Realization) || isset($PaymentDescription) || isset($DigSig) || isset($ExpirationTime)) {
	  $xmldata.="<atrul:AustrianRulesDetails>";
	   foreach($optionals[3] as $value) if (isset($$value)) $xmldata.="<atrul:$value>".$$value."</atrul:$value>";
	  $xmldata.="</atrul:AustrianRulesDetails>";	  
	}
	$xmldata.="</eps:PaymentInitiatorDetails><epsp:TransferMsgDetails><epsp:ConfirmationUrl>$confirmationurl</epsp:ConfirmationUrl>";
	PutLog ("Opt:".print_r($opt, true));
	if ($opt["TargetWindow"]) $targetwnd = " TargetWindow=\"".$opt["TargetWindow"]."\"";
	$xmldata.="<epsp:TransactionOkUrl".$targetwnd.">$transactionokurl</epsp:TransactionOkUrl>";
	$xmldata.="<epsp:TransactionNokUrl".$targetwnd.">$TransactionNokUrl</epsp:TransactionNokUrl>";
// Obsolet ab Spezifikation vom 19072004
//	$xmldata.="<epsp:BasketUrl>$BasketUrl</epsp:BasketUrl><epsp:PaymentModeUrl>$PaymentModeUrl</epsp:PaymentModeUrl>";
	$xmldata.="</epsp:TransferMsgDetails>";
        if (sizeof($articles) > 0)
        {
            $xmldata.="<epsp:WebshopDetails>";
            foreach($articles as $artikel)
                $xmldata.="<epsp:WebshopArticle ArticleName=\"$artikel[0]\" ArticleCount=\"$artikel[1]\" ArticlePrice=\"$artikel[2]\"/>";
            $xmldata.="</epsp:WebshopDetails>";
        }
	$xmldata.="<epsp:AuthenticationDetails><epsp:UserId>".$banks[$bank]['account']['sapPopRequestor']."</epsp:UserId>";
	$hash=$banks[$bank]['account']['HaendlerPIN'].date("Y-m-d").$ReferenceIdentifier.$banks[$bank]['account']['BeneficiaryAccountIdentifier'].
	    $RemittanceIdentifier.$InstructedAmount.$sapUgawwhg.$banks[$bank]['account']['sapPopRequestor'];
// Neu seit 03.05.2005 - md5-Summe von utf-codierten Strings
	$hash=utf8_encode($hash);
// </Neu>
	if (!isset($sigfile)) $xmldata.="<epsp:MD5Fingerprint>".md5($hash)."</epsp:MD5Fingerprint>";
	else if (!file_exists($sigfile)) cry("110", "ERR: Die angegebene Signaturdatei ($sigfile) existiert nicht. Setzen Sie die Variable sigfile richtig, oder kommentieren Sie deren Definition in der globalen Konfigurationsdatei aus, wenn Sie keine Signatur, sondern lieber MD5 verwenden moechten.");
	else {
	  $h=fopen($sigfile, "r");
	  while (!feof($h)) $xmldata.=fgets($h, 1024);
	  fclose($h);
	}

	$xmldata.="</epsp:AuthenticationDetails></epsp:TransferInitiatorDetails></epsp:EpsProtocolDetails>";
	PutLog("(Anfrage an den Netpay-Server ".$banks[$bank]['Login'].") Anfragepaket:\n$xmldata");

//DEBUG
//	fwrite($h=fopen("test.xml", "w"), $xmldata);fclose($h); die("XML-Packet output was written to test.xml, halted.");
	
	// CURL einstellen
	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,$banks[$bank]['Login']);
	curl_setopt($ch, CURLOPT_POST,1);            // es handelt sich um einen POST
	curl_setopt($ch, CURLOPT_POSTFIELDS, utf8_encode($xmldata));
	curl_setopt($ch, CURLOPT_TIMEOUT, 60);     //times out after 61s
	curl_setopt($ch, CURLOPT_HEADER,0);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);  // stellt sicher, dass auch ein Rueckgabeergebnis existieren wird
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
	curl_setopt($ch, CURLOPT_HTTPHEADER, Array("Content-Type: text/xml"));

	// Abfrage durchfuehren
	$result=curl_exec ($ch);
	PutLog("(Anfrage an den Netpay-Server) Rueckantwort:\n$result");
	curl_close ($ch);

	// Parser initialisieren zum Parsen, handler initialisieren
	$currentTag="";
	$xml_parser = xml_parser_create();
        xml_set_element_handler($xml_parser, "DoOrder_startElement", "DoOrder_endElement"); // callback fkt. setzen
        xml_set_character_data_handler($xml_parser, "DoOrder_characterData");

	// parsen
	if (!xml_parse($xml_parser, $result))
	    out("012", sprintf("XML Fehler: %s in Zeile %d",xml_error_string(xml_get_error_code($xml_parser)),xml_get_current_line_number($xml_parser))); 
	xml_parser_free($xml_parser);

	return ($result);
    }



    /* Funktion: DoOrder
       Zweck:	 Sendet eine Bestellung an den netpay-Server ab.
       STATUS:   DEPRECATED! --> USE DoV2Order !
       Anmerkung:Diese Funktion dient nur mehr als Wrapper fuer die DoOrder Funktion
    		 der Vorversion des Scripts, um die Abwaertskompatibilitaet zu wahren.
	  !!	 Sie sollten sobald wie moeglich auf die neue Funktion DoV2Order umstellen.
		 Diese bietet etliche optionale Zusatzparameter, gemaess des neuen
		 eps-Standards!
       Parameter:einkaufswagen: URL des Einkaufswagen des Haendlers, um dem
    			        Benutzer die Moeglichkeit zu geben, nach Abbruch
				der Zahlung weitere Produkte zu kaufen.
				(wird nun ignoriert!)
		zahlungswunsch: URL der Seite des Webshops, auf welcher der 
				Benutzer nach Abbruch der Zahlung wieder zwischen
				den verschiedenen Zahlungsmoeglichkeiten waehlen
				kann
				(wird nun ignoriert!)
		orderid:	Kundendatenfeld
		customerid:	beim Haendler vorhandene Kundennummer
				(kann auch leer sein)
		articles:	ein Array von Artikeln, die wiederum aus einem
				Array bestehen, das sich genau aus den 3
				Komponenten Name, Anzahl und Preis zusammensetzt
				z.B.: array(array('Netzwerkkarte', 1, '10.90'), array('Thinkgeek T-Shirt', 2, '15.10'))
		totalsum:	Gesamtsumme der vom Kunden gekauften Waren
      Rueckgabe:Die Funktion gibt die Antwort des Servers zurueck
      Beispiel: DoOrder('', '', '123456789', '0815', array(array('Netzwerkkarte', 1, '10.90'), array('Thinkgeek T-Shirt', 2, '15.10')), '26.00');
    */
    function DoOrder($einkaufswagen, $zahlungswunsch, $orderid, $customerid, $articles, $totalsum) {
	PutLog("DoOrder called with parameters: $einkaufswagen, $zahlungswunsch, $orderid, $customerid, [Articles], $totalsum");
	return(DoV2Order("", $orderid, $totalsum, $articles, array()));
    }

    /* Funktion: ListBanks
       Zweck:	 Gibt eine Liste der aktiven Banken als <option>... Feld aus
    */
    function ListBanks()
    {
      global $banks;

      foreach ($banks as $key => $bank)
      {
	if ($bank['account']['enabled'])
	{
    	    if ($i>0 && $lastgrp!=$bank['group'])
		echo '<option>------------------------------</option>';
	    echo '<option value="'.$key.'">'.$bank['name']."</option>\n";
	    $i++;
	    $lastgrp=$bank['group'];
	}
      }
    }


    /* Funktion: PutLog
       Zweck:	 vermerkt eine Zeile im log-file, wenn logging erwuenscht ist.
       Parameter:text: Die zu vermerkende Zeile
    */
    function PutLog($text) {
	global $netpay_debug, $logfile;

	if (isset($netpay_debug)) {
	    fputs(($fp=fopen($logfile,"a")),strftime("um: %d.%m.%Y %H:%M:%S",time()).": $text\n-------------------------------------------------------------------------------\n");
	    fclose($fp);
	}
    }

    /* Funktion: EPSHeader
       Zweck:	 Retourniert EPS-Nachrichtenkopf
    */
	function EPSHeader()
	{
		return "<?xml version=\"1.0\" encoding=\"UTF-8\"  standalone=\"yes\"?>".
        "<epsp:EpsProtocolDetails ".
	    "xmlns:epsp=\"http://www.stuzza.at/namespaces/eps/protocol/2013/02\" ".
	    "xmlns:atrul=\"http://www.stuzza.at/namespaces/eps/austrianrules/2013/02\" ".
	    "xmlns:epi=\"http://www.stuzza.at/namespaces/eps/epi/2013/02\" ".
	    "xmlns:eps=\"http://www.stuzza.at/namespaces/eps/payment/2013/02\" ".
	    "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ".
        "xsi:schemaLocation=\"http://www.stuzza.at/namespaces/eps/protocol/2013/02 ".
        "EPSProtocol-V25.xsd\" SessionLanguage=\"DE\">";
	}

    /* Funktion: out
       Zweck:	 Gibt die uebergebene Nachricht aus und schreibt sie ins logfile
    */
    function out($errcode, $text) { 
      global $netpay_errorcode, $ShowErrorMsg;

      $netpay_errorcode=$errcode;
      if (isset($ShowErrorMsg)) print($text);
      PutLog($text); 
    }


    /* Funktion: cry
       Zweck:	 Gibt die uebergebene Fehlermeldung am Schirm aus, schreibt sie ins logfile und
		 beendet das script sofort
       Anm.:	 Diese Funktion wird intern von DoV2Order verwendet
    */
    function cry($errcode, $text) { 
      out($errcode, $text);
      exit();
    }

    /* Funktion: ChkDate
       Zweck:    Ueberprueft Durchfuehrungsdatum einer Terminzahlung auf
                 Korrektheit
       Rueckgabe:bool(true) wenn OK, sonst false
    */
    function ChkDate($day, $month, $year) {
      return true;
      // Gueltigen Datumsbereich als Unix-Zeitstempel erstellen
      if (checkdate($month, $day, $year)==false) return false;
      $today=mktime(0, 0, 0, date('m'), date('d'), date('Y'));
      $starttime=$today-86400; // Vortag 0:00h
      $endtime=$today+86400*27; // Bis zu 27 Tage nach heute
      $date=mktime(0, 0, 0, $month, $day, $year);
      if ($date<$starttime || $date>$endtime) return false; else return true;
    }

    // -- XML-Funktionen
    /* Funktion: DoOrder_startElement
       Zweck:    Parsed die eingehenden XML-Startelemente
       Anm.:	 Diese Funktion wird intern von DoV2Order verwendet
    */
    function DoOrder_startElement($parser, $name, $attrs) {
	global $currentTag;
	$currentTag=$name;
    }

    /* Funktion: DoOrder_endElement
       Zweck:    Parsed die eingehenden XML-Endelemente
       Anm.:	 Diese Funktion wird intern von DoV2Order verwendet
    */
    function DoOrder_endElement($parser, $name) {
	global $currentTag;
	$currentTag = "";
    }

    /* Funktion: DoOrder_characterData
       Zweck:    Parsed die eingehenden XML-Elementdaten
       Anm.:	 Diese Funktion wird intern von DoV2Order verwendet
    */
    function DoOrder_characterData($parser, $data) {
	global $currentTag, $err, $redirect, $netpay_errorcode;
	list($prefix, $tag)=split(":", $currentTag, 2);
	if (!$tag) $tag=$currentTag;
	switch(strtoupper($tag)) {
	    case "ERRORCODE": if ($data>0) { 
				out($data, "ERR: Es ist ein Fehler im aktuellen XML-Paket aufgetreten. Der Bankserver meldete Fehlernummer ".$data); 
				$err=$data;
			      } else { 
			        PutLog("Redirect auf: $redirect");
				$redirect=str_replace(";", urlencode(";"), $redirect);	// RAIKA fix
				if (headers_sent()) 
				    die("<META HTTP-EQUIV=Refresh CONTENT=\"1; URL=$redirect\">");
				else
				    header("Location:$redirect"); exit();
			      }			      
			      break;
	    case "ERRORMSG" : if ($err) cry($netpay_errorcode, "\nFehlerbeschreibung: ".$data); break;
	    case "CLIENTREDIRECTURL": $redirect.=$data;
	}
    }
?>