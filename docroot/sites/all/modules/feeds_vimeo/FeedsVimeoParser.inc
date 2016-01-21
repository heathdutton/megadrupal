<?php

/**
 * @file
 * Feeds parser class for Vimeo
 */

/**
 * Class definition for Vimeo Parser.
 *
 * Parses XML feeds from Vimeo, e.g. http://vimeo.com/api/v2/channel/nytimes/videos.xml
 */
class FeedsVimeoParser extends FeedsParser {

  /**
   * Parse the extra mapping sources provided by this parser.
   *
   * @param $fetcher_result FeedsFetcherResult
   * @param $source FeedsSource
   *
   * @see FeedsParser::parse()
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $vimeo_feed = $fetcher_result->getRaw();
    $result = new FeedsParserResult();

    if (!defined('LIBXML_VERSION') || (version_compare(phpversion(), '5.1.0', '<'))) {
      @$sxml = simplexml_load_string($vimeo_feed, NULL);
    }
    else {
      @$sxml = simplexml_load_string($vimeo_feed, NULL, LIBXML_NOERROR | LIBXML_NOWARNING | LIBXML_NOCDATA);
    }

    // Got a malformed XML.
    if ($sxml === FALSE || is_null($sxml)) {
      throw new Exception(t('FeedsVimeoParser: Malformed XML source.'));
    }

    $result = $this->parseVimeo($sxml, $source, $fetcher_result);

    return $result;
  }

  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'id' => array(
        'name' => t('ID'),
      ),
      'title' => array(
        'name' => t('Video title'),
      ),
      'description' => array(
        'name' => t('Description'),
      ),
    	'url' => array(
        'name' => t('Vimeo URL'),
        'description' => t('The URL the the Vimeo page of the video.'),
      ),
    	'published_datetime' => array(
        'name' => t('Published on (Datetime)'),
      ),
      'published_timestamp' => array(
        'name' => t('Published on (Timestamp)'),
      ),
    	'thumbnail_large' => array(
        'name' => t('Thumbnail (large)'),
        'description' => t('URL of the large thumbnail of the image.'),
      ),
      'author' => array(
        'name' => t('Author\'s name'),
        'description' => t('The user who uploaded the video.'),
      ),
      'author_url' => array(
        'name' => t('Author\'s Vimeo URL'),
        'description' => t('The Vimeo URL of the user who uploaded the video.'),
      ),
      'tags' => array(
        'name' => t('Tags'),
      ),
      'embed_privacy' => array(
        'name' => t('Embed privacy'),
        'description' => t('Describes where these video can be embedded. Possibly values include "anywhere" and "nowhere". Perhaps this could be integrated with the published state of the node.'),
      ),
    );
  }

  /**
   * Parse Vimeo feed
   *
   * @param SimpleXMLElement $sxml
   * @param FeedsFetcherResult $fetcher_result
   * @param FeedsSource $source
   */
  private function parseVimeo(SimpleXMLElement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $result = new FeedsParserResult();

    // Iterate over entries in feed
    foreach ($sxml->xpath('//video') as $entry) {

      $item = array(
        'id' => (string) $entry->id,
      	'title' => (string) $entry->title,
      	'description' => html_entity_decode((string) $entry->description),
        'url' => (string) $entry->url,
      	'published_datetime' => date('Y-m-d H:i:s', strtotime($entry->upload_date)),
      	'published_timestamp' => strtotime($entry->upload_date),
      	'thumbnail_large' => (string) $entry->thumbnail_large,
        'author' => (string) $entry->user_name,
      	'author_url' => (string) $entry->user_url,
      	'tags' => explode(', ', $entry->tags),
      	'embed_privacy' => (string) $entry->embed_privacy,
      );

      // Populate the FeedsFetcherResult object with the parsed results.
      $result->items[] = $item;
    }

    return $result;
  }

}
