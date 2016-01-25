<?php

/**
 * URL's of Reference Material
 * Browse Node IDs
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?BrowseNodeIDs.html
 * SearchIndex-ItemSearch Parameter Combinations for UK
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?UKSearchIndexParamForItemsearch.html
 * Sort Values for UK
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?APPNDX_SortValuesArticle.html
 *
 * No parameters_allowed or sorts information available for Sporting Goods or Tools - used US Info
 */

function _amazon_store_locale() {
  $locale = array('UK' => array(
    'All' => array('friendly_name' => t('All'),
  'BrowseNode' => 2000,
  'parameters_allowed' => array('Keywords'),
  'sorts' => NULL,
  ),

  'Apparel' => array('friendly_name' => t('Apparel'),
  'BrowseNode' => 83451031,
  'parameters_allowed' => array('Author', 'Availability', 'Brand', 'BrowseNode', 'Condition', 'Count', 'DeliveryMethod', 'Format', 'ISPUPostalCode', 'ItemPage', 'Keywords', 'Magazines', 'Manufacturer', 'MerchantId', 'Performer', 'PostalCode', 'Sort', 'State', 'Title' ),
  'sorts' => array(
    '-launch-date' => t('Launch Date'),
	'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling')
  ),
  ),

  'Baby' => array('friendly_name' => t('Baby'),
  'BrowseNode' => 60032031,
  'parameters_allowed' => array('Author', 'Availability', 'Brand', 'BrowseNode', 'Condition', 'Count', 'DeliveryMethod', 'Format', 'ISPUPostalCode', 'ItemPage', 'Keywords', 'Magazines', 'Manufacturer', 'MerchantId', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'Value' => t('Description'),
	  'price' => t('Price, Low to High'),
	  '-price' => t('Price, High to Low'),
	  'relevancerank' => t('Relevance'),
	  'salesrank' => t('Bestselling')
  ),
  ),

  'Beauty' => array('friendly_name' => t('Beauty'),
  'BrowseNode' => 66280031,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'Count', 'ItemPage', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Manufacturer', 'MerchantId', 'Sort', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling')
  ),
  ),

  'Books' => array('friendly_name' => t('Books'),
  'BrowseNode' => 1025612,
  'parameters_allowed' => array('Author', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Power', 'Publisher', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'reviewrank' => t('Average customer review'),
    'pricerank' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
    'daterank' => t('Publication date, Newer to Older'),
    'pubdate' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'Classical' => array('friendly_name' => t('Classical'),
  'BrowseNode' => 505510,
  'parameters_allowed' => array('Actor', 'Availability', 'BrowseNode', 'Composer', 'Conductor', 'Count', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'MusicLabel', 'Orchestra', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
  	'inverse-pricerank' => t('Price, High to Low'),
  	'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'DVD' => array('friendly_name' => t('DVD'),
  'BrowseNode' => 283926,
  'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Count', 'Director', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'Performer', 'PostalCode', 'Publisher', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
  	'inverse-pricerank' => t('Price, High to Low'),
    'daterank' => t('Publication date, Older to Newer'),
    'releasedate' => t('Publication date, Older to Newer'),
  	'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'Electronics' => array('friendly_name' => t('Electronics'),
  'BrowseNode' => 560800,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'HealthPersonalCare' => array('friendly_name' => t('Health Personal Care'),
  'BrowseNode' => 66280031,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'HomeGarden' => array('friendly_name' => t('Home Garden'),
  'BrowseNode' => 11052591,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'Jewelry' => array('friendly_name' => t('Jewelry'),
  'BrowseNode' => 193717031,
  'parameters_allowed' => array('Availability', 'BrowseNode', 'DeliveryMethod', 'Keywords', 'MerchantId', 'Sort', 'Title' ),
  'sorts' => array(
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling'),
  ),
  ),

  'Kitchen' => array('friendly_name' => t('Kitchen'),
  'BrowseNode' => 11052591,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
  	'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'MP3Downloads' => array('friendly_name' => t('MP3 Downloads'),
  'exclude_default' => TRUE,
  'BrowseNode' => 77198031,
  'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Count', 'DeliveryMethod', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'ResponseGroup', 'ReviewSort', 'SearchIndex', 'Sort', 'TagPage', 'TagsPerPage', 'TagSort', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
    '-releasedate' => t('Release date, Most Recent to Oldest'),
  	'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling')
  ),
  ),

  'Music' => array('friendly_name' => t('Music'),
  'BrowseNode' => 505510,
  'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Count', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'MusicLabel', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'inverse-pricerank' => t('Price, High to Low'),
  	'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'OfficeProducts' => array('friendly_name' => t('OfficeProducts'),
  'BrowseNode' => 560800,
  'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'DeliveryMethod', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title' ),
  'sorts' => array(
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
  	'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling'),
  ),
  ),

  'OutdoorLiving' => array('friendly_name' => t('Outdoor Living'),
  'BrowseNode' => 11052591,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'Software' => array('friendly_name' => t('Software'),
  'BrowseNode' => 1025614,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

  'SoftwareVideoGames' => array('friendly_name' => t('Software Video Games'),
  'BrowseNode' => 1025616,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

'SportingGoods' => array('friendly_name' => t('Sporting Goods'),
  'BrowseNode' => 319530011,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'relevancerank' => t('Relevance'),
  	'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    'inverseprice' => t('Price, High to Low'),
  	'launch-date' => t('Launch Date'),
    'sale-flag' => t('On sale')
  ),
  ),

'Tools' => array('friendly_name' => t('Tools'),
  'BrowseNode' => 11052591,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'pmrank' => t('Featured items'),
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

'Toys' => array('friendly_name' => t('Toys'),
  'BrowseNode' => 712832,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title' ),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'-mfg-age-min' => t('Age, High to Low'),
    'mfg-age-min' => t('Age, Low to High')
  ),
  ),

'VHS' => array('friendly_name' => t('VHS'),
  'exclude_default' => TRUE,
  'BrowseNode' => 283926,
  'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Count', 'Director', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'Performer', 'PostalCode', 'Publisher', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'inverse-pricerank' => t('Price, High to Low'),
    'daterank' => t('Publication date, Older to Newer'),
  	'releasedate' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

'Video' => array('friendly_name' => t('Video'),
  'BrowseNode' => 283926,
  'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Count', 'Director', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'Performer', 'PostalCode', 'Publisher', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
  	'inverse-pricerank' => t('Price, High to Low'),
    'daterank' => t('Publication date, Older to Newer'),
  	'releasedate' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

'VideoGames' => array('friendly_name' => t('Video Games'),
  'BrowseNode' => 1025616,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
  	'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
  	'daterank' => t('Publication date, Older to Newer'),
    'titlerank' => t('Alphabetical, A to Z'),
  	'-titlerank' => t('Alphabetical, Z to A')
  ),
  ),

'Watches' => array('friendly_name' => t('Watches'),
  'BrowseNode' => 595312,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'TextStream', 'Title'),
  'sorts' => array(
    '-launch-date' => t('Launch Date'),
  	'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
  	'-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
  	'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A')
 ),
 ),
 ),
 );
  return $locale;
}

