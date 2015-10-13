<?php
/*
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */


  /**
   * The "text" collection of methods.
   * Typical usage is:
   *  <code>
   *   $freebaseService = new Google_FreebaseService(...);
   *   $text = $freebaseService->text;
   *  </code>
   */
  class Google_TextServiceResource extends Google_ServiceResource {

    /**
     * Returns blob attached to node at specified id as HTML (text.get)
     *
     * @param string $id The id of the item that you want data about
     * @param array $optParams Optional parameters.
     *
     * @opt_param string format Sanitizing transformation.
     * @opt_param string maxlength The max number of characters to return. Valid only for 'plain' format.
     * @return Google_ContentserviceGet
     */
    public function get($id, $optParams = array()) {
      $params = array('id' => $id);
      $params = array_merge($params, $optParams);
      $data = $this->__call('get', array($params));
      if ($this->useObjects()) {
        return new Google_ContentserviceGet($data);
      } else {
        return $data;
      }
    }
  }

  /**
   * The "topic" collection of methods.
   * Typical usage is:
   *  <code>
   *   $freebaseService = new Google_FreebaseService(...);
   *   $topic = $freebaseService->topic;
   *  </code>
   */
  class Google_TopicServiceResource extends Google_ServiceResource {

    /**
     * Get properties and meta-data about a topic. (topic.lookup)
     *
     * @param string $id The id of the item that you want data about.
     * @param array $optParams Optional parameters.
     *
     * @opt_param string dateline Determines how up-to-date the data returned is. A unix epoch time, a guid or a 'now'
     * @opt_param string filter A frebase domain, type or property id, 'suggest', 'commons', or 'all'. Filter the results and returns only appropriate properties.
     * @opt_param string lang The language you 'd like the content in - a freebase /type/lang language key.
     * @opt_param string limit The maximum number of property values to return for each property.
     * @opt_param bool raw Do not apply any constraints, or get any names.
     * @return Google_TopicLookup
     */
    public function lookup($id, $optParams = array()) {
      $params = array('id' => $id);
      $params = array_merge($params, $optParams);
      $data = $this->__call('lookup', array($params));
      if ($this->useObjects()) {
        return new Google_TopicLookup($data);
      } else {
        return $data;
      }
    }
  }

/**
 * Service definition for Google_Freebase (v1).
 *
 * <p>
 * Topic and MQL APIs provide you structured access to Freebase data.
 * </p>
 *
 * <p>
 * For more information about this service, see the
 * <a href="https://developers.google.com/freebase/" target="_blank">API Documentation</a>
 * </p>
 *
 * @author Google, Inc.
 */
class Google_FreebaseService extends Google_Service {
  public $text;
  public $topic;
  /**
   * Constructs the internal representation of the Freebase service.
   *
   * @param Google_Client $client
   */
  public function __construct(Google_Client $client) {
    $this->servicePath = 'freebase/v1/';
    $this->version = 'v1 2013-06 + PATCHED';
    $this->serviceName = 'freebase';

    $client->addService($this->serviceName, $this->version);

    // Declare the service endpoints and call signatures I know about.
    // ... in lack of a WSDL.
    $this->text = new Google_TextServiceResource($this, $this->serviceName, 'text', json_decode('{
      "methods": {
        "get": {
          "id": "freebase.text.get",
          "path": "text{/id*}",
          "httpMethod": "GET",
          "parameters": {
            "format":   {"type": "string","default": "plain", "enum": ["html", "plain", "raw"], "location": "query"},
            "id":       {"type": "string", "required": true, "repeated": true, "location": "path"},
            "maxlength":{"type": "integer", "format": "uint32", "location": "query"}
          },
          "response": {"$ref": "ContentserviceGet"}
        }
      }
    }', true));
    $this->topic = new Google_TopicServiceResource($this, $this->serviceName, 'topic', json_decode('{
      "methods": {
        "lookup": {
          "id": "freebase.topic.lookup",
          "path": "topic{/id*}",
          "httpMethod": "GET",
          "parameters": {
            "dateline": {"type": "string", "location": "query"}, "filter": {"type": "string", "repeated": true, "location": "query"},
            "id":       {"type": "string", "required": true, "repeated": true, "location": "path"},
            "lang":     {"type": "string", "default": "en", "location": "query"},
            "limit":    {"type": "integer", "default": "10", "format": "uint32", "location": "query"},
            "raw":      {"type": "boolean", "default": "false", "location": "query"}
          },
          "response": {"$ref": "TopicLookup"}
        }
      }
    }', true));

    $this->image = new Google_ImageServiceResource($this, $this->serviceName, 'image', json_decode('{
      "httpMethod": "GET",
      "path": "image{/id*}",
      "supportsMediaDownload": true,
      "id": "freebase.image",
      "parameters": {
        "fallbackid":{"type": "string", "default": "/freebase/no_image_png", "location": "query"},
        "id":       {"type": "string", "required": true, "repeated": true, "location": "path"},
        "maxheight":{"type": "integer", "format": "uint32", "maximum": "4096", "location": "query"},
        "maxwidth": {"type": "integer", "format": "uint32", "maximum": "4096", "location": "query"},
        "mode":     {"type": "string", "default": "fit", "enum": ["fill", "fillcrop", "fillcropmid", "fit"], "location": "query"},
        "pad":      {"type": "boolean", "default": "false", "location": "query"}
      }
    }', true));

    $this->mqlread = new Google_MqlreadServiceResource($this, $this->serviceName, 'mqlread', json_decode('{
      "httpMethod": "GET",
      "path": "mqlread",
      "supportsMediaDownload": true,
      "id": "freebase.mqlread",
      "parameters": {
        "as_of_time":{"type": "string", "location": "query"},
        "callback": {"type": "string", "location": "query"},
        "cost":     {"type": "boolean", "default": "false", "location": "query"},
        "cursor":   {"type": "string", "location": "query"},
        "dateline": {"type": "string", "location": "query"},
        "html_escape":{"type": "boolean", "default": "true", "location": "query"},
        "indent":   {"type": "integer", "default": "0", "format": "uint32", "maximum": "10", "location": "query"},
        "lang":     {"type": "string", "default": "/lang/en", "location": "query"},
        "query":    {"type": "string", "required": true, "location": "query"},
        "uniqueness_failure": {"type": "string", "default": "hard", "enum": ["hard", "soft"], "location": "query"}
      }
    }', true));
    /*
    $this->mqlwrite = new Google_MqlwriteServiceResource($this, $this->serviceName, 'mqlwrite', json_decode('{
      "httpMethod": "GET",
      "path": "mqlwrite",
      "scopes": ["https://www.googleapis.com/auth/freebase"],
      "supportsMediaDownload": true,
      "id": "freebase.mqlwrite",
      "parameters": {
        "callback": {"type": "string", "location": "query"},
        "dateline": {"type": "string", "location": "query"},
        "indent":   {"type": "integer", "default": "0", "format": "uint32", "maximum": "10", "location": "query"},
        "query":    {"type": "string", "required": true, "location": "query"},
        "use_permission_of": {"type": "string", "location": "query"}
      }
    }', true));
    */
  }
}



class Google_ContentserviceGet extends Google_Model {
  public $result;
  public function setResult( $result) {
    $this->result = $result;
  }
  public function getResult() {
    return $this->result;
  }
}

class Google_TopicLookup extends Google_Model {
  public $id;
  protected $__propertyType = 'Google_TopicLookupProperty';
  protected $__propertyDataType = '';
  public $property;
  public function setId( $id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
  public function setProperty(Google_TopicLookupProperty $property) {
    $this->property = $property;
  }
  public function getProperty($key = NULL) {
    return $this->property;
  }
}

class Google_TopicLookupProperty extends Google_Model {
  protected $___freebase_object_profile_linkcountType = 'Google_TopicStatslinkcount';
  protected $___freebase_object_profile_linkcountDataType = '';
  public $_freebase_object_profile_linkcount;

  // We need to instantiate our data ourselves, the
  // Google Model map_types cannot deal with arbitrary keys?
  // so for each value array passed in, register that it's an
  // object of type topicValue
  public function __construct() {
    #print("<h3>Constructing a " . get_class($this). "</h3>");
    if (func_num_args() ==  1 && is_array(func_get_arg(0))) {
      // Initialize the model with the array's contents.
      // Normally all the data gets dropped on me in an array
      // keyed by arbitrary freebase strings.
      // But each data item there needs to be a data object.
      $array = func_get_arg(0);
      // Call the GoogleModel version as normal first.
      $this->mapTypes($array);
      // Then convert the leftover arrays into property objects.
      // These contain the better accessor funcs to retrieve info from.
      // It's also likely to be extremely heavyweight in large datasets.
      foreach ($array as $key => $value) {
        if (!empty($value['valuetype'])) {
          // Objectify me, baby!
          $this->$key = new Google_TopicPropertyvalue($value);
        }
      }
    }
  }

  /**
   * Return the named Google_TopicPropertyvalue found for the topic.
   *
   * If called with no arg, returns the Google_TopicLookupProperty wrapper
   * object. (only for legacy reasons, may be able to drop this as it's unused)
   *
   * If the topic is not initialized, or the requested key is not set,
   * returns NULL.
   *
   * @param string $key
   * @return a Google_TopicPropertyvalue - which contains an array of values
   * for you to unpack.
   */
  public function getProperty($key = NULL) {
    if (empty($this->$key)) {
      return NULL;
    }
    return $this->$key;
  }

  /**
   * Accessor shortcut to retrieve the simple '/type/object/name' from a topic.
   */
  function getName() {
    return $this->getAttributeValue('/type/object/name');
  }

  /**
   * Accessor shortcut to retrieve the simple value of a named property
   * from a topic.
   */
  function getAttributeValue($key) {
    if (empty($this->$key)) {
      return NULL;
    }
    $propertyValue = $this->$key;
    $values = $propertyValue->getValues();
    return $values[0]->getValue();
  }
  /**
   * Return an array of simple values matching the key.
   *
   * @param unknown_type $key
   * @param $valuetype string type of value. Some objects have data in their
   *   'value', others in their 'text' attribute.
   *   You can also fetch an array of 'id' like this.
   *   If $valuetype is blank, return the best guess.
   * @return Array
   */
  function getAttributeValues($key, $valuetype = 'value') {
    if (empty($this->$key)) {
      return array();
    }
    $propertyValue = $this->$key;
    $values = $propertyValue->getValues();
    $result = array();
    foreach ($values as $i => $v) {
      // Choose what you mean by 'value'.
      $value = NULL;
      switch ($valuetype) {
        case 'value' :
          $value = $v->getValue();
          break;
        case 'text' :
          $value = $v->getText();
          break;
        case 'id' :
          $value = $v->getId();
          break;
        default :
          // Return whichever one of these I can find.
          if (! ($value = $v->getValue())) {
            $value = $v->getText();
          }
          break;
      }
      $result[$i] = $value;
    }
    return $result;
  }

  /**
   * Return a list of defined keys (attribute IDs) for this topic
   *
   * EXPERIMENTAL/DIAGNOSTIC
   */
  public function getKeys() {
    return array_keys(get_object_vars($this));
  }

  public function getImage() {

  }

  // Unknown
  public function set_freebase_object_profile_linkcount(Google_TopicStatslinkcount $_freebase_object_profile_linkcount) {
    $this->_freebase_object_profile_linkcount = $_freebase_object_profile_linkcount;
  }
  // Unknown
  public function get_freebase_object_profile_linkcount() {
    return $this->_freebase_object_profile_linkcount;
  }
}

class Google_TopicPropertyvalue extends Google_Model {
  public $count;
  public $status;
  protected $__valuesType = 'Google_TopicValue';
  protected $__valuesDataType = 'array';
  public $values;
  public $valuetype;
  public function setCount( $count) {
    $this->count = $count;
  }
  public function getCount() {
    return $this->count;
  }
  public function setStatus( $status) {
    $this->status = $status;
  }
  public function getStatus() {
    return $this->status;
  }
  public function setValues(/* array(Google_TopicValue) */ $values) {
    $this->assertIsArray($values, 'Google_TopicValue', __METHOD__);
    $this->values = $values;
  }
  /**
   * @return Array of Google_TopicValue items
   */
  public function getValues() {
    return $this->values;
  }
  public function setValuetype( $valuetype) {
    $this->valuetype = $valuetype;
  }
  public function getValuetype() {
    return $this->valuetype;
  }
}

class Google_TopicStatslinkcount extends Google_Model {
  public $type;
  protected $__valuesType = 'Google_TopicStatslinkcountValues';
  protected $__valuesDataType = 'array';
  public $values;
  public function setType( $type) {
    $this->type = $type;
  }
  public function getType() {
    return $this->type;
  }
  public function setValues(/* array(Google_TopicStatslinkcountValues) */ $values) {
    $this->assertIsArray($values, 'Google_TopicStatslinkcountValues', __METHOD__);
    $this->values = $values;
  }
  public function getValues() {
    return $this->values;
  }
}

class Google_TopicStatslinkcountValues extends Google_Model {
  public $count;
  public $id;
  protected $__valuesType = 'Google_TopicStatslinkcountValuesValues';
  protected $__valuesDataType = 'array';
  public $values;
  public function setCount( $count) {
    $this->count = $count;
  }
  public function getCount() {
    return $this->count;
  }
  public function setId( $id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
  public function setValues(/* array(Google_TopicStatslinkcountValuesValues) */ $values) {
    $this->assertIsArray($values, 'Google_TopicStatslinkcountValuesValues', __METHOD__);
    $this->values = $values;
  }
  public function getValues() {
    return $this->values;
  }
}

class Google_TopicStatslinkcountValuesValues extends Google_Model {
  public $count;
  public $id;
  protected $__valuesType = 'Google_TopicStatslinkcountValuesValuesValues';
  protected $__valuesDataType = 'array';
  public $values;
  public function setCount( $count) {
    $this->count = $count;
  }
  public function getCount() {
    return $this->count;
  }
  public function setId( $id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
  public function setValues(/* array(Google_TopicStatslinkcountValuesValuesValues) */ $values) {
    $this->assertIsArray($values, 'Google_TopicStatslinkcountValuesValuesValues', __METHOD__);
    $this->values = $values;
  }
  public function getValues() {
    return $this->values;
  }
}

class Google_TopicStatslinkcountValuesValuesValues extends Google_Model {
  public $count;
  public $id;
  public function setCount( $count) {
    $this->count = $count;
  }
  public function getCount() {
    return $this->count;
  }
  public function setId( $id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
}

class Google_TopicValue extends Google_Model {
  protected $__citationType = 'Google_TopicValueCitation';
  protected $__citationDataType = '';
  public $citation;
  public $creator;
  public $dataset;
  public $id;
  public $lang;
  public $project;
  protected $__propertyType = 'Google_TopicPropertyvalue';
  protected $__propertyDataType = 'map';
  public $property;
  public $text;
  public $timestamp;
  public $value;
  public function setCitation(Google_TopicValueCitation $citation) {
    $this->citation = $citation;
  }
  public function getCitation() {
    return $this->citation;
  }
  public function setCreator( $creator) {
    $this->creator = $creator;
  }
  public function getCreator() {
    return $this->creator;
  }
  public function setDataset( $dataset) {
    $this->dataset = $dataset;
  }
  public function getDataset() {
    return $this->dataset;
  }
  public function setId( $id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }
  public function setLang( $lang) {
    $this->lang = $lang;
  }
  public function getLang() {
    return $this->lang;
  }
  public function setProject( $project) {
    $this->project = $project;
  }
  public function getProject() {
    return $this->project;
  }
  public function setProperty(Google_TopicPropertyvalue $property) {
    $this->property = $property;
  }
  public function getProperty() {
    return $this->property;
  }
  public function setText( $text) {
    $this->text = $text;
  }
  public function getText() {
    return $this->text;
  }
  public function setTimestamp( $timestamp) {
    $this->timestamp = $timestamp;
  }
  public function getTimestamp() {
    return $this->timestamp;
  }
  public function setValue( $value) {
    $this->value = $value;
  }
  public function getValue() {
    return $this->value;
  }
}

class Google_TopicValueCitation extends Google_Model {
  public $provider;
  public $statement;
  public $uri;
  public function setProvider( $provider) {
    $this->provider = $provider;
  }
  public function getProvider() {
    return $this->provider;
  }
  public function setStatement( $statement) {
    $this->statement = $statement;
  }
  public function getStatement() {
    return $this->statement;
  }
  public function setUri( $uri) {
    $this->uri = $uri;
  }
  public function getUri() {
    return $this->uri;
  }
}


class Google_ImageServiceResource extends Google_ServiceResource {

  /**
   * Pulled out of the garbage from earlier codebase.
   *
   * Returns the scaled/cropped image attached to a freebase node. (image.image)
   *
   *
   * @param string $id Freebase entity or content id, mid, or guid.
   * @param array $optParams Optional parameters. Valid optional parameters are listed below.
   *
   * @opt_param string maxwidth Maximum width in pixels for resulting image.
   * @opt_param string maxheight Maximum height in pixels for resulting image.
   * @opt_param string fallbackid Use the image associated with this secondary id if no image is associated with the primary id.
   * @opt_param bool pad A boolean specifying whether the resulting image should be padded up to the requested dimensions.
   * @opt_param string mode Method used to scale or crop image.
   *
   * @return Google_ContentserviceGet
   */
  public function get($id, $optParams = array()) {
    $params = array('id' => $id);
    $params = array_merge($params, $optParams);
    $data = $this->__call('get', array($params));
    if ($this->useObjects()) {
      return new Google_ContentserviceGet($data);
    } else {
      return $data;
    }
  }
}

class Google_MqlreadServiceResource extends Google_ServiceResource {

  /**
   * ~dman: This code is ridiculously padded with paranoia and error-handling
   * as I can't find info on expected failure states for either the google api
   * lib or the freebase service itself.
   * Everything here is deduced almost black-box, so I'm assuming the worst
   * and wrapping everything verbosely to pad against failures.
   */

  /**
   * I can return gracefully or noisily, as you prefer.
   *
   * Throwing an error would be the right thing to do, but
   * for safer code calls, optionally allow us to quash that.
   * This code style is sub-optimal, but built for robustness
   * while I don't yet know what to *expect* from the API wrt errors.
   * TODO - shift this flag into the GoogleFreebaseService owner instead
   * of in the methods.
   * @var bool
   */
  public $throwException = TRUE;

  /**
   * Like cURL, hang on to the details of the transaction if required for
   * error processing, although normal flow assumes that you just want the
   * data, and that null data may be a valid result.
   */
  public $lastResponse;
  public $lastQuery;

  /**
   *  Pulled out of the garbage from earlier codebase.
   *
   * Performs MQL Queries. (mqlread.mqlread) 2013
   * https://developers.google.com/freebase/v1/mql-overview
   *
   * The query can be either prepared JSON, or an appropriately-formed PHP
   * array or object.
   *
   * eg
   *
   * $query = '{
   *   "/type/object/mid": "/m/04093",
   *   "/type/object/name": null,
   *   "/type/object/type": [{
   *       "/type/object/name": null
   *   }]
   *  }
   *
   * is equivalent to
   *
   * $query = (object)array(
   *   "/type/object/mid" => "/m/04093",
   *   "/type/object/name" => null,
   *   "/type/object/type" => array((object)array(
   *       "/type/object/name" => null
   *   )),
   *  );
   *
   * Note the way an array is cast into an object where needed by MQL.
   *
   * @param string $query An envelope containing a single MQL query.
   * @param array $optParams Optional parameters. Valid optional parameters are listed below.
   *
   * @opt_param string lang The language of the results - an id of a /type/lang object.
   * @opt_param bool html_escape Whether or not to escape entities.
   * @opt_param string indent How many spaces to indent the json.
   * @opt_param string uniqueness_failure How MQL responds to uniqueness failures.
   * @opt_param string dateline The dateline that you get in a mqlwrite response to ensure consistent results.
   * @opt_param string cursor The mql cursor.
   * @opt_param string callback JS method name for JSONP callbacks.
   * @opt_param bool cost Show the costs or not.
   * @opt_param string as_of_time Run the query as it would've been run at the specified point in time.
   */
  public function mqlread($query, $optParams = array()) {
    // Seeing as we are in PHP-land, and hand-editing JSON is error-prone,
    // Let the caller give us a PHP array as its query.
    if (is_array($query) || is_object($query)) {
      $query = json_encode($query);
    }

    $params = array(
      'query' => $query,
    );
    $params = array_merge($params, $optParams);

    $result = NULL;
    $this->lastResponse = NULL;
    $this->lastQuery = $query;

    // Pre-flight check of our JSON before sending it off to Google.
    // Assertions amd paranoia are good in an API, especially as the endpoint
    // error handling is non-existent.
    $unpacked = json_decode($query);
    if (json_last_error() != JSON_ERROR_NONE) {
      // Something is wrong with the JSON; Abort!
      $this->lastResponse = array(
        'code' => json_last_error(),
        'message' => "JSON loks like it's invalid. Failed to parse. Aborting " . __FUNCTION__,
      );
      return $this->lastResponse;
    }
    // Google gives me segfaults if the query string is a little bit too long!
    if (strlen($query) > 10000) {
      // This magic number found by trial.
      // The actual trigger was somewhere near 1050, but as it's maybe the full
      // URI that is inspected, we need fudge space, probably more than this,
      // TODO can repack the JSON for compactness.
      $this->lastResponse = array(
        'code' => 414,
        'message' => "Beware - Query string is too long. As Google API can produce a segfault in some cases that seem to be traced to long string lengths, I'm going to have to abort this lookup. String length was " . strlen($query),
      );
      return $this->lastResponse;
    }

    try {
      $response = $this->__call('mqlread', array($params));

      // Google seems to give us NO status usefulness at all.
      // The full documentation says:
      //
      // https://developers.google.com/freebase/v1/mqlread
      // "If successful, the response is a JSON structure."
      //
      // But no word on failure states.
      // Historicaly, the API used to provide a response code and message.
      // http://mql.freebaseapps.com/ch04.html#responseenvelope
      // I'm not seeing that no more, (Traced it right back to the Google_REST call)
      // but if it ever comes back, don't tromp it.
      //
      // In the meantime, emulate it with something meaningful.
      // At least so we can tell the diff between:
      // result, empty result, unexpected result, and error.
      //
      $myResponse = array();

      if (isset($response['result'])) {
        $result = $response['result'];
        if (empty($result)) {
          // No error, but result is an empty set.
          $myResponse['code'] = 204;
          $myResponse['message'] = "No content";
        }
        else {
          $myResponse['code'] = 200; // HTTP OK
          $myResponse['message'] = "OK";
        }
      }
      else {
        // Unknown state. Possibly an error.
        // JSON was returned but there was no result, not even an empty array.
        $myResponse['code'] = 500;
        $myResponse['message'] = "Invalid response - Unknown reason";
      }
      // It's theoretically not impossible to get *a* result and also an
      // error message (warning?), so anticipate that.

      // Though I can't trigger it from Freebase, other Google API services
      // (at least language.translations.list)
      // do know how to return errors. Assume that's a good thing.
      // Eg (translate API, not Freebase) triggered at
      // https://developers.google.com/apis-explorer/#s/translate/v2/language.translations.list?q=beware+of+the+leopard&target=fr&source=english&_h=4&
      if (!empty($response['error'])) {
        $myResponse['code'] = $response['error']['code'];
        $myResponse['message'] = $response['error']['message'];
      }

      // Only set $myResponse parameters if the native response has not.
      // Legacy and future support for proper API status in the response envelope.
      $response += $myResponse;

      $this->lastResponse = $response;

    } catch (Exception $e) {
      // Known failure messages seen so far:
      // (400) Type /type/object does not have property
      // (400) MID is invalid (failed to parse)
      // (403) Query too difficult.
      // (414) Request-URI Too Large.
      if ($this->throwException) {
        throw($e);
      }
      // Else return a less violent error.
      // Sometimes if the Google API breaks, it sends a full HTML page
      // Which I do NOT want to reflect inline.
      $this->lastResponse = array(
        'code' => $e->getCode(),
        'message' => 'mqlread failed ' . strip_tags($e->getMessage()),
      );
      return $this->lastResponse;
    }
    // $result is an array of items
    return $result;
  }

}
