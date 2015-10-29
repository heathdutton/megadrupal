<?php
/**
 * @file
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Provide views data and handlers for imagepicker.module
 */

/**
 * Implements hook_views_data()
 */
function imagepicker_views_data() {
  $data = array();

  // ----------------------------------------------------------------------
  // imagepicker table
  $data['imagepicker']['table']['group']  = t('Imagepicker');

  // Advertise this table as a possible base table
  $data['imagepicker']['table']['base'] = array(
    'field' => 'img_id',
    'title' => t('Imagepicker'),
    'help' => t('Imagepicker is a user based collection of images.'),
  );

  $data['imagepicker']['table']['join'] = array(
    // imagepicker links to imagepicker_user_groups via img_id in imagepicker_group_images.
    'imagepicker_group_images' => array(
      'left_field' => 'img_id',
      'field' => 'img_id',
    ),
  );
  // img_id
  $data['imagepicker']['img_id'] = array(
    'title' => t('Image ID'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The imagepicker ID.'),
      'handler' => 'views_handler_field_numeric',
    ),
  );
  // image URL
  $data['imagepicker']['img_url'] = array(
    'title' => t('Image URL'), // The item it appears as on the UI,
    'field' => array(
      'field' => 'img_id',
      'help' => t('The imagepicker image URL.'),
      'handler' => 'imagepicker_views_handler_field_image_url',
    ),
  );
  // uid
  $data['imagepicker']['uid'] = array(
    'title' => t('Owner uid'),
    'help' => t('The user owning the image. If you need more fields than the uid add the Imagepicker: Image owner relationship'),
    'relationship' => array(
      'title' => t('Image owner'),
      'help' => t('Relate image to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('image owner'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );
  // img_name
  $data['imagepicker']['img_name'] = array(
    'title' => t('Name'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The image name.'),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('The image name.'),
    ),
  );
  // img_title
  $data['imagepicker']['img_title'] = array(
    'title' => t('Title'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The image title.'),
      'handler' => 'views_handler_field',
    ),
  );
  // img_description
  $data['imagepicker']['img_description'] = array(
    'title' => t('Image description'), // The item it appears as on the UI,
    'help' => t('The image description'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );
  // img_date
  $data['imagepicker']['img_date'] = array(
    'title' => t('Image date'), // The item it appears as on the UI,
    'help' => t('The image date'),
    'field' => array(
      'handler' => 'views_handler_field_date', # should this be date?
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  if (module_exists('colorbox')) {
    $data['imagepicker']['img_trigger'] = array(
      'title' => t('Imagepicker colorbox trigger'), // The item it appears as on the UI,
      'help' => t('Provides a trigger for a gallery of Imagepicker images in a colorbox'),
      'field' => array(
        'handler' => 'imagepicker_colorbox_handler_field_colorbox',
      ),
    );
  }

  // ----------------------------------------------------------------------
  // imagepicker_group_images table
  $data['imagepicker_group_images']['table']['group']  = t('Imagepicker');

  $data['imagepicker_group_images']['table']['join'] = array(
    // imagepicker_user_groups links to imagepicker via gid in imagepicker_group_images.
    'imagepicker' => array(
      'left_field' => 'img_id',
      'field' => 'img_id',
    ),
    'imagepicker_user_groups' => array(
      'left_field' => 'gid',
      'field' => 'gid',
    ),
  );
  // img_id
  $data['imagepicker_group_images']['img_id'] = array(
    'title' => t('Image ID'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The imagepicker ID in linktable.'),
      'handler' => 'views_handler_field_numeric',
    ),
  );
  // gid
  $data['imagepicker_group_images']['gid'] = array(
    'title' => t('Group ID'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The imagepicker group ID in linktable.'),
      'handler' => 'views_handler_field_numeric',
    ),
    'relationship' => array(
      'title' => t('Group'),
      'help' => t('Relate group to the image.'),
      'handler' => 'views_handler_relationship',
      'base' => 'imagepicker_user_groups',
      'base field' => 'gid',
      'field' => 'gid',
      'label' => t('group id'),
    ),
  );

  // ----------------------------------------------------------------------
  // imagepicker_user_groups
  $data['imagepicker_user_groups']['table']['group']  = t('Imagepicker');

  $data['imagepicker_user_groups']['table']['join'] = array(
    // imagepicker_user_groups links to imagepicker via gid in imagepicker_group_images.
    'imagepicker_group_images' => array(
      'left_field' => 'gid',
      'field' => 'gid',
    ),
  );
  // gid
  $data['imagepicker_user_groups']['gid'] = array(
    'title' => t('Group ID'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The imagepicker group ID in imagepicker_user_groups.'),
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'imagepicker_views_handler_filter_group_id',
      'help' => t('The imagepicker group ID in imagepicker_user_groups.'),
    ),
  );
  // uid
  $data['imagepicker_user_groups']['uid'] = array(
    'title' => t('Owner uid'),
    'help' => t('The user owning the group. If you need more fields than the uid add the Imagepicker: Group owner relationship'),
    'relationship' => array(
      'title' => t('Group owner'),
      'help' => t('Relate group to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('group owner'),
    ),
    'field' => array(
      'handler' => 'views_handler_field',  # might need a handler
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  // group_name
  $data['imagepicker_user_groups']['group_name'] = array(
    'title' => t('Group name'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The group name.'),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'imagepicker_views_handler_filter_group_name',
      'help' => t('The imagepicker group Name in imagepicker_user_groups.'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('The group name.'),
    ),
  );
  // group_description
  $data['imagepicker_user_groups']['group_description'] = array(
    'title' => t('Group description'), // The item it appears as on the UI,
    'help' => t('The group description'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );
  // state
  # left out for now

  // public
  if (imagepicker_variable_get('imagepicker_public_enabled', 0)) {
    $data['imagepicker_user_groups']['public'] = array(
      'title' => t('Public'),
      'help' => t('Whether or not the group is public.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'output formats' => array(
          'public-notpublic' => array(t('Public'), t('Not public')),
        ),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Public'),
        'type' => 'yes-no',
        'use equal' => TRUE, // Use public = 1 instead of public <> 0 in WHERE statement
      ),
    );
  }
  // avail_roles
  if (imagepicker_variable_get('imagepicker_publicroles_enabled', 0)) {
    $data['imagepicker_user_groups']['avail_roles'] = array(
      'title' => t('Available roles'),
       'help' => t('Roles that are allowed access.'),
      'field' => array(
        'help' => t('Available roles that are allowed access.'),
        'handler' => 'imagepicker_views_handler_field_avail_roles',
      ),
      'filter' => array(
        'handler' => 'imagepicker_views_handler_filter_avail_roles',
        'help' => t('The available roles in imagepicker_user_groups. These are filtered against the current user.'),
      ),
    );
  }

  return $data;
}

