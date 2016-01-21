<?php
/**
 * @file
 * Defines the GovDeliverySubscriberAPI, which handles creating Topics and
 * assigning them to Catgories.
 */

class GovDeliverySubscriberAPI {
  public $govdeliveryAccountName;
  public $govdeliveryAccountPassword;
  public $govdeliveryUrlBase;
  protected $authHeader;

  /**
   * Constructor.
   */
  public function __construct($govdelivery_account_name, $govdelivery_account_password, $govdelivery_url_base) {
    $this->govdeliveryAccountName = $govdelivery_account_name;
    $this->govdeliveryAccountPassword = $govdelivery_account_password;
    $this->govdeliveryUrlBase = $govdelivery_url_base;
    // Login header:
    // Authorization: Basic base64_encode('username:password');
    $this->authHeader = $this->govdeliveryAccountName . ':' . $this->govdeliveryAccountPassword;
  }

  /**
   * Take a topic item from the queue and create it in GovDelivery.
   *
   * @param object $topic
   *   Topic object.
   *
   * @return array
   *   Return object from topic creation.
   */
  public function createTopic($topic) {
    $url = $this->govdeliveryUrlBase . '/topics.xml';

    if (!isset($topic->topic_id) || is_null($topic->topic_id)) {
      throw new Exception(t('topic_id attribute of supplied topic is unset or null.  You must supply a topic_id'));
    }

    // GovDelivery Subscriber API requires topic IDs to be 32 characers or less.
    $id_length = strlen($topic->topic_id);
    if ($id_length > 32) {
      throw new Exception(t('topic_id must be 32 characters or less. Supplied id of %id is %length characters', array('%id' => $topic->topic_id, '%length' => $id_length)));
    }

    // GovDelivery Subscriber API requires all topic_id's to be all CAPS.
    $topic->topic_id = strtoupper($topic->topic_id);

    $create_topic_xml_request = '
      <?xml version="1.0" encoding="UTF-8"?>
      <topic>
         <code type="string">' . $topic->topic_id  . '</code>
         <default-pagewatch-results type="integer" nil="true"></default-pagewatch-results>
         <description nil="true"></description>
         <lock-version type="integer">0</lock-version>
         <name>' . $topic->title  . '</name>
         <pagewatch-autosend type="boolean">false</pagewatch-autosend>
         <pagewatch-enabled type="boolean">false</pagewatch-enabled>
         <pagewatch-suspended type="boolean">false</pagewatch-suspended>
         <rss-feed-description nil="true"></rss-feed-description>
         <rss-feed-title>Topic Name - Recent Updates</rss-feed-title>
         <rss-feed-url>http://url_for_rss_feed.rss</rss-feed-url>
         <send-by-email-enabled type="boolean">false</send-by-email-enabled>
         <short-name>' . $topic->short_title . '</short-name>
         <subscribers-count type="integer">0</subscribers-count>
         <wireless-enabled type="boolean">false</wireless-enabled>
         <pages type="array">
         <page>
         <url>http://www.govdelivery.com</url>
         </page>
         </pages>
         <visibility>2</visibility>
         </topic>';

    // Set options.
    $topic_options = array(
      CURLOPT_URL => $url,
      CURLOPT_FAILONERROR => TRUE,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 120,
      CURLOPT_USERPWD => $this->authHeader,
      CURLOPT_HEADER => TRUE,
      CURLOPT_HTTPHEADER => array('Content-Type: application/xml'),
      CURLOPT_POST => TRUE,
      CURLOPT_POSTFIELDS => $create_topic_xml_request,
    );

    // Create curl resource.
    $ch = curl_init();
    curl_setopt_array($ch, $topic_options);

    $topic_return = new stdClass();
    // Execute and get response.
    $topic_return->response = curl_exec($ch);
    $topic_return->header = substr($topic_return->response, 0, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $topic_return->body = substr($topic_return->response, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $topic_return->error = curl_error($ch);
    $topic_return->info = curl_getinfo($ch);
    $topic_return->topic_id = $topic->topic_id;

    return $topic_return;
  }

  /**
   * Update the GovDelivery category associated with the given topic.
   *
   * @param object $topic
   *   Topic object.
   *
   * @return array
   *   Return object from category updating.
   */
  public function updateTopicCategory($topic) {
    if (!isset($topic->topic_id) || is_null($topic->topic_id)) {
      throw new exception(t('topic_id attribute of supplied topic is unset or null.  you must supply a topic_id'));
    }

    if (!isset($topic->category_id) || is_null($topic->category_id)) {
      throw new exception(t('category_id attribute of supplied topic is unset or null.  you must supply a category_id'));
    }

    $url = $this->govdeliveryUrlBase . '/topics/' . $topic->topic_id . '/categories.xml';
    $set_category_xml_request = '
        <?xml version="1.0" encoding="UTF-8"?>
        <topic>
           <categories type="array">
            <category>
              <code>' . $topic->category_id . '</code>
            </category>
          </categories>
        </topic>';

    // Write the request to a tmp file since PUT isn't happy taking a string.
    $fp = fopen('php://memory', 'w+');
    fwrite($fp, $set_category_xml_request);
    fseek($fp, 0);

    // Set options.
    $category_options = array(
      CURLOPT_URL => $url,
      CURLOPT_FAILONERROR => TRUE,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 30,
      CURLOPT_USERPWD => $this->authHeader,
      CURLOPT_HEADER => TRUE,
      CURLOPT_HTTPHEADER => array('Content-Type: application/xml'),
      CURLOPT_PUT => TRUE,
      CURLOPT_INFILE => $fp,
      CURLOPT_INFILESIZE => strlen($set_category_xml_request),
    );

    // Create curl resource.
    $ch = curl_init();
    curl_setopt_array($ch, $category_options);

    // Execute and get response.
    $category_return = new stdClass();
    $category_return->response = curl_exec($ch);
    $category_return->header = substr($category_return->response, 0, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $category_return->body = substr($category_return->response, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $category_return->error = curl_error($ch);
    $category_return->info = curl_getinfo($ch);

    // Kill the file stream we created above.
    fclose($fp);
    curl_close($ch);
    return $category_return;
  }
}
