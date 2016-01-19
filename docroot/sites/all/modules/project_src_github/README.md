Project Source: GitHub
=======================

This module communicates with GitHub using your GitHub account to automatically
generate project status and release XML suitable for use by the core Drupal
Update module and/or drush.

By simply following existing drupal.org module/repository/release naming
conventions, any GitHub repository will be automatically registered as a project
and listed at http://example.com/drupal/release-history/MY_PROJECT/API_VERSION.

For more information on drupal.org release naming conventions, please see
[release naming conventions][].

For more details on integrating your custom Drupal extensions with Drupal's
release/update status workflow, see the README.txt of the Project Source module.

### Installation instructions

1. Install this module and its dependencies by following the usual Drupal module
   [installation guide][].
2. Once all dependencies are installed, you must configure GitHub API to use
   your GitHub credentials at */admin/config/development/github_api*.
3. Next, you'll need to specify a GitHub organization to which your account is
   already associated at */admin/config/development/github-src*.
4. Optionally, you may install and configure [GitHub Webhook][] to ensure that
   any commits made to your GitHub repositories will result in fresh data used
   to pull down your latest projects and releases.

At this point, you should be able to point drush (or Drupal's Update module via
.info file properties) at http://example.com/drupal/release-history and be able
to download and update your custom projects!

### Dependencies
* [Project Source][]
* [GitHub API][]
* [GitHub Webhook][]

Note: currently, only projects associated with an organization to which you have
access are registered with this module. If you aren't already tied to one, [get
started with organizations][]. If you'd like to see additional functionality
beyond organization repository registration, be sure to create a feature request
in the Project Source: GitHub [issue queue][]!

[release naming conventions]: https://drupal.org/node/1015226
[installation guide]: https://drupal.org/documentation/install/modules-themes/modules-7
[project source]: https://drupal.org/project/project_src
[github api]: https://drupal.org/project/github_api
[github webhook]: https://drupal.org/project/github_webhook
[get started with organizations]: https://github.com/account/organizations/new
[issue queue]: https://drupal.org/project/issues/project_src_github
