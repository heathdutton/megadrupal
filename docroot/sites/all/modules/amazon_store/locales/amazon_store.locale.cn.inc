<?php

/**
 * URL's of Reference Material
 * Browse Node IDs
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/BrowseNodeIDs.html
 * SearchIndex-ItemSearch Parameter Combinations for CN
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/CNSearchIndexParamForItemsearch.html
 * Sort Values for CN
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/CNSortValuesArticle.html
 *
 * No parameters_allowed information available for Photo used US Info
 * No sort information available for OfficeProducts used US Info
 */

function _amazon_store_locale() {
  $locale = array('CN' => array(
      'All' => array(
        'friendly_name' => t('All'),
        'BrowseNode' => 2000,
        'parameters_allowed' => array('Keywords'),
        'sorts' => NULL,
      ),

      'Apparel' => array(
        'friendly_name' => t('Apparel'),
        'BrowseNode' => 2016156051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launchdate' => 'Latest arrivals',
        ),
      ),

      'Appliances' => array(
        'friendly_name' => t('Appliances'),
        'BrowseNode' => 80207071,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launchdate' => 'Latest arrivals',
        ),
      ),

      'Automotive' => array(
        'friendly_name' => t('Automotive'),
        'BrowseNode' => 1947899051,
        'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launchdate' => 'Latest arrivals',
        ),
      ),

      'Baby' => array(
        'friendly_name' => t('Baby'),
        'BrowseNode' => 42692071,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'Beauty' => array(
        'friendly_name' => t('Beauty'),
        'BrowseNode' => 746776051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'Books' => array(
        'friendly_name' => t('Books'),
        'BrowseNode' => 658390051,
        'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Power', 'Publisher', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'pricerank' => 'Price: Low to high',
          '-pricerank' => 'Price: High to low',
          'daterank' => 'Publish date: New to old',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'Electronics' => array(
        'friendly_name' => t('Electronics'),
        'BrowseNode' => 2016116051,
        'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'release-date' => 'Release date: Older to newer',
          '-release-date' => 'Release date: Newer to older',
        ),
      ),

      'Grocery' => array(
        'friendly_name' => t('Grocery'),
        'BrowseNode' => 2127215051,
        'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'HealthPersonalCare' => array(
        'friendly_name' => t('Health Personal Care'),
        'BrowseNode' => 852803051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'release-date' => 'Release date: Older to newer',
          '-release-date' => 'Release date: Newer to older',
        ),
      ),

      'Home' => array(
        'friendly_name' => t('Home'),
        'BrowseNode' => 2016126051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'HomeImprovement' => array(
        'friendly_name' => t('Home Improvement'),
        'BrowseNode' => 1952920051,
        'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'Jewelry' => array(
        'friendly_name' => t('Jewelry'),
        'BrowseNode' => 816482051,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
        ),
      ),

      'Misc' => array(
        'friendly_name' => t('Miscellaneous'),
        'BrowseNode' => 899280051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launch-date' => 'Launch date: Newer to older',
        ),
      ),

      'Music' => array(
        'friendly_name' => t('Music'),
        'BrowseNode' => 754386051,
        'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'MusicLabel', 'ReleaseDate', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'orig-rel-date' => 'Original release date: earliest to latest',
          '-orig-rel-date' => 'Original release date: latest to earliest',
        ),
      ),

      'OfficeProducts' => array(
        'friendly_name' => t('Office Products'),
        'BrowseNode' => 2127221051,
        'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        // @todo fix Sort Values as CN has none listed
        // @see http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/CNSortValuesArticle.html
        // From US Sort Values
        'sorts' => array(
          'pmrank' => 'Featured items',
          'salesrank' => 'Bestselling',
          'reviewrank' => 'Average customer review: high to low',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'titlerank' => 'Alphabetical: A to Z',
        ),
      ),

      'Photo' => array(
        'friendly_name' => t('Photo'),
        'BrowseNode' => 755653051,
        // @todo fix allowed parameters as CN has none listed
        // @see http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/CNSearchIndexParamForItemsearch.html
        // From DE ItemSearch parameters
        // 'parameters_allowed' => ('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
        // From US ItemSearch parameters
        'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'ItemPage', 'Keywords', 'Sort', 'State', 'TextStream', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launchdate' => 'Launch date: Latest to earliest',
        ),
      ),

      'Shoes' => array(
        'friendly_name' => t('Shoes'),
        'BrowseNode' => 2029189051,
        'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          '-launch-date' => 'Launch date: Latest to earliest',
        ),
      ),

      'Software' => array(
        'friendly_name' => t('Software'),
        'BrowseNode' => 863872051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'releasedate' => 'Release date: Oldest to newest',
          '-releasedate' => 'Release date: Newest to oldest',
        ),
      ),

      'SportingGoods' => array(
        'friendly_name' => t('Sporting Goods'),
        'BrowseNode' => 836312051,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'releasedate' => 'Release date: Oldest to newest',
          '-releasedate' => 'Release date: Newest to oldest',
        ),
      ),

      'Toys' => array(
        'friendly_name' => t('Toys'),
        'BrowseNode' => 647070051,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'reviewrank' => 'Average customer review: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'releasedate' => 'Release date: Oldest to newest',
          '-releasedate' => 'Release date: Newest to oldest',
        ),
      ),

      'Video' => array(
        'friendly_name' => t('Video'),
        'BrowseNode' => 2016136051,
        'parameters_allowed' => array('Actor', 'Availability', 'BrowseNode', 'Director', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Publisher', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'pricerank' => 'Price: Low to high',
          '-pricerank' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'orig-rel-date' => 'Original release date: Oldest to newest',
          '-orig-rel-date' => 'Original release date: Newest to oldest',
        ),
      ),

      'VideoGames' => array(
        'friendly_name' => t('Video Games'),
        'BrowseNode' => 897415051,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'ReleaseDate', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
          'releasedate' => 'Release date: Earliest to latest',
          '-releasedate' => 'Release date: Latest to earliest',
        ),
      ),

      'Watches' => array(
        'friendly_name' => t('Watches'),
        'BrowseNode' => 1953164051,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'salesrank' => 'Best to worse selling',
          'price' => 'Price: Low to high',
          '-price' => 'Price: High to low',
          'titlerank' => 'Alphabetical: A to Z',
          '-titlerank' => 'Alphabetical: Z to A',
        ),
      ),
    ),
  );
  return $locale;
}
