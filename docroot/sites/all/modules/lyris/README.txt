Lyris MLM Integration Module
================================================================================
Version 7.x-2.x-dev
Author, Chris Albrecht (chris@162unlimited.com)

--------------------------------------------------------------------------------
+ API Version Support
--------------------------------------------------------------------------------
This module is built to support multiple APIs, however, testing each one proves
to be a bit difficult as they all require separate licenses.  Currently, support
is being built in for API versions 1.5.2a, 10.x and 11.x.

--------------------------------------------------------------------------------
+ Lyris Users and Administrators
--------------------------------------------------------------------------------
In order to connect to Lyris, a Lyris Server Administrator needs to be defined
in the settings.php file to allow general requests to Lyris.  The credentials
should be set in the following manner.

<code>
$conf['lyris_connection'] = array(
  'sandbox' => array(
    'admin_name' => 'SandboxAdminUser',
    'admin_pass'  => 'password',
  ),
  'production' => array(
    'admin_name' => 'ProdAdminUser',
    'admin_pass'  => 'password',
  ),
);
</code>

It is not required that both production and sandbox credentials are set, as long
as the connection mode you choose has the proper ones set.

In addition to the server admin, each user who uses the Lyris module to manage
any aspect of a mailing list or its content must have a Lyris account and must
set their Lyris username and password in their Drupal account settings.


+ Lyris Object Integration
********************************************************************************
+ Lists
  Lyris lists are represented as entity bundles, where Lyris Content are the
  entities.  This relationship mirrors the node entity when node types are
  bundles that comprise nodes.

Lists
--------------------------------------------------------------------------------
+ Create
  Needs default fields added.
  Complete and tested. (1.5.2a)

+ Update
  Needs default fields added.
  Complete and tested. (1.5.2a)

+ Delete
  Complete and tested. (1.5.2a)

+ Members
  -


Lyris Content
--------------------------------------------------------------------------------
+ View
  - Incomplete.
  - Need to build the formatter for embedded iFrame, floating iFrame, new window
    and None.

+ Create
  - Needs default fields added.
  - Does not push to Lyris.  There is no Lyris API 1.5.2a call to create content.
  - Comeplete and tested. (1.5.2a)

+ Update
  - Needs default fields added.
  - Does not push to Lyris.  There is no Lyris API 1.5.2a call to create content.
  - Complete and tested. (1.5.2a)

+ Delete
  - (1.5.2a) Tested and complete.

+ Mailings
  -

+ Templating

+ Test Deliveries


+ User Interfaces Systems
********************************************************************************
Content Listing
--------------------------------------------------------------------------------
- Started.  Basic.

Subscribe UI
--------------------------------------------------------------------------------


Unsubscribe UI
--------------------------------------------------------------------------------


+ Module Integration
********************************************************************************
+ Views
--------------------------------------------------------------------------------
