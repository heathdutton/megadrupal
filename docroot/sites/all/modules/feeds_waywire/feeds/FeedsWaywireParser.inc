<?php

/**
 * @file
 * Feeds parser class for Waywire.
 */

/**
 * Parses a waywire feed.
 */
class FeedsWaywireParser extends FeedsParser {
  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $result = new FeedsParserResult();

    $xml = new SimpleXMLElement($fetcher_result->getRaw());

    $result = $this->parseAtom($xml, $source, $fetcher_result);

    return $result;
  }

  /**
   * Parse Atom feed.
   *
   * @param SimpleXMLElement $sxml
   *   Simple XMLElement.
   * @param FeedsSource $source
   *   The Feed source.
   * @param FeedsFetcherResult $fetcher_result
   *   The FeedsFetcherResult.
   */
  private function parseAtom(SimpleXMLElement $sxml, FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $result = new FeedsParserResult();

    $fetcher_result->title = $feed_title = (string) $sxml->title;

    // Iterate over entries in feed.
    foreach ($sxml->entry as $entry) {

      $item = array(
        'title' => (string) $entry->title,
        'id' => (string) $entry->id,
        'magnify_id' => (string) $entry->children('magnify', TRUE)->id,
        'magnify_externalid' => (string) $entry->children('magnify', TRUE)->externalid,
        'updated' => (string) $entry->updated,
        'author_id' => (string) $entry->author->children('magnify', TRUE)->id,
        'author_name' => (string) $entry->author->name,
        'author_uri' => (string) $entry->author->uri,
        'content' => (string) $entry->content,
        'thumbnail' => (string) $entry->children('media', TRUE)->thumbnail->attributes()->url,
        'published' => (string) $entry->published,
        'media_width' => (string) $entry->children('media', TRUE)->content->attributes()->width,
        'media_medium' => (string) $entry->children('media', TRUE)->content->attributes()->medium,
        'media_url' => (string) $entry->children('media', TRUE)->content->attributes()->url,
        'media_duration' => (string) $entry->children('media', TRUE)->content->attributes()->duration,
        'media_type' => (string) $entry->children('media', TRUE)->content->attributes()->type,
        'media_height' => (string) $entry->children('media', TRUE)->content->attributes()->height,
      );

      // Get link attributes.
      foreach ($entry->link as $link) {
        $rel = (string) $link->attributes()->rel;
        $href = (string) $link->attributes()->href;
        $item['link_' . $rel] = $href;
      }

      // Get all categories.
      $categories = array();
      foreach ($entry->category as $category) {
        $label = (string) $category->attributes()->label;
        $term = (string) $category->attributes()->term;
        $categories[$label] = $term;
        $item['categories'] = json_encode($categories);
      }

      // Populate the FeedsFetcherResult object with the parsed results.
      $result->items[] = $item;
    }

    return $result;
  }

  /**
   * Implements FeedsParser::getMappingSources().
   */
  public function getMappingSources() {
    $sources['title'] = array(
      'name' => t('title'),
      'description' => t('Entry title.'),
    );
    $sources['id'] = array(
      'name' => t('id'),
      'description' => t('Entry id.'),
    );
    $sources['magnify_id'] = array(
      'name' => t('magnify_id'),
      'description' => t('Entry magnify id (waywire id).'),
    );
    $sources['magnify_externalid'] = array(
      'name' => t('magnify_externalid'),
      'description' => t('Entry magnify external id (waywire external id).'),
    );
    $sources['updated'] = array(
      'name' => t('updated'),
      'description' => t('Date entry was updated.'),
    );
    $sources['author_id'] = array(
      'name' => t('author_id'),
      'description' => t('Author ID who created the video in Waywire.'),
    );
    $sources['author_name'] = array(
      'name' => t('author_name'),
      'description' => t('Author name who created the video in Waywire.'),
    );
    $sources['author_uri'] = array(
      'name' => t('author_uri'),
      'description' => t('Author uri who created the video in Waywire.'),
    );
    $sources['content'] = array(
      'name' => t('content'),
      'description' => t('Entry content.'),
    );
    $sources['thumbnail'] = array(
      'name' => t('thumbnail'),
      'description' => t('Entry thumbnail.'),
    );
    $sources['published'] = array(
      'name' => t('published'),
      'description' => t('Entry published date.'),
    );
    $sources['media_width'] = array(
      'name' => t('media_width'),
      'description' => t('Media Width.'),
    );
    $sources['media_medium'] = array(
      'name' => t('media_medium'),
      'description' => t('Media Medium.'),
    );
    $sources['media_url'] = array(
      'name' => t('media_url'),
      'description' => t('Media URL.'),
    );
    $sources['media_duration'] = array(
      'name' => t('media_duration'),
      'description' => t('Media Duration.'),
    );
    $sources['media_type'] = array(
      'name' => t('media_type'),
      'description' => t('Media Type.'),
    );
    $sources['media_height'] = array(
      'name' => t('media_height'),
      'description' => t('Media Height.'),
    );
    $sources['link_self'] = array(
      'name' => t('link_self'),
      'description' => t('Entry link_self value.'),
    );
    $sources['link_alternative'] = array(
      'name' => t('link_alternative'),
      'description' => t('Entry link_alternative value.'),
    );
    $sources['link_author'] = array(
      'name' => t('link_author'),
      'description' => t('Entry link_author value.'),
    );
    $sources['categories'] = array(
      'name' => t('categories'),
      'description' => t('JSON string of tags for the entry.'),
    );

    return $sources;
  }
}
