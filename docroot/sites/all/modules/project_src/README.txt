-- SUMMARY --

This is an API module that allows other modules to expose Drupal projects in the
same format as updates.drupal.org/release-history.

Unless you are a developer, you probably don't need this module. Even then, you
only need this if another module depends on it, or if you're developing a module
using the API.


-- USE CASES --

Maybe you have custom modules, themes, or Features that you use widely across
your organization, but for whatever reason (licensing, proprietary IP, etc),
it's not appropriate to publish those projects to drupal.org. Even so, you want
to easily manage updates to your extensions across your Drupal properties.

Maybe you run a Drupal shop or consulting agency that keeps a stack of custom
modules, themes, or Features that are, for whatever reason, not suitable to be
released on drupal.org. Even so, you want to make it easy for your clients to
get the latest versions of your module/theme/feature stack.

Project Source allows you to expose update status feeds for your custom
extensions, supporting the above objectives.


-- FOR DEVELOPERS --

The primary hooks you need to invoke are:
- hook_project_src_info(): Used to declare custom project definitions, keyed by
  project short name.
- hook_project_src_releases(): Used to return project release definitions for a
  given project/API version, keyed by release name.

Each of the above also has an associated alter; you may use also want to use
hook_project_src_terms() and its relevant alter to declare project and release
terms.

In an ideal world, your module will be using the above API to integrate with a
version control repository service or system (like github or gitlab) and its
API to dynamically generate project and release definitions.

All project_src-related hooks should be placed in a special inc file whose name
is of the form "my_module.project_src.inc" and located in the root module dir.

For full details, see project_src.api.php.

In your custom projects' .info files, you'll also want to include several
properties normally reserved for drupal.org's packaging systems:
- version: With each point release, you'll want to change this to a full Drupal
  project version string, e.g. "7.x-1.0". After point releases, you may need to
  revert this to "7.x-1.x-dev" or something similar during development.
- project: The project short name used as the primary array key in your
  implementation of hook_project_src_info(), e.g. "views"
- project status url: The URL, including the base of the XML callback, against
  which the Drupal Update module and drush can check for updates to your custom
  extensions, e.g. "http://example.com/drupal/release-history"

Note you can easily test your implementation by checking, for example:
http://example.com/drupal/release-history/my_project_shortname/7.x
