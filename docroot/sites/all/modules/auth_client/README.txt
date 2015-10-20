DESCRIPTION
-----------
This module allows people to login using a username and a password from another Drupal installation (authentication server).
The "authentication server" needs to run the "services" module: http://drupal.org/project/services.

SETUP
-----
To use this module you first need to configure the other Drupal install to act as an authentication server. In order
to do this you will need to follow these steps (again, this is meant to be done on a different Drupal install):

1. install and enable the "services" module and the "REST server" module. Both of them are in the "services" package -
http://drupal.org/project/services;
2. go to admin/structure/services and add a new service;
3. give your new service a name, select "REST" for the server, choose an endpoint (you will need this later) and check
"session authentication";
4. edit the newly created service, go to resources, select the user checkbox (make sure that everything bellow user is
 also selected) and choose an alias (you will need this also).
5. construct your authentication server URL: http://your.domain.com/[endpoint from point 3]/[alias from point 4]/login

With you new authentication server URL switch to the client Drupal installation and follow these steps:

1. install and enable this module;
2. go to /admin/config/people/authclient; 
3. fill in the "authentication server URL"

If everything went well, you are now ready to go. Every user that will try to login on the client website will go through
the following process:

- a normal (local) authentication will be tried by matching the username and password with the accounts stored on the local
(client) installation;
- if no match is found, a cross install authentication will be tried by matching the username and password against the
server installation;
- if the credential are valid, a new user will be creates on the local (client) install and the user will be logged in.

LIMITATIONS
-----------
- The system is only one-way. The users from the client install will NOT be available on the server install;
- There is no synchronization  between the accounts. If one account is changed (either on the client install or on the
server install), the changes will not be reflected in the other system. This is on my TODO list.