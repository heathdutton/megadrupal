XML-RPC method tester suite
---------------------------

Module provides a number of helpful features for developers to test their callbacks, or server implementations.

Server
======



Client
======

Available server methods are listed on the xmlrpctester/methods/list path, clicking on them will show a method form that submits its values to the remote server, for the given method. Return values are printed, if devel is available (recommended, but not necessary) using dsm(), otherwise with a simple drupal_set_message().

Sometimes servers do not implement the RFC standard system methods. For this scenario new custom methods can be defined on xmlrpctester/methods/new. These will show up in a separate list on the main methods list. Calling them is the same as the ones provided by the server.

Example error messages for this scenario: 
  Error code: 0
  Error message: java.lang.Exception: RPC handler object "system" not found and no default handler registered.

