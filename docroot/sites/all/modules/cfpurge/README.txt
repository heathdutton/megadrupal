CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirement
 * Installation
 * Credits


INTRODUCTION
------------

CloudFlare Purge (CFPurge) is a plugin for the Expire module which enables it to
clear specific pages from the CloudFlare CDN when you update, delete or add a
page.

CloudFlare with "cache everything" enabled provides the ultimate performance for
your anonymous users, down to a 20 millisecond response time and extremely fast
transfer rate. When you enable this setting your site can instantly handle
millions of anonymous users, even if you're using a shared host.

By the way, Google also likes really fast websites, so it also improves your
SEO rating.


REQUIREMENT
-----------

A CloudFlare account with "cache everything" enabled on a Page Rule. Make sure
you create Page Rules for directories that you don't want to cache, such as
/services/service, in case you have a service set up using that path. The free
package lets you cache everything and also let's you create a few page rules.

PHP with curl enabled. This module uses curl for issuing the http POST
requests to CloudFlare.

CloudFlare Purge requires the Expire module.


INSTALLATION
------------

If using Drush, drush dl cfpurge and then drush en cfpurge otherwise unpack,
place and enable just like any other module.

Navigate to Administration » Configuration » Development » Performance »
CloudFlare Purge Settings (admin/config/development/performance/cfpurge) and
enter your CloudFlare info.

Navigate to Administration » Configuration » Development » Performance
(admin/config/development/performance) and set the Expiration of Cached Pages to
3 hours or more. This means that CloudFlare will store your page for 3 hours or
more if you don't edit or delete it beforehand. You can ignore the other
settings on this page. If you enable local caching--which wont be necessary
--your local cache will be purged as well.


NOTE
----

When using CloudFlare all requests will come from one IP, to remedy this you
will have to add this line to the bottom of your settings.php file:
$_SERVER['REMOTE_ADDR'] = !empty($_SERVER["HTTP_CF_CONNECTING_IP"]) ?
  $_SERVER["HTTP_CF_CONNECTING_IP"] : $_SERVER["REMOTE_ADDR"];


CREDIT
------

Gerald Melendez / geraldmelendez on Drupal.org

The Purge plugin, http://drupal.org/project/purge , for providing the
inspiration and guidance to develop this plugin.

John Roberts from Cloudflare, for reaching out to me and offering additional
info on purging CloudFlare
