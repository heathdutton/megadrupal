<?php

/**
 * Class definition for Microdata Syndication Parser.
 *
 * Parses RSS and Atom feeds and retrieves microdata from url.
 */
class MicrodataImportSyndicationParser extends MicrodataImportParser {

  /**
   * Override parent::parse().
   *
   * This function replicates the FeedsSyndicationParser code to create the
   * list of items, then crawls the items' pages (respecting the Crawl-delay
   * directive in robot.txt) to gather extra microdata and attaches the
   * microdata to the item.
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    feeds_include_library('common_syndication_parser.inc', 'common_syndication_parser');
    $feed = common_syndication_parser_parse($fetcher_result->getRaw());
    $result = new FeedsParserResult();
    $result->title = $feed['title'];
    $result->description = $feed['description'];
    $result->link = $feed['link'];
    if (is_array($feed['items'])) {
      foreach ($feed['items'] as $item) {
        if (isset($item['geolocations'])) {
          foreach ($item['geolocations'] as $k => $v) {
            $item['geolocations'][$k] = new FeedsGeoTermElement($v);
          }
        }
        $result->items[] = $item;
      }
    }

    microdata_import_include_library();
    foreach ($result->items as &$item) {
      $url = $item['url'];
      // Get the domain of the item.
      preg_match("/(https{0,1}:\\/\\/[^\\/]*)\\/*.*/i", $url, $matches);
      $domain = $matches[1];

      // If this domain isn't in the log yet, get the crawl delay for it.
      if (!isset($this->crawl_log[$domain])) {
        $robots = file_get_contents("$domain/robots.txt");

        // If the Crawl-delay directive is found, store it.
        if (preg_match("/crawl-delay[^:]*:([^#^$]+)/i", $robots, $directive)) {
          $delay = trim($directive[1]);
        }
        // Otherwise, default to 10.
        // @todo Possibly make this default configurable through the UI.
        else {
          $delay = 10;
        }

        $this->crawl_log[$domain]['delay'] = $delay;
      }
      // Otherwise, check to see when the site was last accessed and sleep
      // until we can access it again.
      else {
        $elapsed = time() - $this->crawl_log[$domain]['last_access'];
        if ($elapsed < $this->crawl_log[$domain]['delay']) {
          sleep($this->crawl_log[$domain]['delay'] - $elapsed);
        }
      }

      $this->crawl_log[$domain]['last_access'] = time();
      $md = new MicrodataPhp($url);
      $data = $md->obj();
      $item['microdata'] = $data->items;
    }
    return $result;
  }

  /**
   * Return mapping sources.
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
      'author_name' => array(
        'name' => t('Author name'),
        'description' => t('Name of the feed item\'s author.'),
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
      'tags' => array(
        'name' => t('Categories'),
        'description' => t('An array of categories that have been assigned to the feed item.'),
      ),
      'geolocations' => array(
        'name' => t('Geo Locations'),
        'description' => t('An array of geographic locations with a name and a position.'),
      ),
     ) + parent::getMappingSources();
  }
}
