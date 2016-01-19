<!DOCTYPE html>
<html>
<body>
  <h1>Test if the Freebase connection to the Google API service works at all</h1>
  <p>This test page is stand-alone, using only PHP and CURL, not Drupal or the
    Google library. It will demonstrate if the network lets us talk to the
    endpoint, and if the API key is valid.</p>
  <p>
    Example code originally taken from <a href="https://developers.google.com/freebase/v1/topic-overview">https://developers.google.com/freebase/v1/topic-overview</a> 2013-12.
    Compare with <a href="http://mql.freebaseapps.com/ch04.html#metaweb.php">http://mql.freebaseapps.com/ch04.html#metaweb.php</a> (Legacy example)
  </p>
  <p>Attempting to pull back 'topic' information. You should see "Bob Dylan"
    below the next line.</p>
  <hr />
  <?php

  include('freebase_googlecode_key.inc');
  $service_url = 'https://www.googleapis.com/freebase/v1/topic';
  $topic_id = '/en/bob_dylan';
  $params = array('key'=>$freebase_googlecode_key);
  $url = $service_url . $topic_id . '?' . http_build_query($params);
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_VERBOSE, true);
  // Um, SSL was failing in some combinations of PHP?
  // PHP 5.3.1.8 (Acquia dev desktop); cURL 7.21.2; 2013-12.
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);

  $time_start = microtime(true);
  $response = curl_exec($ch);
  $time_end = microtime(true);
  $time = $time_end - $time_start;
  $info = curl_getinfo($ch);
  curl_close($ch);

  $topic = json_decode($response, true);
  $time2 = microtime(true) - $time_end;
  $result = $topic['property']['/type/object/name']['values'][0]['value'];
  echo $result;
  ?>
  <hr />
  <p>
    Request to web service endpoint took
    <?php printf("%.3f", $time); ?>
    seconds. Data decode took
    <?php printf("%.3f", $time2); ?>
    seconds.

  </p>
  <?php
  if ($result != 'Bob Dylan') {
    ?>
  <p>
    If it did not show up, it means that the request to "<a
      href="&lt;?php echo $url ?&gt;"><?php echo $url ?> </a>" failed for some
    reason. Maybe firewall, or the API key has become invalid or overloaded, or
    Google changed <a href="https://developers.google.com/freebase/index">their
      APIs</a>.
  </p>
  <?php
  }
  ?>

  <pre><?php
    #print_r(get_defined_vars());
  ?></pre>
</body>
</html>
