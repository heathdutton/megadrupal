
=== Meetup API Module ===
This module, as the name implies, allows using the Meetup API via a Drupal module. It depends on the PHP Meetup API Client by WizOne Solutions, which you can download from http://github.com/wizonesolutions/meetup_api/downloads.

=== Requirements ===
You need the Libraries API (http://drupal.org/project/libraries) and cURL (http://drupal.org/project/curl) modules to use this one.

You also need the PHP Meetup API Client (see Installation just below).

Finally, the Meetup API service requires that you have a Meetup API key. The module will help you find yours if you don't already have one. It is free to obtain one and requires that you create a meetup.com account.

=== Installation ===
Download the PHP Meetup API Client from the link above and unpack it into your libraries directory. If you are not sure what this is, then unpack it into sites/all/libraries/meetup_api.

=== Before Using This Module ===
To understand how best to utilize the Meetup API, it is recommended to familiarize yourself with the Meetup API documentation, located at: http://www.meetup.com/meetup_api/docs. The method names and and available parameters are similar, and usually identical, when used from within the Meetup API module.

=== Usage Examples ===
This section will be expanded into its own API.txt document as time goes on. For now, the basic pattern to make a request is as follows.

  <?php
  //Find gnostic groups near the zip code 90046, and return the ones within a radius of 50 miles
  $request = array('topic' => 'gnostic', 'zip' => '90046', 'radius' => 50); //Set the parameters for the API request
  $options = array('pages' => 1); //Set options that are not specific to the method: page (page size), pages (total number of result pages to return), and desc (sort descending) are currently supported
  $results = meetup_api_groups($request, $options); // Execute the request. After this call, $results will contain $results->results (an array of returned group objects, each containing Meetup API properties) and $results->meta (Meetup API meta information).

  //Cycle through the results and displaying the name of each
  foreach($results->results as $result) {
    drupal_set_message(t("Group name: @name", array('@name' => $result->name)));
  }
  ?>

The pattern for the other methods is the same. You can also use any READ method that haven't been specifically implemented yet. In this case, you have to do a little more.

  <?php
  // Retrieve the event comments for the fictitious event ID 1234567
  $request = array('event_id' => 1234567);
  $options = array('method' => '2/event_comments'); //Note the usage of the method option. Here you pass what comes after http://api.meetup.com/
  
  $results = meetup_api_request($request, NULL, $options); // The second parameter can also be an instance of the MeetupAPIBase class or a child class, if for some reason you have created one. If not, leave the argument as NULL, and an instance of MeetupAPIBase will be created for you.

  // Continue as in the previous example...
  ?>

=== Error Handling ===
Error handling is currently left to the Meetup API itself. You should check the $results->meta response code to ensure you receive results from the API.

=== Meetup API Limits ===
You should know that the Meetup API service itself limits you to 100 requests per hour and 200 results per request. The PHP Meetup API Client comes with a configuration file that configures the default page size to 200 *** and the number of pages to 1***. If you want all results up to the total count to be returned, then set $options['pages'] to 0 in your API request. Note that you will exhaust your limits more quickly since the API Client will need to make multiple requests to retrieve all the results. Meetup will also raise your limits for you if you ask them; see http://www.meetup.com/meetup_api/docs#support for their Developer Support contact information.
