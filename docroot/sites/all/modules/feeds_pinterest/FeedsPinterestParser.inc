<?php
/**
 * @file
 *  Feeds parser class for Pinterest.
 */

/**
 * Class definition for Pinterest Parser.
 *
 * Parse RSS feeds.
 */
class FeedsPinterestParser extends FeedsSimplePieParser {

  /**
   * Parse the extra mapping sources provided by this parser.
   */
  protected function parseExtensions(&$item, $simplepie_item) {
    // Extract media fields out of the enclosure.
		// --not currently used.
		/*
    $enclosure = $simplepie_item->get_enclosure();
    if (empty($enclosure) || !is_object($enclosure)) {
      return;
    }
    */
    
    $doc = new DOMDocument();
    $doc->loadHTML($item['description']);
    $item['pinterest_photo'] = $doc->getElementsByTagName('img')->item(0)->getAttribute('src');
    
  }

  /**
   * Add the extra mapping sources provided by this parser.
   */
  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'pinterest_photo' => array(
        'name' => t('Pinterest: Photo'),
        'description' => t('Pinterest: Pinned photograph.'),
      ),
    );
  }

}
