<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombEndpoint extends GiantBombBaseEndpoint {
  /**
   * @param $object
   *   Giant Bomb object type.
   * @see GiantBombBaseEndpoint::object_properties()
   */
  function object_properties($object) {
    $localized = array(
      '@endpoint' => 'Giant Bomb',
      '@endpoint_name' => 'giantbomb',
      '@object' => $object,
    );

    $standard_properties = array(
      // For all types (internal: required)
      // Context variables
      GIANTBOMB_FIELD_ENDPOINT => array(
        'title' => t('Endpoint'),
        'description' => t('Endpoint machine name (@endpoint_name).', $localized),
        'data type' => 'magic',
        'required' => TRUE,
      ),
      GIANTBOMB_FIELD_TYPE => array(
        'title' => t('Type'),
        'description' => t('Object type (@object).', $localized),
        'data type' => 'magic',
        'required' => TRUE,
      ),
      GIANTBOMB_FIELD_ID => array(
        'title' => t('Object ID'),
        'description' => t('Object ID.'),
        'data type' => 'magic',
        'required' => TRUE,
      ),
  
      // Environment variables
      GIANTBOMB_FIELD_NEXT_UPDATE => array(
        'title' => t('Next Update'),
        'description' => t('Use date field only.'),
        'data type' => 'magic',
        'required' => TRUE,
      ),

      // API variables
      // Giant Bomb specific
      'api_detail_url' => array(
        'title' => t('API detail URL'),
        'description' => t('@endpoint API URL.', $localized),
        'data type' => 'url',
      ),
      'date_added' => array(
        'title' => t('Date added'),
        'description' => t('Date the @object was added.', $localized),
        'data type' => 'date',
      ),
      'date_last_updated' => array(
        'title' => t('Date last updated'),
        'description' => t('Date the @object last updated.', $localized),
        'data type' => 'date',
      ),
      'deck' => array(
        'title' => t('Short description'),
        'description' => t('A short summary of this @object.', $localized),
        'data type' => 'text',
      ),
      'description' => array(
        'title' => t('Description'),
        'description' => t('Full body text of variable length.', $localized),
        'data type' => 'html',
      ),
      'id' => array(
        'title' => t('Object ID'),
        'description' => t('@endpoint ID of @object.', $localized),
        'data type' => 'int',
      ),
      'name' => array(
        'title' => t('Title'),
        'description' => t('Title of the @object.', $localized),
        'data type' => 'text',
      ),
      'site_detail_url' => array(
        'title' => t('Original URL'),
        'description' => t('@endpoint URL.', $localized),
        'data type' => 'url',
      ),
    );

    if (!in_array($object, array('review', 'search', 'theme', 'user_review', 'video_type'))) {
      $standard_properties['image'] = array(
        'title' => t('Image FID'),
        'description' => t('One image. Logo, poster, cover art, box shot etc.', $localized),
        'data type' => 'image',
      );
    }

    if (in_array($object, array('game'))) {
      $standard_properties['images'] = array(
        'title' => t('Images'),
        'description' => t('Contains a variable number of images. Screenshots, photos etc.', $localized),
        'data type' => 'images',
      );
    }

    if (in_array($object, array('game'))) {
      $standard_properties['original_release_date'] = array(
        'title' => t('Original release date'),
        'description' => t('Date the @object was first released.', $localized),
        'data type' => 'date',
      );
    }

    if (in_array($object, array('game'))) {
      $standard_properties['number_of_user_reviews'] = array(
        'title' => t('Number of user reviews'),
        'description' => t('Number of user reviews of the @object on @endpoint.', $localized),
        'data type' => 'int',
      );
    }

    if (in_array($object, array('review', 'user_review'))) {
      $standard_properties['score'] = array(
        'title' => t('Score'),
        'description' => t('Score, from 1-5.', $localized),
        'data type' => 'int',
      );
    }

    if (in_array($object, array('review', 'user_review'))) {
      $standard_properties['reviewer'] = array(
        'title' => t('Reviewer'),
        'description' => t('Name of user who created the review.', $localized),
        'data type' => 'text',
      );
    }

    if (in_array($object, array('review'))) {
      $standard_properties['dlc_name'] = array(
        'title' => t('DLC Name'),
        'description' => t('Name of the DLC.', $localized),
        'data type' => 'text',
      );
    }

    if (in_array($object, array('review', 'video'))) {
      $standard_properties['publish_date'] = array(
        'title' => t('Published date'),
        'description' => t('Date the @object was published on @endpoint.', $localized),
        'data type' => 'date',
      );
    }

    return $standard_properties;
  }

  /**
   * (non-PHPdoc)
   * @see GiantBombBaseEndpoint::property_is_list()
   */
  function property_is_list($type, $property) {
    // Special list prefixes.
    $special_lists = array(
      'first_appearance_TYPE' => NULL,
      'publishers' => 'companies',
      'developers' => 'companies',
      'similar_games' => 'games',
      'killed_characters' => 'characters',
    );

    foreach ($special_lists as $list => $destination_list) {
      // Is this property in special lists.
      if (drupal_substr($property, 0, drupal_strlen($list)) == $list) {
        if ($destination_list) {
          return $destination_list;
        }

        // Is the rest of property a name of known list.
        $suffix = drupal_substr($property, drupal_strlen($list));
        if (in_array($suffix, $lists)) {
          return $suffix;
        }
      }
    }

    return FALSE;
  }
} 

/**
 *
 */
function _giantbomb_list_definition_giantbomb($type) {
  if ($type == 'game') {
    $standard_special_lists = array(
      'first_appearance_TYPE' => NULL,// turn into regex?
      'publishers' => 'companies',
      'developers' => 'companies',
      'similar_games' => 'games',
      'killed_characters' => 'characters',
    );
    return $standard_special_lists;
  }
}
