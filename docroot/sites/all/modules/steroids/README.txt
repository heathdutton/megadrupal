For installationg instructions, see INSTALL.txt

1. What is steroids?
====================

Steroids is a module that bundles a few existing modules, together with some of
our own code, into one big module that handles setting expire headers and handling
cache purges for Varnish.

We always ended up with using the same collection of modules for handling our
caching, so we dediced to group them into one module. A few of the modules also
had no D7 version, so those were ported as a new version into steroids.

Modules currently ported into steroids are:
- esi (Edge Side Includes, a Varnish feature)
- expire
- purge
- varnish

Modules we custom built:
- cache_bypass (allow certain user roles to bypass the cache)
- max-age (set custom expire times per content type)


2. What is Varnish?
===================

From the Varnish website [1]: "Varnish is a web application accelerator. You
install it in front of your web application and it will speed it up significantly."

[1] https://www.varnish-cache.org/

3. How does it work?
====================




4. Authors
==========

Steroids started as a project by Kirstof Van Roy at Nascom.be and was completed
with a little help of Karel Bemelmans at Nascom.be

5. Contact and website
======================

http://drupal.org/project/steroids


