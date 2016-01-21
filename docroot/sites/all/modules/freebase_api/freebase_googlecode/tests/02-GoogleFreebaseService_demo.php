<!DOCTYPE html>
<html>
<body>
  <h1>Test if the Google API library works at all</h1>
  <p>
    This test page loads the provided Google API PHP lib and tries to use it. <br />
    It will demonstrate if the library is available and can connect. and if the
    API key is valid.
  </p>

  <h2>Loading library service instance</h2>

  <?php
  include('../freebase_googlecode.inc');
  include('freebase_googlecode_key.inc');

  // Options used when setting up the library.
  $config = array(
      'use_objects' => TRUE,
      'use_patched' => TRUE,
  );
  // Options used when making a request.
  $params = array(
      'key'=>$freebase_googlecode_key
  );
  $params = array();

  // Request to test.
  $topic_id = '/en/bob_dylan';
  #$topic_id = '/m/0199g'; // Bicycle

  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  if ($fb) {
    echo "Library instantiated OK " . (!empty($fb->version)) ? $fb->version : "UNVERSIONED";
  }
  else {
    ?>
    Failed to initialize a Service handle.
    This is most likely if the google-api-php-client library cannot be found,
    or the Drupal freebase_googlecode utility library was not available.
    <?php
    exit;
  }

  ?>

  <hr />

  <h2>
    Making request for info on the topic
    <?php echo $topic_id?>
  </h2>
  <p>
    Attempting to pull back 'topic' information. Depending on the API
    use_objects setting, the result will either be a big nested array, or a
    complex mesh of data objects. Both are pretty nasty to get data out of. <br />
    You should see "Bob Dylan" as a result.
  </p>
  <hr />

  <?php
  // The $fb->topic is a Google_TopicServiceResource extends Google_ServiceResource
  // It was initialized with json instructions when instantiated.
  // Dunno why it's the instantiators job to define the service, surely
  // that should be bundled with the service object itself.
  // That big config bundle associated the callback method 'lookup' with
  //   "path": "topic{/id*}"
  // It also registered that the expected response was type TopicLookup.
  // Based on examples, this is not an error, and it does figure out that that
  // means the response is a Google_TopicLookup object.
  //
  // https://developers.google.com/freebase/v1/topic-overview
  // Fetching a topic means
  // * making a call to the endpoint
  //   https://www.googleapis.com/freebase/v1/topic
  // * with arguments of the key=>$topicid
  // * and json_decode()ing it


  echo("<h2>Running lookup with options</h2>");
  echo("<pre>");
  print_r(array(
      'config' => $config,
      'params' => $params,
      'topic_id' => $topic_id,
  ));
  echo("</pre>");

  // Do the request!
  $time_start = microtime(true);
  $topic = $fb->topic->lookup($topic_id, $params);
  $time_end = microtime(true);
  $time = $time_end - $time_start;
  ?>

  <hr />

  <?php
  // Array-based is the default
  if (is_array($topic)) { ?>
    $value = $topic['property']['/type/object/name']['values'][0]['value'];

    <h2>
      topic->lookup() returned an <em>array</em>
    </h2>
    <p>
      Array-based Topic name value is '<b><?php echo $value; ?> </b>'
    </p>
  <?php } else { ?>
    <p>
      topic->lookup() did not return an <em>array</em>. This probably means that
      the use_objects flag is on.
    </p>
  <?php } ?>

  <hr>

  <?php
  // Else, using the API OO methods as best I can guess.
  if (is_object($topic)) {
    // $topic is a Google_TopicLookup
    $topicProperty = $topic->getProperty();
    // $topicProperty isa Google_TopicLookupProperty
    $key = '/type/object/name';
    $nameProperty = $topicProperty->$key;
    // This should have returned a Google_TopicPropertyvalue.
    $values = $nameProperty->getValues();
    // $values is therefore an array of Google_TopicValue.
    $value = $values[0]->getValue();
    ?>

    <h2>
      topic->lookup() returned an <em>object</em>
    </h2>
    <p>
      Used the API methods to retrieve an OO value: '<b><?php echo $value; ?> </b>'.
      Note - these functions are from the unofficial fork that patches the <a href="https://code.google.com/p/google-api-php-client/source/browse/trunk/src/contrib/Google_FreebaseService.php?r=582">broken Google_FreebaseService object</a>.
    </p>

  <h3>Data tests</h3>
  Demonstrate extractions of some of the data that was returned about the topic.
  <dl>
    <dt>$topicProperty->getName() = </dt><dd>"<?php print $topicProperty->getName();?>"
    </dd>
    <dt>$topicProperty->getKeys() = </dt><dd>
      <pre><?php print_r($topicProperty->getKeys());?></pre>
    </dd>
    <dt>$topicProperty->getProperty('/common/topic/image') = </dt><dd>
      <pre><?php print_r($topicProperty->getProperty('/common/topic/image'));?></pre>
    </dd>
    <dt>$topicProperty->getAttributeValues('/type/object/type', 'text') = </dt><dd>
      <pre><?php print_r($topicProperty->getAttributeValues('/type/object/type', 'text'));?></pre>
    </dd>
  </dl>

  <?php } else { ?>
    <p>
      topic->lookup() did not return an <em>object</em>
    </p>
  <?php } ?>

  <hr />
  <p>
    Request to web service endpoint took
    <?php printf("%.3f", $time); ?>
    seconds.
  </p>
  <hr />

  <pre>
  <?php
  #print_r(get_defined_vars());
  ?>
  </pre>

  <p>
    If it did not show up, it means that the service request failed for some
    reason. Maybe firewall, or the API key has become invalid or overloaded, or
    Google changed <a href="https://developers.google.com/freebase/index">their
      APIs</a>.
  </p>
</body>
</html>
