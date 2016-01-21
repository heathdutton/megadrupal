
Context Resolution for Drupal 7.x
---------------------------------

Context Breakpoint brings CSS media queries to the server.
By using  Context and  Breakpoints (which will be moved into the core in Drupal 8) you can alter the page based on the visitors screen resolution, browser window size,  or aspect ratio.

A new context condition "Breakpoint" is available after install.

Adaping to resolution should mostly be done with CSS (and media queries),
but sometimes you just can not do everything you need to in plain CSS, 
and more extensive changes - like different markup - are required.

IMPORTANT NOTE:

This module detects screen and browser size with Javascript and
sets a cookie accordingly.

This means that Javascript and cookies must be enabled.
Also, after changing the browser size, a reload will obviously be required
for the changes to show up. 
If you enable "autoreload" when configuring the context, a reload
happens automatically.

FOR ISSUES WITH CACHING, SEE BELOW!

Supported media queries
-----------------------

(min/max-)(device-)height
(min/max-)(device-)width
(min/max-)(device-)aspect-ratio

Installation
------------

Context Resolution can be installed like any other Drupal module -- place it in
the modules directory for your site and enable it (and its requirements,
CTools and Context) on the `admin/modules` page.

You will probably also want to install Context UI which provides a way for
you to edit contexts through the Drupal admin interface.


Configuration
-------------

-) Create one or more breakpoints at admin/config/media/breakpoints
     Breakpoints look like media queries.
     So if you would like to, for example, show a block only when the browser size is
     between 500 and 1000 pixels, you should create a new breakpoint with the value 
     "(min-width: 500px) and (max-widht: 1000px)"

-) Create a new or edit an existing context under admin/structure/context.
-) Add a "Breakpoint" condition.
-) For the chosen breakpoint condition, select a breakpoint.
     If you would like the page to auto-reload when a breakpoint becomes
     active or inactive, check the box.

Caching
-------

This module makes page caching harder.

Especially, the Drupal page cache will not work properly anymore!

Drupal caches pages by url. But with context_breakpoint, the url always stays the same, 
even though the content of the page can change due to context.
Standard Drupal page cache ("Cache site for anonymous visitors") will not work out of the box.
No context will be active for anonymous visitors, or a random one will, depending on what user
first opened the page.

This is, obviously, not ideal.

The recommended way of caching is to use a cache backend that allows to cache by cookie.
When you include the cookie "context_breakpoints", which holds the active context,
in your cache paremeters, caching will work.
This cookie holds the name of the active contexts, so each page will only be generated
once per context.

This is possible, for example, with the VARNISH reverse proxy.

SAVE RESOLUTION COOKIE:

Note that, if you enable "Save resolution in cookie", you also 
have to exclude that cookie from the cache backend, since this cookie will
have many different values and will render caching very ineffective.

EXPERIMENTAL OTHER OPTION:

The module includes an experimental way to enable caching with the standard Drupal page cache.
You can enable it at admin/config/media/context-breakpoint with "Add active breakpoints url"

When this option is checked, the currently active breakpoints will be added to the url of all links as a get variable. 

An additional get parameter will be added to every link that is created by Drupal. 
This way, each visitor with a different breakpoint enabled ends up on a different url. 
The page cache then will work. 

PLEASE NOTE that this is very messy, and you should test thoroughly if it works for your setup. 
ALSO ENABLE AUTORELOADING for at least one breakpoint.
Then, if the user comes from an url with the wrong get parameter, the page will 
be reloaded automatically.

Hooks
-----
None.


Maintainers
-----------

- theduke (Christoph Herzog) - chris@theduke.at


Contributors
------------

- theduke (Christoph Herzog)
