<?php

/**
 * @file
 * Contains a FeedsKMLParser
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

/**
 * Feeds parser plugin that parses OPML feeds.
 */
class FeedsKMLParser extends FeedsParser {

  /**
   * Overrides FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    feeds_include_library('kml_parser.inc', 'kml_parser');
    $kml = kml_parser_parse($fetcher_result->getRaw());
    $result = new FeedsParserResult($kml['items']);
    $result->title = $kml['title'];
    return $result;
  }

  /**
   * Return mapping sources.
   */
  public function getMappingSources() {
    return array(
      'name' => array(
        'name' => t('Location Title'),
        'description' => t('Title of the location area.'),
      ),
      'description' => array(
        'name' => t('Location Description'),
        'description' => t('Description for maps popup.'),
      ),
      'geometry' => array(
        'name' => t('Location Overlay Geometry'),
        'description' => t('Coordinates for maps overlay.'),
      ),
    ) + parent::getMappingSources();
  }
}
