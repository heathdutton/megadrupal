<!DOCTYPE html>
<html>
<body>
  <h1>Test if the Google API MQL library works</h1>
  <p>
    This test page loads the Google API PHP lib and tries to use it to run Mql queries.
    Demonstrates the php array syntax alternative to JSON MQL.
  </p>


  <?php
  include('../freebase_googlecode.inc');
  include('freebase_googlecode_key.inc');

  // Options used when setting up the library.
  $config = array();
  // Options used when making a request.
  $params = array();

  $topic_mid = '/m/04093'; // Jules Verne
  // Search for the thing with the given mid and list its types.
  $query = (object)array(
    "/type/object/mid" => $topic_mid,
    "/type/object/name" => null,
    "/type/object/type" => array((object)array(
      "/type/object/name" => null,
      "/type/object/id" => null,
    )),
  );

  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  if (!$fb) {
    echo("Failed to initialize a Service handle");
    exit;
  }
  $fb->mqlread->throwException = FALSE;
  ?>

  <hr />
  <h3>Sample PHP code</h3>

  <pre>
  $topic_mid = '/m/04093'; // Jules Verne
  // Search for the thing with the given mid and list its types.
  $query = (object)array(
    "/type/object/mid" => $topic_mid,
    "/type/object/name" => null,
    "/type/object/type" => array((object)array(
      "/type/object/name" => null,
      "/type/object/id" => null,
    )),
  );
  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  // Make the request.
  $result = $fb->mqlread->mqlread($query);
  print_r($result);
  </pre>

  <hr />
  <h2>Making MQL read request</h2>

  <pre>
  <?php
  print(htmlentities(print_r(array(
    'config' => $config,
    'params' => $params,
    'query' => $query,
  ), 1)));
  ?>
  </pre>

  <hr />

  <?php
  // Do the request!
  $time_start = microtime(true);
  $result = $fb->mqlread->mqlread($query, $params);
  $time_end = microtime(true);
  $time = $time_end - $time_start;
  ?>

  <h2>Result</h2>
  <h3><?php echo $result['/type/object/name']?> is a:</h3>
  <ul>
  <?php
  foreach ($result['/type/object/type'] as $type) {
    echo "<li title='${type['/type/object/id']}'>${type['/type/object/name']}</li>";
  }
  ?>
  </ul>

  <hr/>
  <small><pre>
  <?php print_r($result); ?>
  </pre></small>

  <hr />
  <p>
    Request to web service endpoint took
    <?php printf("%.3f", $time); ?>
    seconds.
  </p>
  <hr />

</body>
</html>