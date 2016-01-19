# $Id$

Instapaper API for Drupal

--- Overview ---
This module is designed as a way to allow other developers to implement the Instapaper API within a single function call in Drupal.  The module creates a function called instapaper_api_call that takes two variables, an Operator ($op) and the Date ($data).  All data should be sanitized before being sent to the function.  The function returns a stdObj with the result code from Instapaper as well as the URL that Instapaper saved as well as the title that Instapaper saved it as.  These can be retrieved as follows:
 $results->code will provide the response code (documented below)
 $results->url will provide the saved URL on a successful save (only for $op ADD)
 $results->title will provide the saved TITLE on a successful save (only for $op ADD)

--- Variables ---
The Instapaper API function takes two variables, $op and $data.
  $op must be either AUTH (authentication) or ADD (add to Instapaper)
  $data must be an array of data to be sent to Instapaper. (see usage examples)
    an AUTH array must include username and password
		an ADD array must include username, password, and url
		
		username: Required field for both AUTH and ADD
		password: Required field for both AUTH and ADD. Remember that not all users have a password
		url: Required for ADD
		title: Optional for ADD. Plain Text, no HTML or UTF-8. If omitted will be automatically generated
		selection: Optional for ADD. Plain Text, no HTML or UTF-8. Will show up as the description under an item in the interface. Can be used to describe where the item came from. Also a good place for the Node teaser if saving a node
		redirect=close: Optional for ADD. Specifies that, instead of returning the status code, the resulting page should show an HTML “Saved!” notification that attempts to close its own window with Javascript after a short delay. This is useful if you’re sending people directly to /api/add URLs from a web application. - Currently untested.

--- Usage Examples ---
Here are two snippets of example code that could be used to call the Instapaper API complete with code handling. For a more complete listing of what the response codes mean, see Response Codes below.

AUTH (Authentication) Example:
	$op = 'AUTH';
	$data = array ('username' => 'example@foo.bar',
	               'password' => 'myPassword',);
	$results = instapaper_api_call($op, $data);
	switch ($results->code) {
		case 200: return 'Username and Password Accepted!';
		case 403: return 'Invalid Username or Password';
		case 500: return 'An error has occurred, please try again later';
	}

ADD (Add to Instapaper) Example:
	$op = 'ADD';
	$data = array ('username' => 'example@foo.bar',
	               'password' => 'myPassword',
	               'url' => 'www.foobar.com/node/2'
	               'title' => 'My Node Title',
	               'selection' => 'From My Website',
	               'redirect' => 'close');
	$results = instapaper_api_call($op, $data);
	switch ($results->code) {
		case 201: return ($results->url.' saved as '.$results->title.' in your Instapaper account!');
		case 400: return 'A bad request was made to Instapaper';
		case 403: return 'Invalid Username or Password';
		case 500: return 'An error has occurred, please try again later';
	}
	

--- Response Codes ---
These are the possible response codes that can be returned from the API

Responses for AUTH Method
 200: Username and Password Combination OK
 403: Invalid Username or Password
 500: The service encountered an error
Responses for ADD Method
 201: URL has been added to the Instaaper account
 400: Bad request or exceded rate limit, probably missing required parameter, such as url
 403: Invalid Username or Password
 500: The service encountered an error