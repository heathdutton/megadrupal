<!DOCTYPE html>
<html>
<body>
  <h1>Test if the Google API MQL library works</h1>
  <p>
    This test page runs through a few of the examples found at <a href="https://developers.google.com/freebase/v1/mql-cookbook">https://developers.google.com/freebase/v1/mql-cookbook (2013)</a>
  </p>
  <p>
    <em>Allegedly</em> in the API docs I should be seeing a <code>status</code> in the response.
    I'm not, but instead I add my own <code>code</code> value from the request.
  </p>

  <?php
  include('../freebase_googlecode.inc');
  include('freebase_googlecode_key.inc');

  // Options used when setting up the library.
  $config = array(
  );
  // Options used when making a request.
  $params = array(
    #'throw_exception' => FALSE,
  );

  $recipes[] = array(
    'description' => 'Count the list of types in "architecture", (return a list for no good reason?).',
    'query' => '
    [
      {
        "domain" : "/architecture",
        "id" : null,
        "return" : "count",
        "timestamp" : null,
        "type" : "/type/type"
      }
    ]
    ');

  $recipes[] = array(
    'description' => 'Count the list of types in "architecture", (return a value, because that makes more sense).',
    'query' => '
      {
        "domain" : "/architecture",
        "id" : null,
        "return" : "count",
        "timestamp" : null,
        "type" : "/type/type"
      }
    ');

  $recipes[] = array(
    'description' => 'Fetch the /music/artist item named "The Police", filling in just its genre values.',
    'query' => '
   {
     "active_start":null,
     "genre":[],
     "name":"The Police",
     "type":"/music/artist"
   }
    ');

  $recipes[] = array(
    'description' => 'Inspect the expected types of some properties of a /music/artist.',
    'query' => '
   {
     "id":"/music/artist",
     "properties":[{
       "expected_type":{
         "default_property":null,
         "id":null
       },
       "id":null,
       "id|=":["/music/artist/genre","/music/artist/active_start"]
     }],
     "type":"/type/type"
   }
    ');

  $recipes[] = array(
    'description' => 'Fetch simple info about the item found by ID.',
    'query' => '
{
 "*": [],
 "id": "/en/skull"
}
      ');

  $recipes[] = array(
    'description' => 'Fetch the types of each of the different items found by the same name.',
    'query' => '
[{
 "id": null,
 "name": "Kiwifruit",
 "type": []
}]
      ');


  $recipes[] = array(
    'description' => 'Search by name, limited by domain for disambiguation. Still expect a list though, hopefully of one item. <small>(again, if limiting to 1 item, the list is unneccessary, but may be handy convention)</small>',
    'query' => '
[{
 "id": null,
 "name": "Malaria",
 "type": {
   "key": {
     "namespace": "/medicine",
     "limit": 1
   },
   "limit": 1
 }
}]
      ');

  $recipes[] = array(
    'description' => "Intentionally empty - search for something that does not exist, expect an empty set back and a 204 code.</small>",
    'query' => '
[{
 "id": "/en/black",
 "name": "White"
}]
      ');

  $recipes[] = array(
    'description' => "Intentionally invalid. This is JSON, but makes no sense to Freebase. Expect a 400 error response.",
    'query' => '
[{
 "what": "is this",
 "1": {}
}]
      ');

  $recipes[] = array(
    'description' => "Intentional error. This not JSON. Expect a JSON-parser code 4.",
    'query' => '
Throw an error if you see this text string!
      ');

  $recipes[] = array(
    'description' => "Unintentional error. JSON is picky about its trailing commas. Hate that.",
    'query' => '
[{
 "id": "/en/black",
 "name": "Black",
}]
      ');

  $recipes[] = array(
    'description' => "Either an Unknown error or intentionally blank. Currently this returns an unparsable (blank?) result. This is because I asked for a single item by id and there was no match so I got nothing. However it's indistinguishable from an unknown error.",
    'query' => '
  {
"/type/object/id": "/book/writtenwork",
"/type/object/name": null
}
      ');

  $recipes[] = array(
    'description' => "Break the API. This is apparently '403 query too difficult'.",
    'query' => '
  [{
  "/type/object/id": "/book/book",
  "/type/object/name": null,
  "/type/reflect/any_reverse" : [{
    "*" : null,
    "type" : "/type/type"
   }]
}]
      ');

  $recipes[] = array(
    'description' => "Kill the API. The service can produce uncatchable death (segfault) if you ask it for some stuff.",
    'query' => '
[{
  "/type/object/id": "/time/event",
  "/type/object/name": null,
  "/type/object/type": "/type/type",
  "/type/reflect/any_reverse" : [{
    "id": null,
    "type" : "/type/type"
   }]
}]
      ');


  /*
  $recipes[] = array(
    'description' => 'Places inside places (Explicit transitive lookup).',
    'query' => '
    [{
      "/location/location/containedby":[{
        "/location/location/containedby":[{
          "/location/location/containedby":[{
            "id":"/guid/9202a8c04000641f8000000000959f60"
          }],
          "name":null
        }],
        "name":null
      }],
      "name":null,
      "type":"/education/university"
    }]

      ');
*/

  ?>
  <p>Loading library service instance..</p>
  <?php
  // Create the service handle.
  $fb = freebase_googlecode_get_freebase($config);
  if ($fb) {
    echo("Library instantiated OK ");
    echo(!empty($fb->version) ? $fb->version : "UNVERSIONED");
    $fb->mqlread->throwException = FALSE;
  }
  else {
    echo("Failed to initialize a Service handle");
    exit;
  }
  ?>
  <hr />

  <h2>Making MQL read requests</h2>

  <table>
  <?php
  foreach ($recipes as $recipe) {
    ?>
    <tr><td colspan="2"><hr/><?php echo $recipe['description']; ?></td></tr>
    <tr><td>
    <pre><?php echo $recipe['query']; ?></pre>
    </td><td>

    <?php
      $time_start = microtime(true);
      $result = $fb->mqlread->mqlread($recipe['query'], $params);
      $time_end = microtime(true);
      $time = $time_end - $time_start;
    ?>

    <hr/>
    <pre><?php print_r($result); ?></pre>
    <hr/>
    <dl>
    <dt>code</dt><dd><?php @print_r($fb->mqlread->lastResponse['code']); ?></dd>
    <dt>message</dt><dd><?php @print_r($fb->mqlread->lastResponse['message']); ?></dd>
    <dt>status</dt><dd><?php @print_r($fb->mqlread->lastResponse['status']); ?></dd>
    <dt>time</dt><dd><?php printf("%.3f", $time); ?> seconds</dd>

    </dl>
    <pre><?php
      #print_r($fb->mqlread->lastResponse);
    ?></pre>

    </td></tr>
    <?php
  }
  ?>
  </table>


</body>
</html>