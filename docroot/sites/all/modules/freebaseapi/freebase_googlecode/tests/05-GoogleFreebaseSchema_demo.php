<!DOCTYPE html>
<html>
<body>
  <h1>Test loading schema definitions from Freebase using the Google API MQL library</h1>
  <p>This test page runs an MQL query to retrieve schema info about Freebase Topic types.</p>

  <?php

  include('../freebase_googlecode.inc');
  function fb_link($id) {
    return "<code><a href='http://freebase.com${id}'>$id</a></code>";
  }
  function self_link($id) {
    return "<code><a href='?type_id=${id}'>$id</a></code>";
    # "<sup> <a href='http://freebase.com{id}' title='View on Freebase'>[FB]</a></sup>";
  }

  // Options used when setting up the library.
  $config = array(
  );
  // Options used when making a request.
  $params = array(
      #'key'=>$freebase_googlecode_key
  );

  // Request to test.
  $type_id = "/book/book";

  if (isset($_REQUEST['type_id'])) {
    // Allow user input, heavily sanitized.
    $type_id = preg_replace('/[^a-zA-Z0-9\/_]/', '', strip_tags($_REQUEST['type_id']));
  }

  ?>
    <form>
    <input name="type_id" value="<?php echo $type_id; ?>" />
    <input type="submit" />
    </form>
  <?php

  // Fetch its data schema.
  // This retrieves most of everything I want to know about the data
  // model and its properties.
  $schema_query_string = '{
"/type/object/id": "'. $type_id .'",
"/type/object/name": null,
"/type/object/type": "/type/type",
"/type/type/properties": [{
  "/type/object/id": null,
  "/type/object/name": null,
  "/type/property/expected_type": null,
  "/type/property/reverse_property": null,
  "/type/property/unique": null,
  "optional":"optional"
}],
"/freebase/type_hints/included_types": [{
  "/type/object/id": null,
  "/type/object/name": null,
  "/type/property/expected_type": null,
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/property/reverse_property": null,
    "/type/property/unique": null,
    "optional":"optional"
  }],
  "optional":"optional"
}],

"/type/reflect/any_reverse" : [{
  "id" : null,
  "name" : null,
  "link" : "/freebase/type_hints/included_types",
  "type" : "/type/type",
  "optional":"optional"
}]
}';
  // Watch out - this query is on the edge of too long!
  // It can trigger a segfault if a few more characters are added!

/*
 * Take care with "/type/reflect/any_reverse"
 * Adding this CAN make the query 'too difficult' for /book/book
 * though it's not too difficult for /time/event
 * Adding the additional desired filter by link type fixed this.
 * Otherwise I was getting all things that linked to book in all ways.
 */

/* Skipping the following useful bits for (string-length) safety right now:
 *
  "/common/topic/article" : [{
    "/type/object/id" : null,
    "optional":"optional"
  }]

    "/freebase/documented_object/documentation": [{
    "/common/document/content" : null,
    "optional":"optional"
  }]
*/


  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  $fb->mqlread->throwErrors = FALSE;

  // Do the request!
  $time_start = microtime(true);
  $schema = $fb->mqlread->mqlread($schema_query_string, $params);
  $time_end = microtime(true);
  $time = $time_end - $time_start;
  ?>

  <h2>Class Diagram</h2>

  <div style="border:1px solid black">
  <h4>
    <?php echo $schema['/type/object/name'] ?>
    (<code><a href="http://www.freebase.com<?php echo $schema['/type/object/id']; ?>" title="View on Freebase"><?php echo $schema['/type/object/id']; ?></a></code>)
  </h4>
  <table style="border:1px solid black">
  <tr><th>Field</th><th>Type</th><th>Included from</th></tr>
  <?php
  foreach ($schema['/type/type/properties'] as $property) {
    ?>
    <tr><td>
      <strong><?php echo $property['/type/object/name']; ?></strong> (<code><?php echo $property['/type/object/id']; ?></code>)
    </td><td>
      : <?php echo self_link($property['/type/property/expected_type']); ?>
    </td></tr>
    <?php
  }
  ?>

  <?php
  foreach ($schema['/freebase/type_hints/included_types'] as $included_type) {
    foreach ($included_type['/type/type/properties'] as $property) {
    ?>
    <tr><td>
      <strong><?php echo $property['/type/object/name']; ?></strong> (<code><?php echo $property['/type/object/id']; ?></code>)
    </td><td>
      : <?php echo self_link($property['/type/property/expected_type']); ?>
    </td><td>
      <em><a href="?type_id=<?php echo $included_type['/type/object/id']; ?>"><?php echo $included_type['/type/object/name']; ?></a></em>
    </td></tr>
    <?php
    }
  }
  ?>

  </table>
  <?php
  if (! empty($schema['/type/reflect/any_reverse'])) {
    ?>

    <h4>Derived types:</h4>
    <?php
    foreach ($schema['/type/reflect/any_reverse'] as $derivative_type) {
      echo("<a href='?type_id=${derivative_type['id']}'>${derivative_type['name']}</a>, ");
    }
  }
  ?>

  </div>

  </table>

  <hr />
    Request to web service endpoint took
    <?php printf("%.3f", $time); ?>
    seconds.
  <hr />

  <h2>Data</h2>

  <h3>MQL Request</h3>
  <pre><?php
  print_r(array(
      'config' => $config,
      'params' => $params,
      'query' => $schema_query_string,
  ));
  ?>
  </pre>

  <h3>MQL Response</h3>
  <small><pre>
  <?php print_r($schema);  ?>
  </pre></small>

</body>
</html>
