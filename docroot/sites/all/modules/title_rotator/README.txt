
Site name & slogan rotator
==========================

Born one silly afternoon for a bit of fun, this module allows you to enter site
name and site slogan alternatives. These change in round-robin fashion, moving
to the next option with every click.

Enter as many alternatives as you like at admin/config/system/title_rotator

If site name and site slogan have the same number of alternatives, they'll
remain in sync.

Safe HTML is allowed, e.g. <em>, <strong>, ...

The current selections for site name and site slogan are remembered in the
visitor's $_SESSION variable.
If you're using a web accelerator or caching engine that balks at anonymous
users having $_SESSIONs, then enable and configure the Session Cache API
module, http://drupal.org/project/session_cache.
