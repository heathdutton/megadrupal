<?php

function _amazon_store_locale() {

  $locale = array('IT' => array(


  'All' => array(
    'friendly_name' => t('All'),
    'BrowseNode' => 2000,
    'parameters_allowed' => array('Keywords'),
    'sorts' => NULL,
  ),
  'Books' => array(
    'friendly_name' => t('Books'),
    'BrowseNode' => 411663031,
    'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Publisher',  'ReviewSort', 'Title'),
    'sorts' => array(
      'relevancerank' => t('Relevance'),
      'salesrank' => t('Bestselling'),
      'reviewrank' => t('Average customer review: high to low'),
      'pricerank' => t('Price: low to high'),
      'inverse-pricerank' => t('Price: high to low'),
      'daterank' => t('Publication date: newer to older'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-titlerank' => t('Alphabetical: Z to A'),
    ),
  ),
  'DVD' => array(
    'friendly_name' => t('DVD'),
    'BrowseNode' => 412606031,
    'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Director', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Publisher', 'Sort', 'Title'),
    'sorts' => array(
      'relevancerank' => t('Relevance'),
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-video-release-date' => t('Release date: newer to older'),
      'releasedate' => t('Release date: newer to older'),
    ),
  ),
  'Electronics' => array(
    'friendly_name' => t('Electronics'),
    'BrowseNode' => 412609031,
    'parameters_allowed' => array('BrowseNode', 'Availability', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
    'sorts' => array(
      'pmrank' => t('Featured items'),
      'salesrank' => t('Bestselling'),
      'reviewrank' => t('Average customer review: high to low'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
    ),
  ),
   'Garden' => array(
    'friendly_name' => t('Garden'),
    'BrowseNode' => 635016031,
    'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
    'sorts' => array(
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-titlerank' => t('Alphabetical: Z to A'),
    ),
  ),
  'Kitchen' => array(
    'friendly_name' => t('Kitchen'),
    'BrowseNode' => 524015031,
    'parameters_allowed' => array('Availability', 'Author','Brand', 'BrowseNode', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Keywords', 'Sort', 'Title'),
    'sorts' => array(
      'pmrank' => t('Featured items'),
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-titlerank' => t('Alphabetical: Z to A'),
    ),
  ),
  'Music' => array(
    'friendly_name' => t('Music'),
    'BrowseNode' => 412600031,
    'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Keywords', 'MusicLabel', 'MinimumPrice', 'MaximumPrice', 'Sort', 'Title'),
    'sorts' => array(
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-titlerank' => t('Alphabetical: Z to A'),
      'artistrank' => t('Artist name: A to Z'),
      'orig-rel-date' => t('Original Release Date'),
      'release-date' => t('Latest Release Date'),
      'releasedate' => t('Release date: most recent to oldest'),
      '-releasedate' => t('Release date: oldest to most recent'),
      'relevancerank' => t('Relevance'),
    ),
  ),
  'Shoes' => array(
    'friendly_name' => t('Shoes'),
    'BrowseNode' => 524006031,
    'parameters_allowed' => array('Brand', 'BrowseNode', 'Condition', 'ItemPage', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MerchantId', 'MinimumPrice', 'Sort', 'State', 'TextStream', 'Title'),
    'sorts' => array(
      '-launch-date' => t('Newest arrivals'),
      'pmrank' => t('Featured items'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'xsrelevancerank' => t('Relevance'),
      'reviewrank' => t('Average customer review: high to low'),
    ),
  ),
  'Software' => array(
    'friendly_name' => t('Software'),
    'BrowseNode' => 412612031,
    'parameters_allowed' => array('Author', 'Availability', 'Brand', 'BrowseNode', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Keywords', 'Sort', 'Title'),
    'sorts' => array(
      'pmrank' => t('Featured items'),
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
    ),
  ),
  'Toys' => array(
    'friendly_name' => t('Toys'),
    'BrowseNode' => 523997031,
    'parameters_allowed' => array('BrowseNode', 'Availability', 'Keywords', 'Sort', 'Title'),
    'sorts' => array(
      'pmrank' => t('Featured items'),
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
      '-age-min' => t('Age: high to low'),
    ),
  ),
  'VideoGames' => array(
    'friendly_name' => t('Video Games'),
    'BrowseNode' => 412603031,
    'parameters_allowed' => array('Author', 'Availability','Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
    'sorts' => array(
      'pmrank' => t('Featured items'),
      'salesrank' => t('Bestselling'),
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'titlerank' => t('Alphabetical: A to Z'),
    ),
  ),
  'Watches' => array(
    'friendly_name' => t('Watches'),
    'BrowseNode' => 524009031,
    'parameters_allowed' => array('BrowseNode', 'Availability', 'Keywords', 'Sort'),
    'sorts' => array(
      'price' => t('Price: low to high'),
      '-price' => t('Price: high to low'),
      'relevancerank' => t('Relevance'),
      'reviewrank' => t('Average customer review: high to low'),
      'salesrank' => t('Bestselling to worst selling'),
    ),
  ),
  'ForeignBooks' => array(
    'friendly_name' => t('ForeignBooks'),
    'BrowseNode' => 433842031,
    'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Power', 'Publisher', 'Title'),
    'sorts' => array(
      'relevancerank' => t('Relevance'),
      'salesrank' => t('Bestselling'),
      'pricerank' => t('Price: low to high'),
      'inverseprice' => t('Price: high to low'),
      'launch-date' => t('Newest arrivals'),
      'sale-flag' => t('On sale'),
    ),
  ),
  ),
  );

  return $locale;
}
