<?php

// configurable fields
$ncip_url = 'http://128.151.244.132:8080/ncipv2/NCIPResponder/';
$ils_url = 'http://catalog.lib.rochester.edu/vwebv/holdingsInfo?bibId=';
// --------------------

$xml_template = <<<EOT
<?xml version="1.0"?><ns1:NCIPMessage xmlns:ns1="http://www.niso.org/2008/ncip" ns1:version="http://128.151.244.69/xc-1.2-test/?q=ncip/schemas/ncip_v2_0.xsd"><ns1:Ext><ns1:LookupItemSet><ns1:InitiationHeader><ns1:FromAgencyId><ns1:AgencyId></ns1:AgencyId></ns1:FromAgencyId><ns1:ToAgencyId><ns1:AgencyId>XC</ns1:AgencyId></ns1:ToAgencyId></ns1:InitiationHeader><ns1:BibliographicId><ns1:BibliographicRecordId><ns1:BibliographicRecordIdentifier>[BibID]</ns1:BibliographicRecordIdentifier><ns1:AgencyId ns1:Scheme="http://www.nru.org">NRU</ns1:AgencyId></ns1:BibliographicRecordId></ns1:BibliographicId><ns1:ItemElementType ns1:Scheme="http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm">Circulation Status</ns1:ItemElementType><ns1:ItemElementType ns1:Scheme="http://www.niso.org/ncip/v1_0/schemes/itemelementtype/itemelementtype.scm">Location</ns1:ItemElementType></ns1:LookupItemSet></ns1:Ext></ns1:NCIPMessage>
EOT;

$bibIDs = array(
  3006384,
  86962,
  1296836,
  2126047,
  1973380,
  1296836,
  2126047,
  1704901,
  113444,
  1076703,
  1282822,
  1271726,
  558189,
  3032877,
  170413,
  915040,
  904335,
  611929,
  915040,
  4323754,
  436310,
  1532893,
  4350094,
  3697243,
  537274,
  2901139,
  2005583,
  1290937,
);

$ch = get_curl();

foreach ($bibIDs as $bibID) {
  echo $bibID, "\n";
  $xml = strtr($xml_template, array('[BibID]' => $bibID));
  // send request XML
  curl_setopt($ch, CURLOPT_POSTFIELDS, $xml);
  $content = curl_exec($ch);

  // process response XML
  $obj = new SimpleXMLElement($content, NULL, FALSE, 'http://www.niso.org/2008/ncip');
  foreach ($obj->Ext->LookupItemSetResponse->BibInformation as $bib) {
    if (!isset($bib->HoldingsSet)) {
      echo "\tERROR: ", $bib_information->asXML(), "\n";
      continue;
    }
    foreach ($bib->HoldingsSet as $holdings) {
      if (!isset($holdings->ItemInformation)) {
        echo "\tERROR: ", $holdings->asXML(), "\n";
        continue;
      }
      foreach ($holdings->ItemInformation as $item) {
        if (!isset($item->ItemOptionalFields->CirculationStatus)) {
          echo "\tERROR: ", $item->asXML(), "\n";
          continue;
        }
        printf("\t[agency: %s, id: %s] call number: %s, circ status: %s\n",
          (string) $item->ItemId->AgencyId,
          (string) $item->ItemId->ItemIdentifierValue,
          (string) $item->ItemOptionalFields->ItemDescription->CallNumber,
          (string) $item->ItemOptionalFields->CirculationStatus);
      }
    }
  }

  $status = get_from_voyager($bibID);
  printf("\tVoyager:\n\t%s\n", join("\n\t", $status));
}

$data = <<<EML
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<ns1:NCIPMessage xmlns:ns1="http://www.niso.org/2008/ncip"
  ns1:version="http://www.niso.org/schemas/ncip/v2_0/imp1/xsd/ncip_v2_0.xsd">
  <ns1:Ext>
    <ns1:LookupItemSetResponse>
      <ns1:BibInformation>
        <ns1:BibliographicId>
          <ns1:BibliographicRecordId>
            <ns1:BibliographicRecordIdentifier>558189</ns1:BibliographicRecordIdentifier>
            <ns1:AgencyId ns1:Scheme="http://www.nru.org">NRU</ns1:AgencyId>
          </ns1:BibliographicRecordId>
        </ns1:BibliographicId>
        <ns1:HoldingsSet>
          <ns1:HoldingsSetId>623300</ns1:HoldingsSetId>
          <ns1:CallNumber>ZZ6080.M4 G5</ns1:CallNumber>
          <ns1:ItemInformation>
            <ns1:ItemId>
              <ns1:AgencyId>NRU</ns1:AgencyId>
              <ns1:ItemIdentifierValue>834517</ns1:ItemIdentifierValue>
            </ns1:ItemId>
            <ns1:ItemOptionalFields>
              <ns1:CirculationStatus ns1:Scheme="http://www.extensiblecatalog.ncip.v2.org/schemes/circulationstatus/xccirculationstatus.scm">Charged</ns1:CirculationStatus>
              <ns1:ItemDescription>
                <ns1:CallNumber>ZZ6080.M4 G5</ns1:CallNumber>
                <ns1:CopyNumber>0</ns1:CopyNumber>
              </ns1:ItemDescription>
              <ns1:Location>
                <ns1:LocationType ns1:Scheme="http://www.niso.org/ncip/v1_0/imp1/schemes/locationtype/locationtype.scm">Permanent Location</ns1:LocationType>
                <ns1:LocationName>
                  <ns1:LocationNameInstance>
                    <ns1:LocationNameLevel>1</ns1:LocationNameLevel>
                    <ns1:LocationNameValue>Special Collections, Rhees 225 (Non-circulating)</ns1:LocationNameValue>
                  </ns1:LocationNameInstance>
                </ns1:LocationName>
              </ns1:Location>
            </ns1:ItemOptionalFields>
          </ns1:ItemInformation>
        </ns1:HoldingsSet>
      </ns1:BibInformation>
    </ns1:LookupItemSetResponse>
  </ns1:Ext>
</ns1:NCIPMessage>
EML;

function get_curl() {
  global $ncip_url;

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $ncip_url);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_HEADER, 0); //1
  curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); //1
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_NOPROGRESS, 1);
  curl_setopt($ch, CURLOPT_USERAGENT, 'eXtensible Catalog Drupal Toolkit');
  curl_setopt($ch, CURLOPT_ENCODING, "gzip,deflate");

  return $ch;
}

function get_from_voyager($bibID) {
  global $ils_url;
  static $ch;

  if (!isset($ch)) {
    $ch = get_curl();
    curl_setopt($ch, CURLOPT_POST, 0);
  }

  curl_setopt($ch, CURLOPT_URL, $ils_url . $bibID);
  $voyager = curl_exec($ch);
  $status = array();
  if (preg_match_all('/name="Status:" class="fieldData" id="Status:">(.*?)<\/div>/s', $voyager, $matches)) {
    $raw_status = trim($matches[1][0]);
    $raw_status = preg_replace('/[\n\r]/', '', $raw_status);
    $raw_status = preg_replace('/\s*<br>\s*/', '<br>', $raw_status);
    $raw_status = preg_replace('/<br>$/', '', $raw_status);
    $status = explode('<br>', $raw_status);
  }
  // curl_close($ch);
  return $status;
}
