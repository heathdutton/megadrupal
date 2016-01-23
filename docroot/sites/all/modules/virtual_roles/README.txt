
Virtual Roles is a module that allows the website to dynamically assign roles to a
user, based on conditions being met on that particular pageview. In other words, for
each page viewed, specific user roles may be added, removed, or toggled on a user's
account based on whether that page view meets certain conditions.

Virtual Roles also functions as an API to allow other modules to supply their own
conditions (called contexts) which may be used to determine whether or not a role
should be granted to a user.

INSTALLATION
----------------------------------------------------
To install the module, simply unzip it into the contrib modules directory on your
site (sites/all/modules) and enable it on the module administration page. By
default, all roles will be set to "Do not process", and so each role that you want
to evaluate must be enabled by un-checking the "Do not process" checkbox.

CONFIGURATION
----------------------------------------------------
Basic configuration happens on the module settings page, which is found under the
default user roles administration page (admin/user/roles/virtual_role). This page
controls the overall cache settings, as well as the sanity check (discussed later)
and sets the order in which roles are evaluated.

Administration for the individual roles is controlled by a role configuration page.
The link for these pages appear as tabs on the module administration page. It should
be noted that, if you add more roles to your site, then you must also clear the menu
cache in order for the new roles to show up in the tabs.

SPECIAL CONSIDERATIONS
----------------------------------------------------
In order to take control of Drupal's role system and dynamically assign roles,
Virtual Roles must be able to operate on a very basic level and very early in the
page loading mechanism. This happens before any other modules are loaded, including
other core modules. Therefore, functions that would otherwise be available (like
node_load() and user_load()) do not exist yet, and must be replaced with custom SQL
and PHP.

In addition, because a role may influence whether or not the user has access to a
particular page, the processing must be done on every page call, before caching is
implemented. Special care must be taken in order to avoid unnecessary or redundant
SQL calls.

SANITY CHECK
----------------------------------------------------
A quick way to save unnecessary processing is to introduce a sanity check into the
workflow. If the sanity check returns TRUE, then all futher processing will be
skipped. The default sanity check returns TRUE if the user is an anonymous user,
but the code can be customized. It is probably best practice to avoid any SQL or
intensive PHP processing in this code, because this code will be processed on
every page view.

CONTEXTS
----------------------------------------------------
A "context" is a condition that may be true on one page, but false on the next.
For example, "User is node's author" is a pre-defined context that ships with the
module. When multiple contexts are enabled for a role, then the contexts are treated
in an OR fashion. They are processed one by one until a TRUE result is found. Once
a context returns TRUE, then no further contexts are processed. If no contexts have
returned TRUE, then the custom PHP or the paths are processed.

CUSTOM PHP FOR A ROLE EVALUATION
----------------------------------------------------
Custom PHP can also be used to help determine whether or not a role should be added.
You may access a copy of the $user variable that is being processed by calling the
function virtual_role_test_user().

As mentioned above, contexts are ORed together. If you need more complex logic, then
you can opt to evaluate the contexts programmatically by calling the function
virtual_roles_test($test), in which $test is the name of the context that you wish
to evaluate. Contexts are defined by a module's hook_virtual_roles().
virtual_roles_get_contexts() will return a keyed array of all available contexts, in
which the key of the array is the name of the context.

VIRTUAL ROLES API
----------------------------------------------------
Virtual Roles provides an API so that other modules can provide their own contexts.
Contexts are defined using hook_virtual_roles(), as seen in includes/core.inc:

function node_virtual_roles() {
  return array(
    'node_user_is_author' => array(
      'title'    => t("User is node's author"),
      'callback' => 'node_user_is_author',
      'file'     => 'node_user_is_author.inc',
      'path'     => drupal_get_path('module', 'virtual_roles') .'/modules/node',
    ),
  );
}

The name of the context is the key of the array, 'node_user_is_author'.
'title' is the text that is seen next to the context checkbox.
'callback' is the function that should be called to evaluate the context.
'file' is the name of the file in which the callback resides. If in a sub-folder of
  the module, then the sub-folder can be included here. Contexts may appear in the
  same include file if they need common supporting functions.
'path' is optional. If not set, it will default to the path of the module that
  declared the context.

The callback should appear in the form of:

function callback($op, $user = NULL) {
  switch ($op) {
    case 'cache' :
      // insert code here to return the cache_id name
      break;
    case 'process' :
      // insert code here to evaluate the context
      break;
  }
}

If $op is "cache", the the return variable should be one of the following:
  1) A string, representing the cache ID that the result of the context should be
     stored as. The cache ID should begin with the module name so as to avoid naming
     conflicts with other modules. The cache ID should be as unique as the
     information that it reflects. If the context changes depending on the user,
     then the user's id should be part of the cache ID.
  2) Boolean FALSE. This indicates that the context test should not be cached because
     it is faster to process the test again than to store and retrieve it from the
     database. It could also be used when the context could change depending on
     other, unpredictable factors. If the latter is the case, then caching should
     be disabled on the module administration page.
  3) NULL, meaning that the context does not apply to this page, and no further
     processing should take place for this context.

If $op is "process", then the return variable should be a boolean TRUE or FALSE,
indicating whether or not the context applies to the page being viewed. $user is a
copy of the user whose permissions are being determined.

Remember that the code entered here will not have access to many of the common
Drupal functions. The only functions available are those found in cache.inc,
database.inc, session.inc, module.inc, path.inc, common.inc, and bootstrap.inc. No
other modules have been loaded yet, so you may have to write custom SQL and PHP.

Inefficiencies in any code used here can cripple a site's performance, so write
carefully.

