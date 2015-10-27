<?php

//
// NCIPHTTPClient no longer used
//
//include_once '../../ncip/lib/NCIPHTTPClient.php';
//
//$xml_template =<<<EOT
//<?xml version="1.0"
?>
<ns1:NCIPMessage xmlns:ns1="http://www.niso.org/2008/ncip" ns1:version="http://128.151.244.69/xc-1.2-test/?q=ncip/schemas/ncip_v2_0.xsd"><ns1:Ext><ns1:LookupItemSet><ns1:InitiationHeader><ns1:FromAgencyId><ns1:AgencyId></ns1:AgencyId></ns1:FromAgencyId><ns1:ToAgencyId><ns1:AgencyId>XC</ns1:AgencyId></ns1:ToAgencyId></ns1:InitiationHeader><ns1:BibliographicId><ns1:BibliographicRecordId><ns1:BibliographicRecordIdentifier>[BibID]</ns1:BibliographicRecordIdentifier><ns1:AgencyId ns1:Scheme="http://www.nru.org">NRU</ns1:AgencyId></ns1:BibliographicRecordId></ns1:BibliographicId><ns1:ItemElementType ns1:Scheme="http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm">Circulation Status</ns1:ItemElementType><ns1:ItemElementType ns1:Scheme="http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm">Location</ns1:ItemElementType></ns1:LookupItemSet></ns1:Ext></ns1:NCIPMessage>
//EOT;
//
//$bibIDs = array(
//  3006384, 86962, 1296836, 2126047, 1973380, 1296836, 2126047, 1704901, 113444,
//  1076703, 1282822, 1271726, 558189, 3032877, 170413, 915040, 904335, 611929,
//  915040, 4323754, 436310, 1532893, 4350094, 3697243, 537274, 2901139, 2005583, 1290937
//);
//
//$method = 'curl';
//
//$path = '/ncipv2/NCIPResponder/';
//if ($method == 'client') {
//  $client = new NCIPHTTPClient('128.151.244.132', '8080');
//  // $client->cookie_host = '128.151.244.132';
//  $client->debug_on = 0;
//  $client->use_gzip = FALSE;
//  $client->timeout = 20;
//  $client->content_type = 'application/xml; charset="utf-8"';
//}
//
//foreach ($bibIDs as $bibID) {
//  echo $bibID, "\n";
//  $xml = strtr($xml_template, array('[BibID]' => $bibID));
//  // send XML
//
//  $posted = $client->post($path, $xml);
//  if ($posted) {
//    $content = $client->get_content();
//  }
//  else {
//    echo 'errormsg: ', $client->errormsg, "\n";
//  }
//
//  $obj = new SimpleXMLElement($content, NULL, FALSE, 'http://www.niso.org/2008/ncip');
//  foreach ($obj->Ext->LookupItemSetResponse->BibInformation as $bib) {
//    if (!isset($bib->HoldingsSet)) {
//      echo "\tERROR: ", $bib_information->asXML(), "\n";
//      continue;
//    }
//    foreach ($bib->HoldingsSet as $holdings) {
//      if (!isset($holdings->ItemInformation)) {
//        echo "\tERROR: ", $holdings->asXML(), "\n";
//        continue;
//      }
//      foreach ($holdings->ItemInformation as $item) {
//        if (!isset($item->ItemOptionalFields->CirculationStatus)) {
//          echo "\tERROR: ", $item->asXML(), "\n";
//          continue;
//        }
//        printf("\t[%s %s] %s: %s\n",
//          (string)$item->ItemId->AgencyId,
//          (string)$item->ItemId->ItemIdentifierValue,
//          (string)$item->ItemOptionalFields->ItemDescription->CallNumber,
//          (string)$item->ItemOptionalFields->CirculationStatus);
//      }
//    }
//  }
//}

