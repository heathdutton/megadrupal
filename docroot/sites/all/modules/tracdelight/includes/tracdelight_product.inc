<?php
class TracdelightProduct extends Entity {

  public function defaultLabel() {
    return isset($this->ein) ? $this->title . '-' . $this->ein : NULL;
  }
}

/**
 * Access callback for Product.
 */
function tracdelight_product_access($op, $tracdelight_product, $account = NULL, $entity_type = NULL) {

  global $user;
  if (!isset($account)) {
    $account = $user;
  }
  switch ($op) {
    case 'create':
      return user_access('create tracdelight_product entities', $account);

    case 'view':
      return user_access('view tracdelight_product entities', $account);

    case 'edit':
    case 'update':
      return user_access('edit any tracdelight_product entities') || (user_access('edit own tracdelight_product entities') && ($tracdelight_product->uid == $account->uid));

    case 'delete':
      return user_access('delete any tracdelight_product entities') || (user_access('delete own tracdelight_product entities') && ($tracdelight_product->uid == $account->uid));
  }
}

function tracdelight_product_uri($tracdelight_product) {
  return array(
    'path' => 'tracdelight_product/' . $tracdelight_product->id,
  );
}

function tracdelight_product_load($ein = NULL, $reset = FALSE) {
  $eins = (isset($ein) ? array($ein) : array());
  $tracdelight_product = tracdelight_product_load_multiple($eins, $reset);
  return $tracdelight_product ? reset($tracdelight_product) : FALSE;
}

function tracdelight_product_load_multiple($eins = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('tracdelight_product', $eins, $conditions, $reset);
}

function tracdelight_product_save(&$tracdelight_product) {
  return entity_get_controller('tracdelight_product')->save($tracdelight_product);
}

function tracdelight_product_page_view($tracdelight_product, $view_mode = 'full') {

  // Our entity type, for convenience.
  $entity_type = 'tracdelight_product';
  // Start setting up the content.
  $tracdelight_product->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($tracdelight_product->id => $tracdelight_product),
    $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($tracdelight_product->id => $tracdelight_product));
  // Now field_attach_view() generates the content for the fields.
  $tracdelight_product->content += field_attach_view($entity_type, $tracdelight_product, $view_mode);

  // OK, Field API done, now we can set up some of our own data.


  $tracdelight_product->content['ein'] = array(
    '#type' => 'item',
    '#title' => t('EIN:'),
    '#markup' => $tracdelight_product->ein,
  );

  $tracdelight_product->content['sku'] = array(
    '#type' => 'item',
    '#title' => t('SKU:'),
    '#markup' => $tracdelight_product->sku,
  );

  $tracdelight_product->content['detailpageurl'] = array(
    '#type' => 'item',
    '#title' => t('Page URL:'),
    '#markup' => theme_link(array(
      'text' => $tracdelight_product->title,
      'path' => $tracdelight_product->detailpageurl,
      'options' => array(
        'attributes' => array(),
        'html' => array(),
      ),
    )),
  );

  $tracdelight_product->content['title'] = array(
    '#type' => 'item',
    '#title' => t('Title:'),
    '#markup' => $tracdelight_product->title,
  );

  $tracdelight_product->content['manufacturer'] = array(
    '#type' => 'item',
    '#title' => t('Manufacturer:'),
    '#markup' => $tracdelight_product->manufacturer,
  );

  $tracdelight_product->content['brand'] = array(
    '#type' => 'item',
    '#title' => t('Brand:'),
    '#markup' => $tracdelight_product->brand,
  );

  $tracdelight_product->content['description'] = array(
    '#type' => 'item',
    '#title' => t('Description:'),
    '#markup' => $tracdelight_product->description,
  );

  $tracdelight_product->content['shippingcosts'] = array(
    '#type' => 'item',
    '#title' => t('Shipping costs:'),
    '#markup' => $tracdelight_product->shippingcosts,
  );

  $tracdelight_product->content['deliverytime'] = array(
    '#type' => 'item',
    '#title' => t('Delivery time:'),
    '#markup' => $tracdelight_product->deliverytime,
  );

  $tracdelight_product->content['shop'] = array(
    '#type' => 'item',
    '#title' => t('shop:'),
    '#markup' => $tracdelight_product->shop,
  );

  $tracdelight_product->content['active'] = array(
    '#type' => 'item',
    '#title' => t('Active:'),
    '#markup' => $tracdelight_product->active,
  );

  $tracdelight_product->content['modified'] = array(
    '#type' => 'item',
    '#title' => t('Modified:'),
    '#markup' => $tracdelight_product->modified,
  );

  $tracdelight_product->content['formattedprice'] = array(
    '#type' => 'item',
    '#title' => t('Price:'),
    '#markup' => $tracdelight_product->formattedprice,
  );

  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language;
  $langcode = $language->language;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $tracdelight_product, $entity_type, $view_mode,
    $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('tracdelight_product_page_view', 'entity_view'),
    $tracdelight_product->content, $entity_type);

  // And finally return the content.
  return $tracdelight_product->content;

}

