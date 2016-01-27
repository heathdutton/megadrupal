<?php

/**
 * URL's of Reference Material
 * Browse Node IDs
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/BrowseNodeIDs.html
 * SearchIndex-ItemSearch Parameter Combinations for ES
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/ESSearchIndexParamForItemsearch.html
 * Sort Values for ES
 * http://docs.amazonwebservices.com/AWSECommerceService/latest/DG/ESSortValuesArticle.html
 *
 * No parameters_allowed information available for Photo used US Info
 * No sort information available for OfficeProducts used US Info
 */

function _amazon_store_locale() {
  $locale = array('ES' => array(
      'All' => array(
        'friendly_name' => t('All'),
        'BrowseNode' => 2000,
        'parameters_allowed' => array('Keywords'),
        'sorts' => NULL,
      ),

      'Books' => array(
        'friendly_name' => t('Books'),
        'BrowseNode' => 599364031,
        'parameters_allowed' => array('Author', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Power', 'Publisher', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-pubdate' => 'Publication date: oldest to most recent',
          '-publicationdate' => 'Publication date: oldest to most recent',
        ),
      ),

      'DVD' => array(
        'friendly_name' => t('DVD'),
        'BrowseNode' => 599379031,
        'parameters_allowed' => array('Actor', 'AudienceRating', 'Availability', 'BrowseNode', 'Director', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Publisher', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-releasedate' => 'Release date: newer to older',
        ),
      ),

      'Electronics' => array(
        'friendly_name' => t('Electronics'),
        'BrowseNode' => 667049031,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
        ),
      ),

      'ForeignBooks' => array(
        'friendly_name' => t('Foreign Books'),
        'BrowseNode' => 599367031,
        'parameters_allowed' => array('Availability', 'Author', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'Power', 'Publisher', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-pubdate' => 'Publication date: oldest to most recent',
          '-publicationdate' => 'Publication date: oldest to most recent',
        ),
      ),

      'Kitchen' => array(
        'friendly_name' => t('Kitchen'),
        'BrowseNode' => 599391031,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
        ),
      ),

      'Music' => array(
        'friendly_name' => t('Music'),
        'BrowseNode' => 599373031,
        'parameters_allowed' => array('Artist', 'Availability', 'BrowseNode', 'Keywords', 'MaximumPrice', 'MinimumPrice', 'MusicLabel', 'ReleaseDate', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-releasedate' => 'Release date: newer to older',
        ),
      ),

      'Software' => array(
        'friendly_name' => t('Software'),
        'BrowseNode' => 599376031,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-releasedate' => 'Release date: newer to older',
        ),
      ),

      'Toys' => array(
        'friendly_name' => t('Toys'),
        'BrowseNode' => 599385031,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
        ),
      ),

      'VideoGames' => array(
        'friendly_name' => t('Video Games'),
        'BrowseNode' => 599382031,
        'parameters_allowed' => array('Availability', 'Author', 'Brand', 'BrowseNode', 'Keywords', 'Manufacturer', 'MaximumPrice', 'MinimumPrice', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
          '-releasedate' => 'Release date: newer to older',
        ),
      ),

      'Watches' => array(
        'friendly_name' => t('Watches'),
        'BrowseNode' => 599388031,
        'parameters_allowed' => array('Availability', 'BrowseNode', 'Keywords', 'Sort', 'Title'),
        'sorts' => array(
          'relevancerank' => 'Relevance',
          'salesrank' => 'Bestselling',
          'price' => 'Price: low to high',
          '-price' => 'Price: high to low',
          'reviewrank' => 'Average customer review: high to low',
          'reviewrank_authority	' => 'Review rank: high to low',
        ),
      ),
    ),
  );
  return $locale;
}
