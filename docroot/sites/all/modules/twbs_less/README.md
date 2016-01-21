TWBS LESS
=========

A helper module for [TWBS](https://drupal.org/project/twbs), handle
[less.js](http://lesscss.org/) and
[less.php](http://lessphp.gpeasy.com/) library integration, provide
Drupal LESS support in both server-side pre-compile / client-side
live-compile mode.

Add your files just like any other CSS file, just with .less as the
extension, and they will be automatically processed. No additional
configuration is required.

### Key Features

-   Provide [drush
    make](https://raw.github.com/drush-ops/drush/master/docs/make.txt)
    file for library download
-   Confirm library successfully initialized with
    [hook\_requirements()](https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_requirements/7)
-   Support both LTR \*.less and RTL \*-rtl.less, as like as that of
    [locale\_css\_alter()](https://api.drupal.org/api/drupal/modules!locale!locale.module/function/locale_css_alter/7)
    supported
-   If Drupal's CSS aggregation enabled, all .less will automatically
    pre-compile as .css at server-side by
    [less.php](http://lessphp.gpeasy.com/) before Drupal CSS aggregation
-   If Drupal's CSS aggregation disabled, all .less will link with rel
    set to "stylesheel/less" individually, so
    [less.js](http://lesscss.org/) will handle in client-side
    live-compile
-   Support
    [FireLESS](https://github.com/abstract-open-solutions/fireless) for
    .less debug with [Firebug](https://getfirebug.com/) in client-side
    compile mode

### Getting Started

Download and install with [drush](https://github.com/drush-ops/drush)
manually:

    drush -y dl --dev twbs_less
    drush -y make --no-core sites/all/modules/twbs_less/twbs_less.make

Package into your own drush .make file (e.g.
[build-drustack.make](http://drupalcode.org/project/drustack.git/blob/refs/heads/7.x-25.x:/build-drustack.make)):

    api = 2
    core = 7.x
    projects[twbs_less][download][branch] = 7.x-3.x
    projects[twbs_less][download][type] = git
    projects[twbs_less][download][url] = http://git.drupal.org/project/twbs_less.git
    projects[twbs_less][subdir] = contrib

### Live Demo

[TWBS LESS](https://drupal.org/project/twbs_less) is now integrated into
[DruStack](https://drupal.org/project/drustack) distribution, so you can
try it in a live sandbox with
[simplytest.me](http://simplytest.me/project/drustack/7.x-25.x).

### Why Another LESS Module?

For general and generic LESS support you should always consider another
[LESS CSS Preprocessor](https://drupal.org/project/less) module which
started since 2010-03-04.

On the other hand you should consider about using this module because
of:

-   Purely design for assist [TWBS](https://drupal.org/project/twbs),
    which means you will have the best compatibility when using both
    together
-   Support both server-side pre-compile / client-side live-compile with
    debug mode enable; [LESS CSS
    Preprocessor](https://drupal.org/project/less) support server-side
    pre-compile
-   RTL support with \*-rtl.less just as simple as CSS \*-rtl.css that
    Drupal core supported; [LESS CSS
    Preprocessor](https://drupal.org/project/less) RTL support will work
    as long as your files are named "somename.css.less"
-   Server-side pre-compile CSS will name based on its original .less
    [md5\_file()](http://www.php.net/manual/en/function.md5-file.php)
    result, which means even you add a single space the cache file will
    also be regenerate; [LESS CSS
    Preprocessor](https://drupal.org/project/less) calculate based on
    [filemtime()](http://www.php.net/manual/en/function.filemtime.php)
-   Much simple implementation which handle all LESS support
    automatically as like as that of Drupal core CSS support; [LESS CSS
    Preprocessor](https://drupal.org/project/less) provide more
    customization options

### Known Issues

IMHO, below limitation as not directly due to our current
implementation:

-   Can't package into Drupal distribution hosted under drupal.org GIT
    repository directly (but able to include into your own
    [build-\*.make](http://drupalcode.org/project/drustack.git/blob/refs/heads/7.x-25.x:/build-drustack.make)
    or
    [simplytest.make](http://drupalcode.org/project/drustack.git/blob/refs/heads/7.x-25.x:/simplytest.make)),
    because:
    -   less.js is now licensing with Apache License 2.0 which conflict
        with [drupal.org GIT repository GPLv2+ only
        policy](https://drupal.org/node/1449452)
    -   Unless drupal.org GIT repository policy changed or [less.js
        dual/re-license with
        MIT](https://github.com/less/less.js/issues/1029#issuecomment-31380360)

-   LESS [variables](http://lesscss.org/#-variables) and
    [mixins](http://lesscss.org/#-mixins) that defined by other else
    module/theme can't be reuse directly (e.g. You can't reuse .fa mixin
    that provided by [TWBS Font
    Awesome](https://drupal.org/project/twbs_fontawesome) into your own
    module/theme), because:
    -   If using server-side pre-compile, all LESS/CSS should be sorted
        in correct order, all url() and @import should replace as
        correct absolute URL, and aggregated into a single-fat-file
        before compile into CSS at once
    -   If using client-side live-compile, external .less should be
        referenced by @import with absolute URL correctly

### Author

-   Developed by [Edison Wong](http://drupal.org/user/33940).
-   Sponsored by [PantaRei Design](http://drupal.org/node/1741828).

