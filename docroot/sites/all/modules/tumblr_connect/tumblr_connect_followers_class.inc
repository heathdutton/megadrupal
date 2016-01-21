<?php

/**
 * @file
 * tumblr followers
 * referred to as statistics to allow for enhancements such as number of
 * times our posts have been reblogged, voted up, etc.
 * Thus, the initial statistic is how many followers and who they are
 */

/*
 * Represents a follower who resides on the remote tumblr service.
 */
interface TumblrFollowerInterface {

  /**
   * Gets the image url property.
   *
   * @return string
   *   The image url property.
   * .
   */
  public function getImage();

  /**
   * Sets the image url property.
   *
   * @param $image
   *   The image url of this follower..
   */
  public function setImage($image);

  /**
   * Gets the name property.
   *
   * @return string
   *   The name of this follower..
   * .
   */
  public function getName();

  /**
   * Sets the name property.
   *
   * @param $name
   *   The name of this follower..
   */
  public function setName($name);

  /**
   * Gets the url of this follower's tumblr page.
   *
   * @return string
   *   The he url of this follower's tumblr page.
   * .
   */
  public function getUrl();

  /**
   * Sets the url of this follower's tumblr page.
   *
   * @param $url
   *   The he url of this follower's tumblr page.
   */
  public function setUrl($url);
}

/*
 * Represents a follower who resides on the remote tumblr service.
 *
 * Default implementation of TumblrFollowerInterface.
 */

class TumblrFollower implements TumblrFollowerInterface {

  /**
   * Image url of tubmlr remote service follower.
   *
   * @var string.
   */
  protected $image;

  /**
   * Name of tubmlr remote service follower.
   *
   * @var string.
   */
  protected $name;

   /**
   * Url of tubmlr remote service follower.
   *
   * @var string.
   */
  protected $url;

  /**
   * Local unique identifier of tubmlr remote service follower.
   *
   * @var integer
   */
  protected $id;

  /**
   * Constructs a TumblrFollower object.
   *
   * @param string $image
   *   Image of remote tumblr follower..
   * @param string $name
   *   Name of remote tumblr follower.
   * @param string $url
   *   Url of remote tumblr follower.
   * @param string $id
   *  Local unique identifier of tumblr follower.
   *
   */
  function __construct($image, $name, $url, $id = NULL) {
    $this->image = $image;
    $this->name = $name;
    $this->url = $url;
    if (isset($id)) {
      $this->id = $id;
    }
  }

  public function getImage() {
    return $this->image;
  }

  public function setImage($image) {
    $this->image = $image;
  }

  public function getName() {
    return $this->name;
  }

  public function setName($name) {
    $this->name = $name;
  }

  public function getUrl() {
    return $this->url;
  }

  public function setUrl($url) {
    $this->url = $url;
  }

  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }

  /**
   * Implements TumblrFollowerInterface::toArray().
   */
  public function toArray() {
    return array(
        'id' => $this->id,
        'name' => $this->name,
        'url' => $this->url,
        'image' => $this->image,
    );
  }

}

/**
 * Represents database access for tumblr follower data.
 */

class DbTumblrFollower {

  /**
   * Adds tumblr follower to the database.
   *
   * @return TumblrFollower
   *   The tumblr follower object.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function add(TumblrFollower $follower) {
    try {
      $obj_array = $follower->toArray();
      db_insert('tumblr_follower')
        ->fields($obj_array)
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to add tumblr follower to the database'));
    }
    return $follower;
  }

  /**
   * Modifies tumblr follower in the database.
   *
   * @return TumblrFollower
   *   The tumblr follower object.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function modify(TumblrFollower $follower) {
    try {
      $obj_array = $follower->toArray();
      db_update('tumblr_follower')
        ->fields($obj_array)
        ->condition('id', $follower->getId())
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to update tumblr follower to the database'));
    }
    return $follower;
  }

  /**
   * Deletes tumblr follower from the database.
   *
   * @return TumblrFollower
   *   The tumblr follower object.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function delete($id) {
    try {
      $result = db_delete('tumblr_follower')
        ->condition('id', $id)
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t('Unable to delete tumblr follower from the database'));
    }
    return $result;
  }

  /**
   * Add a list of tumblr followers to the database.
   *
   * @param TumblrFollowerList $list
   *   List of tumblr follower objects.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function setAll(TumblrFollowerList $list) {

    try {
      $result = db_delete('tumblr_follower')->execute();
      foreach ($list->getList() as $follower) {
        $this->add($follower);
      }
    } catch (Exception $e) {
      throw tumblr_database_exception($e->getMessage());
    }
  }

  /**
   * Gets a list of all tumblr followers from the database.
   *
   * @return TumblrFollowerList $list
   *   List of tumblr follower objects.
   *
   * @throws TumblrConnectDatabaseException
   */
  public function pagerGetAll() {

    $query = 'SELECT id, name, image, url from {tumblr_follower}';
    $rows = db_query('SELECT id, name, image, url from {tumblr_follower}');

    if (!$rows) {
      throw new TumblrConnectDatabaseException(t('Unable select al followers from the database.'));
    }
    $list = new TumblrFollowerList();
    foreach ($rows as $obj) {
      $follower = new TumblrFollower($obj->image, $obj->name, $obj->url, $obj->id);
      $list->addFollower($follower);
    }
    return $list;
  }

}

/*
 * Represents a list of tumble followers.
 */

interface TumblrFollowerListInterface {

  /**
   * Gets a list of tumblr followers.
   *
   * @return array
   *   A list of tumblr followers.
   */
  public function getList();

  /**
   * Sets list of tumblr followers.
   *
   * @param array $list
   *   Array of tumblr follower objects.
   */
  public function setList($list);

  /**
   * Adds a single tumblr follower to this list.
   *
   * @param TumblrFollowerInterface $follower
   *   A single tumblr follower.
   */
  public function addFollower(TumblrFollowerInterface $follower);
}

/*
 * Represents a list of tumble followers.
 *
 * Default implementation of TumblrFollowerListInterface.
 */

class TumblrFollowerList implements TumblrFollowerListInterface {

  protected $list;

  function __construct() {
    $this->list = array();
  }

  public function getList() {
    return $this->list;
  }

  public function setList($list) {
    $this->list = $list;
  }

  public function addFollower(TumblrFollowerInterface $follower) {
    $this->list[] = $follower;
  }

}

/*
 * Represents a remote procedure call to get tumblr followers.
 *
 * Requests followers from the remote tumblr service.
 * Caches an image of the followers in our local Drupal database.
 */

class TumblrStatisticsRequestor {

  function __construct() {

  }

  /*  Processes list of followers from remote tumblr service.
   *
   *  @return TumblrFollowerList $list
   *   List of tumblr follower objects.
   *
   *  Details:
   *  Requests list of followers from remote tumblr service.
   *  Processes response, extracting follower data from html response message.
   *  Stores followers in the local datbase.
   */
  public function getFollowersFromTumblr() {
    try {
      $conf = DbTumblrConnectConfig::factory()->getCachedConfig();
    } catch (TumblrConnectDatabaseException $te) {
      throw $te;
    }
    $request_data = "email=" . $conf->getEmail() . "&password=" . $conf->getPassword();
    $temp_file_path = file_directory_temp();
    $cookie_url = $temp_file_path . '/cookie.txt';

    for ($index = 0; $index < 2; $index++) {
      switch ($index) {
        case 0 :
          $url = TumblrGateway::LOGIN;
          break;
        case 1:
          $url = TumblrGateway::FOLLOWERS;
          break;
      }
      // Note! Logical request failed using the builtin drupal method drupal_http_request.
      // Therefore we are using curl.
      $c = curl_init($url);
      curl_setopt($c, CURLOPT_POST, TRUE);
      curl_setopt($c, CURLOPT_POSTFIELDS, $request_data);
      curl_setopt($c, CURLOPT_COOKIEFILE, $cookie_url);
      curl_setopt($c, CURLOPT_COOKIEJAR, $cookie_url);
      curl_setopt($c, CURLOPT_USERAGENT, TumblrGateway::USER_AGENT);
      curl_setopt($c, CURLOPT_REFERER, TumblrGateway::LOGIN);
      curl_setopt($c, CURLOPT_FOLLOWLOCATION, 1);
      curl_setopt($c, CURLOPT_RETURNTRANSFER, TRUE);
      $result = curl_exec($c);
      $status = curl_getinfo($c, CURLINFO_HTTP_CODE);
    }
    // use the dom library to scrape the individual followers
    $follower_list = new TumblrFollowerList();
    $html = new simple_html_dom();
    $html->load($result);
    $elements = $html->find('.follower a');
    foreach ($elements as $element) {
      if (!empty($element->plaintext) && !empty($element->href)) {
        $name_text = $element->innertext;
        $link_text = $element->href;
        if ($name_text != 'block') {
          $follower = new TumblrFollower($image_src, $name_text, $link_text);
          $follower_list->addFollower($follower);
        }
      }
      else {
        $image = $element->innertext;
        $image_src = $element->children[0]->getAttribute('src');
      }
    }
    curl_close($c);
    return $follower_list;
  }

}