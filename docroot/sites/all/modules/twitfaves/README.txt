Introduction
============
Twitfaves integrates with the Twitter API in order to build a block with the
favorite tweets of a Twitter account.

Multiple blocks can be set up for different Twitter accounts.

This module relies on Twitter module in order to authenticate Twitter
accounts, since accessing to the favorite tweets of an account requires
authentication.

Installation
============

Please follow the installation steps of the Twitter module at
http://drupal.org/node/1346824.

Once you have an authenticated Twitter account at admin/config/services/twitter,
go to the Block administration at admin/structure/block and configure the
block named "Favorite tweets":
  * Optionally set a title.
  * Type the screen name of the Twitter account from which you want to extract
    its favorite Tweets.
  * Define the amount of tweets that you want to list.
  * Define how often the cache should be cleared.
  * Choose a region for your block.

The Favorite Tweets block is built on page load and cached for the time specified
at the block configuration unless caches are cleared.

Theming
=======
Twitfaves uses theme('twitfaves') to render its tweets. In order to override this
template implement your own in your default theme or a custom module.
