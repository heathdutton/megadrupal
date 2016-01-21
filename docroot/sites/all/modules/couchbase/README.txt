Introduction
============
The Couchbase module provides integration with Couchbase 2.0 and above.  This module 
consists of helper functions to instantiate couchbase connections from the $conf['couchbase']
variable in settings.php.

The module also provides a REST interface to Management REST API provided by Couchbase.  
The Management REST API is described here: http://www.couchbase.com/docs/couchbase-manual-2.0/couchbase-admin-restapi.html

Finally, the module provides an api for accessing creating and deleting Couchbase Views.  *These
functions will probably go away once the php-ext-couchbase matures.  Currently, the php-ext-couchbase
library doesn't support creating/deleting views.*

The module couchbase_watchdog is an example implementation of watchdog in Couchbase.
It serves as an example of what can be done with the API. 

The Couchbase module is being worked on and will support Couchbase 2.0 and above.  Since Couchbase 2.0 is in
active development, this module will not see a stable release until such time.

*This module is in active development.  Please help test and report bugs back*


Prerequisite
============
In order to use the couchbase module, the php-ext-couchbase extension and the libcouchbase native libraries 
have to be installed.  

Download the php-ext-couchbase extension from: http://www.couchbase.com/develop/php/next

Download the libcouchbase library from: http://www.couchbase.com/develop/c/current

Installation
============
The Couchbase module requires a $conf['couchbase'] associative array to be declared in 
settings.php before the module can be enabled.  The module will not be enabled if 
without $conf['couchbase'].

* Note, the 'default' cluster must exist.  Additional couchbase connections can be declared
and used.  The Couchbase module and its submodules use the default connection. 

The $conf['couchbase'] is configured as followed:

$conf['couchbase'] = array(
  'default' => array(
    'host' => 'localhost',
    'port' => 8091,
    'user' => 'username',
    'password' => 'password',
    // options can be passed
    'options' => array(
      'name' => 'value',
    ),
  ),
);

Module Usage
============
Once installed, the couchbase_watchdog module can be enabled.  This module demonstrates how send/retrieve data
from Couchbase.

*TL;DR version*
To get Ccouchbase connection, the following variations can be used:
$cb = couchbase();  // this makes a connection to the default server connection and uses the default bucket
$cb = couchbase('myserver', 'mybucket');  // this makes a connection to 'myserver' connection, and uses the 'mybucket' bucket

To programmatically create a new Couchbase bucket via this module: 
$cb_admin = couchbase_admin();  // initializes a connection to the 'default' server
$cb_admin->createBucket('mybucket', 128); // create a bucket called 'mybucket' and give it 128MB of RAM

To programmatically create a new Couchbase view:
$documents = array(
  '_id' => '_design/watchdog_views',
  'views' => array(
    'list_by_date' => array(
      'map' => 'function (doc) {if (doc) {emit([doc.timestamp], doc);}}',
    ),
  ),
);
$cb_rest = couchbase_rest(); // initializes a connection to the 'default' server
$cb_rest->createDesignDoc('drupal_watchdog', 'watchdog_views', $documents);

RoadMap
=======
- Implement caching with Couchbase (much like memcached)
- Implement Views Integration
- Implement entity api


Copyright
=========

Copyright (c) 2011-2012 DOCOMO Innovations, Inc.

End of README.txt