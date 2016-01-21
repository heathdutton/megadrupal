<!DOCTYPE html>
<html>
<body>
  <h1>Test if the Google API MQL library works</h1>
  <p>
    This test page loads the Google API PHP lib and tries to use it to run Mql queries.
  </p>
  <p>You should be able to insert samples from the <a href="https://developers.google.com/freebase/v1/mql-cookbook">MQL cookbook</a> here
  and get results a tiny bit like the Freebase MQL (<a href="http://wiki.freebase.com/wiki/Acre">Acre - Cuecard</a>) console used to do - until Google changed things and it broke.
  Note that <a href="https://developers.google.com/freebase/v1/">the current Freebase handbook</a> on developers.google.com is only a tiny fraction of the <a href="http://mql.freebaseapps.com/ch03.html">Original Freebase developers guide</a> which is much more useful. <em>But</em> it may be out of date in key or undocumented places. YMMV.
  </p>
  <p>If experimenting with cookbook recipes, just use the fragment <em>inside</em>
  the <code>"query": ...</code> part. Ignore the <code>{"q1":{"query":</code> wrapper stuff.
  </p>

  <?php
  include('../freebase_googlecode.inc');
  include('freebase_googlecode_key.inc');

  // Options used when setting up the library.
  $config = array(
  );
  // Options used when making a request.
  $params = array(
    #'throw_exception' => TRUE,
  );

  $topic_mid = '/m/04093'; // Jules Verne
  // Search for the thing with the given mlid and list its types.
  $query_string = '
[{
  "/type/object/mid": "'. $topic_mid .'",
  "/type/object/name": null,
  "/type/object/type": [{
    "/type/object/id": null,
    "/type/object/name": null
  }]
}]';


  if (isset($_REQUEST['query_string'])) {
    // Allow user input, naively sanitized.
    // If it parses as JSON, it's probably therefore encoded in a way
    // that's safe to reflect on screen?
    $unpacked = json_decode($_REQUEST['query_string']);
    if (json_last_error() == JSON_ERROR_NONE) {
      $query_string = $_REQUEST['query_string'];
    }
    else {
      echo "Submitted JSON did not parse, probably syntax error. Discarding user input and resetting.";
    }
  }

  ?>
    <form>
    <textarea name="query_string" cols="80" rows="10"><?php echo htmlentities($query_string); ?>
    </textarea>
    <input type="submit" />
    </form>
  <?php


  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  if (!$fb) {
    echo("Failed to initialize a Service handle");
    exit;
  }
  $fb->mqlread->throwException = FALSE;
  ?>

  <hr />

  <h2>Making MQL read request</h2>
  <h3>Running lookup with options</h3>
  <pre>
  <?php
  print(htmlentities(print_r(array(
    'config' => $config,
    'params' => $params,
    'query' => $query_string,
  ), 1)));
  ?>
  </pre>
  <hr />

  <?php
  // Do the request!
  $time_start = microtime(true);
  $result = $fb->mqlread->mqlread($query_string, $params);
  $time_end = microtime(true);
  $time = $time_end - $time_start;
  ?>

  <h2>Result</h2>
  <pre>
  <?php
  print_r($result);
  ?>

  <hr />
  </pre>
  <h2>Response</h2>
  <pre>
  <?php
  print_r($fb->mqlread->lastResponse);
  ?>
  </pre>

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

</body>
</html>