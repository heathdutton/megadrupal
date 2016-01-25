<?php
class TracdelightProductUIController extends EntityDefaultUIController {
}

function tracdelight_product_add() {
  $tracdelight_product = (object) array(
    'ein' => '',
    'detailpageurl' => '',
    'type' => 'tracdelight_product',
  );

  return drupal_get_form('tracdelight_product_form', $tracdelight_product);
}

function tracdelight_product_add_form_submit($form, &$form_state) {

  $form_state['values']['is_new'] = isset($form_state['tracdelight_product']->is_new) ? $form_state['tracdelight_product']->is_new : FALSE;

  $tracdelight_product_submission = (object) $form_state['values'];
  field_attach_submit('tracdelight_product', $tracdelight_product_submission, $form, $form_state);
  $tracdelight_product = tracdelight_product_save($tracdelight_product_submission);

  $form_state['redirect'] = "tracdelight_product/$tracdelight_product->id";
}

function tracdelight_product_form($form, &$form_state, $tracdelight_product) {

  $form_state['tracdelight_product'] = $tracdelight_product;

  $form['ein'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('EIN'),
    '#default_value' => isset($tracdelight_product->ein) ? $tracdelight_product->ein : NULL,
    '#disabled' => !(isset($tracdelight_product->is_new) && $tracdelight_product->is_new),
  );

  $form['sku'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('SKU'),
    '#default_value' => isset($tracdelight_product->sku) ? $tracdelight_product->sku : NULL,
  );

  $form['detailpageurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Page URL'),
    '#default_value' => isset($tracdelight_product->detailpageurl) ? $tracdelight_product->detailpageurl : NULL,
    '#required' => TRUE,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($tracdelight_product->title) ? $tracdelight_product->title : NULL,
    '#required' => TRUE,
  );

  $form['manufacturer'] = array(
    '#type' => 'textfield',
    '#title' => t('Manufacturer'),
    '#default_value' => isset($tracdelight_product->manufacturer) ? $tracdelight_product->manufacturer : NULL,
  );

  $form['brand'] = array(
    '#type' => 'textfield',
    '#title' => t('Brand'),
    '#default_value' => isset($tracdelight_product->brand) ? $tracdelight_product->brand : NULL,
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($tracdelight_product->description) ? $tracdelight_product->description : NULL,
    '#required' => TRUE,
  );

  $form['shippingcosts'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping costs'),
    '#default_value' => isset($tracdelight_product->shippingcosts) ? $tracdelight_product->shippingcosts : NULL,
  );

  $form['deliverytime'] = array(
    '#type' => 'textfield',
    '#title' => t('Delivery time'),
    '#default_value' => isset($tracdelight_product->deliverytime) ? $tracdelight_product->deliverytime : NULL,
  );

  $form['formattedprice'] = array(
    '#type' => 'textfield',
    '#title' => t('Price'),
    '#default_value' => isset($tracdelight_product->formattedprice) ? $tracdelight_product->formattedprice : NULL,
    '#required' => TRUE,
  );

  $form['shop'] = array(
    '#type' => 'textfield',
    '#title' => t('Shopname'),
    '#default_value' => isset($tracdelight_product->shop) ? $tracdelight_product->shop : NULL,
  );

  $form['active'] = array(
    '#type' => 'radios',
    '#title' => t('Active'),
    '#default_value' => isset($tracdelight_product->active) ? $tracdelight_product->active : 1,
    '#options' => array(0 => t('Closed'), 1 => t('Active')),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('tracdelight_product_add_form_submit'),
    '#weight' => 100,
  );

  field_attach_form('tracdelight_product', $tracdelight_product, $form, $form_state);

  return $form;
}

function tracdelight_product_admin_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('Administration page for tracdelight products.'),
  );

  return $content;
}

/**
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function tracdelight_admin($form, &$form_state) {
  $form = array();

  $form['tracdelight_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('tracdelight AccessKey'),
    '#default_value' => variable_get('tracdelight_access_key', ''),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
  );

  $form['tracdelight_import_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Import number of items'),
    '#default_value' => variable_get('tracdelight_import_count', '10'),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

function tracdelight_admin_import() {
  $form = array();

  $form['tracdelight_query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query string'),
    '#size' => 32,
    '#maxlength' => 32,
  );
  $form['tracdelight_query_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import',
  );

  return $form;
}

function tracdelight_admin_import_validate($form, &$form_state) {
  $message = '';
  $query = $form_state['values']['tracdelight_query'];
  if (!$query) {
    $message = t('You need to specify a search query.');
  }
  else {
    if (strlen($query) < 2) {
      $message = t('Please make sure you enter a valid query.');
    }
  }

  if ($message) {
    form_set_error('tracdelight_query', $message);
  }
}

function tracdelight_admin_import_submit($form, &$form_state) {
  $query = $form_state['values']['tracdelight_query'];
  $items = variable_get('tracdelight_import_count', '10');

  try {
    tracdelight_import_products(array('Query' => $query), $items);

    drupal_set_message(t("Imported @items for query string '@query'.", array(
      '@items' => format_plural($items, '1 item', '@count items'),
      '@query' => $query,
    )));
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

function tracdelight_api_form($form, &$form_state) {


  $form['query_string'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Query'),
  );

  $form['tracdelight_query_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import',
  );


  return $form;
}


function tracdelight_api_form_submit($form, &$form_state) {
  $query = $form_state['values']['query_string'];

  $api_query = array();

  if (tracedelight_string_seems_to_be_ein($query)) {
    drupal_set_message(t('Query seems to be "<strong>@ein</strong>"' , array(
      '@ein' => $query,
    )));

    $api_query['EIN'] = $query;
  } else if ($ein = tracdelight_get_ein_from_uri($query)) {
    drupal_set_message(t('Found ein "<strong>@ein</strong>" in "<strong>@uri</strong>"' , array(
        '@ein' => $ein,
        '@uri' => $query,
    )));

    $api_query['EIN'] = $ein;
  } else {
    drupal_set_message(t('we just issue query "<strong>@query</strong>"' , array(
      '@query' => $query,
    )));

    $api_query['Query'] = $query;
  }

  drupal_set_message(t('api_query is <strong>@query</strong>' , array(
    '@query' => print_r($api_query, 1),
  )));

  $messages = array();
  $response = tracdelight_call_api($api_query, 20, $messages);

  foreach ($messages as $key => $message) {
    drupal_set_message(t('api <strong>@type</strong> is <strong>@message</strong>' , array(
      '@type' => $key,
      '@message' => $message,
    )));
  }
  drupal_set_message('response code <strong>' . $response->code . '</strong>');
  drupal_set_message('response is <xmp>' . print_r(drupal_json_decode($response->data), 1) . '</xmp>');

//  try {
//
//    drupal_set_message(t("Imported @items for query string '@query'.", array(
//          '@items' => format_plural($items, '1 item', '@count items'),
//          '@query' => $query,
//        )));
//  } catch (Exception $e) {
//    drupal_set_message($e->getMessage(), 'error');
//  }
}



