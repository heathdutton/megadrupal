<?php

/**
 * @file
 * Feeds parser class for Cablecast
 */

/**
 * Class definition for Cablecast Parser.
 *
 * Parses RSS feeds returned from Cablecast schedule.
 */
class FeedsCablecastParser extends FeedsParser {

  /**
   * Parse the extra mapping sources provided by this parser.
   *
   * @param $fetcher_result FeedsFetcherResult
   * @param $source FeedsSource
   *
   * @see FeedsParser::parse()
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
   
    $mediarss_feed = $fetcher_result->getRaw();
    
    $result = new FeedsParserResult();

    /**
     * @see common_syndication_parser_parse()
     */
    if (!defined('LIBXML_VERSION') || (version_compare(phpversion(), '5.1.0', '<'))) {
      @$sxml = simplexml_load_string($mediarss_feed, NULL);
    }
    else {
      @$sxml = simplexml_load_string($mediarss_feed, NULL, LIBXML_NOERROR | LIBXML_NOWARNING | LIBXML_NOCDATA);
    }

    // Got a malformed XML.
    if ($sxml === FALSE || is_null($sxml)) {
      throw new Exception(t('FeedsCablecastParser: Malformed XML source.'));
    }
      
    if ($this->isRssFeed($sxml)) {
      $result = $this->parseRss20($sxml, $source, $fetcher_result);    
    }
    else {
      throw new Exception(t('FeedsCablecastParser: Unknown type of feed.'));
    }
    return $result;
  }

  /**
   * Check if given feed object is a RSS feed.
   *
   * @param SimpleXMLElement $sxml
   *
   * @return boolen
   *   TRUE if given SimpleXML object is RSS feed or FALSE
   */
  protected function isRssFeed(SimpleXMLElement $sxml) {
    return $sxml->getName() == 'rss';
  }
  
  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'guid' => array(
        'name' => t('GUID'),
      ),
      'link' => array(
        'name' => t('Link'),
      ),
      'title' => array(
        'name' => t('Title'),
        'description' => t('Title.'),
      ),
      'author' => array(
        'name' => t('Author'),
        'description' => t('Author or uploader of the video.'),
      ),
      'published' => array(
        'name' => t('Published'),
      ),
      'description' => array(
        'name' => t('Description'),
      ),
      'tags' => array(
        'name' => t('Tags'),
        'description' => t('This can be imported directly with Taxonomy "tags" vocabularies.'),
      ),
      
      
      // http://www.telvue.com/support/product-documentation/princeton-server-api-docs/schedule-methods/get-channel-schedule-as-rss-rest/
      //
      // Brewer question:
      // This doesn't look like the data in the cablecast feed. 
      // Should this get reorganized (pared) to what's in the 
      // tightrope feed?
      //
      'channel' => array(  
        'name' => t('channel'),
      ),
      'cablecast_eventid' => array(
        'name' => t('eventId'),
      ),
      'cablecast_duration' => array(  
        'name' => t('duration'),
      ),
      'cablecast_start_datetime' => array(  
        'name' => t('start_datetime'),
      ),
      'cablecast_end_datetime' => array(  
        'name' => t('end_datetime'),
      ),
      'cablecast_programcode' => array(  
        'name' => t('programCode'),
      ),
      'cablecast_episode' => array(  
        'name' => t('episode'),
      ),
      'cablecast_episodecode' => array(  
        'name' => t('episodeCode'),
      ),
      'cablecast_thumbnail' => array(  
        'name' => t('thumbnail'),
      ),
      
    );
  }

 
   
  /**
   * Parse RSS 2.0 feed
   *
   * @param SimpleXMLElement $sxml
   * @param FeedsFetcherResult $fetcher_result
   * @param FeedsSource $source
   */
  private function parseRss20(SimpleXMLElement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {
  
    // XML was parsed successfully, so we can begin to process items
    $result = new FeedsParserResult();
    $fetcher_result->title = (string) $sxml->channel->title;
    $fetcher_result->description = (string) $sxml->channel->description;
    $fetcher_result->link = (string) $sxml->channel->link;
    $feed_title = (string) $sxml->channel->title;
    
 
    $namespaces = $sxml->getNamespaces(true);

    foreach ($sxml->xpath('//item') as $entry) {
           
      // munge feed
      // strip off "Cablecast" leaving "CH xx - Blah Blah Blah"
      $channel = preg_replace('/^.* - CH/', 'CH',$feed_title);

      // get duration from description field
      list($h,$m,$s) = explode(":",$entry->description);
      // convert to seconds
      $duration = ($h * 60 * 60) + ($m * 60) + $s;

      // read pubDate as GMT
      date_default_timezone_set("GMT");
      $start = strtotime($entry->pubDate);

      // write Start Time and End time in UCT
      date_default_timezone_set("UCT");
      $start_format = date("Y-m-d H:i:s", $start);
      // add duration to start to get end
      $end = $start + $duration;
      $end_format = date("Y-m-d H:i:s", $end);
  
      $tags =  isset($media->group->keywords) ? explode(',', (string) $media->group->keywords) : NULL;                         
      
      $item = array(
        'guid' => (string) $entry->guid,
        'link' => (string) $entry->link,
        'title' => html_entity_decode((string) $entry->title),
        'author' => (string) $entry->author,
        'description' => html_entity_decode((string) $entry->description),
        'tags' => $tags,
        'published' => (string) $start,
        'channel' => (string) $channel,   
        'cablecast_start_datetime' => (string) $start_format,
        'cablecast_duration' => (string) $duration,
        'cablecast_end_datetime' => $end_format,       
      );

      // Populate the FeedsFetcherResult object with the parsed results.
      $result->items[] = $item;
    }
   
    return $result;
  }
    
  /**
   *  Display seconds as HH:MM:SS, with leading 0's.
   *
   *  @param $seconds
   *    The number of seconds to display.
   */
  public function secsToTime($seconds) {
    // Number of seconds in an hour.
    $unith = 3600;
    // Number of seconds in a minute.
    $unitm = 60;

    // '/' given value by num sec in hour... output = HOURS
    $hh = intval($seconds / $unith);

    // Multiply number of hours by seconds, then subtract from given value.
    // Output = REMAINING seconds.
    $ss_remaining = ($seconds - ($hh * 3600));

    // Take remaining seconds and divide by seconds in a min... output = MINS.
    $mm = intval($ss_remaining / $unitm);
    // Multiply number of mins by seconds, then subtract from remaining seconds.
    // Output = REMAINING seconds.
    $ss = ($ss_remaining - ($mm * 60));

    $output = '';

    // If we have any hours, then prepend that to our output.
    if ($hh) {
      $output .= "$hh:";
    }

    // Create a safe-for-output MM:SS.
    $output .= sprintf($hh ? "%02d:%02d" : "%d:%02d", $mm, $ss);

    return $output;
  }
}


