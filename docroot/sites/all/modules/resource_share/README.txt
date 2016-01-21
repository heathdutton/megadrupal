RESOURCE SHARE MODULE DOCUMENTATION

DESCRIPTION AND FEATURES
The Resource Share module is designed to serve a Drupal site's content and custom data via a URL. A content consumer can initiate a transfer by a URL call. It can be done in a client or in a browser.
The URL on the host site takes intuitive and flexible query requests. Here is an example: www.YourDrupalSite.com/api/content.json&before=2012-04-12&type=page
This query will return all content of type "page" created on YourDrupalSite before 2012-04-12, provided you have access to it.

Authentication is optional. It can be required or disabled by the admin of the host site (YourDrupalSite). If Authentication is “on” the consumer client will be required to provide a login/password pair. These can be obtained by registering at the host Drupal site (YourDrupalSite). If Authentication is “off” content will be served to the anonymous user with anonymous permissions. See "AUTHENTICATION" section below on details of how to authenticate a client.

Admin settings allow to choose what content types and fields will be served.

Resource Share returns JSON or XML, or other formats enabled in the Services module. Resource Share extends Services to create an API endpoint and to format output. It handles input parsing, security, data querying, and other aspects of serving content via a URL.

Resource Share depends on Services.

The Resource Share development is supported as part of www.SBA.gov site development at www.REIsystems.com.

A list of implemented features includes:
1. Intuitive and Flexible URL Query language
2. Drupal Session Authentication or Anonymous Querying depending on admin settings at ./admin/settings/Resource Share
3. User access to the API controlled by role for authenticated users
4. Admin settings of what content types and fields to expose to the consumer, logged and anonymous
5. Usage Logs and Stats
6. Solr Search Integration
7. Error Reporting and Tracking
8. Security Heuristics for Query Handling
9. Batch download for big data
10. Size of download estimate
11. Help call with a list of available content types and fields


INSTALLATION:
Download Resource Share, unzip, put into ./sites/all/modules or your favorite subdirectory. Install the module the usual Drupal way.
If the installation was successful you should be able to find:
1.	View page Conent Share statistics at /content_share_stats (should be empty after install)
2.	Settings at /admin/settings/content_share
--for expert users only:
3. table content_share_stats table in the db
4.	variables in “variable” table:
content_share_limit
content_share_require_authentication
content_share_content_types
content_share_content_fields



SETUP:
Install Services. Enable Services, Autoload, REST Server.
Configure an API endpoint in Services. You can do it by going to the Services set up page or by IMPORTing the API config. provided with this module in the SERVICES_ENDPOINT_IMPORT.txt file.
Or:
at admin/build/services/add
add: name
Server:REST
path: same as name
Debug Mode Enabled: NO
Apply Content Permissions: OFF
Session Authentication: YES

Resources: check all
Servers: check all

For Sol search enable CS Solr module and set server to local or Acquia service. Set server location and port for a local server.

SETTINGS:
1. Default Number of Hits:
The default maximum number of hits to return per query. Can be overidden in query and in Max number.
2. Query Cap:
Max number of hits to return in any single query. Overrides the default and user limits. Can be used to cap the server load.
3. Batch Size:
The number of hits to return per batch. To use with the 'batch' query term.
4. Block Anonymous Users, Require Authentication:
Block all anonymous usage and require Session Authentication from every user. Needs Session Authentication in Services Module Enabled.
If this box is checked off, no anonymous usage will be possible and anonymous users will get an error. 
If the box is unchecked, BOTH anonymous and logged-in (authenticated) users will be able to use Resource Share.
There are two columns of settings for the anonymous and the authenticated user as to what content types and fields
they can access. These settings are limiting. If a content type or a field is not checked off it will not be served.
5. Allowed Roles:
This setting affects only logged-in (authenticated) users. Only the checked off roles will be able to use Resource Share. Note that 
checking off "authenticated user" role enables anyone who is logged in to use Resource Share.
6. Exposed Types:
These setting applies to BOTH anonymous and logged-in (authenticated) users. The corresponding column sets the available
content types to either. Machine-readable names are in the left-most column. "Check/uncheck all" affects both columns.
7. Exposed Fields:
These setting applies to BOTH anonymous and logged-in (authenticated) users. The corresponding column sets the available
fields to either. "Check/uncheck all" affects both columns.


CONSUMER USE:
Query your data with the following format:
www.YourDrupalSite.com/YourAPIname/content.json&YourQuery
where:
	YourDrupalSite.com --- the URL of the site (host site) from which you want to get shared content.
	YourAPIname --- the API name set up in Services module on the host site, YourDrupalSite.com in our case
	YourQuery --- "query term"="query string"
		where:
			query term is one of:
				before  -- last edited on or before a date. The date can be specified as year-month-day or year-month. Note that year alone will cause an error.
				after   -- last edited on or after a date. The date can be specified as year-month-day or year-month. Note that year alone will cause an error.
				type    -- can be give as one type, e.g. type=story or as a plural, e.g. type=story+page. These are machine-readable type names. They typically do not contain spaces in them.
				search  -- Solr search. It takes a query term. Works only if CS Solr module is enabled and set up on the host site. 
				limit   -- the number of hits to return. This can be overiddern in host site admin settings to avoid server time outs.
				field   -- custom field(s) to include in output along with default fields. Can be singular e.g. field=myfield or plural, e.g. field=myfield+yourfield. Each field should be the machine readable name of the field with no spaces.
				sort    -- sorts on the created date, can take values "asc" for ascending sorting or "desc" for descending
				batch   -- possible values: 1, 2, 3,.. for large data, will return consecutive batches of size set in Batch Size
        size    -- only one possible value of 'size'. size=size returns ONLY the number of hits in your query. This is to estimate the size of your return prior to requesting it in full.
        help    -- returns a list of available content types and fields as well as a help message and a URL for further reference
Examples of use:
1. To return all content with last edit before 2012/04/12
www.YourDrupalSite.com/YourAPIname/content.json&before=2012-04-12
You can also write www.YourDrupalSite.com/YourAPIname/content.json&before=2012-nov-12

2.  To return all content with last edit after 2012/04/12. 
Same as in (1), but using "after" term
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12

3. To return only content of a certain type or types:
www.YourDrupalSite.com/YourAPIname/content.json&type=page
OR
www.YourDrupalSite.com/YourAPIname/content.json&type=page+story

4. To search in title and body:
www.YourDrupalSite.com/YourAPIname/content.json&search=needle in haystack

5. To limit the number of output hits in queries with other terms:
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&limit=50
Will output only 50 hits. Most recent if sort is not set to "asc"
Limit does not work with the "batch" term.

6. To return a custom (CCK) field:
www.YourDrupalSite.com/api/content.json&type=mycustomcontent&field=mycustomfield
To return two or more custom fields:
www.YourDrupalSite.com/api/content.json&type=mycustomcontent&field=mycustomfield+yourcustomfield

7. To sort the query output on the last edit date in ascending or descending order
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&limit=50&sort=asc
OR 
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&limit=50&sort=desc

8. To download large volumes of data that would otherwise time out the host server, use
the batch mode. It can be combined with all other terms with the exception of "limit". The
batch size is set by the admins of the host site.
To get the first batch:
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&batch=1
To get the second batch
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&batch=2
etc.
A client can use a recursive function to query the API incrementally until no more 
content is returned.

9. To get the number of hits in your query (without the hits themselves)
www.YourDrupalSite.com/YourAPIname/content.json&after=2012-04-12&size=size
Returns only "size":'Number of hits on your site'

10. To get a list of available content types and fields as well as a URL for further reference
www.YourDrupalSite.com/YourAPIname/content.json&help=help
Returns only the help info

MIXING AND MATCHING
Query terms can be mixed and matched in any order with the exception of limit with batch. Batch limits 
are set on the host site. For example, to get all content of type "page" created on YourDrupalSite 
before 2012-04-12 with the field "location" you would write "before=2012-04-12&type=page&field=location". 
www.YourDrupalSite.com/YourAPIname/content.json&before=2012-04-12&type=page&field=location
			
			
For all queries you can specify XML or JSON. For XML your query would look like this:
www.YourDrupalSite.com/YourAPIname/content.xml&YourQuery

The query returns only nodes that are currently in the "published" state on the host site.

ERROR HANDLING
If the query returns an error the output in most cases will contain error:error pair and the particular error message.
							
AUTHENTICATION
If you use a software client to consume the content with this API and the admin of the host site has set an authentication requirement, you will need to provide an authentication mechanism in you client.

The basic workflow for authentication is as follows:
1. register with the host site and obtain a login/password pair

2. from your client, make a call to:
www.YourDrupalSite.com/YourAPIname/user/login
with the login/password included in the POST request and the format set to e.g. JSON
$user_data = array(
  'username' => 'my_user_name',
  'password' => 'my_password',
);
array('Accept: application/json')
This can be accomplished with cURL or any other module/class for URL calls depending on the programming language and your preferences.

3. Get the server response and if its 200, extract a cookie for future authenticated requests.
This can be done in PHP and JSON like this:
  $logged_user = json_decode($response);
  $cookie_session = $logged_user->session_name . '=' . $logged_user->sessid;
  
4. Query Resource Share over the usual URL www.YourDrupalSite.com/YourAPIname/content.json&YourQuery
providing the $cookie_session in your headers.

Further detail and sample client code can be found at http://drupal.org/node/910598.

FAQ
1. Why does my query returns empty?
This can be because:
a) there are no nodes on the site that match your query criteria
b) there are no nodes of the content TYPE that are exposed to you. This can be true if you are an anonymous user 
or if you are logged in and your ROLE on this site does not allow you to view this particular content type.
