
Shopify eCommerce
+----------------------------------------------------+
|    Maintained by Bonify, LLC  (http://bonify.io)   |
+----------------------------------------------------+

"The easiest way to do ecommerce on Drupal."

Project page: https://www.drupal.org/project/shopify
Documentation: https://www.drupal.org/node/2479703
Demo site: http://dev-bonify-demo.pantheon.io

What does this module do?
- This module provides seamless integration of Shopify with your Drupal site.
- Products, images, tags and collections are synced instantly via webhooks or on demand via batches.
- Complete webhook support for Shopify events. Listen for events in Shopify and trigger your own events in code. (Rules support coming soon.)
- Product tags and collections are represented by customizable taxonomy terms and pages.
- Products, tags, and collections are fieldable. Add your own custom fields to Shopify's. Custom field values are preserved when products are synced.
- Complete Views support for all Shopify product fields and the "add to cart" form.
- Creates a customizable set of Views pages for displaying products located at /products, /products/tags/TAG_ID, and /products/collections/COLLECTION_ID
- Creates a block with links to each Collection page to use as a menu. New collections are added automatically.
- Provides a cart block which shows the amount of items in a user's cart and links to their cart. Will create a font awesome cart image if Font Awesome is installed.
- Product variants and product variant pricing is supported.
- A Shopify theme generator is included to ensure that the transition from your Drupal site to the Shopify checkout process is seamless.
- Handy links to edit products and other common places on Shopify right from Drupal.
- Drush integration to retrieve products, sync, and more.
- Products are integrated with Drupal core search and custom view modes for search are provided.
- Currently the module does not tie orders to Drupal users, but it's possible using the Shopify API.

Dependencies
- A Shopify subscription is required.
- The Shopify API module (https://www.drupal.org/project/shopify_api) must be configured before this module can be installed.
- The Shopify API library (https://github.com/cmcdonaldca/ohShopify.php) must be installed (just follow the instructions on the Shopify API module's project page).
- Pathauto Entity (https://www.drupal.org/project/pathauto_entity) is recommended.

*** This module is the author's own work and is in no way an official project of Shopify. ***
