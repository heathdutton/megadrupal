Description
-----------

Cookie-Aware Page Cache module extends Drupal core's default database cache to
key page cache entries by cookie in addition to URL.

You may need this if you are preparing different markup for pages depending on
the value of a user's browser cookie, and the default database cache is the only
caching mechanism available to you. For example, you may be displaying local
information on a user's favorite store location, which is kept in a cookie. Or
you might be using Context Breakpoint module, which uses a cookie to pass
information about the user's screen size to use as a condition in the Context
system.

You can specify more than one cookie to take into account when querying the
page cache. However, you should only select cookies with limited sets of
possible values. The performance benefits of page caching decrease as the
number of possible cookie value combinations increases.

Installation
------------

Installing and configuring the module takes place entirely in your site's
settings.php file.

1. Download and unpack the module to your Drupal site.

2. Open your settings.php file for editing.

3. Add Cookie-Aware Page Cache to the list of available cache backends by adding
   this line:

   $conf['cache_backends'][] = 'sites/all/modules/cookie_aware_page_cache/CookieAwarePageCache.inc';

   If you placed the module in a directory other than 'sites/all/modules',
   adjust the line accordingly.

4. Configure the page cache to use CookieAwarePageCache:

   $conf['cache_class_cache_page'] = 'CookieAwarePageCache';

5. Configure the list of cookies to make relevant to the page cache:

   $conf['cookie_aware_page_cache_cookies'][] = 'name_of_cookie';
   $conf['cookie_aware_page_cache_cookies'][] = 'name_of_another_cookie';

6. Switch on the anonymous page cache in admin/config/development/performance.

