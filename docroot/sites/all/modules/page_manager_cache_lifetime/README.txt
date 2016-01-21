Page manager cache lifetime
===========================

This module allows you to override the cache expiration variables on each page manager variant.
Once the module is enabled, an extra operation will appear when editing a variant.
This will allow the page to override the default variables and use a custom one.

The variables supported are:
* Minimum cache lifetime (cache_lifetime)
* Expiration of cached pages (page_cache_maximum_age)

These can both by edited globally on the admin/config/development/performance page.

Why would this be useful?
Panel pages are often landing pages, and are updated more frequently than regular
content pages, so often require different caching rules from those other pages.
