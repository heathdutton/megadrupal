<?php
/**
 * @file
 * Facebook feeds parser class.
 */

/**
 * Class definition for Facebook Parser.
 */
class FeedsFacebookParser extends FeedsParser {

  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $facebook_feed = $fetcher_result->getRaw();
    $result = new FeedsParserResult();

    $json = json_decode($facebook_feed);

    $items = array();

    foreach ($json->data as $entry) {
      if (!isset($entry->to)) {
        $items[] = array(
          'post_id' => $entry->id,
          'from_name' => $entry->from->name,
          'from_category' => $entry->from->category,
          'from_id' => $entry->from->id,
          'to_name' => (isset($entry->to) ? $entry->to->data[0]->name : ''),
          'to_category' => (isset($entry->to) ? $entry->to->data[0]->category : ''),
          'to_id' => (isset($entry->to) ? $entry->to->data[0]->id : ''),
          'message' => $entry->message,
          'picture' => (isset($entry->picture) ? $entry->picture : ''),
          'link' => (isset($entry->link) ? $entry->link : ''),
          'source' => (isset($entry->source) ? $entry->source : ''),
          'name' => (isset($entry->name) ? $entry->name : ''),
          'caption' => (isset($entry->caption) ? $entry->caption : ''),
          'description' => (isset($entry->description) ? $entry->description : ''),
          'icon' => (isset($entry->icon) ? $entry->icon : ''),
          'type' => (isset($entry->type) ? $entry->type : ''),
          'likes_count' => (isset($entry->likes) ? $entry->likes->count : 0),
          'likes' => (isset($entry->likes) ? json_encode($entry->likes) : ''),
          'comments_count' => (isset($entry->comments) ? $entry->comments->count : 0),
          'comments' => (isset($entry->comments) ? json_encode($entry->comments) : ''),
          'created_time' => $entry->created_time,
          'updated_time' => $entry->updated_time,
        );
      }
    }
    $result->items = $items;
    return $result;
  }

  public function getMappingSources() {
    return parent::getMappingSources() + array(
      'post_id' => array(
        'name' => t('Post id'),
        'description' => t('Facebook post id'),
      ),
      'from_name' => array(
        'name' => t('From: name'),
        'description' => t(''),
      ),      
      'from_category' => array(
        'name' => t('From: category'),
        'description' => t(''),
      ),
      'from_id' => array(
        'name' => t('From: id'),
        'description' => t(''),
      ),
      'to_name' => array(
        'name' => t('To: name'),
        'description' => t(''),
      ),      
      'to_category' => array(
        'name' => t('To: category'),
        'description' => t(''),
      ),
      'to_id' => array(
        'name' => t('To: id'),
        'description' => t(''),
      ),  
      'message' => array(
        'name' => t('Message'),
        'description' => t(''),
      ),
      'picture' => array(
        'name' => t('Picture'),
        'description' => t('Picture or screenshot of the post'),
      ),
      'name' => array(
        'name' => t('Name'),
        'description' => t('Post name, could be a file name'),
      ),
      'link' => array(
        'name' => t('Link'),
        'description' => t(''),
      ),
      'source' => array(
        'name' => t('Source'),
        'description' => t('Music file source'),
      ),      
      'description' => array(
        'name' => t('Description'),
        'description' => t(''),
      ),
      'icon' => array(
        'name' => t('Icon'),
        'description' => t(''),
      ),
      'type' => array(
        'name' => t('Type'),
        'description' => t('Type of post'),
      ),
      'likes' => array(
        'name' => t('Likes'),
        'description' => t('Likes json encoded'),
      ),
      'likes_count' => array(
        'name' => t('Likes count'),
        'description' => t('Number of likes'),
      ),      
      'comments' => array(
        'name' => t('Comments'),
        'description' => t('Comments json encoded'),
      ),
      'comments_count' => array(
        'name' => t('Comments count'),
        'description' => t('Number of comments'),
      ),            
      'created_time' => array(
        'name' => t('Created time'),
        'description' => t('Date/time'),
      ),
      'updated_time' => array(
        'name' => t('Updated time'),
        'description' => t('Date/time'),
      ),
    );
  } 
}
