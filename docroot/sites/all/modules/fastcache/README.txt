This module is aimed at providing a robust yet lightweight caching option for
sites that use views and panels. It can be considered as a faster and more advanced
alternative to rules and standard caching provided by views/panels.

Main features:
* caches views inside panels: Ajax views are also supported;
* caches panel variants;
* caches any custom Ctools content type panes;
* supports expiration handling rules - you need to define those via 
  special hook (hook_fastcache_views);
* supports entity-triggered (node/user insert/update/delete) and flag-triggered
  (flag/unflag) expiration rules.

Why do I need to cache contents on my site?
You are not always expected to care about caching (a way to speed-up an application
and at the same time save computer resources). Small sites that are not highly 
visited and have anonymous traffic mostly shouldn't be interested in anything
except probably a caching proxy (Varnish or Nginx). However, when you have a 
limited computer power (say, on a shared hosting or tiny VPS instance) you might
want to get quicker response times for your site, especially when it becomes 
popular. Caching of "heavy" pages of your site will help you.

How to use?
You have to define your caching rules via hook_fastcache_views() and select 
fastcache caching mechanism in panels where you configure which content to 
be shown on a page.
See fastcache_example module for more details.

How it works?
Any caching solution has two major questions to deal with:
* How to cache?
* How to expire?
These questions are actually related to each other, because you can't expire
a cache without knowing how it was created. In Drupal expiration can be done by
cache key, and it is possible to know only part of the key to expire all caches
that start with it (aka wildcard cache clear).
Every time you choose to cache something we create a unique cache key for that
view or panel variant. As some views/panels may show different contents based on
who is viewing it, which page number is requested or which URL is being viewed,
multiple caches will be generated that will consider all these arguments and
contexts.

What types of contexts are supported by the module? They are:
* user context: every user (current or being viewed) will have its own version of 
  the view. E.g: list of favorite books (previously bookmarked), list of posts owned;
* path context: contents of a view depends on current URL being viewed.
  E.g: list of related (to item being viewed) articles, list of users who liked
  the post being viewed etc.

Additional (implicit) contexts are also considered, depending on situation:
* anonymous views: anonymous content of a view may be different from authenticated;
* language: same view may render different items for different languages;
* views/panels view arguments:
  Example 1. Filtering a view by certain criteria will 
  create a separate cached version specific for that criteria.
  Example 2. Every page of a paginated view has own cache.
  Example 3. A search result page will have different cache for each keyword searched
  (provided that you enabled search panel in page manager settings).

Expiration.
Each time contents shown in a cached view is updated (created/updated/deleted)
we must expire the cache so that the cache is regenerated based on new data. 
There are 2 types of events that trigger cache clear:
* entity insert/update/delete: entity is any valid Drupal entity;
* entity flag/unflag action: valid for those configurations that use flag contrib
  (a pretty popular module for implementing various flagging scenarios: likes, 
  follows etc.)
We handle both triggers in an intelligent way that allows to expire only specific
caches based on triggering event conditions. For example, if a node of type "article"
was created, we only expire caches for those views that list articles. If a node 
of type "page" was added to favorites by a user, we will only expire that specific
user's cache of favorite pages.

Known issues.
There are some known cases which are not fully handled by this module:
* multiple cached views with pagination on the same page (pager IDs are not handled);
  non-paginated views and one paginated cached view per page will however work fine;

Recommendations.
It is recommended to use this module together with memcache and entitycache. The
first one will replace your DB-based cache storage with a super-fast memcached
memory storage (a common and first recommendation for site optimization). 
The second will ensure you don't load entity (nodes and users) data every time 
from scratch, resulting in noticeable (although not crucial) performance gains.
