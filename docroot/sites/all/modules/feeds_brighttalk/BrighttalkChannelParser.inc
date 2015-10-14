<?php

/**
 * @file
 * feeds parser class for brighttalk
 */

/**
 * class definition for brighttalk
 *
 * parses xml feeds from brighttalk.com, e.g. http://www.brighttalk.com/channel/288/feed
 */
class BrighttalkChannelParser extends FeedsParser {

  /**
   * parse the extra mapping sources provided by this parser.
   *
   * @param $fetcher_result feedsfetcherresult
   * @param $source feedssource
   *
   * @see feedsparser::parse()
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $brighttalk_feed = $fetcher_result->getraw();
    $result = new feedsparserresult();

    if (!defined('libxml_version') || (version_compare(phpversion(), '5.1.0', '<'))) {
      @$sxml = simplexml_load_string($brighttalk_feed, null);
    }
    else {
      @$sxml = simplexml_load_string($brighttalk_feed, null, libxml_noerror | libxml_nowarning | libxml_nocdata);
    }

    // got a malformed xml.
    if ($sxml === false || is_null($sxml)) {
      throw new exception(t('brighttalkchannelparser: malformed xml source.'));
    }

    $result = $this->parsechannel($sxml, $source, $fetcher_result);

    return $result;
  }

  /**
   * add the extra mapping sources provided by this parser.
   */
  public function getmappingsources() {
    return parent::getmappingsources() + array(
      'guid' => array(
    	'name' => t('Unique ID'),
      ),
      'title' => array(
        'name' => t('Webcast title'),
      ),
      'summary' => array(
        'name' => t('Summary'),
        'description' => t('A short description of the webcast contents'),
      ),
      'author' => array(
        'name' => t('Author'),
        'description' => t('A string containing information about the author(s) of a webcast'),
      ),
      'channel' => array(
        'name' => t('Channel ID'),
        'description' => t('The uniqie identifier for the channel on brighttalk.com'),
      ),
      'webcast' => array(
        'name' => t('Webcast ID'),
        'description' => t('A unique identifier for this webcast'),
      ),
      'url' => array(
        'name' => t('Webcast url'),
        'description' => t('The url of the individual Brighttalk.com webcast.'),
      ),
      'date' => array(
        'name' => t('Start date and time'),
        'description' => t('The webcast start date and time (timestamp)'),
      ),
      'duration' => array(
        'name' => t('Duration'),
        'description' => t('The webcast duration, in seconds'),
      ),
      'image' => array(
        'name' => t('Webcast image'),
        'description' => t('The URL of the webcast image.'),
      ),
      'status' => array(
        'name' => t('Webcast status'),
        'description' => t('The current webcast status (recorded, live or upcoming).'),
      ),
      'tags' => array(
        'name' => t('Tags'),
        'description' => t('Freetagged taxonomy'),
      ),
      'player' => array(
        'name' => t('Player'),
        'description' => t('Embedded webcast'),
      ),
    );
  }

  /**
   * parse brighttalk channel feed
   *
   * @param simplexmlelement $sxml
   * @param feedsfetcherresult $fetcher_result
   * @param feedssource $source
   */
  private function parsechannel(simplexmlelement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {

    $result = new feedsparserresult();
    $title = (array)$sxml->title;
    $result->title = $title[0];
    $description = (array)$sxml->subtitle;
    $result->description = $description[0];
    $link = (array)$sxml->link;
    $result->link = $link['@attributes']['href'];

    // iterate over entries in feed
    $sxml->registerXPathNamespace("a", "http://www.w3.org/2005/Atom");
    $sxml->registerXPathNamespace("bt", "http://brighttalk.com/2009/atom_extensions");

    foreach ($sxml->xpath('/a:feed/a:entry') as $entry) {
      $entry_xml = $entry->asxml();
      // remove bt the namespace because bt namespace is not defined in this section
      // this is a bit of a hack. I would prefer to register the namespace correctly
      $entry_xml = preg_replace('/bt:([a-zA-Z]+[\w=>])/', '$1', $entry_xml);
      $row = simplexml_load_string($entry_xml);

      $item = array();
      $item['title'] = (string)$row->title;
      $item['summary'] = (string)$row->summary;
      $item['author'] = (string)$row->author->name;

      foreach ($row->link as $link) {
        if ($link['rel'] == 'enclosure') {
          $item['image'] = (string)$link['href'];
        }
        else if ($link['rel'] == 'alternate') {
          $item['url'] = (string)$link['href'];
        }
      }

      $item['channel'] = (string)$row->channel['id'];
      $item['webcast'] = (string)$row->communication['id'];
      $item['guid'] = $item['channel'].$item['webcast'];
      $item['date'] = (string)$row->start;
      $item['duration'] = (string)$row->duration;
      $item['status'] = (string)$row->status;

      foreach($row->category as $tag) {
        $item['tags'][]=(string)$tag['term'];
      }

      if (is_numeric($item['channel']) && is_numeric($item['webcast'])) {
        $item['player'] = '<script type="text/javascript" src="http://www.brighttalk.com/clients/js/embed/embed.js"></script>
          <object class="BrightTALKEmbed" width="705" height="660">
          <param name="player" value="channel_player"/>
          <param name="domain" value="http://www.brighttalk.com"/>
          <param name="channelid" value="' . $item['channel'] . '"/>
          <param name="communicationid" value="' . $item['webcast'] . '"/>
          <param name="autoStart" value="false"/>
          </object>';
      }

      // populate the feedsfetcherresult object with the parsed results.
      $result->items[] = $item;
    }

    return $result;
  }
}



