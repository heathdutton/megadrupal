<?php

/**
 * @file
 * main interface.
 */

/**
 * Interface definition of message operations with tumblr web service.
 */
interface TumblrConnectionInterface {

  /**
   * Deletes remote content on remote tumblr service.
   *
   * @param: $node
   *   node object populated with nid and vid field values.
   *
   * @return $operationStatus
   *   TRUE upon sucess, Scheduled in batch queue upon failure.
   *
   * @throws TumblrConnectException
   */
  function deletePost($node);

  /**
   * Updates remote content on tumblr.
   *
   * @param $node
   *   node object related to the remote tumblr post we are updating.
   * @param $format
   *   a value defined by tumblr (html or plain text), we are defaulting to html.
   * @param $postType
   *    a value defined by tumblr - we are defaulting to a regular tumblr type (similar to a drupal node)
   *
   * @return $operationStatus
   *   TRUE upon sucess, Scheduled in batch queue upon failure.
   *
   * @throws TumblrConnectException
   */
  function modifyPost($node, $format = TumblrConnection::DEFAULT_FORMAT, $postType = TumblrPostTypes::REGULAR);

  /**
   * Adds new content to the remote tumblr service.
   *
   * @param $node
   *   Node object related to the remote tumblr post we are updating.
   * @param $format
   *   A value defined by tumblr (html or plain text), we are defaulting to html.
   * @param $postType
   *   A value defined by tumblr - we are defaulting to a regular tumblr type (similar to a drupal node)
   *
   * @return $operationStatus
   *   TRUE upon sucess, Scheduled in batch queue upon failure.
   */
  function addPost($node, $format = TumblrConnection::DEFAULT_FORMAT, $postType = TumblrPostTypes::REGULAR);
}

/**
 * Class implementation of message operations with tumblr web service.
 */
class TumblrConnection implements TumblrConnectionInterface {

  /**
   * Request message content bound for tubmlr remote service.
   *
   * @var object.
   */
  protected $msg_request;

  /**
   * Generator name required by tubmlr remote service.
   *
   * @var string.
   */
  protected $generator;

  /**
   * Response message received from tubmlr remote service.
   *
   * @var TumblrResponse.
   */
  protected $response;

  /**
   * Email credential required to connect to the tubmlr remote service.
   *
   * @var string.
   */
  protected $email;

  /**
   * Password credential required to connect to the tubmlr remote service.
   *
   * @var string.
   */
  protected $password;

  /**
   * Command message bound for tubmlr remote service.
   *
   * @var TumblrRequestCommand.
   */
  protected $command;

  /**
   * Relationship between local content and tumblr remote service content.
   *
   * @var TumblrConnectNode.
   */
  protected $tumblrNode;

  /**
   * Persists relationship between local content and tumblr remote service content.
   *
   * @var DbTumblrConnectNode.
   */
  protected $dbTumblrNode;

  /**
   * Batch command request object.
   *
   * @var TumblrConnectBatch.
   */
  protected $batchRequest;

  /**
   * Persists batch command requests.
   *
   * @var DbTumblrConnectBatch.
   */
  protected $dbBatchRequest;

  /**
   * Sets content view.
   *
   * @var boolean.
   *  TRUE for teaser view, FALSE for full view.
   */
  protected $teaserOnly;
  protected static $instance = NULL;

  const DEFAULT_FORMAT = 'html';
  const DEFAULT_GENERATOR = 'public-action';

  private function __construct() {

  }

  /**
   * Sets multiple properties.
   *
   * @var sting $email
   *  Email credential for this connection.
   * @var sting $password
   *  Password credential for this connection.
   * @var sting $generator
   *  Generator name for this connection.
   * @var boolean $teaser_only
   *  TRUE sets teaser view, FALSE sets full view.
   */
  public static function factory($email = NULL, $password = NULL, $generator = TumblrConnection::DEFAULT_GENERATOR, $teaser_only = TRUE) {
    if (self::$instance === NULL) {
      self::$instance = new TumblrConnection();
      self::$instance->init($email, $password, $generator, $teaser_only);
    }
    return self::$instance;
  }

  /**
   * Prepares and constructs connection fields.
   *
   * @param $email
   *   email address used to authenticate our remote tunblr account.
   * @param $password
   *   password used to authenticate our remote tumblr account.
   * @param $generator_name
   *   a value the tumblr service uses to identify the requestor
   * @param $teaser_only
   *   TRUE means we only send the node teaser to tumblr, FALSE means we send the whole node body to tumblr
   *
   */
  function init($email, $password, $generator_name, $teaser_only) {

    $initConfig = $this->getInitConfig();

    if (is_null($email)) {
      $this->email = $initConfig['email'];
    } else {
      $this->email = $email;
    }

    if (is_null($password)) {
      $this->password = $initConfig['password'];
    } else {
      $this->password = $password;
    }

    $this->msg_request = new TumblrRequestRegular();
    $this->generator = new TumblrConnectGenerator();
    $this->generator->setGenerator($generator_name);
    $this->generator->setEmail($this->email);
    $this->generator->setPassword($this->password);
    $this->teaserOnly = $initConfig['teaserOnly'];

    $this->command = new TumblrRequestCommand();

    $obj = new stdClass;
    $obj->code = '200';
    $this->response = new TumblrResponse($obj);
    $this->tumblrNode = new TumblrConnectNode();
    $this->dbTumblrNode = new DbTumblrConnectNode();
    $this->batchRequest = new TumblrConnectBatch();
    $this->dbBatchRequest = new DbTumblerConnectBatch();
  }

  /**
   * Implements TumblrConnectionInterface::addPost().
   *
   * Method adds new content to remote tumblr.
   * Method alwas stores a new tumblr_connect_node record that defines a relationship with
   * or local Drupal node and the intended remote tumblr post.
   * If remote add fails (i.e. tumblr service not availalbe), method adds
   * a batch request record to our database. This module includes an implentation of
   * hook_cron() which uses the batch request record to attempt an add new content later.
   * @see tumblr_connect_cron()
   */
  function addPost($node, $format = TumblrConnection::DEFAULT_FORMAT, $postType = TumblrPostTypes::REGULAR) {

    $this->msg_request->setType($postType);
    $this->msg_request->setFormat($format);
    switch ($node->status) {
      case 0:
        $this->msg_request->setState(TumblrPublishingStates::DRAFT);
        break;
      case 1:
        $this->msg_request->setState(TumblrPublishingStates::PUBLISHED);
        break;
    }

    $this->msg_request->setTitle($node->title);
    $node_language = $node->language;
    $node_body = $node->body[$node_language][0]['value'];
    $teaser = $node->body[$node_language][0]['summary'];
    if (empty($teaser)) {
      $teaser = $node_body;
      $teaser .= '&nbsp;';
    }
    if ($this->teaserOnly) {
      $read_more = $this->buildReadMore($node->nid);
      $this->msg_request->setBody(urlencode($this->expandUrlPath($teaser . $read_more)));
    } else {
      $this->msg_request->setBody(urlencode($this->expandUrlPath($node_body)));
    }

    $tags = tumblr_connect_get_taxonomy_terms($node);
    if (!empty($tags)) {
      $this->msg_request->setTags($tags);
    }

    // let's put a try around this whole block if any of it fails we want
    // to schedule a batch request
    $operationStatus = TumblrConnectDefinitions::SUCCESS;
    try {
      $this->response->init($this->generator->addNew($this->msg_request));

      if (is_numeric($this->response->getData())) {
        $this->tumblrNode->setPostId($this->response->getData());
        $this->tumblrNode->init($node->nid, $node->vid);
        $this->dbTumblrNode->add($this->tumblrNode);
      } else {
        $this->scheduleAction($node, TumblrConnectBatch::ADD);
        $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
      }
    } catch (TumblrConnectRemoteException $re) {
      $this->scheduleAction($node, TumblrConnectBatch::ADD);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    } catch (TumblrConnectDatabaseException $de) {
      $this->scheduleAction($node, TumblrConnectBatch::ADD);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    }
    return $operationStatus;
  }

  /**
   * Implements TumblrConnectionInterface::modifyPost().
   *
   * Method updates remote content on tumblr.
   * Method always updates that tumblr_connect_node record that defines a relationship with
   * the local Drupal node and the remote tumblr post.
   * If remote update fails (i.e. tumblr service not availalbe), method adds
   * a batch request record to our database. This module includes an implentation of
   * hook_cron() which uses the batch request record to attempt an update later.
   * @see tumblr_connect_cron()
   */
  function modifyPost($node, $format = TumblrConnection::DEFAULT_FORMAT, $postType = TumblrPostTypes::REGULAR) {

    try {
      // we need to get the record so we can support tumblr draft to publish state
      $tumblr_node = $this->dbTumblrNode->getByNode($node->nid, $node->vid);
    } catch (TumblrConnectDatabaseNoRecordException $nrf) {
      return TumblrConnectDefinitions::SUCCESS;
    } catch (TumblrConnectDatabaseException $de) {
      throw new TumblrConnectException(t('During Modify Tumblr Post unable find local record node nid: @nid', array('@nid' => $node->nid)));
    }

    $this->msg_request->setType($postType);
    $this->msg_request->setFormat($format);

    switch ($node->status) {
      case 0:
        $this->msg_request->setState(TumblrPublishingStates::DRAFT);
        break;
      case 1:
        $this->msg_request->setState(TumblrPublishingStates::PUBLISHED);
        break;
    }

    $node_language = $node->language;
    $node_body = $node->body[$node_language][0]['value'];
    $teaser = $node->body[$node_language][0]['summary'];
    if (empty($teaser)) {
      $teaser = $node_body;
      $teaser .= '&nbsp;';
    }
    if ($this->teaserOnly) {
      $read_more = $this->buildReadMore($node->nid);
      $this->msg_request->setBody(urlencode($this->expandUrlPath($teaser . $read_more)));
    } else {
      $this->msg_request->setBody(urlencode($this->expandUrlPath($node_body)));
    }
    $this->msg_request->setTitle($node->title);

    $tags = tumblr_connect_get_taxonomy_terms($node);
    if (!empty($tags)) {
      $this->msg_request->setTags($tags);
    }

    $this->command->setPostId($tumblr_node->getPostId());
    $this->command->setRequest($this->msg_request);

    $operationStatus = TumblrConnectDefinitions::SUCCESS;
    try {
      $this->response->init($this->generator->update($this->command));
      if (is_numeric($this->response->getData())) {
        $this->tumblrNode->setPostId($this->response->getData());
        $this->tumblrNode->setNid($node->nid);
        $this->tumblrNode->setVid($node->vid);
        $this->tumblrNode->setPostId($tumblr_node->getPostId());
        $this->dbTumblrNode->modify($this->tumblrNode);
      } else {
        $this->batchRequest->setPostId($tumblr_node->getPostId());
        $this->scheduleAction($node, TumblrConnectBatch::UPDATE);
        $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
      }
    } catch (TumblrConnectRemoteException $re) {
      $this->batchRequest->setPostId($tumblr_node->getPostId());
      $this->scheduleAction($node, TumblrConnectBatch::UPDATE);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    } catch (TumblrConnectDatabaseException $de) {
      $this->batchRequest->setPostId($tumblr_node->getPostId());
      $this->scheduleAction($node, TumblrConnectBatch::UPDATE);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    }
    return $operationStatus;
  }

  /**
   *  Implements TumblrConnectionInterface::deletePost()
   *
   * Method deletes remote content on tumblr.
   * If remote delete fails (i.e. tumblr service not availalbe), method adds
   * a batch request record to our database. This module includes an implentation of
   * hook_cron() which uses the batch request record to attempt an delete later.
   * @see tumblr_connect_cron()
   */
  function deletePost($node) {

    try {
      $tumblr_node = $this->dbTumblrNode->getByNode($node->nid, $node->vid);
    } catch (TumblrConnectDatabaseNoRecordException $nrf) {
      return TumblrConnectDefinitions::SUCCESS;
    } catch (TumblrConnectDatabaseException $ge) {
      // if we can't find this, nothing to do
      throw new TumblrConnectException(t("Could Not Find Corresponding Tumblr Connect Record"));
    }

    $this->command->setPostId($tumblr_node->getPostId());
    $operationStatus = TumblrConnectDefinitions::SUCCESS;
    try {
      $this->response->init($this->generator->delete($this->command));
      if ($this->response->getError()) {
        $this->batchRequest->setPostId($tumblr_node->getPostId());
        $this->scheduleAction($node, TumblrConnectBatch::DELETE);
        $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
      } else {
        $this->dbTumblrNode->delete($tumblr_node->getId());
      }
    } catch (TumblrConnectRemoteException $re) {
      $this->batchRequest->setPostId($tumblr_node->getPostId());
      $this->scheduleAction($node, TumblrConnectBatch::DELETE);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    } catch (TumblrConnectDatabaseException $de) {
      $this->batchRequest->setPostId($tumblr_node->getPostId());
      $this->scheduleAction($node, TumblrConnectBatch::DELETE);
      $operationStatus = TumblrConnectDefinitions::BATCH_SCHEDULE;
    }
    return $operationStatus;
  }

  /**
   * Converts paths to full url.
   *
   * @param $content
   *   either a node body or teaser
   * @return $html
   *   the modified node or tease
   * Method changes the src attribute on html image tags, and the href attribute
   * on a link tags. We want to makes sure the html references point back to our
   * drupal site. Example: <a href='frontage' is converted to full url path
   * <a href='http://myDrupalSite/frontpage' .
   */
  function expandUrlPath($content) {

    $html = new simple_html_dom();
    $html->load($content);

    $images = $html->find('img');
    if (count($images) > 0) {
      foreach ($images as $image) {
        $src = $image->src;
        $full_src = url($src, array('absolute' => TRUE));
        $image->src = $full_src;
      }
    }

    $links = $html->find('a');
    if (count($links) > 0) {
      foreach ($links as $link) {
        $href = $link->href;
        $full_src = url($href, array('absolute' => TRUE));
        $link->href = $full_src;
      }
    }
    // force to string
    return '' . $html;
  }

  /*
   * Builds a simple readmore link back to our drupal website and page
   *
   * @param int $nid
   *   Nid identifier for node.
   *
   * @return
   *   Hyperlink result as returned by l().
   */

  function buildReadMore($nid) {
    global $base_url;
    $url = $base_url . "/?q=node/$nid";
    return l(t('read more'), $url);
  }

  /**
   * Gets the cached login information.
   *
   * @return
   *   Array of of login information.
   */
  function getInitConfig() {
    $tumblr_conf = DbTumblrConnectConfig::factory()->getCachedConfig();
    $initConfig['email'] = $tumblr_conf->getEmail();
    $initConfig['password'] = $tumblr_conf->getPassword();
    $initConfig['teaserOnly'] = $tumblr_conf->getTeaserOnly();
    return $initConfig;
  }

  /**
   * Schedules content for batch operation.
   *
   * @param DatabaseConnection $node
   *   Node object..
   * @param String $sction
   *   add, mod, or del.
   */
  function scheduleAction($node, $action) {
    $this->batchRequest->setNid($node->nid);
    $this->batchRequest->setVid($node->vid);
    $this->batchRequest->setOperation($action);
    $this->dbBatchRequest->schedule($this->batchRequest);
  }

}

/**
 * Extracts a comma-delimted list of taxonomy terms names
 * associated with a node instance.
 *
 * @param  $node
 *   Node object..
 * @return
 *   String - commited delimeted list of taxonomy term names..
 */
function tumblr_connect_get_taxonomy_terms($node) {
  $node_language = $node->language;
  $tags = '';
  // iterate through the node elements
  foreach ($node as $node_element_key => $node_element_value) {
    if (is_array($node_element_value)) {
      $pos = strpos($node_element_key, 'field_');
      if ($pos === 0) {
        // if the element is a attached field machine name (starts with _field)
        $nested_array = $node_element_value[$node_language];
        foreach ($nested_array as $delta => $delta_value) {
          // now look for a tid element
          if (isset($delta_value['tid'])) {
            $tid = $delta_value['tid'];
            $term = taxonomy_term_load($tid);
            if (isset($term)) {
              $tags .= $term->name . ',';
            }
          }
        }
      }
    }
  }

  return $tags;
}