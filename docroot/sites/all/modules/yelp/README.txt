Yelp 2.x for Drupal 7.x
-----------------------
The Yelp package contains two modules which allow for searching and displaying 
business results from Yelp, using the Yelp API.

Yelp API 2.x for Drupal 7.x
---------------------------
- The Yelp API module provides the nessisary functions and hooks to query the Yelp 
API and return results. It does not display results on its own, mearly provides 
a framework for fetching, caching and returning results. To use the Yelp API 
(and any modules that depend on it) you will need to obtain API keys[1].

- The Yelp API module provides form fields which other modules can add to custom 
forms to define search parameters. When implemented the Yelp API form fields 
support tokens[2] and can use the addressfield[3] module for location data 
(The addressfield module must be patched[4]).

- Checkout the 'yelp_api/README.txt' for more information.

- Checkout the 'yelp_api/API.TXT' for information on how to use the Yelp API 
module with your module.

- Checkout the 'yelp_api.api.inc' for information on provided hooks.


Yelp Blocks 2.x for Drupal 7.x
------------------------------
- The Yelp Blocks module provides custom blocks to display Yelp API search results.
The Yelp Blocks module depends on the Yelp API module and uses the Yelp API form 
fields for it's block configuration form (thus Yelp blocks supports tokens[2] 
and addressfield[3]). 

- The Yelp Blocks module supports <features> for exporting/importing blocks and 
their results.

- All Yelp blocks and their results are completly themable. See comments in 
yelp-block-business.tpl.php and yelp-block-search-results.tpl.php for available 
variables and template naming suggestions.


Yelp.com Release information
-----------------------------
- This module was not developed by Yelp.com, nor is it registered to or endorsed
by Yelp.com

- The yelp.com logo, all search information, photos & images belong to yelp.com 
where applicable

For Developers
--------------
Please read `yelp_api/API.txt` for more information about the concepts and integration
points in the Yelp API module.

Maintainer
-----------
- mikemiles86 (Mike Miles)

[1]: http://www.yelp.com/developers
[2]: http://drupal.org/project/token
[3]: http://drupal.org/project/addressfield
[4]: http://drupal.org/node/970048#comment-5712492