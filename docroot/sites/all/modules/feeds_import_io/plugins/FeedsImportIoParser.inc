<?php
/**
 * @file
 * Custom FeedsParser.
 */

class FeedsImportIoParser extends FeedsParser {
  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $json = $fetcher_result->getRaw();
    $result = new FeedsParserResult($json['results']);
    $result->title = $json['title'];
    $result->link = $json['link'];
    return $result;
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {
    return FALSE;
  }
}
