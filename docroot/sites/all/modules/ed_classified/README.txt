
Classified Ads 7.3
==================

Classified Ads 7.3 is a straight port from the 6.3 version.

As such it has no notion of Drupal 7 concepts like non-node entities, but is
still a traditional node module. It completely leverages the new APIs in D7,
though: DBTNG, EFQ, Token...

More generally, Classified Ads is a new module suite for Classified Ads, which 
happens to be implemented in the same Drupal.org project as the early 
ED Classified module.


Installing
----------

Follow the normal Drupal installation procedure for a module, including 
installing and enabling its dependencies: core Taxonomy and Field 
modules. Then, configure the module: 

- Optional: allow statistics collection 
  - Enable the "Statistics" module
  - Browse to http://<your site URL>/admin/reports/settings
  - Check "Count content view" and click "Save configuration".

- Browse to http://<your site url>/admin/structure/taxonomy
- Click on the "list terms" link on the vocabulary designated as "Classified Ads categories"
- The module installs two categories by default: "For sale" and "Wanted". Define 
  or remove categories to your liking. Take care to keep them in a tree: no term 
  should have more than one parent.
  
- Browse to http://<your site url>/admin/config/content/classified
- You can now define the various settings, including Ad lifetime for each of the
  categories you defined previously. Any category without an explicit lifetime
  defined inherits the lifetime of its parent category.
  
- Browse to http://<your site url>/admin/structure/block
- You can enable and configure 3 blocks

- Optional: enable notifications
  - Enable the Classified Notifications module and the core Token module.
  - Browse to http://<your site url>/admin/config/content/classified/notifications
  - Configure your notification messages: various "Replacement patterns"
    are available to customize the content, the most important being 
    [user:classified-ads] and [user:classified-ads-url] in the Users category.

- Optional: getting advanced help
  - Download and enable the Advanced Help module for information on theming or
    using the module API to extend its functionality.


Updating from ED Classified 6.x or Classified Ads 6.x-3.x
---------------------------------------------------------

Although the user interface and features of the modules are largely similar, 
they are quite different under the hood, meaning there is no integrated
migration path from 6.x versions to Classified 7.x-3.x.

Customizing Classified Ads
--------------------------

You can adjust the settings and content of the Classified Ads vocabulary and
its associated Taxonomy field, but MUST NOT remove them or the module will no
longer work.

This module can be customized and extended in a number of ways:

- A bundled Context 3 condition plugin allowing you to trigger reactions on
  paths controlled by the module
- Views integration allows you to build views including the fields of ad nodes
  in addition to the core and contrib fields
- Fields integration allows you 
  - to customize the placement of the Expiration widget on ad pages
  - to customize the rendering of the default Ads list page by modifying the
    "Classified Ads" view mode  
- The main Ads listing page, in addition to being customizable at the view mode
  level, is also a theme template which you can override in your site theme for
  even more flexibility
- The module defined three hooks which custom modules can use to override its
  default behaviours. 
  
These features are detailed in the Advanced Help section for the module.


Knowns Limitations
------------------

Classified Ads 7.x-3.0/3.1 has no multilingual support. Single-language sites
in a language other than english may work with appropriate settings, 
Multilingual sites will not.
