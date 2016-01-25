INTRODUCTION
----------------
This module is designed to emulate simpler caching systems like those included
in modern PHP frameworks, there are four simple functions: set, get, has, and
del (delete).

PLEASE NOTE
----------------

This module, as with any other database-based utility, will not sanitise
data for you, as a result of this, you should ensure that any inputted data
is properly sanitised into a safe state before inserting new data, or printing
stored data.

INSTALLATION
----------------
Simply activate the module.


USAGE
----------------
Simply set a value with dx_cache_set($key, $value, $time_to_live),
for example you can set an item that will expire in 30 seconds like this:

dx_cache_set("my.key", array('my' => 'value'), 30);

Then you can check if it exists (and is valid) using:

dx_cache_has('my.key'); // Return true or false

You can retrieve the value using:

$myvalue = dx_cache_get('my.key'); - Returns the value if valid,
null if expired, and null if it does not exist.

NOTE: This is a change from version 1.0, which would return false instead of null.

You can also set a default value to return if the key is not set or expired:

$myval = dx_cache_get('my.key', 'value to return if not found or expired');

And you can delete values using:

dx_cache_del('my.key');

Cron will also periodically delete expired keys every time it runs.
