Cache Control is a module for integrating your site with the Varnish HTTP
accelarator in a fashion that not only allows for caching pageloads for
anonymous users but also for authenticated users.

In order to make best use of this module, you need to write some custom code. In
other words, this is a module for advanced use and cannot be considered a
component that will make your site perform better just by being enabled.

How does it work?

The basic idea of Cache Control is to check whether the current pageload can be
cached by Varnish, and if that is the case, switch the execution context of the
pageload to that of anonymous user's (in order to ensure cacheability). When a
user requests a page that is found in the cache, it is served by Varnish and
user-specific parts of it are dynamically added on it via AJAX, if the user has
authenticated. For anonymous users, the content will be served as-is, with no
Drupal involvement required.

Some details
------------

When a pageload is made, the following checks are performed
(cache_control_init()):

    * If it's a POST request, an access denied page or just a non-cacheable
path, no-cache headers are sent.
    * If it's a cacheable path, the execution context for the whole pageload is
switched to an anonymous user (in order to ensure the cached result doesn't
contain any personalized data). The menu item access callback is re-executed for
the anonymous user, and if it still passes, the pageload is actually cached
(otherwise the execution context is switched back to the current user and
pageload is done normally without caching). HTTP headers allowing caching are
sent at this point.
    * TODO more checks have been added since this was written

If there are components on the page that may require personalized content, they
notify cache_control by invoking hook_cache_control() (see API). The
implementation of this hook (cache_control_cache_control()) stores the function
and arguments needed to generate the content and adds a caching id to the page
Javascript. The invoking module must react to the return value of the
implementation: if the pageload is being cached, the generated content is put
into a hidden container (this content is used for anonymous users) and a
placeholder (a throbber, a 'please wait' message or whatever) is made visible on
the site. See the example module.

The JS frontend (cache_control.js) checks if the user is anonymous (based on a
cookie). If yes, the hidden containers created before are shown and no AJAX call
is made. If the user is authenticated, an AJAX call with the caching id is made
to _cache_control/get_components/%. The backend executes the functions needed to
generate personalized content and returns the resulting HTML. The JS frontend
replaces all placeholders with the generated content (based on the HTML ids).

Not all details were described here, but this should give a good idea about how
the module works together with Varnish.

Cache purging: a separate module, Cache Control Purge, implements some purging
functionalities. Basically it sends HTTP PURGE requests to the varnish servers
(and allows also manual purging). It has some default purge conditions (such as
when a node is updated, its path is purged) and also offers a
hook_cache_control_purge() (see API) which other modules can implement if they
need to react to content being updated or deleted.
Is it secure?

It may feel discomforting to have Varnish cache pages requested by authenticated
users. Doesn't it imply that users' private data may be cached by Varnish and
served to the entire world. Fortunately, no: When executing a page load that
will end up in the cache, it will always be executed as an anonymous user, even
if the request was made by an authenticated user. Note that you may actually
want to configure Cache Control to cache user-specific get_components-URLs as
well; In this case user-specific data _is_ cached and available to the world,
but the URL from which it is available from contains a session id-based hash
only known to the correct user.

The hash is generated from the user's session id, and a user-configurable secret
in Cache Control settings. Note that in a multisite installation, all the
multisites should share the same secret, if the users/sessions are shared
between the multisites.

Does it work with my favorite contrib module?

Cache Control plays fairly nice with others. However, we've had to write some
code for specific modules in order to keep things from breaking apart. Some
examples:

    * CAPTCHA: Forms with CAPTCHA challenges get a special treatment.
    * Webform: Webform node pages are never cached.
    * Statistics: If you're using statistics module, you need to explicitly
enable statistics tracking in Cache Control settings. Also, for performance
reasons, tracking anonymous page views requires some extra measures. See
installation instructions for more information
    * WYSIWYG: You may need to manually initialize your WYSIWYG profiles

Let us know if a module you're using doesn't get along with Cache Control.

Note that any modules that alter Drupal's login functionality may conflict with
Cache Control. Cache Control sets "session" cookies of its own that are read by
the JS front-end. If the cookie lifetime of Drupal's session cookie is altered
from session.cookie_lifetime, unexpected functionality may result. Cache Control
attempts to keep the cookies in synch, but since Drupal's session cookie isn't
readable by JS, malfunctions may happen in cases where no request goes all the
way to Drupal(i.e. when everything is served from Varnish).

How do I use it?
----------------

This section gives you guidance in installing and using Cache Control.

*Drupal configuration

You need to enable reverse proxy support in Drupal's settings.php (set
reverse_proxy to TRUE and configure reverse_proxy_addresses). The settings are
explained in detail in default.settings.php

*PHP configuration

Cache Control requires that PHP's output buffering is enabled. This is due to
the fact that Cache Control needs to be able to override the HTTP headers Drupal
sets in bootstrap (see drupal_page_header() in bootstrap.inc).

*Varnish configuration

Your Varnish will most likely require some configuring before it can fully
operate with Cache Control.

Detailed instructions and example VCL files will be available on this site
shortly.

*Installation and basic usage

After enabling Cache Control and Cache Control Purge modules, you need to go to
admin/settings/cache_control and check "Enable Cache Control". The main admin
page also allows you to set a global cache lifetime.

Under 'Paths being cached currently' (admin/settings/cache_control/cache_paths),
you'll find the paths that can be cached by Varnish (i.e. HTTP headers allowing
caching can be sent for those paths). On this page, you can also override the
global cache lifetime per path. Under 'Paths not currently cached'
(admin/settings/cache_control/available_paths) you'll find all other paths.

You can make a path cacheable by selecting it on the 'Paths not currently
cached' list and clicking 'Make selected paths cacheable'. The paths are for
generic menu router items (like node/%) rather than individual aliases/paths
(such as node/3), i.e. there's no fine-grained control per path yet (but it's on
the roadmap).

If you are using Statistics module, you may also want to check "Count content
views" in admin/settings/cache_control. This makes sure node views get counted
correctly even when the pages are served from Varnish. If you choose to use node
statistics tracking, copy cache_control_statistics_tracker.php from
cache_control's directory to Drupal root. This component tracks node views for
anonymous users. It is separated into a standalone script to avoid unnecessary
execution of whole Drupal bootstrap when tracking anonymous users.

If you enabled Cache Control Purge (which is very recommended), you can manually
purge cached pages (or the whole cache) in
admin/settings/cache_control/manual_purge.

*Dependencies

Cache Control has no dependencies to other modules. Cache Control Purge requires
Cache Control. Some dependencies may be added in the future (see Roadmap).

Further, Cache Control is designed in a way that doesn't require you to add a
dependency to Cache Control in your module.

Examples
--------

This section provides some useful examples of how to use the Cache Control API.

The example module

The main module includes an example module cache_control_examples that
implements a simple block that uses cache_control. Enable the module and assign
the block to a region to see how it works.
Invoking hook_cache_control() in your module

Let's look at an imaginary module that has a function for generating
personalized links. To find out if the current pageload is being cached and to
act accordingly, you would do something like this:

function mymodule_generate_links($arg1, $arg2) {
  $id = 'someIdForTheHtmlWrapper';
  $classes = 'someClass';
  ...
  // Do what you would normally do when preparing to generate the final output.
  // In this example we're generating some links
  $links = ...
  ...

  $output = '';

  //Before generating the final output, notify cache control
  $cached = module_invoke('cache_control', 'cache_control',
  'mymodule_personalized_links', 'mymodule_generate_links', func_get_args());

  if ($cached) {
    // This pageload is cached, so we're storing the content for the anonymous
user

    // As a placeholder (that will be replaced with either the anonymous
    // content or personalized content), we show a throbber.
    // This is just an example, you need to implement your own placeholders
    // (just make sure they have the HTML id)
    $output = theme('throbber', $id);

    $id .= '-anonymous';
    $classes .= ' cacheControlAnonymous';
  }

  // This is the regular output you would generate anyway.
  // If the pageload happens to be cached, it's just generated with
  // such id and classes that cache_control may find it in the page source
later.
  $output .= theme('links', $links, array('id' => $id, 'class' => $classes));

  return $output;
}
Implementing hook_cache_control_cached_pageload() in your module

Consider the following scenario: On a page, you have a form that contains a
WYSIWYG editor for authenticated users and a regular textarea for anonymous
users. Now, if the page is being cached, it's being generated for an anonymous
user. This implies that not all WYSIWYG settings may not be available in the
page Javascript, causing the WYSIWYG editor to fail if we dynamically rebuild
parts of the page for an authenticated user. In this case, you would want to
implement hook_cache_control_cached_pageload() in order to ensure that necessary
JS settings are always available in the page source. This is how you would do
it:

function my_cache_control_cached_pageload() {

  if (module_exists('wysiwyg')) {
    //Includes needed WYSIWYG JS's
    $profile = wysiwyg_get_profile(1);
    if ($profile) {
      $theme = wysiwyg_get_editor_themes($profile,
      (isset($profile->settings['theme']) ? $profile->settings['theme'] : ''));

      // Add plugin settings (first) for this input format.
      wysiwyg_add_plugin_settings($profile);
      // Add profile settings for this input format.
      wysiwyg_add_editor_settings($profile, $theme);
    }
  }
  drupal_add_js(base_path().'misc/collapse.js');
}
Implementing hook_cache_control_purge() in your module

Cache Control Purge automatically handles purging the paths to your nodes for
you, but sometimes you may want to react to node changes in some additional way.
For example, you may want to refresh your front page if a promoted node is
changed. This is how you would go about doing such thing:

function mymodule_cache_control_purge($op, $content) {
  if ($op == 'node update' || $op == 'node delete' || $op == 'node insert') {
    if ($content->promote) {
      // Purge front page cache
      cache_control_purge_purge_path(variable_get('site_frontpage', 'node'));
    }
  }
}
How are we planning to develop this module further?

We have drafted a roadmap for this module and are planning to follow it in a
rapid fashion. At least the following development needs have been identified (in
order of priority):

    Admin interface rewrite (both cache_control and cache_control_purge). The
current admin interface is a bit unintuitive and hasn't been built according to
all best Drupal practices.
    Explore the possibility to replace parts of cache_control_purge by utilizing
either Varnish HTTP Accelerator Integration or Purge module.
    More granular control over pages that can/cannot be cached
    More granular (e.g. role-based) control over what needs to be built per user
after a pageload. This is expected to further improve the performance of this
module.
    Make sure the module plays nice out-of-the-box with as many contrib modules
as possible.
    Tools/practices to help developers to automate the use of this module as
much as possible; the target is to minimize the amount of custom code needed.

We welcome any additional proposals for further development.

Who is maintaining this module?
-------------------------------

Cache Control is maintained by Exove Ltd. We have also received contributions
from A-lehdet.