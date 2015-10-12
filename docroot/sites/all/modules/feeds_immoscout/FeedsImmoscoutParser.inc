<?php

/**
 * @file
 * Home of the FeedsImmoscoutParser and related classes.
 */

/**
 * A parser for Immoscout XML API
 */
class FeedsImmoscoutParser extends FeedsParser {
  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $parseAsDate = array('CreationDate', 'LastModificationDate', 'beginRent', 'endRent');
    // Set time zone to GMT for parsing dates with strtotime().
    $tz = date_default_timezone_get();
    date_default_timezone_set('GMT');
    // Yes, using a DOM parser is a bit inefficient, but will do for now
    $docs = unserialize($fetcher_result->getRaw());
    $xml = new SimpleXMLElement($docs['request']); // root is QueryResult
    $result = new FeedsParserResult();
//    $result->title =
//    $result->description =
//    $result->link =

    foreach ($xml->children() as $element) { // element is ResultLine
      $c = $element->children();
      $a = $c[0]; // e.g. AppartmentRentProxy
      $uuid = (string) $a['uuid'];
      $item = array();
      $item['proxyType'] = $a->getName();
      $item['title'] = (string) $a->Heading;
//    $item['description'] =
      $item['url'] = "http://www.immobilienscout24.de/{$uuid}";
      // Use UNIX time. If no date is defined, fall back to REQUEST_TIME.
      $item['timestamp'] = _immoscout_api_parse_date((string) $a->LastModificationDate);
      if (empty($item['timestamp'])) {
        $item['timestamp'] = REQUEST_TIME;
      }
      $item['guid'] = $uuid;

      foreach ($a->children() as $info) {
        // elements
        $item[$info->getName()] = (string) $info;
        if (in_array($info->getName(), $parseAsDate)) {
          $item[$info->getName()] = _immoscout_api_parse_date((string) $info);
        }
        // attributes on each element
        foreach ($info->attributes() as $attr => $val) {
          $item[$info->getName() . '_' . $attr] = (string) $val;
        }
      }

//    $item['description'] = '';
//    $item['description'] .= "EXPOSE $uuid:\n\n". htmlspecialchars($docs[$uuid]) ."\n\n";
      try {
        $expose = new SimpleXMLElement($docs[$uuid]); // root is ExposeResult
        $c = $expose->children();
        $a = $c[0]; // e.g. AppartmentRent
        $item['exposeType'] = $a->getName();
        foreach ($a->children() as $info) {
          // elements
          $item[$info->getName()] = (string) $info;
          if (in_array($info->getName(), $parseAsDate)) {
            $item[$info->getName()] = _immoscout_api_parse_date((string) $info);
          }
          // attributes on each element
          foreach ($info->attributes() as $attr => $val) {
            $item[$info->getName() . '_' . $attr] = (string) $val;
          }
        }
        foreach ($a->Attachments->Picture as $pic) {
            $caption = $pic->caption;
            $url = $pic->url;
            $item['enclosures'][] = new FeedsEnclosure((string)$url, 'image/jpeg'); //(string)$url['mimeType']);
        }
      }
      catch (Exception $ignore) {
        // just ignore
//    $item['description'] .= "ERROR: $ignore\n\n";
      }
//    $item['description'] .= "ALL VALUES\n\n";
//    $item['description'] .= print_r($item, true);

      $result->items[] = $item;
    }

    date_default_timezone_set($tz);

    return $result;
  }

  /**
   * Implements FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('Title of the feed item.'),
      ),
      'description' => array(
        'name' => t('Description'),
        'description' => t('Description of the feed item.'),
      ),
      'timestamp' => array(
        'name' => t('Published date'),
        'description' => t('Published date as UNIX time GMT of the feed item.'),
      ),
      'url' => array(
        'name' => t('Item URL (link)'),
        'description' => t('URL of the feed item.'),
      ),
      'guid' => array(
        'name' => t('Item GUID'),
        'description' => t('Global Unique Identifier of the feed item.'),
      ),
      'proxyType' => array(
        'name' => t('Item type (taken from Results)'),
        'description' => t('todo...'),
      ),
      'exposeType' => array(
        'name' => t('Item type (taken from Expose)'),
        'description' => t('todo...'),
      ),

      'enclosures' => array(
        'name' => t('enclosures'),
        'description' => t('An array of attachments to the feed item.'),
      ),

      // Query Result

      'addressCity' => array(
        'name' => t('Item addressCity'),
        'description' => t('todo...'),
      ),
      'areaSize' => array(
        'name' => t('Item areaSize'),
        'description' => t('todo...'),
      ),
      'association' => array(
        'name' => t('Item association'),
        'description' => t('todo...'),
      ),
      'beginRent' => array(
        'name' => t('Item beginRent'),
        'description' => t('todo...'),
      ),
      'condition' => array(
        'name' => t('Item condition'),
        'description' => t('todo...'),
      ),
      'contactHomePage' => array(
        'name' => t('Item contactHomePage'),
        'description' => t('todo...'),
      ),
      'CreationDate' => array(
        'name' => t('Item CreationDate'),
        'description' => t('todo...'),
      ),
      'endRent' => array(
        'name' => t('Item endRent'),
        'description' => t('todo...'),
      ),
      'foreignKey' => array(
        'name' => t('Item foreignKey'),
        'description' => t('todo...'),
      ),
      'geoX' => array(
        'name' => t('Item geoX'),
        'description' => t('todo...'),
      ),
      'geoY' => array(
        'name' => t('Item geoY'),
        'description' => t('todo...'),
      ),
      'hasBalcony' => array(
        'name' => t('Item hasBalcony'),
        'description' => t('todo...'),
      ),
      'hasElevator' => array(
        'name' => t('Item hasElevator'),
        'description' => t('todo...'),
      ),
      'hasFurniture' => array(
        'name' => t('Item hasFurniture'),
        'description' => t('todo...'),
      ),
      'hasGarden' => array(
        'name' => t('Item hasGarden'),
        'description' => t('todo...'),
      ),
      'Heading' => array(
        'name' => t('Item Heading'),
        'description' => t('todo...'),
      ),
      'heating' => array(
        'name' => t('Item heating'),
        'description' => t('todo...'),
      ),
      'houseNo' => array(
        'name' => t('Item houseNo'),
        'description' => t('todo...'),
      ),
      'kitchen' => array(
        'name' => t('Item kitchen'),
        'description' => t('todo...'),
      ),
      'LastModificationDate' => array(
        'name' => t('Item LastModificationDate'),
        'description' => t('todo...'),
      ),
      'Latitude' => array(
        'name' => t('Item Latitude'),
        'description' => t('todo...'),
      ),
      'Longitude' => array(
        'name' => t('Item Longitude'),
        'description' => t('todo...'),
      ),
      'marketing' => array(
        'name' => t('Item marketing'),
        'description' => t('todo...'),
      ),
      'maxPersons' => array(
        'name' => t('Item maxPersons'),
        'description' => t('todo...'),
      ),
      'netArea' => array(
        'name' => t('Item netArea'),
        'description' => t('todo...'),
      ),
      'netRent' => array(
        'name' => t('Item netRent'),
        'description' => t('todo...'),
      ),
      'netRent_currency' => array(
        'name' => t('Item netRent_currency'),
        'description' => t('todo...'),
      ),
      'noRooms' => array(
        'name' => t('Item noRooms'),
        'description' => t('todo...'),
      ),
      'pictureURL' => array(
        'name' => t('Item pictureURL'),
        'description' => t('todo...'),
      ),
      'pictureURL_pictureType' => array(
        'name' => t('Item pictureURL_pictureType'),
        'description' => t('todo...'),
      ),
      'pictureURL_isFloorplan' => array(
        'name' => t('Item pictureURL_isFloorplan'),
        'description' => t('todo...'),
      ),
      'pictureURL_mimeType' => array(
        'name' => t('Item pictureURL_mimeType'),
        'description' => t('todo...'),
      ),
      'price' => array(
        'name' => t('Item price'),
        'description' => t('todo...'),
      ),
      'price_currency' => array(
        'name' => t('Item price_currency'),
        'description' => t('todo...'),
      ),
//      'Quarter' => array(
//        'name' => t('Item Quarter'),
//        'description' => t('Always empty!'),
//      ),
      'Quarter_uuid' => array(
        'name' => t('Item Quarter_uuid'),
        'description' => t('todo...'),
      ),
      'Quarter_name' => array(
        'name' => t('Item Quarter_name'),
        'description' => t('todo...'),
      ),
      'quarterId' => array(
        'name' => t('Item quarterId'),
        'description' => t('todo...'),
      ),
      'rentDurationInDays' => array(
        'name' => t('Item rentDurationInDays'),
        'description' => t('todo...'),
      ),
      'rentInterval' => array(
        'name' => t('Item rentInterval'),
        'description' => t('todo...'),
      ),
      'showAddress' => array(
        'name' => t('Item showAddress'),
        'description' => t('todo...'),
      ),
      'siteArea' => array(
        'name' => t('Item siteArea'),
        'description' => t('todo...'),
      ),
      'sitePrice' => array(
        'name' => t('Item sitePrice'),
        'description' => t('todo...'),
      ),
      'sitePrice_currency' => array(
        'name' => t('Item sitePrice_currency'),
        'description' => t('todo...'),
      ),
      'smoker' => array(
        'name' => t('Item smoker'),
        'description' => t('todo...'),
      ),
      'street' => array(
        'name' => t('Item street'),
        'description' => t('todo...'),
      ),
      'totalArea' => array(
        'name' => t('Item totalArea'),
        'description' => t('todo...'),
      ),
      'totalRent' => array(
        'name' => t('Item totalRent'),
        'description' => t('todo...'),
      ),
      'totalRent_currency' => array(
        'name' => t('Item totalRent_currency'),
        'description' => t('todo...'),
      ),
      'wazImmoType' => array(
        'name' => t('Item wazImmoType'),
        'description' => t('todo...'),
      ),
      'zip' => array(
        'name' => t('Item zip'),
        'description' => t('todo...'),
      ),


      // Expose

      'Address_street' => array(
        'name' => t('Item Address_street'),
        'description' => t('todo...'),
      ),
      'Address_houseNo' => array(
        'name' => t('Item Address_houseNo'),
        'description' => t('todo...'),
      ),
      'Address_zip' => array(
        'name' => t('Item Address_zip'),
        'description' => t('todo...'),
      ),
      'Address_city' => array(
        'name' => t('Item Address_city'),
        'description' => t('todo...'),
      ),
      'Continent_uuid' => array(
        'name' => t('Item Continent_uuid'),
        'description' => t('todo...'),
      ),
      'Continent_name' => array(
        'name' => t('Item Continent_name'),
        'description' => t('todo...'),
      ),
      'Country_uuid' => array(
        'name' => t('Item Country_uuid'),
        'description' => t('todo...'),
      ),
      'Country_name' => array(
        'name' => t('Item Country_name'),
        'description' => t('todo...'),
      ),
      'Region_uuid' => array(
        'name' => t('Item Region_uuid'),
        'description' => t('todo...'),
      ),
      'Region_name' => array(
        'name' => t('Item Region_name'),
        'description' => t('todo...'),
      ),
      'City_uuid' => array(
        'name' => t('Item City_uuid'),
        'description' => t('todo...'),
      ),
      'City_name' => array(
        'name' => t('Item City_name'),
        'description' => t('todo...'),
      ),
      'Quarter_uuid' => array(
        'name' => t('Item Quarter_uuid'),
        'description' => t('todo...'),
      ),
      'Quarter_name' => array(
        'name' => t('Item Quarter_name'),
        'description' => t('todo...'),
      ),
      'geoX' => array(
        'name' => t('Item geoX'),
        'description' => t('todo...'),
      ),
      'geoY' => array(
        'name' => t('Item geoY'),
        'description' => t('todo...'),
      ),
      'Latitude' => array(
        'name' => t('Item Latitude'),
        'description' => t('todo...'),
      ),
      'Longitude' => array(
        'name' => t('Item Longitude'),
        'description' => t('todo...'),
      ),
      'buildingType' => array(
        'name' => t('Item buildingType'),
        'description' => t('todo...'),
      ),
      'netArea' => array(
        'name' => t('Item netArea'),
        'description' => t('todo...'),
      ),
      'totalArea' => array(
        'name' => t('Item totalArea'),
        'description' => t('todo...'),
      ),
      'netRent' => array(
        'name' => t('Item netRent'),
        'description' => t('todo...'),
      ),
      'netRent_currency' => array(
        'name' => t('Item netRent_currency'),
        'description' => t('todo...'),
      ),
      'kaution' => array(
        'name' => t('Item kaution'),
        'description' => t('todo...'),
      ),
      'marketing' => array(
        'name' => t('Item marketing'),
        'description' => t('todo...'),
      ),
      'marketingPrice' => array(
        'name' => t('Item marketingPrice'),
        'description' => t('todo...'),
      ),
      'marketingPrice_currency' => array(
        'name' => t('Item marketingPrice_currency'),
        'description' => t('todo...'),
      ),
      'additionalCosts' => array(
        'name' => t('Item additionalCosts'),
        'description' => t('todo...'),
      ),
      'additionalCosts_currency' => array(
        'name' => t('Item additionalCosts_currency'),
        'description' => t('todo...'),
      ),
      'availableDate' => array(
        'name' => t('Item availableDate'),
        'description' => t('todo...'),
      ),
      'condition' => array(
        'name' => t('Item condition'),
        'description' => t('todo...'),
      ),
      'flooring' => array(
        'name' => t('Item flooring'),
        'description' => t('todo...'),
      ),
      'floors' => array(
        'name' => t('Item floors'),
        'description' => t('todo...'),
      ),
      'courtageInfo_hasCourtage' => array(
        'name' => t('Item courtageInfo_hasCourtage'),
        'description' => t('todo...'),
      ),
      'courtageInfo_courtage' => array(
        'name' => t('Item courtageInfo_courtage'),
        'description' => t('todo...'),
      ),
      'courtageInfo_courtageDescription' => array(
        'name' => t('Item courtageInfo_courtageDescription'),
        'description' => t('todo...'),
      ),
      //lanCables
      'heating' => array(
        'name' => t('Item heating'),
        'description' => t('todo...'),
      ),
      'timeAirport' => array(
        'name' => t('Item timeAirport'),
        'description' => t('todo...'),
      ),
      'timeBusStop' => array(
        'name' => t('Item timeBusStop'),
        'description' => t('todo...'),
      ),
      'timeMotorway' => array(
        'name' => t('Item timeMotorway'),
        'description' => t('todo...'),
      ),
      'timeRailway' => array(
        'name' => t('Item timeRailway'),
        'description' => t('todo...'),
      ),
      'airCondition' => array(
        'name' => t('Item airCondition'),
        'description' => t('todo...'),
      ),
      // energyConsumption : energyPerformanceCertification
      'description' => array(
        'name' => t('Item description'),
        'description' => t('todo...'),
      ),
      'interior' => array(
        'name' => t('Item interior'),
        'description' => t('todo...'),
      ),
      'interiorQuality' => array(
        'name' => t('Item interiorQuality'),
        'description' => t('todo...'),
      ),
      'otherInfo' => array(
        'name' => t('Item otherInfo'),
        'description' => t('todo...'),
      ),
      'position' => array(
        'name' => t('Item position'),
        'description' => t('todo...'),
      ),

    ) + parent::getMappingSources();
  }
}

/**
 * Parse a date comes from a feed.
 *
 * @param $date_string
 *  The date string in various formats.
 * @return
 *  The timestamp of the string or the current time if can't be parsed
 */
function _immoscout_api_parse_date($date_str) {
  $parsed_date = strtotime($date_str);
  if ($parsed_date === FALSE || $parsed_date == -1) {
    $parsed_date = _immoscout_api_parse_w3cdtf($date_str);
  }
  return $parsed_date === FALSE ? time() : $parsed_date;
}

/**
 * Parse the W3C date/time format, a subset of ISO 8601.
 *
 * PHP date parsing functions do not handle this format.
 * See http://www.w3.org/TR/NOTE-datetime for more information.
 * Originally from MagpieRSS (http://magpierss.sourceforge.net/).
 *
 * @param $date_str
 *   A string with a potentially W3C DTF date.
 * @return
 *   A timestamp if parsed successfully or FALSE if not.
 */
function _immoscout_api_parse_w3cdtf($date_str) {
  if (preg_match('/(\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2})(:(\d{2}))?(?:([-+])(\d{2}):?(\d{2})|(Z))?/', $date_str, $match)) {
    list($year, $month, $day, $hours, $minutes, $seconds) = array($match[1], $match[2], $match[3], $match[4], $match[5], $match[6]);
    // Calculate the epoch for current date assuming GMT.
    $epoch = gmmktime($hours, $minutes, $seconds, $month, $day, $year);
    if ($match[10] != 'Z') { // Z is zulu time, aka GMT
      list($tz_mod, $tz_hour, $tz_min) = array($match[8], $match[9], $match[10]);
      // Zero out the variables.
      if (!$tz_hour) {
        $tz_hour = 0;
      }
      if (!$tz_min) {
        $tz_min = 0;
      }
      $offset_secs = (($tz_hour * 60) + $tz_min) * 60;
      // Is timezone ahead of GMT?  If yes, subtract offset.
      if ($tz_mod == '+') {
        $offset_secs *= -1;
      }
      $epoch += $offset_secs;
    }
    return $epoch;
  }
  else {
    return FALSE;
  }
}
