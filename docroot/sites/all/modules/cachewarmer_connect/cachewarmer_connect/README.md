CacheWarmer Connect is a module to integrate your site with the
[cache-warmer.com](https://www.cache-warmer.com) service. After enabling the
module a cache clear will trigger cache-warmer.com to immediately warm your
Drupal site's cache.

Installation
------------
* Move the module to your site's modules folder.
* Enable the CacheWarmer Connect module.
* Enter your project ID and secret key in the settings form via Configuration >
  System -> CacheWarmer and check the "Enable CacheWarmer integration" checkbox.

Usage
-----
When a cache_clear_all is triggered CacheWarmer Connect will send a request with
your project ID to the [cache-warmer.com](https://www.cache-warmer.com) service.
This will automatically schedule a cachewarmer run to your website.

FAQ
---

* **Where can I find my project ID and secret key?**
  Log in to your account on [cache-warmer.com](https://www.cache-warmer.com) and
  go to your project. Below the "Warm your cache" header there's a Webhook
  section where your id and secret are displayed.


