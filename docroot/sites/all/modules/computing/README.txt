Drupal Computing: Documentation
===============================

This documention is about installation, configuration and development with the Drupal Computing module. For an overview of the module, see http://drupal.org/project/computing. To learn more about the Java and Python client library, see https://github.com/danithaca/drupal-computing.



Installation
------------

Install the Drupal module following the standard procedure. Next, install the companion [Java and Python client library](https://github.com/danithaca/drupal-computing) to any server, preferrably not on the same Drupal production server.



Administration
--------------


Go to the admin page at: admin/config/system/computing (requires "administer computing module" and "administer computing records" permissions). In addition to the Overview two, you will see two other tabs:

  1. Command Console. Here is for admins to input data for agent programs. You'll see more options after you install other modules (such as Recommender API).
  2. Records. You can see the list of "computing record" entities, which server the purpose of passing data between Drupal module and agent programs. You can use "Views" module to customize display, and the "Views Bulk Operations" module to manage individual record, such as "Delete".


### Configure Drush ###

This step is required if you plan to use Drush in your agent program to interact with Drupal. On the server where you will run the agent program, create a "drush site alias" that points to your Drupal installation. Read more about "drush site alias" in the Drush documentation.


### Configure Services Module ###

This step is required if you plan to use Services module in your agent program to interact with Drupal. First, follow the Services module documentation and create an endpoint. You need REST Server for the endpoint, and make sure to check "json" in "Response formatter", "application/json" in "Request parsing". Turn on at least the following resources:

  * "computing": everything.
  * "system": "connect".
  * "user => 'login' and 'logout'.
  
You will also need to create a user and assign at least "access computing services endpoints" and "administer computing records" permissions. Your agent program will use that user to authenticate. Other authentication approaches (e.g. OAuth) are not support yet. 


### Rules Integration ###

Events will be triggered when a computing record is created, updated, claimed, finished and released. Configure rules actions as needed (e.g., send an email when an agent program marks a record as finished). You can also enable the default rule that handles 'result callback' when a computing record is finished successfully.




Development
-----------


### Computing Record ###

Computing records are Drupal entities, which serve these purposes:
  
  * Passing data between Drupal and agent programs. Alternative: Drupal variables (`variable_get()`, `variable_set()`) but less flexible.
  * Enabling a work queue system for distributed computing. Alternatives: beanstalkd, but not as tightly integrated to Drupal; or Drupal's SystemQueue, but w/o Services support.
  * Tracking computational logs. Alternative: Drupal's watchdog, but mixed with other logs.
  * Allowing Rules, Views, and EntityAPI integration and centralized admin interface for user input. Alternative: your own customized solutions.
  
If you don't need those or if you prefer the alternatives, you don't have to use computing record. Otherwise, you would use computing record in one of the two ways:

  * If you don't pass data from Drupal to agent programs (i.e., no dynamic input from admins), you can directly use your agent program to save a newly created computing record with computational results back into Drupal (using the Java/Python APIs) .
  * Or usually, you would use your Drupal module to create a new computing record entity with input data and put it in a queue. Your agent program claims the computing record, takes the input data, computes results, and saves results back to the computing record in Drupal.

Developers should use the following APIs to access computing record entities, instead of using Entity API directly. See more details in the computing.module file about how to use the functions.

  * computing_load(): Load a computing record entity.
  * computing_create(): Create a computing record entity and save to Drupal database.
  * computing_update(): Update a computing record.
  * computing_update_field(): Update a field of the given computing record.
  * computing_claim(): Claim a computing record not yet handled from the queue.
  * computing_release(): Release a computing record previously claimed by an agent back to the queue.
  * computing_finish(): Mark a computing record as either "successful" or "failed".

The "status" property is encoded as 3-character long string:

  * RDY (Ready): ready for agent.
  * RUN (Running): agent is running.
  * SCF (Successful): agent runs successfully.
  * FLD (Failed): agent runs failed.
  * ABD (Aborted, or Abandoned): program aborted for whatever reasons.
  
Usually you should use the "input" and "output" properties to pass data between Drupal and agent programs. Those are stored as JSON blob strings in the database, but are automatically encoded or decoded from or into PHP arrays when they are handled with the APIs above. If you need more flexible data structure for your agent applications, you could use Field API to add customized fields to a new computing record bundle (see below).



### Computing Application ###

A "computing application" is the "bundle" type for "computing record" entities, similar to that of "content types" to "contents" in Drupal. Each computing application is responsible to handle commands registered with it. Usually you only need to use the default "computing" application. To create a new computing application, use the following code in `hook_install()`, e.g.:

    $app = computing_application_create_entity(array(
      'application' => 'recommender',
      'label' => 'Recommender Application',
      'description' => 'Application for Recommender API.'
    ));
    $app->save();

Drupal Computing doesn't provide UIs to administer fields for computing record bundles. If you want to add/edit fields, you will have to do it pragmatically. For example, to add a new field:

    $instance = array(
      'label' => 'Quantity',
      'field_name' => 'field_quantity',
      'entity_type' => 'computing_record',
      'bundle' => 'recommender',
      'settings' => array(),
      'required' => FALSE,
    );
    field_create_instance($instance);



### hook_computing_data() ###

The hook returns an array, where the keys are the compution application names, and the values are arrays of commands whose keys are command names and values are command definitions. For example:

    array(
      'application_name_1' => array(
        'app1_command1' => array(
          // command definition array for 'app1_command1'...
        ),
        'app1_command2' => array(
          // command definition array for 'app1_command2'...
        ),
      ),
      // definitions for other applications.
    );
        
Command definitions are as follows:

  * __'title'__: Human-readable name of the command.
  * __'description'__: More description about the command.
  * __'command'__: The real command to put in the computing record's "command" field, if different from the "command name" key of the array.
  * __'form callback'__: Name of the callback function that generates a form in "Command Console". It takes these parameters: $form, &$form_state, $command_data. Default is 'computing_command_form'.
  * __'form elements callback'__: Valid when 'form callback' is 'computing_command_form'. Specify this callback to generate a list of form elements to add to 'computing_command_form', which will automatically populate as to the computing record's "input" field.
  * __'result callback'__: Name of the callback function to handle "output" from agent. Valid only when rules are enabled. It takes these parameters: $output (json array from agent), $record (computing record).
  * __'file'__: the file to look for the callback functions.

For example:

    $data = array(
      'computing' => array(
        'echo' => array(
          'title' => 'Echo message',
          'description' => 'Echos input message as output message.',    
          'form elements callback' => 'computing_echo_optional_form_elements',
          'result callback' => 'computing_echo_handle_result',
          'file' => array('type' => 'inc', 'module' => 'computing', 'name' => 'computing.admin')
        )
      ),
    );
    return $data;



### Using Drush ###

Drupal Computing module exposes two Drush commands:

  * __drush computing-call__ (see function `drush_computing_call()`): Execute any Drupal function and returns results in JSON (with the --pipe option), where the first argument is the funcation name, and the var args are parameters encoded as JSON string. E.g. `drush computing-call node_load 1`
  * __drush computing-eval__: Execute any piece of Drupal code and returns results in JSON (with the --pipe option). E.g., `drush computing-eval "return node_load(1);"`. This is different from `drush php-eval` because the latter does not return results in JSON unless you explicitly use `echo`.
  
Both drush commands use full Drupal bootstrap, which might be slow. 



### Using Services Module ###

Drupal Computing module exposes these services resources:

  * CRUD on computing record entities:
    - Get computing record entity: __GET computing/%id.json__
    - Create computing record entity: __POST computing.json__. Parameters: "application", "command", "label" (optional), "input" (optional) and other fields.
    - Update computing record entity: __PUT computing/%id.json__. Parameters: "id" and other fields.
    - Update a single field of a computing record: __POST computing/%id/field.json__. Parameters: "id", "field_name", "field_value".
  * Queue related:
    - Claim a computing record to process: __POST computing/claim.json__.
    - Mark a computing record as finished: __POST computing/finish.json__. Parameters: "id", "status" (3-character code), "message", "output", "options".
  * System info related (Drupal version, server time):
    - Get system info: __POST computing/info.json__.



### Direct Drupal Database Access in Agent Programs ###

Instead of directly access the Drupal database, consider these alternatives to use large amount of data in Drupal:

  * Expose Drupal data using either Services or Feeds, and use your agent program to access those exposed data.
  * Export data to a file, and saves the file name and location in a computing record to pass to your agent program.
  * Configure another database in Drupal's settings.php, e.g., `$database['data']['slave']`, and have both Drupal and agent programs use that database.
  * Directly save data in a computing record to pass to your agent program.
  
If you do need direct access to Drupal database, consider these approaches:

  * Use the drush utitity, e.g., `drush.computing_eval("return db_query(...);")`. You can execute any db queries in the Drupal way. The returned data would be a large JSON array in RAM. Be aware that this approach uses the "drush" system command and might have OS limitation on how big data can be returned.
  * Set database access info in config.properties, particularly the dcomp.database.properties.* settings. Then code your agent program to access database directly. You might need to configure the firewall on Drupal server in order for remote agent program to access database directly.
  * Save settings.php `$database['default']['default']` or `$database['data']['slave']` in the computing record to tell your agent program how to access database.

