(Drupal Inspect module)

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Permissions and safety
 * Program security
 * Requirements
 * Installation and uninstallation
 * Configuration
 * Hooks and extendability
 * Documentation
 * Suggestions and bug reporting

INTRODUCTION
------------

Maintainer: Jacob Friis Mathiasen <jacob.friis@simplecomplex.net>

Inspect is a variable dumper, stack tracer and execution time profiler.

It aims at delivering simple and safe measures for:
 * developers to investigate the structure, types and values of variables
 * securing detailed information about errors
 * making detection of performance hogs easier and more accurate
 * all in manners that do not jeopardize the safety of neither system nor data

It consists of a backend PHP-based utility as well as a minor frontend
javascript library.

PERMISSIONS AND SAFETY
----------------------

Inspect's set of permissions - and clever administration of these privileges
- is an important aspect of the module and it's use.
Generally restricting the inspect permissions to tester and developer roles
is vital. However, it may make sense _temporarily_ to permit other user roles
to _log_ inspections, as a means of collecting data and insights on the
workings of your site.

_Getting_ dumps and traces should generally _not_ be permitted for _any_ kind
of role but testers/developers/administrators.
Getting means that backend functions and methods of the module are allowed
to return data. Putting a PHP echo in front of such a function/method call
means that these data may turn up on a page.
- Well, it's only a temporary thing, just checking...
Think twice: everybody forgets stuff. Particularly during cumbersome debugging.

Thus getting - potentially echo'ing - dumps and traces is security-wise quite
a different matter than _logging_ them.

That said, you should not without further consideration enable logging
(and logging scalar values and paths) for all kinds of roles (i.e. users).

For reviewing/modifying Inspect permissions, go to:
http://your-drupal-site.tld/admin/people/permissions#module-inspect

For prohibiting admin user 1 from _getting_ dumps and traces, check:
http://your-drupal-site.tld/admin/config/development/inspect

For restricting access to logs, uncheck 'View site reports' at:
http://your-drupal-site.tld/admin/people/permissions#module-system

PROGRAM SECURITY
----------------

The backend part is implemented as self-contained static singletons,
to secure encapsulation (apart from modularity and optimal performance).
The public functions are mainly a means of obtaining ease of access
as well as conforming with Drupal coding standards.

The frontend part is also a self-contained static singleton.

Both parts pursue security by design, attempt to foresee mistakes
and limitations, and do perform risky procedures within try-catches.

REQUIREMENTS
------------

 * Drupal 7.x
 * PHP>=5.3 (because of late static binding)
 * Folding of variable dump outputs is not supported for Internet Explorer <9

INSTALLATION AND UNINSTALLATION
-------------------------------

Ordinary installation and unstallation.
Inspect introduces no database tables.
But does introduce persistent variables; these are automatically deleted when
uninstalling.

CONFIGURATION
-------------

Check the modules administrative page (requires that a role of your user
has the 'Administer variable and trace inspections' privilege):
http://your-drupal-site.tld/admin/config/development/inspect

HOOKS AND EXTENDABILITY
-----------------------

No hooks.

Classes Inspect, InspectTrace and InspectProfile are all extendable (not final).

The maintainer of this module would be very pleased to receive suggestions
about other features than those currently implemented.

DOCUMENTATION
-------------

PHP documentation, phpdoc Doxygen dialect:
http://www.simplecomplex.net/docs/drupal/modules/inspect/php/inspect_8module.html
Javascript documentation, jsdoc:
http://www.simplecomplex.net/docs/drupal/modules/inspect/js/symbols/inspect.html

SUGGESTIONS AND BUG REPORTING
-----------------------------

It's your civil duty as a subject of the state of Drupal to report bugs ;-)

Please create an issue at http://drupal.org/project/inspect,
or mail to <jacob.friis@simplecomplex.net>.
