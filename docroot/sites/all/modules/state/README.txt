(Drupal State module)

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Frontend-driven session/form expiration checking and session prolongation
 * Program security
 * Requirements
 * Installation and uninstallation
 * Configuration
 * Hooks and extendability
 * Documentation
 * Suggestions and bug reporting

INTRODUCTION
------------

Maintainer: Jacob Friis Mathiasen <jacob.friis@simple-complex.net>

Role-based session expiration/prolongation
Drupal 7's default session settings makes sessions live almost indefinitely
- that presents security issues, and it's also bad for performance.
State get's round the problem by combining roles with user-activitity
sensing and AJAX calls.
Sessions may still continue indefinetely for some kinds of users
- say non-logged-in visitors to your web shop.
Good bye to 'form expired'

And no visitor will ever encounter 'form expired' upon posting a form again.
State will warn the user, if the current page has hung around too long.

Check this technical page to get the full picture of session prolongation
and countermeasures to form expiration.

Safe and easy session variables
Drupal provide no means for securing against session variable collisions.
If your module or theme sets $_SESSION['my_session'] that may easily
interfere with the core system or another module - using a session variable
of the same name.
State provides functions for setting/getting/increasing/removing session
variables like:
$_SESSION['module']['my_module'] and $_SESSION['application']['my_application']

Safe and easy cookie handing
Setting cookies using Drupals cookie domain, path etc. is overly complicated.
And conflicts may occur, if two cookies (of different sites) un-intendly use
the same cookie domain.
State has simple setters and getters - PHP as well as Javascript
- for working with domain-safe cookies.

Is current request a page or an AJAX request?
State provides simple means for checking and counting requests, by type.

FRONTEND-DRIVEN SESSION/FORM EXPIRATION CHECKING AND SESSION PROLONGATION
-------------------------------------------------------------------------

Drupal 7's default session settings makes sessions live almost indefinitely
- that presents security issues, and it's also bad for performance.
State get's round the problem by combining roles with user-activitity sensing
and AJAX calls.

Restrictive session timeout for high-risk users - endless sessions for
low-risk users
Say your site has one group of users who perform critical tasks using sensitive
data, and another group (role) who only perform security-wise trivial tasks.
Could be a webshop - service and accounting staff (high-risk) versus consumers
(low-risk, the payflow routines at another site).

You would like the high-risk group to be subject to a restrictive session
timeout of 30 minutes. But members of the low-risk group would far too often
get logged out - or get their shopping basket emptied - prematurely with that
short a session timeout.

Solution

Prolong a high-risk group session whenever the user exhibits activity on a page
(mouse-move, scroll etc.). Or don't prolong the session at all (paranoid)
- the session only gets prolonged the conventional way, when the user
posts/changes page.
And on the other hand, prolong a low-risk group's sessions unconditionally and
indefinitely - ultimately only limited by form expiration (6 hours).

How does it work?

At set intervals - say 5 minutes - State's Javascript part checks if the
conditions for session prolongation are met, and if so; performs a simple AJAX
request, effectively prolonging the session. State also warns the visitor
- in stylable jQuery dialog box - before the current session or form expires.

PROGRAM SECURITY
----------------

The backend part is implemented as s self-contained static singleton,
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

INSTALLATION AND UNINSTALLATION
-------------------------------

Ordinary installation and unstallation.
State introduces no database tables.
But does introduce persistent variables; these are automatically deleted when
uninstalling.

CONFIGURATION
-------------

Check the modules administrative page (requires that a role of your user
has the 'Administer session management' privilege):
http://your-drupal-site.tld/admin/config/people/state

HOOKS AND EXTENDABILITY
-----------------------

No hooks.

The State class is extendable (not final).

DOCUMENTATION
-------------

PHP documentation, phpdoc Doxygen dialect:
http://www.simple-complex.net/docs/drupal/modules/state/php/state_8module.html
Javascript documentation, jsdoc:
http://www.simple-complex.net/docs/drupal/modules/state/js/symbols/State.html

SUGGESTIONS AND BUG REPORTING
-----------------------------

It's your civil duty as a subject of the state of Drupal to report bugs ;-)

Please create an issue at http://drupal.org/project/state,
or mail to <jfm@simple-complex.net>.