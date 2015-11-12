<?php

/**
 * URL's of Reference Material
 * Browse Node IDs
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?BrowseNodeIDs.html
 * SearchIndex-ItemSearch Parameter Combinations for UK
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?UKSearchIndexParamForItemsearch.html
 * Sort Values for UK
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/index.html?APPNDX_SortValuesArticle.html
 */

function _amazon_store_locale() {
  $locale = array(
'DE' => array(
   'All' => array('friendly_name' => t('All'),
    'BrowseNode' => 2000,
    'parameters_allowed' => array('Keywords'),
    'sorts' => NULL,
    ),


  'Apparel' => array('friendly_name' => t('Apparel'),
  'BrowseNode' => 78689031,
  'parameters_allowed' => array('Author', 'Availability', 'Brand', 'BrowseNode', 'Condition', 'Count', 'DeliveryMethod', 'Format', 'ISPUPostalCode', 'ItemPage', 'Keywords', 'Magazines', 'Manufacturer', 'MerchantId', 'Performer', 'PostalCode', 'ReviewRank', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling'),
  ),
),


  'Automotive' => array('friendly_name' => t('Automotive'),
  'BrowseNode' => 78194031,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'TextStream', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'reviewrank' => t('Average customer review'),
  ),
),

  'Baby' => array('friendly_name' => t('Baby'),
  'BrowseNode' => 357577011,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'salesrank' => t('Bestselling'),
  ),
),

  'Beauty' => array('friendly_name' => t('Beauty'),
  'BrowseNode' => 64257031,
  'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'Count', 'ItemPage', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Manufacturer', 'MerchantId', 'Sort', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling'),
  ),
),

  'Books' => array('friendly_name' => t('Books'),
  'BrowseNode' => 541686,
  'parameters_allowed' => array('Author', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Power', 'Publisher', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'reviewrank' => t('Average customer review'),
    'pricerank' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Classical' => array('friendly_name' => t('Classical'),
  'BrowseNode' => 542676,
  'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Composer', 'Conductor', 'Count', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'MusicLabel', 'Orchestra', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'pubdate' => t('Publication date, Most Recent to Oldest'),
    '-pubdate' => t('Publication date, Oldest to Most Recent'),
    'publication_date' => t('Publication date, Most Recent to Oldest'),
    '-publication_date' => t('Publication date, Oldest to Most Recent'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'DVD' => array('friendly_name' => t('DVD'),
  'BrowseNode' => 547664,
   'parameters_allowed' => array('Actor', 'Availability', 'BrowseNode', 'Composer', 'Conductor', 'Count', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'MusicLabel', 'Orchestra', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),


  'Electronics' => array('friendly_name' => t('Electronics'),
  'BrowseNode' => 569604,
   'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),


  'ForeignBooks' => array('friendly_name' => t('Foreign Books'),
  'BrowseNode' => 54071011,
  'parameters_allowed' => array('Author', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Publisher', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'reviewrank' => t('Average customer review'),
    'pricerank' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'HealthPersonalCare' => array('friendly_name' => t('Health and Personal Care'),
  'BrowseNode' => 64257031,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'HomeGarden' => array('friendly_name' => t('Home and Garden'),
  'BrowseNode' => 10925241,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Jewelry' => array('friendly_name' => t('Jewelry'),
  'BrowseNode' => 327473011,
  'parameters_allowed' => array('Availability', 'BrowseNode', 'DeliveryMethod', 'Keywords', 'MerchantId', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
  ),
),

  'Kitchen' => array('friendly_name' => t('Kitchen'),
  'BrowseNode' => 3169011,
  'parameters_allowed' => array('Author', 'Availability', 'Brand', 'BrowseNode', 'Condition', 'Count', 'DeliveryMethod', 'Format', 'ISPUPostalCode', 'ItemPage', 'Keywords', 'Magazines', 'Manufacturer', 'MerchantId', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'salesrank' => t('Bestselling'),
  ),
),

  'Magazines' => array('friendly_name' => t('Magazines'),
  'BrowseNode' => 1198526,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Publisher', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'MP3Downloads' => array('friendly_name' => t('MP3 Downloads'),
  'exclude_default' => TRUE,
  'BrowseNode' => 77256031,
  'parameters_allowed' => array('Availability', 'BrowseNode', 'DeliveryMethod', 'Keywords', 'MerchantId', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'artistalbumrank' => t('Artist, A to Z'),
    '-artistalbumrank' => t('Artist, Z to A'),
    'albumrank' => t('Album, A to Z'),
    '-albumrank' => t('Album, Z to A'),
    'runtime' => t('Time, Short to Long'),
    '-runtime' => t('Time, Long to Short'),
    '-releasedate' => t('Release date, Newer to Older'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Music' => array('friendly_name' => t('Music'),
  'BrowseNode' => 542676,
  'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Count', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'MusicLabel', 'Performer', 'PostalCode', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'pubdate' => t('Publication date, Most Recent to Oldest'),
    '-pubdate' => t('Publication date, Oldest to Most Recent'),
    'publicationdate' => t('Publication date, Most Recent to Oldest'),
    '-publicationdate' => t('Publication date, Oldest to Most Recent'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'OfficeProducts' => array('friendly_name' => t('Office Products'),
  'BrowseNode' => 16291311,
  'parameters_allowed' => array('Availability', 'Brand', 'BrowseNode', 'DeliveryMethod', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'relevancerank' => t('Relevance'),
    'reviewrank' => t('Average customer review'),
    'salesrank' => t('Bestselling'),
  ),
),

  'OutdoorLiving' => array('friendly_name' => t('Outdoor Living'),
  'BrowseNode' => 10925051,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'PCHardware' => array('friendly_name' => t('PC Hardware'),
  'BrowseNode' => 569604,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Photo' => array('friendly_name' => t('Photo'),
  'BrowseNode' => 569604,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Software' => array('friendly_name' => t('Software'),
  'BrowseNode' => 542064,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    '-date' => t('Publication date, Newer to Older'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'SoftwareVideoGames' => array('friendly_name' => t('Software Video Games'),
  'BrowseNode' => 541708,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'reviewrank' => t('Average customer review'),
    'price' => t('Price, Low to High'),
    'inverse-pricerank' => t('Price, High to Low'),
    '-date' => t('Publication date, Newer to Older'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'SportingGoods' => array('friendly_name' => t('Sporting Goods'),
  'BrowseNode' => 16435121,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
    'release-date' => t('Release date, Older to Newer'),
    '-release-date' => t('Release date, Newer to Older'),
  ),
),

  'Tools' => array('friendly_name' => t('Tools'),
    // TODO: Find BrowseNode for Tools

  // 'BrowseNode' => ,

  'parameters_allowed' => array('Condition', 'ItemPage', 'Keywords', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-pricerank' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Toys' => array('friendly_name' => t('Toys'),
  'BrowseNode' => 12950661,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'Count', 'ItemPage', 'Keywords', 'Sort', 'MerchantId', 'Title'),
  'sorts' => array(
    'pmrank' =>	'Featured items',
    'salesrank' => t('Bestselling first'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    '-date' => t('Pub Date, Old to New'),
    'titlerank' => t('Alphabetical, A to Z'),
   ),
),

  'VHS' => array('friendly_name' => t('VHS'),
  'BrowseNode' => 547082,
  'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Count', 'Director', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'Performer', 'PostalCode', 'Publisher', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Video' => array('friendly_name' => t('Video'),
  'BrowseNode' => 547664,
  'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Count', 'Director', 'Format', 'ItemPage', 'Keywords', 'Magazines', 'Performer', 'PostalCode', 'Publisher', 'Sort', 'State', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'VideoGames' => array('friendly_name' => t('Video Games'),
  'BrowseNode' => 541708,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'MPAARating', 'Sort', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    '-date' => t('Publication date, Newer to Older'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
),

  'Watches' => array('friendly_name' => t('Watches'),
  'BrowseNode' => 193708031,
  'parameters_allowed' => array('BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'TextStream', 'Title'),
  'sorts' => array(
    'salesrank' => t('Bestselling'),
    'price' => t('Price, Low to High'),
    '-price' => t('Price, High to Low'),
    'titlerank' => t('Alphabetical, A to Z'),
    '-titlerank' => t('Alphabetical, Z to A'),
  ),
  ),
  ),
);

  return $locale;
}
