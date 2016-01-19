Tealium module
==============

The [Tealium][] module provides a standard, supported implementation of the
[Tealium iQ][] Enterprise Tag Management service with your Drupal web-site by:

1. Outputting the Tealium Universal Tag to all non-administrative pages on your
  web-site.
1. Providing an easy user interface to configure the Tealium Universal Tag with
  the correct account, profile and desired environment.
1. Giving developers an API to set Universal Tag Data (`utag_data`) for the Data
  Sources configured in Tealium for your web-site.

For more information about what [Tealium iQ][] is and what you can do with the 
Tealium Tag Management console, visit the [Tealium Tag Management][] 
web-site.

Requirements
------------

- [Tealium iQ][] account and profile
- Drupal 7.x
- [XAutoload][] module

Installation
------------

1. Place the [Tealium][] module into your modules directory.
1. Place the [XAutoload][] module into your modules directory.
1. Go to the `admin/modules` page and enable the Tealium module. The Tealium
  module is located in the _Statistics_ section of the _Modules_ list.

Read more about [installing modules](http://drupal.org/node/895232) in the
Drupal Installation Guide.

We recommend using  drush 6.1.0 or higher to install the [Tealium][] module.

For instructions on installing drush see the [drush project][] page.

To install Tealium using drush, simply run the following commands from your
within your Drupal install directory:

    drush pm-download tealium
    drush pm-enable tealium

When enabling a module using drush 6.1.0 any missing dependencies will be
downloaded, installed and enabled for you automatically.

Upgrading
---------

Remember to run update.php after every module update. With a standard,
recommended installation, updates will be automatic.

Minor version updates - ie. 7.x-1.0 to 7.x-1.2 - should not require manual
steps.

If updating to a new major version - ie. 7.x-1.0 to 7.x-2.0 - please check the
release notes and the [Tealium][] module page for any special considerations or
manual steps required.

Configuration
-------------

The Tealium module provides configuration settings for the Tealium Universal
Tag that is outputted to every page on your web-site.

### Tealium Account and Profile

To configure the Tealium module, you need to know your Tealium Account and
Profile names.

The easiest way to find these is to:

1. Log-in to [Tealium iQ][]
1. Look in the _top right-hand corner_ of the Tealium Management Console, under
   your user-name
1. There you'll see your Account, Profile and Version separated by slashes.
  For the account named "drupal" and profile named "website" you will see
  something like the following: **drupal / website / Version 2014.07.28.1830**

### Tealium Settings

Now that you've found your Account and Profile name:

1. Log-in to your Drupal site as an administrator
1. Go to the _Configuration > Web Services > Tealium Settings_ page at
  `admin/config/services/tealium`
1. Enter the _Tealium account_ and _Tealium profile_ names you identified
  earlier using the steps above
1. Choose a _Tealium environment_ (if you don't know what this means, choose
  from either:
    - _Development_ for your development only version of the web-site
    - _Testing / QA_ for your internal testing version of the web-site
    - _Production_ for the web-site your visitors see on the public Internet
1. Leave the checkbox _Load Asynchronously_ checked unless otherwise directed
  by a Web Developer or Tealium Support Technician

### Hard-coding Settings

For multi-site or multi-environment configurations you may want to hard-code
these settings in the site `settings.php` file. These are the variable names
for each of the Tealium Settings listed above:

1. `tealium_account` (string) Tealium Account name
1. `tealium_profile` (string) Tealium Profile name
1. `tealium_environment` (string) Tealium Environment - set to one of the
  following values `dev`, `qa` or `prod`
1. `tealium_async` (boolean) Load Asynchronously

To hard-code one or other of these settings, add something like the following
to your `settings.php` file, usually located at `sites/default/settings.php`:

    <?php
    /**
     * Tealium module settings
     */
    $conf['tealium_account'] = 'example-account';
    $conf['tealium_profile'] = 'example-profile';
    $conf['tealium_environment'] = 'dev';
    $conf['tealium_async'] = TRUE;
    ?>


Developer API
-------------

While the Tealium module does not provide any hooks, it does use standard
Drupal theme template and hook implementations to output data to the page.

### Simple API

To set the data source values you want to output to the page you can use the
following Tealium module API methods:

1. `tealium_add_data()` - works a lot like `drupal_add_js()` or `drupal_add_css`
  to add Tealium `utag_data` values to a page.
1. `tealium_get_data()` - Get the Tealium `utag_data` values that have already
  been set so far during a page request.

The easiest way to set some `utag_data` data source values for a page request
is by including a call or multiple calls to `tealium_add_data()`  in your 
module or theme prior to the page being rendered by the Drupal theme engine.

#### Add data source values from a node

For example, to send the Drupal node type to Tealium whenever a node page is
returned, you could add the following
[template_preprocess_node](http://api.drupal.org/api/search/7/template_preprocess_node)
implementation to your module or theme:


    <?php
    /**
     * Implements template_preprocess_node().
     */
    function THEME_preprocess_node(&$variables) {
      if (arg(0) == 'admin'
          || !isset($variables['page'])
          || !isset($variables['node']->type)
      ) {
        return;
      }
      $node = $variables['node'];
      $is_page = $variables['page'];
      if ($is_page && !empty($node->type)) {
        tealium_add_data('page_type', $node->type);
      }
    }
    ?>


#### Add data source values for a user

Let's say you want to tell Tealium when a user is member as opposed to an
anonymous visitor. As a logged-in user, the page returned will not be cached, 
so we can add some `utag_data` data source values in a 
[hook_init](http://api.drupal.org/api/search/7/hook_init) implementation.

In addition, because a Vendor tag requires a user's name, we want to send that 
too. We can use the [format_username](http://api.drupal.org/api/search/7/format_username) 
helper to get the user's display name, which should be safe in terms of 
privacy. 

    <?php
    /**
     * Implements hook_init().
     */
    function MODULE_init(&$variables) {
      global $user;
      if (arg(0) == 'admin') {
        return;
      }
      if (isset($user->uid) && isset($user->name)) {
        $account = $user;
      }
      else {
        $account = drupal_anonymous_user();
      }
      $user_type = 'visitor';
      if (!empty($account->uid)) {
        $user_type = 'member';
      }
      $user_name = format_username($account);
      tealium_add_data('user_type', $user_type);
      tealium_add_data('user_name', $user_name);
    }
    ?>


### Advanced APIs

1. `tealium_add_link_data()` - Add `utag_data` values to be sent to Tealium as
  a _link_ tracking event.
1. `tealium_add_view_data()` - Add `utag_data` values to be sent to Tealium as
  a _view_ tracking event.
1. `tealium_add_bind_data()` - Add `utag_data` values to be sent to Tealium
  only when a particular JavaScript event fires on an HTML DOM element.

Road Map
--------

It is expected that other sub-modules and projects will leverage the APIs provided 
by the [Tealium][] module to allow site editors to set Tealium data source values 
using standard Drupal content creation tools - ie. [Context][], [Views][], etc. 

There is also a plan to provide a single, unified API to populate analytics data 
for output to multiple 3rd Party Tag Management platforms. This would unify both the 
[Tealium][] and [Qubit][] modules allowing you to set data once and output to either 
or both those Analytics Tag Managers. 

Related Links
-------------

### Community Documentation

There is more community documentation available for both Tealium and the 
Drupal Tealium module online. Check out these sources:

- [Tealium module handbook][] 
- [Tealium Leaning Community][] 
- [Tealium Quickstart Guide][]

### Related Projects

- [Qubit][] module
- [XAutoload][] module

Maintainers
-----------

- [chOP](http://www.drupal.org/user/116649)

Sponsorship
-----------

Development of this module has been sponsored by [Peak Adventure Travel][].

[Tealium iQ]: https://my.tealiumiq.com
[Tealium Tag Management]: http://tealium.com
[Tealium]: http://www.drupal.org/project/tealium
[Qubit]: http://www.drupal.org/project/qubit
[Context]: http://www.drupal.org/project/context
[Views]: http://www.drupal.org/project/views
[XAutoload]: http://drupal.org/project/xautoload
[drush project]: https://github.com/drush-ops/drush
[Peak Adventure Travel]: http://www.peakadventuretravel.com/
[Intrepid Group]: http://www.intrepidgroup.travel/
[Tealium module handbook]: http://www.drupal.org/node/2310763
[Tealium leaning community]: https://community.tealiumiq.com/
[Tealium Quickstart Guide]: https://community.tealiumiq.com/posts/539901
