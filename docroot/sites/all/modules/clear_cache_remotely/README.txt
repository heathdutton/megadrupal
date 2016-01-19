This module allows you to clear the cache remotely. This can 
be useful if you have multisite installation with multiple databases.

Once enabled, you will need to set up a secret key to be passed as a 
part of a call.

To make it a little more secure there is is also an option to lock the 
processing of request to a list of IP addresses.

Example usage:

Say you have set up a secret key: "EXAMPLE_KEY_NUMBER"

You should send a request to
http://yourdomain.com/clear_cache_remotely/all/EXAMPLE_KEY_NUMBER

There is also a number of options possible for only certain types of 
cache to be cleared which is determined by the first parameter you pass. 

Available options are:

1. "all" - will clear all the caches
Example URLs:
http://yourdomain.com/clear_cache_remotely/all/EXAMPLE_KEY_NUMBER

2. "css" - will flush CSS cache

Example URLs:
http://yourdomain.com/clear_cache_remotely/all/EXAMPLE_KEY_NUMBER

3. "js" - will flush JS cache

Example URLs:
http://yourdomain.com/clear_cache_remotely/all/EXAMPLE_KEY_NUMBER

4. "boost" - if Boost is installed you can clear all boost static files

Example URLs:
http://yourdomain.com/clear_cache_remotely/all/EXAMPLE_KEY_NUMBER

5. "table_*" - will check if cache table exists and if so, will clear it. 
"table_" will clear "cache" table, "table_page" will clear "cache_page" 
and so on.

Example URLs:
http://yourdomain.com/clear_cache_remotely/table_/EXAMPLE_KEY_NUMBER
http://yourdomain.com/clear_cache_remotely/table_page/EXAMPLE_KEY_NUMBER

The output will be a JSON output.
