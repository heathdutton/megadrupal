<?php

// Main page entry points
/**
 * Primary Amazon Store page, with search form, results, and SearchBins if they are available
 *
 * This page will respond to the various GET parameters and display the resultant search results
 * SearchIndex
 * Keywords
 * MinimumPrice
 * MaximumPrice
 * BrowseNode
 * MerchantId
 * Brand
 * Sort
 * author, composer, manufacturer, artist (not really well implemented at this point)
 *
 * If no keywords are supplied, it goes to the top-level browsenode of the selected SearchIndex
 */
function _amazon_store_page() {

  $output = "";
  $parameters = array();
  $do_search = FALSE;

  // Grab the URL args that we'll allow and put them in as parameters for Amazon search
  extract($_GET, EXTR_PREFIX_ALL, "get");
  if (!empty($get_SearchIndex)) {
    $parameters['SearchIndex'] = check_plain($get_SearchIndex);
  }
  if (!empty($get_Keywords)) {
    $parameters['Keywords'] = preg_replace('/[\'\"]/', '', $get_Keywords);
  }
  if (!empty($get_ItemId)) {
    $parameters['ItemId'] = filter_xss($get_ItemId);
  }
  if (!empty($get_BrowseNode)) {
    $parameters['BrowseNode'] = intval($get_BrowseNode);
  }
  if (!empty($get_MinimumPrice)) {
    $parameters['MinimumPrice'] = intval($get_MinimumPrice);
  }
  if (!empty($get_MaximumPrice)) {
    $parameters['MaximumPrice'] = intval($get_MaximumPrice);
  }
  if (!empty($get_Brand)) {
    $parameters['Brand'] = filter_xss($get_Brand);
  }
  if (!empty($get_Sort)) {
    $parameters['Sort'] = check_plain($get_Sort);
  }
  if (!empty($get_page)) {
    // Drupal pager is 0-based. Amazon is 1-based. Adjust for that.
    $parameters['ItemPage'] = intval($get_page) + 1;
  }
  else {
    $get_page = 1;
  }

  if (!empty($parameters['SearchIndex']) && $parameters['SearchIndex'] == 'All' && !empty($parameters['BrowseNode'])) {
    $parameters['SearchIndex'] = variable_get('amazon_store_default_search_index', "Books");
  }

  foreach (array('Author', 'Composer', 'Artist', 'Manufacturer') as $entity) {
    $getarg = "get_$entity";
    if (!empty($$getarg) && amazon_store_allowed_parameter($parameters['SearchIndex'], $entity)) {
      $parameters[$entity] = $$getarg;
      $do_search = TRUE;
    }
  }

  // If we have no keywords, browse the SearchIndex.
  if (empty($parameters['Keywords']) && empty($parameters['BrowseNode']) && empty($parameters['ItemId'])) {
    $browseNodes = $GLOBALS['amazon_store_search_indexes']->getBrowseNodes();
    if (empty($parameters['SearchIndex']) || $parameters['SearchIndex'] == 'All') {
      $parameters['SearchIndex'] = variable_get('amazon_store_default_search_index', "Books");
    }
    // If they have configured a browsenode, use that
    $defaultlist = variable_get('amazon_store_default_items', 'searchindex');
    switch ($defaultlist) {
      case 'searchindex': // Use the default browsenode for searchindex
        if (!empty($browseNodes[$parameters['SearchIndex']])) {
          $parameters['BrowseNode'] = $browseNodes[$parameters['SearchIndex']];
        }
        break;
      case 'browsenode':
        $parameters['BrowseNode'] = variable_get('amazon_store_default_browsenode_id', 0);
        break;
      case 'itemlist':
        $parameters['ItemId'] = variable_get('amazon_store_default_item_list', "");
        break;
    }
  }

  if (!empty($parameters['ItemId']) && $do_search == FALSE) {
    $results = _amazon_store_itemlist(array('ItemId' => $parameters['ItemId']));
  }
  else {
    $results = _amazon_store_search($parameters );
  }

  $output .= theme('amazon_store_search_results', array('results' => $results));
  return $output;
}

/**
 * Empty the cart
 *
 */
function amazon_store_clear_cart($form, &$form_state, $destination = "amazon_store") {
  if (!$destination) {
    $destination = AMAZON_STORE_PATH;
  }
  $form = array();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Clear cart")
  );
  $form_state['redirect'] = $destination;
  return $form;
}

function amazon_store_clear_cart_submit($form, &$form_state) {
  if ($cart_creds = amazon_store_get_cart_creds()) {
    $results = amazon_store_http_request('CartClear', $cart_creds);
    if (!empty($results->error)) {
      amazon_store_report_error('Failed to clear cart.', $results->Cart->Request->Errors);
      drupal_set_message(t('Failed to clear shopping cart.', 'error'));
      return;
    }
  }
  drupal_set_message(t('Your shopping cart is now empty.'));
}

/**
 * The cart display page
 *
 * @return rendered page contents
 */
function amazon_store_show_cart() {
  // Prevent caching. This one shouldn't be cached.
  $GLOBALS['conf']['cache'] = FALSE;

  if ($cart_creds = amazon_store_get_cart_creds()) {
    $results = amazon_store_http_request('CartGet', $cart_creds);
    // Check results
    if (!empty($results->error) || $results->Cart->Request->IsValid != 'True') {
      amazon_store_report_error('Failed to get cart', $results->Cart->Request->Errors);
      drupal_set_message(t('Failed to get cart.'), 'error');
    }
  }
  // Display the cart
  if (!empty($results->Cart)) {
    $result = theme('amazon_store_cart', array('cart' => $results->Cart));
  }
  else {
    $result = t("There is nothing in your cart.");
  }
  return "<div>$result</div>";
}

function template_preprocess_amazon_store_cart(&$variables) {
  $cart = & $variables['cart'];
  $variables['fullrecords'] = array();
  $fullrecords = & $variables['fullrecords'];
  // Add the full info on the item and the availability of this offer to the
  // cart info
  if (!empty($cart->CartItems)) {
    foreach ($cart->CartItems->CartItem as $item) {
      $fullinfo = amazon_store_retrieve_item($item->ASIN);
      $fullrecords[] = $fullinfo;
      $merchantId = (string) $fullinfo->Offers->Offer[0]->Merchant->Name;
      $availability = (string) $fullinfo->Offers->Offer[0]->OfferListing->Availability;
      $item->addChild("Availability",  $availability);
    }
  }
}

function amazon_store_item_detail($asin) {
  $item = amazon_store_retrieve_item($asin);
  if (empty($item)) {
    $output = t('Sorry, this is either an invalid URL or this item is no longer available from Amazon');
  }
  else {
    $output = theme('amazon_store_item_detail', array('amazon_item' => $item));
  }
  return $output;
}

/**
 * Process variables for amazon_store_item_detail.
 */
function amazon_store_preprocess_amazon_store_item_detail(&$variables) {
  $amazon_item = $variables['amazon_item'];
  $variables['asin'] = (string)$amazon_item->ASIN;
  if (!empty($amazon_item->EditorialReviews->EditorialReview[0]->Content)) {
    $variables['editorialreview'] =  _filter_htmlcorrector(filter_xss_admin($amazon_item->EditorialReviews->EditorialReview[0]->Content));
  }
}

/**
 * Form to "add to cart". Normally embedded in page either of search results
 * or on item display page
 *
 * Most form elements and the form itself are identified with the ASIN
 * attached to their name. This is a result of Drupal's failure to handle
 * multiple forms on a page, especially with AJAX.  @TODO: This should be
 * fixed now!
 *
 * The various parts of the form are all driven by AJAX, so no page refresh
 * is required to add to cart or to change the size/color or whatever permutations
 *
 * @param $form
 * @param  $form_state
 * @param  $asin
 *   The ASIN identifying the item we're working with
 * @param  $context  string
 *   Pretty hacky. This allows a hook_form_alter to behave differently based
 *   on how the form was constructed. All it does here is add the "context"
 *   string as a hidden element in the form so it can be used later.
 * @return prepared form
 */
function amazon_store_addcart_form($form, &$form_state, $asin, $context = 'normal') {
  $moduledir = drupal_get_path('module', 'amazon_store');
  $form['#attached']['css'] = array("$moduledir/amazon_store.css");
  $form['#attached']['js'] = array("$moduledir/amazon_store.js");

  // We either have asin passed in, or get it from values array
  if (empty($asin)) {
    $asin = $form_state['values']['fields']['asin'];
  }

  if (AS_DEBUG) {
    $load_path = $asin . ": ";
  }

  $item = amazon_store_retrieve_item($asin);
  if (empty($item)) {
    return;
  }
  $itemsByAttribute =  _amazon_store_process_variations($item);
  $nextSelect = & $itemsByAttribute;

  // It may be that we have no offers, probably due to Amazon *either*
  // returning things from Amazon OR from other sellers.
  if (empty($nextSelect)) {
    $form['not_available'] = array(
      '#type' => 'item',
      '#markup' => t('This item is not available from the configured sellers.'),
    );
    return $form;
  }
  $dimensions = _amazon_store_get_variation_dimensions($item);
  $formwrapper = "addcart-form-$asin";

  $form['#prefix'] = "<div id='$formwrapper' class='form-wrapper'>";
  $form['#suffix'] = '</div>';
  $form['#tree'] = TRUE;

  // These are required because the form_id is no longer just the plain one, but
  // has ASIN attached to it (many forms driven by one handler)
  $form['#validate'][] = 'amazon_store_addcart_form_validate';
  $form['#submit'][] = 'amazon_store_addcart_form_submit';
  $form['item_title'] = array(
      '#type' => 'value',
      '#value' => (string) $item->ItemAttributes->Title,
  );
  $fieldswrapper = "fields-$asin-wrapper";

  $form['fields'] = array(
      '#prefix' => "<div id='$fieldswrapper' class='fields-wrapper'>",
      '#suffix' => "</div>",
      '#type' => 'fieldset',
  );

  $form['fields']['asin'] = array(
      '#type' => 'value',
      '#value' => $asin,
  );

  // Hidden; used to inform hook_form_alter about how form was constructed.
  $form['fields']['context'] = array(
      '#type' => 'value',
      '#value' => $context,
  );

  if (!empty($item->Variations)) {

    // for each dimension, create a widget
    $nextSelect = & $itemsByAttribute;
    foreach ($dimensions as $dimension) {

      $form['fields'][$dimension . "-$asin"] = array(
        '#title' => $dimension,
        '#type' => 'select',
        '#ajax' => array(
          'callback' => '_amazon_store_addcart_callback',
          'wrapper' => "$formwrapper"
        ),
      );
      if (AS_DEBUG) {
        $form['fields'][$dimension]['#title'] .= " $asin";
      }

      // Generate a select with the legal values for this dimension, given the previous one
      if (!empty($nextSelect)) {
        // $selects = array("Select {$dimension}...");
        $selects = array();
        $possible_keys = array_keys($nextSelect);
        if (AS_DEBUG) {
          $possible_keys[] = "WIDENED $dimension " . date("His");
        }
        // Process the selects here with a debug addition to the value
        $selects += array_combine($possible_keys, $possible_keys);
        $curValue = !empty($form_state['values']['fields'][$dimension . "-$asin"]) ?
          $form_state['values']['fields'][$dimension . "-$asin"] : NULL;
        // If our current value is not in our array of possible values,
        // replace it with the first value of the possible
        if (empty($nextSelect[$curValue])) {
          $curValue = key($nextSelect);
        }
        $form['fields'][$dimension . "-$asin"]['#default_value'] = $curValue;
        if (AS_DEBUG) {
          $load_path .= $curValue . ": ";
        }

        $nextSelect = & $nextSelect[$curValue];
      }
      else { // NextSelect was empty
        $selects = array(t('Make other selections first'));
      }
      $form['fields'][$dimension . "-$asin"]['#options'] = $selects;
    }
  }

  // Now for the offers pulldown
  $selects = array();
  if (!empty($nextSelect)) {
    $key = key($nextSelect);
    $possible_keys = array_keys($nextSelect[$key]);
    foreach ($possible_keys as $offer) {
      $selects[$offer] = "{$nextSelect[$key][$offer]['merchantname']} - {$nextSelect[$key][$offer]['price']}";
    }
    if (AS_DEBUG) {
      $selects += array("WIDENED $dimension " . date("His"));
    }

  }
  else { // No offers yet due to missing selections
    $selects = array(t('Make other selections first'));
  }
  $form['fields']['offer' . "-$asin"] = array(
    '#title' => t('Merchant'),
    '#type' => 'select',
    '#options' => $selects,
    '#ajax' => array(
      'callback' => '_amazon_store_addcart_callback',
      'wrapper' => "$formwrapper"
    ),
  );
  $curValue = empty($form_state['values']['fields']['offer' . "-$asin"]) ? NULL : $form_state['values']['fields']['offer' . "-$asin"];

  // If the previously-set value is not in selects, we'll abandon it
  if (empty($selects[$curValue])) {
    $curValue = key($selects); // Just set to the first entry
  }
  if (AS_DEBUG) {
    $num_offers = count($nextSelect[$key]);
    $load_path .= $curValue . ": sub-asin={$key} ($num_offers offers), merchant={$nextSelect[$key][$curValue]['merchantname']}";
  }

  $form['fields']['offer' . "-$asin"]['#default_value'] = $curValue;
  $form['submits']['#type'] = 'fieldset';
  $form['submits']['offer-info' . "-$asin"] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="offer-info-div">',
    '#suffix' => '</div>',
    '#markup' => "<span class='offer-status'><span class='sold-by'>" . t('Sold by') . " <span class='merchantname'>{$nextSelect[$key][$curValue]['merchantname']}</span></span>:
  <span class='availability'>{$nextSelect[$key][$curValue]['availability']}</span></span>",
  );

  $form['submits']['submit' . "-$asin"] = array(
    '#type' => 'submit',
    '#value' => t('Add to cart'),
    '#ajax' => array(
      'callback' => '_amazon_store_addcart_callback',
      'wrapper' => "$formwrapper"
    ),
  );


  if (strlen(key($form['fields']['offer' . "-$asin"]['#options'])) > 1) {
    $form['submits']['submit' . "-$asin"]['#value'] = t('Add to cart');
  }

  if ($item->ItemAttributes->Binding == 'Kindle Edition') {
    $form['submits']['submit' . "-$asin"]['#disabled'] = TRUE;
    $form['submits']['submit' . "-$asin"]['#value'] = t('Kindle Edition: Not available to purchase');
  }

  $form['info-update'] = array(
      '#value' => "<div id='info-$asin'></div>",
      '#suffix' => AS_DEBUG ? "load_path=$load_path" : "",
  );

  return $form;
}

/**
 * Validate the form by making sure that each widget has values that
 * relate to the parent widget. For example, the color green may be offered
 * in size M, but not in size S. So we have to make sure that the colors that
 * actually are in size M are in the widget.  Rebuild the form if necessary
 * by exiting with $form_state['rebuld'] = true.
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function amazon_store_addcart_form_validate($form, &$form_state) {
  $asin = $form_state['values']['fields']['asin'];
  $item = amazon_store_retrieve_item($asin);
  if (empty($item)) {
    form_error($form['asin'], t('Sorry, An error occurred accessing this item. It may no longer be available from Amazon.'));
  }
  $itemsByAttribute =  _amazon_store_process_variations($item);
  $dimensions = _amazon_store_get_variation_dimensions($item);
  $nextSelect = $itemsByAttribute;

  foreach ($dimensions as $dimension) {
    $thisValue = $form_state['values']['fields'][$dimension . "-$asin"];
    $possibleSelections = array_keys($nextSelect);
    if (!empty($thisValue) && !empty($nextSelect[$thisValue])) {
      $nextSelect = & $nextSelect[$thisValue];
      continue; // We found a valid entry for this dimension
    }
    else { // There is no path forward from here, so the selections have changed and we'll rebuild
      $form_state['rebuild'] = TRUE;
      drupal_set_message(t('Please make a selection for %dimension', array('%dimension' => $dimension)), 'error');
      return;
    }
  }
  if (empty($form_state['values']['fields']['offer' . "-$asin"])) { // We don't have an offer selected yet
    form_set_error("fields[offer-$asin]", ('Please choose a merchant'));
    $form_state['rebuild'] = TRUE;
    return;
  }
  else { // We do have an offer, but need to check it for validity
    $thisValue = $form_state['values']['fields']['offer' . "-$asin"];
    if (!empty($nextSelect[key($nextSelect)][$thisValue])) {
      // The offer is good. validation complete.
      $form_state['rebuild'] = FALSE;
      return;
    }
  }
  // All other cases we have to rebuild the form and get a valid combination
  $form_state['rebuild'] = TRUE;
  return;
}

/**
 * Submit function - do the add-to-cart
 *
 * @param unknown_type $form
 * @param unknown_type $form_state
 */
function amazon_store_addcart_form_submit($form, &$form_state) {
  $asin = $form_state['values']['fields']['asin'];
  $offer = $form_state['values']['fields']["offer-$asin"];

  $result = _amazon_store_add_to_cart(NULL, $offer);
  if ($result) {
    $directory = drupal_get_path('module', 'amazon_store');
    drupal_set_message(t('%item was added to your cart. <br/><em>!link</em>', array('%item' => $form['item_title']['#value'], '!link' => l(t("View Cart"), AMAZON_STORE_PATH . '/cart'))));
  }
  else {
    drupal_set_message(t('Error: Item not added to cart.'), 'error');
  }
  $params = _amazon_store_revise_query_parameters();
  $form_state['redirect'] = array('amazon_store', array('query' => $params));
}

/**
 * Small form used to set item quantity for the Shopping Cart page.
 * Each element and the form itself have their names keyed to the ASIN,
 * since Drupal can't handle multiple forms of the same type on a page,
 * especially with AJAX
 *
 * @param $form_state
 * @param $cart_entry
 * @param $item_number
 */
function _amazon_store_cart_quantity_form($form, &$form_state, $cart_entry, $item_number) {
  $moduledir = drupal_get_path('module', 'amazon_store');
  $form['#attached']['css'] = array("$moduledir/amazon_store.css");
  $form['#attached']['js'] = array("$moduledir/amazon_store.js");


  $asin = (string) $cart_entry->ASIN;
  $formwrapper = "addcart-form-$asin";
  $form['#prefix'] = "<div id='$formwrapper' class='form-wrapper'>";
  $form['#suffix'] = '</div>';
  $form['#tree'] = TRUE;

  // These are required because the form_id is no longer just the plain one.
  $form['#submit'][] = '_amazon_store_cart_quantity_form_submit';
  $form['#validate'][] = '_amazon_store_cart_quantity_validate';
  $form['asin'] = array(
    '#type' => 'value',
    '#value' => $asin,
  );
  $form['cartitemid' . "-$asin"] = array(
    '#type' => 'value',
    '#value' => (string) $cart_entry->CartItemId,
  );

  // This item is the nth item in the cart.
  $form['item_number' . "-$asin"] = array(
    '#type' => 'value',
    '#value' => $item_number,
  );

  $form['quantity' . "-$asin"] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#default_value' => $cart_entry->Quantity,
    '#size' => 2,
  );

  $form['submit' . "-$asin"] = array(
    '#type' => 'submit',
    '#value' => t('Update Quantity'),
  );

  return $form;
}

function _amazon_store_cart_quantity_validate($form, &$form_state) {
  $asin = $form_state['values']['asin'];
  $quantity = $form_state['values']['quantity' . "-$asin"];
  if (!is_numeric($quantity) || $quantity < 0 ) {
    form_set_error('quantity' . "-$asin", "Quantity must be a number equal to or greater than zero");
  }
}

function _amazon_store_cart_quantity_form_submit($form, &$form_state) {
  $asin = $form_state['values']['asin'];
  amazon_store_update_cart_quantity($form_state['values']['cartitemid' . "-$asin"], $form_state['values']['quantity' . "-$asin"]);
  // And return to this same page
}

/**
 * Turn a link or path into a form button.
 * This is only intended to handle internal paths and simple links like
 * the one to the amazon checkout.
 * @param $form
 * @param $form_state
 * @param $name
 *   button name.
 * @param $link
 *   URL we'll link to.
 * @return form.
 */
function amazon_store_buttonize_link($form, &$form_state, $name, $link) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $name,
  );
  $form_state['storage']['linkbutton_link'] = $link;
  $form_state['redirect'] = $link;
  return $form;
}

function amazon_store_buttonize_link_submit($form, &$form_state) {
  $form_state['redirect'] = $form_state['storage']['linkbutton_link'];
}

/**
 * Theme a link into a button using the current theme's button definition.
 * @param $variables
 */
function theme_amazon_store_link_button($variables) {
  $hash = drupal_hash_base64($variables['url']);
  $form = drupal_get_form("amazon_store_buttonize_link_" . $hash, $variables['text'], $variables['url']);
  $markup = drupal_render($form);
  return $markup;
}

