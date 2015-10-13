Unfuddle API Drupal Module

-- ABOUT --

The Unfuddle API Drupal Module provides an interation layer with Unfuddle for developers and other Drupal modules.

This module does not provide a Drupal graphical interface for communicating with Unfuddle, it is up to other modules
to invoke the API by using the classes provided.

The module does provide an administration page for setting the default Unfuddle connection information such as URL, user, password and an optional project number.

-- USAGE --

The Unfuddle API Drupal module is class-based, all interactions with the Unfuddle API happen through methods of the Unfuddle class.

To use the Unfuddle class you use $unfuddle = new Unfuddle(); which gives you an instantiated Unfuddle object. Unfuddle's API is
now available as methods of the object. Details on the API can be found on Unfuddle.com at http://unfuddle.com/docs/api.

For example, to get the projects you would need only the following two lines of code:
$unfuddle = new Unfuddle();
$projects = $unfuddle->getProjects();

To create a ticket you invoke the createTicket() method. Refer to inline documentation on required parameters.
$response = $unfuddle->createTicket(<parameters>);

The Unfuddle class defaults to using the connection data stored in the Drupal database but can be overridden by passing a URL, username,
and password to the class: new Unfuddle($url, $user, $pass, $project); or by invoking methods setURL() and setAuth() on an instantiated object.

-- CREDITS --

Written by Ben Jeavons (drupal.org username coltrane) from code originally by Andy Kirkham and later modified by Matt Tucker and Jody Hamilton. Inspiration
for class by the Drupal Twitter module.

