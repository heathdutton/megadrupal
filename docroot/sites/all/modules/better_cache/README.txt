This is an enhancement over drupal's default cache.

About the default Drupal7 cache There is a lot of confusion and misunderstanding
about Drupal's default cache. In particular regarding the 'Minimum cache
lifetime' setting. Contrary to common understanding, each cached page does not
have a lifetime according the 'Minimum cache lifetime'. In fact, every cached
page is flagged as TEMPORARY in the cache_page table. On every cron run, if time
from the last cache clear is equal or above the 'Minimum cache lifetime' then
all TEMPORARY cache entries are deleted. Than means that a page which is cached
just 2 minutes ago will also be removed from cache even if the 'Minimum cache
lifetime' is set to 1 WEEK. Apart from that, default cache makes no
discrimination between different types of pages. All pages are cached as
'TEMPORARY' and are handled the same.

So, default drupal cache poses 2 main problems. 1) No discrimination between
different types of pages and requests 2) Everything is deleted when Minimum
cache lifetime expires.

More information about the above problem can be found in the following article
https://www.drupal.org/node/1279654

What Better Cache does This module does mainly 3 things 1) Allows cache to
handle differently each page type 2) Allows the admin to set different cache
lifetimes for the different page types 3) Lifetime now behaves as expected. Each
page cache respects its own lifetime.

Module Technical Details The module provides extra functionality for page
caching. This means that it only affects records saved by the default cache in
the database table 'cache_page'. Each page url which matches at least one of the
rules you define, is not cached as 'TEMPORARY' any more but with a specific
expiration date. The expiration date is the current timestamp + the minutes that
are defined with the rule. On every cron run, only the cached pages which have
an expired expiration date will be deleted from the cache.
