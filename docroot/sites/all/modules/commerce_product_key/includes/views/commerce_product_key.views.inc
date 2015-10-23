<?php

/**
 * @file
 * Export Drupal Commerce product keys to Views.
 */

/**
 * Implements hook_views_data().
 */
function commerce_product_key_views_data() {
  $data = array();

  $data['commerce_product_key']['table']['group']  = t('Commerce Product Key');

  $data['commerce_product_key']['table']['base'] = array(
    'field' => 'product_key_id',
    'title' => t('Commerce Product Key'),
    'help' => t('Product keys from the store.'),
  );

  // Expose the product key ID.
  $data['commerce_product_key']['product_key_id'] = array(
    'title' => t('Product Key ID'),
    'help' => t('The unique internal identifier of the product key.'),
    'field' => array(
      'handler' => 'commerce_product_key_handler_field_product_key',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_product_key_handler_argument_product_key_id',
    ),
  );

  // Expose the product code.
  $data['commerce_product_key']['code'] = array(
    'title' => t('Code'),
    'help' => t('The unique human-readable identifier of the product key.'),
    'field' => array(
      'handler' => 'commerce_product_key_handler_field_product_key',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the product sequence.
  $data['commerce_product_key']['type'] = array(
    'title' => t('Sequence'),
    'help' => t('The human-readable name of the sequence of the product key.'),
    'field' => array(
      'handler' => 'commerce_product_key_handler_field_product_key_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_product_key_handler_filter_product_key_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the owner uid.
  $data['commerce_product_key']['key_owner_id'] = array(
    'title' => t('Key Owner'),
    'help' => t("The owner's user ID."),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // display this field in the summary
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Owner'),
      'help' => t("Relate this order to its owner's user account"),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Order owner'),
    ),
  );

  // Expose the order ID.
  $data['commerce_product_key']['order_id'] = array(
    'title' => t('Order Number', array(), array('context' => 'a drupal commerce order')), //CHEAT MIGHT BITE
    'help' => t('The unique internal identifier of the order.'),
    'field' => array(
      'handler' => 'commerce_order_handler_field_order',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_order_handler_argument_order_order_id',
      'name field' => 'order_number',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
  );

  // Expose the creator uid.
  $data['commerce_product_key']['uid'] = array(
    'title' => t('Creator'),
    'help' => t('Relate a product key to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Product creator'),
    ),
  );

  //Expose the product key activated status.
  $data['commerce_product_key']['activated'] = array(
    'title' => t('Activated'),
    'help' => t('Whether or not the product key is activated.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Unactivated')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the product key revoked status.
  $data['commerce_product_key']['revoked'] = array(
    'title' => t('Revoked'),
    'help' => t('Whether or not the product key is revoked.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Revoked'), t('Active')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the product status.
  $data['commerce_product_key']['status'] = array(
    'title' => t('Visible to user'),
    'help' => t('Whether or not the product key is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'active-disabled' => array(t('Active'), t('Disabled')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Active'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the created and changed timestamps.
  $data['commerce_product_key']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the product key was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['commerce_product_key']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the product key was last updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Expose links to operate on the product keys.
  $data['commerce_product_key']['view_product'] = array(
    'field' => array(
      'title' => t('Product Key ID'),
      'help' => t('Provide a simple link to the administrator view of the product key.'),
      'handler' => 'commerce_product_key_handler_field_product_key',
    ),
  );
  $data['commerce_product_key']['edit_product'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the product key.'),
      'handler' => 'commerce_product_key_handler_field_product_key_link_edit',
    ),
  );
  $data['commerce_product_key']['delete_product'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the product key.'),
      'handler' => 'commerce_product_key_handler_field_product_key_link_delete',
    ),
  );

  $data['commerce_product_key']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for the product key.'),
      'handler' => 'commerce_product_key_handler_field_product_key_operations',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */


function commerce_product_key_views_plugins() {
  return array(
    'argument validator' => array(
      'current_user_or_role' => array(
        'title' => t('Current user or role'),
        'handler' => 'commerce_product_key_plugin_argument_validate_user',
      ),
    ),
  );
}

