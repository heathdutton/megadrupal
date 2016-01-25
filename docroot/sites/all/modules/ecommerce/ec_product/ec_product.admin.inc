<?php

/**
 * @file
 * Administration screens for configuring e-Commerce products features,
 * attributes and types.
 */

/**
 * Elements common to every single product: price, SKU and
 * the option to show a "Add to cart" as a link or a button.
 */
function ec_product_form_base_elements(&$node) {
  $form['ptype'] = array(
    '#type' => 'value',
    '#value' => $node->ptype,
  );
  $name = ec_product_get_types('name', $node->ptype);
  $form['edit_ptype_description'] = array(
    '#type' => 'hidden',
    '#value' => $name ? $name : $node->ptype,
    '#attributes' => array(
      'id' => 'edit-ptype-description',
    ),
  );
  $name = ec_product_get_types('name', $node->ptype);
  $form['ptype_description'] = array(
    '#type' => 'hidden',
    '#value' => $name ? $name : $node->ptype,
  );
  $form['price'] = array(
    '#type' => 'price',
    '#title' => t('Price'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => !empty($node->price) ? format_currency($node->price) : '',
    '#description' => t('How much does this product retail for? Note: This price may be different from the selling price due to price adjustments elsewhere.'),
    '#weight' => -20,
    '#element_validate' => array('valid_price'),
  );
  $form['sku'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#size' => 25,
    '#maxlength' => 50,
    '#default_value' => !empty($node->sku) ? $node->sku : '',
    '#description' => t('If you have an unique identifier for this product from another system or database, enter that here. This is optional, as system IDs are automatically created for each product.'),
    '#weight' => -10,
  );

  return $form;
}

/**
 * Generates product type options fieldset.
 * Calling function (product_form_alter) has already checked that the node is a product.
 */
function _ec_product_alter_node_form(&$node) {
  // Product type collapsed fieldset
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Product'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => array('ec-product-details'),
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'product') . '/css/product.css'),
      'js' => array(drupal_get_path('module', 'ec_product') . '/js/ec_product.node.js'),
    ),
    '#group' => 'additional_settings',
  );
  // Choose a product type.
  $ptypes[0] = array(
    'name' => t('Not a product'),
    'help' => t('This item not for sale.'),
  );
  $types = variable_get('ec_product_ptypes_' . $node->type, NULL);
  if (empty($types)) {
    $types = array_keys(ec_product_get_types('names'));
  }

  // Populate product type properties.
  foreach ($types as $key) {
    $info = ec_product_get_types('type', $key);
    if (ec_product_ptypes_access($info->name)) {
      $ptypes[$key] = array(
        'name' => $info->name,
        'help' => $info->description,
      );
    }
  }
  $form['wrapper'] = array(
    '#prefix' => '<div id="ec-product">',
    '#suffix' => '</div>'
  );
  // Display each radio option, with descriptive text.
  foreach ($ptypes as $key => $data) {
    $form['wrapper']['ptype'][$key] = array(
      '#type' => 'radio',
      '#title' => $data['name'],
      '#return_value' => $key,
      '#description' => $data['help'],
      '#parents' => array('ptype'),
    );
  }
  $form['wrapper']['product_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add to store'),
    '#submit' => array('ec_product_convert_node'),
    '#ajax' => array(
      'callback' => '_ec_product_alter_node_form_callback',
      'wrapper' => 'ec-product',
    ),
  );
  return $form;
}

/**
 * Generates product form fields to be added into the node form.
 * Called from ec_product_form_alter().
 *
 * @param $form_id, $form_values, as passed to hook_form_alter.
 * @ingroup form
 */
function _ec_product_alter_product_form(&$form, &$node_form) {
  $product_type = ec_product_get_types('type', $node_form['#node']);
  // Get base form elements.
  $pform = array(
    '#type' => 'fieldset',
    '#title' => t('Product'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Product details') . ' (' . $product_type->name . ')',
    '#attributes' => array(
      'class' => array('ec-product-details'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'ec_product') . '/js/ec_product.node.js'),
    ), 
    '#group' => 'additional_settings',
  );
  $pform += ec_product_form_base_elements($node_form['#node']);
  $pform['ptype']['#type'] = 'hidden';

  // If this type is from a specific module, get its bits.
  // TODO: alter receiving modules so that they listen for hook_product_form_alter().
  $pform = array_merge($pform, (array) ec_product_invoke_productapi($node_form['#node'], 'form'));

  // Allow other modules to alter the product form ($pform).
  // Preferable over them implementing hook_form_alter directly.
  foreach ($product_type->features as $feature) {
    if (isset($feature->fileinc)) {
      require_once DRUPAL_ROOT . '/' . $feature->fileinc;
    }
  }
  foreach (module_implements('product_form_alter') as $module) {
    $func = $module . '_product_form_alter';
    $func($pform, $node_form);
  }

  // Add remove option if applicable.
  if (variable_get('ec_product_convert_' . $node_form['#node']->type, EC_PRODUCT_CONVERT_NEVER) != EC_PRODUCT_CONVERT_ALWAYS) {
    $pform['product_remove'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove from store'),
      '#description' => t('Check here to delete product information. Takes effect when changes are submitted.'),
      '#weight' => 99,
    );
  }
  return $pform;
}

/**
 * Additional settings that are added to the node type form at admin/content/types/*
 *
 * @param $form_id, $form_values, as passed to hook_form_alter.
 * @ingroup form
 */
function _ec_product_alter_node_type_form(&$form, &$form_state) {
  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product'),
    '#collapsible' => TRUE,
    '#weight' => 1,
    '#attributes' => array(
      'class' => array('ec-product-node-type-settings-form'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'ec_product') . '/js/ec_product.admin.js'),
    ), 
    '#group' => 'additional_settings',
  );
  $type = $form['old_type']['#value'];
  $form['product']['ec_product_convert'] = array(
    '#type' => 'select',
    '#title' => t('Node type is always a product'),
    '#default_value' => ec_product_can_be_converted($type),
    '#options' => array(
      EC_PRODUCT_CONVERT_NEVER => t('Never'),
      EC_PRODUCT_CONVERT_ALWAYS => t('Always'),
      EC_PRODUCT_CONVERT_OPTIONAL => t('Optional'),
    ),
    '#description' => t('Select the setting which will determine how the product is going to interact with the node'),
  );
  $ptypes = ec_product_get_types();
  $options = array();
  foreach ($ptypes as $ptype => $info) {
    $options[$ptype] = t('!name<dt>!description</dt>',
      array('!name' => $info->name, '!description' => $info->description));
  }
  $ptypes = variable_get('ec_product_ptypes_' . $type, array_keys(ec_product_get_types('names')));
  $form['product']['ec_product_ptypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Product types'),
    '#default_value' => $ptypes,
    '#options' => $options,
    '#description' => t('Indicate the products types that will be available on the current type of node.'),
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'product') . '/css/product.css',
      ),
    ),
  );
}

/**
 * Update the node form to include a product form.
 */
function ec_project_convert_node_js() {
  $cached_form_state = array();
  $files = array();

  // Load the form from the Form API cache.
  if (!($cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state)) || !isset($cached_form['#node'])) {
    form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    $output = theme('status_messages');
    print drupal_json_encode(array('status' => TRUE, 'data' => $output));
    exit();
  }

  $form_state = array('values' => $_POST);

  $cached_form['#node']->ptype = $form_state['values']['ptype'];
  $node = $cached_form['#node'];
  $old_form = array();

  $form = array(
    '#node' => $node,
    'type' => array(
      '#type' => 'value',
      '#value' => $node->type,
    ),
  );
  $form['product'] = _ec_product_alter_product_form($old_form, $cached_form);

  // Render the form for output.
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
    '#tree' => FALSE,
    '#parents' => array(),
  );
  drupal_alter('form_' . $cached_form['form_id']['#value'], $form, $form_state);
  drupal_alter('form', $form, $form_state, $form_id);

  $cached_form['product'] = $form['product'];

  form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);

  $form_state = array('submitted' => FALSE);
  $form = form_builder($cached_form['form_id']['#value'], $form, $form_state);

  $product_form = array();
  foreach (element_children($form['product']) as $id) {
    $product_form[$id] = $form['product'][$id];
  }

  $output = theme('status_messages') . drupal_render($product_form);

  // We send the updated file attachments form.
  // Don't call drupal_json(). ahah.js uses an iframe and
  // the header output by drupal_json() causes problems in some browsers.
  print drupal_json_encode(array('status' => TRUE, 'data' => $output));
  exit;
}

/**
 * List all product types
 * Path: admin/config/store/products/types
 */
function ec_product_admin_ptypes() {
  $ptypes = ec_product_get_types();
  uasort($ptypes, create_function('$a, $b', 'if ($a->name == $b->name) { return 0; }
  else { return strcmp($a->name, $b->name); }'));
  $output = '';
  if (!empty($ptypes)) {
    $head = array(t('Name'), t('Type'), t('Description'), array(
        'data' => t('Operations'),
        'colspan' => 2,
      ));
    foreach ($ptypes as $ptype => $item) {
      $rows[] = array(
        array('data' => l($item->name, "admin/config/store/products/types/{$ptype}")),
        array('data' => check_plain($ptype)),
        array('data' => check_plain($item->description)),
        array('data' => l(t('edit'), "admin/config/store/products/types/{$ptype}")),
        array('data' => !isset($item->internal) ? l(t('delete'),
          "admin/config/store/products/types/{$ptype}/delete") : ''),
      );
    }
    $output .= theme('table', array('header' => $head, 'rows' => $rows));
  }
  else {
    drupal_set_message(t('No product types defined.'));
  }
  return $output;
}

/**
 * Add or Edit product type form.
 * Path: admin/config/store/products/types/add, admin/config/store/products/types/*
 */
function ec_product_admin_ptypes_form($form, $form_state, $ptype = NULL) {
  if (!empty($ptype) && (!$info = ec_product_get_types('type', $ptype))) {
    drupal_not_found();
  }

  // Fix up the breadcrumbs
  $bc = drupal_get_breadcrumb();

  $bc[] = l(t('Types'), 'admin/config/store/products/types');

  drupal_set_breadcrumb($bc);

  $form = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'ec_product') . '/css/product.css',
      ),
    ),
  );
  if (empty($info)) {
    $info = new stdClass;
    $info->name = $info->ptype = $info->description = '';
  }
  else {
    $form['old_ptype'] = array(
      '#type' => 'value',
      '#value' => $info->ptype,
    );
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $info->name,
    '#description' => t('The human readable name of this product type.'),
    '#required' => TRUE,
  );
  $form['ptype'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
    '#default_value' => $info->ptype,
    '#description' => t('The machine-readable name of this product type. This is used by the system internally to refer to this product type. The Product type may not include space or -'),
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $info->description,
    '#description' => t('A brief description of what this product type is.'),
    '#required' => TRUE,
  );
  $form['info'] = array(
    '#type' => 'value',
    '#value' => $info,
  );

  $defaults = array();
  $options = array();
  foreach (node_type_get_types() as $type => $info) {
    $options[$type] = t('@name<dt>!description<dt>', array('@name' => $info->name, '!description' => $info->description));
    $product = variable_get('ec_product_ptypes_' . $type, array());
    if (in_array($ptype, $product)) {
      $defaults[] = $type;
    }
  }

  if (!empty($options)) {
    $form['content_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Content Types'),
      '#description' => t('Select content types which this this product type can be used on.'),
      '#default_value' => $defaults,
      '#options' => $options,
    );
  }

  $form['submit'] = array();
  $form['submit']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Submit product type'),
  );
  if (isset($info->internal) && empty($info->internal)) {
    $form['submit']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset product type'),
    );
  }
  elseif (!empty($info->ptype) && !db_query_range("SELECT COUNT(*) FROM {ec_product}
      WHERE ptype = :ptype", array(':ptype' => $info->ptype))->fetchField()) {
    $form['submit']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete product type'),
    );
  }

  return $form;
}

/**
 * Validate product_admin_ptypes_form()
 * @ingroup form
 */
function ec_product_admin_ptypes_form_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['old_ptype']) && $form_state['values']['ptype'] != $form_state['values']['old_ptype'] &&
      ec_product_get_types('type', $form_state['values']['ptype'])) {
    form_set_error('ptype', t('Product type %ptype already exists', array('%ptype' => $form_state['values']['ptype'])));
  }
  elseif (preg_match('/[- ]/i', $form_state['values']['ptype'])) {
    form_set_error('ptype', t('Product type may not include any - or spaces'));
  }
}

/**
 * Submit product_admin_ptypes_form()
 * @ingroup form
 */
function ec_product_admin_ptypes_form_submit(&$form, &$form_state) {
  $form_state['values']['old_ptype'] = isset($form_state['values']['old_ptype']) ? $form_state['values']['old_ptype'] : NULL;
  $info = $form_state['values']['info'];
  $info->name = $form_state['values']['name'];
  $info->ptype = $form_state['values']['ptype'];
  $info->old_ptype = $form_state['values']['old_ptype'];
  $info->description = $form_state['values']['description'];

  if ($form_state['values']['op'] == t('Reset product type')) {
    ec_product_ptypes_delete($info->ptype);
    drupal_set_message(t('Product type %ptype has been reset',
      array('%ptype' => $info->ptype)));
    $form_state['redirect'] = 'admin/config/store/products/types/' . $info->ptype;
  }
  elseif ($form_state['values']['op'] == t('Delete product type')) {
    $form_state['redirect'] = 'admin/config/store/products/types/' . $info->ptype . '/delete';
    return;
  }
  elseif ($form_state['values']['op'] == t('Submit product type')) {
    ec_product_ptypes_save($info);

    foreach (node_type_get_types() as $type => $info) {
      $ptypes = variable_get('ec_product_ptypes_' . $type, array());

      if ($form_state['values']['old_ptype'] != $form_state['values']['ptype'] && isset($form_state['values']['content_types'][$type]) && $form_state['values']['content_types'][$type] && ($pos = array_search($form_state['values']['old_ptype'], $ptypes)) !== FALSE) {
        unset($ptypes[$pos]);
        $ptypes[] = $form_state['values']['ptype'];
      }
      elseif (isset($form_state['values']['content_types'][$type]) && $form_state['values']['content_types'][$type]) {
        $ptypes[] = $form_state['values']['ptype'];
      }
      elseif (isset($form_state['values']['content_types'][$type]) && !$form_state['values']['content_types'][$type] && ($pos = array_search($form_state['values']['old_ptype'], $ptypes)) !== FALSE) {
        unset($ptypes[$pos]);
      }
      variable_set('ec_product_ptypes_' . $type, $ptypes);
    }
    if (isset($form_state['values']['old_ptype'])) {
      drupal_set_message(t('Product type %ptype updated', array('%ptype' => $form_state['values']['ptype'])));
    }
    else {
      drupal_set_message(t('New product type %ptype created', array('%ptype' => $form_state['values']['ptype'])));
    }
  }
  $form_state['redirect'] = 'admin/config/store/products/types';

  menu_rebuild();
}

/**
 * Implementation of hook_delete().
 */
function _ec_product_delete($node, $revision_only = FALSE) {
  if (!empty($revision_only)) {
    db_delete('ec_product')
      ->condition('nid', $node->nid)
      ->condition('vid', $node->vid)
      ->execute();
  }
  else {
    db_delete('ec_product')
      ->condition('nid', $node->nid)
      ->execute();
  }
  module_invoke('cart', 'productapi', $node, 'delete');
  ec_product_invoke_productapi($node, 'delete', $revision_only);
  ec_product_invoke_feature_all($node, 'delete', $revision_only);
  drupal_set_message(t('Product deleted'));
}

/**
 * @param $form_id
 * @param $form_values
 * @ingroup form
 */
function _ec_product_edit_form(&$invoice) {
  $form['#theme'] = 'ec_product_edit_form';
  $form['products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Products'),
    '#tree' => TRUE,
    '#theme' => 'ec_product_edit_form_product',
  );
  foreach ((array) $invoice->items as $key => $item) {
    $node = node_load($item->nid);
    $form['products'][$node->nid]['#node'] = $item;
    $form['products'][$node->nid]['title'] = array(
      '#value' => l($item->title, 'node/' . $node->nid),
    );
    $price = store_adjust_misc($invoice, $item);
    $form['products'][$node->nid]['#total'] = ec_product_has_quantity($node) ?
      $price * $item->qty : $price;
    if (!empty($node->is_recurring)) {
      $form['products'][$node->nid]['recurring'] = array(
        '#value' => ec_product_recurring_nice_string($node),
      );
    }
    if ($node->ptype == 'tangible' && $node->availability != NULL && $node->availability != 1) {
      $form['products'][$node->nid]['availability'] = array(
        '#value' => availability_get_message($node->availability),
      );
    }
    $form['products'][$node->nid]['qty'] = array(
      '#type' => ec_product_has_quantity($node) ? 'textfield' : 'value',
      '#default_value' => $item->qty,
      '#size' => 2,
      '#maxlength' => 2,
    );
    $form['products'][$node->nid]['delete'] = array(
      '#type' => 'checkbox',
    );
    if ($extra = ec_product_invoke_productapi($item, 'cart_form', $invoice)) {
      $form['#product_cart_form'] = TRUE;
      $form['products'][$node->nid]['data'] = array();
    }
  }
  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Products'),
    '#collapsible' => TRUE,
    '#collapsed' => count($invoice->items) ? TRUE : FALSE,
  );
  $form['add']['new_products'] = array(
    '#type' => 'textfield',
    '#title' => t('Product Ids'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'product/autocomplete/all',
    '#description' => t('Enter the id\'s of the products that you wish to add to the invoice. Here is a <a href="!product_quicklist" onclick="window.open(this.href, \'!product_quicklist\', \'width=480,height=480,scrollbars=yes,status=yes\'); return FALSE">list of all products</a>.', array('!product_quicklist' => url('admin/store/products/quicklist'))),
  );
  $form['submit']['invoiceop'] = array(
    '#type' => 'submit',
    '#value' => t('Update Products'),
    '#attributes' => array('id' => 'invoiceop'),
  );
  $form['submit']['continue'] = array(
    '#type' => 'submit',
    '#value' => t('Continue'),
  );
  return $form;
}

function ec_product_admin_features_overview($ptype) {
  return theme('ec_product_admin_features_overview', array('ptype' => $ptype));
}

/**
 * List all features from a given product type.
 * Path: admin/config/store/products/types/.../features
 *
 * @param $info
 *   Object, the product type
 */
function ec_product_admin_ptypes_feature_list($form, $form_state, $ptype = NULL) {
  if (!$info = ec_product_get_types('type', $ptype)) {
    drupal_not_found();
    exit();
  }

  $form = array();
  $form['features'] = array(
    '#tree' => TRUE,
  );

  $features = db_select('ec_product_features', 'epf')
    ->fields('epf')
    ->condition('ptype', $ptype)
    ->orderBy('weight', 'ASC')
    ->execute()
    ->fetchAll();

  foreach ($features as $feature) {
    $ftype = $feature->ftype;

    $form['features'][$ftype]['ptype'] = array(
      '#type' => 'value',
      '#value' => $info->ptype,
    );
    $form['features'][$ftype]['ftype'] = array(
      '#type' => 'value',
      '#value' => $feature->ftype,
    );
    $form['features'][$ftype]['name'] = array(
      '#markup' => $info->features[$ftype]->name,
    );
    $form['features'][$ftype]['description'] = array(
      '#markup' => $info->features[$ftype]->description,
    );
    $form['features'][$ftype]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $feature->weight,
      '#disabled' => isset($info->features[$ftype]->allow_edit) ? !$info->features[$ftype]->allow_edit : FALSE,
    );
    if (ec_product_feature_get_function($feature->ftype, 'admin_form')) {
      $form['features'][$ftype]['ops']['edit'] = array(
        '#markup' => l(t('edit'), 'admin/config/store/products/types/' . $info->ptype . '/' . $feature->ftype . '/edit'),
        '#suffix' => ' ',
      );
    }
    if (!isset($info->features[$ftype]->allow_disable) || $info->features[$ftype]->allow_disable) {
      $form['features'][$ftype]['ops']['delete'] = array(
        '#markup' => l(t('delete'), 'admin/config/store/products/types/' . $info->ptype . '/' . $feature->ftype . '/delete'),
      );
    }
  }

  $elements = element_children($form['features']);
  if (!empty($elements)) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Update features',
    );
  }
  elseif (empty($_POST)) {
    drupal_set_message(t('No features have been added to this product'));
  }
  
  return $form;
}

/**
 * Submit for ec_product_admin_ptypes_feature_list()
 * @ingroup form
 */
function ec_product_admin_ptypes_feature_list_submit(&$form, &$form_state) {
  foreach ($form_state['values']['features'] as $feature) {
    ec_product_feature_update((object) $feature);
  }
  drupal_set_message(t('Features have been updated'));
}

/**
 * Link product types to features.
 * @ingroup form
 * Path: 'admin/config/store/products/types/.../add_feature
 */
function ec_product_admin_ptypes_feature_form($form, $form_state, $ptype) {
  if (!$info = ec_product_get_types('type', $ptype)) {
    drupal_not_found();
    exit();
  }

  $options = $form = array();
  $features = ec_product_feature_get();
  foreach ($features as $ftype => $feature) {
    if (!isset($info->features[$ftype]) && (!isset($feature->hidden) || !$feature->hidden)) {
      $options[$ftype] = t('@name<dt>@description</dt>',
        array('@name' => $feature->name, '@description' => $feature->description));
    }
  }

  if (!empty($options)) {
    $function = ec_product_feature_get_function($ftype, 'admin_form');
    if (isset($form_state['storage']['ftype'])) {
      $ftype = $form_state['storage']['ftype'];
      $form = $function($form, $form_state);

      $form['title'] = array(
        '#value' => '<h3>'. t('Settings for %name', array('%name' => ec_product_feature_get('name', $ftype))) .'</h3>',
        '#weight' => -99,
      );
      $form['ftype'] = array(
        '#type' => 'value',
        '#value' => $ftype,
      );
    }
    else {
      $form['info'] = array(
        '#type' => 'value',
        '#value' => $info,
      );
      $form['ftype'] = array(
        '#type' => 'radios',
        '#title' => t('Select a product feature to be added'),
        '#options' => $options,
      );
    }
    $form['ptype'] = array(
      '#type' => 'value',
      '#value' => $info->ptype,
    );
    $form['ptype'] = array(
      '#type' => 'value',
      '#value' => $info->ptype,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add feature'),
    );
  }
  else {
    drupal_set_message(t('No more features can be added to @name',
      array('@name' => $info->name)));
    drupal_goto('admin/config/store/products/types/' . $info->ptype . '/features/list');
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_admin_ptypes_feature_form_validate(&$form, &$form_state) {
  if ($function = ec_product_feature_get_function($form_state['values']['ftype'], 'admin_validate')) {
    $function($form, $form_state);
  }
}

/**
 * Submit for ec_product_admin_ptypes_feature_form()
 * @ingroup form
 */
function ec_product_admin_ptypes_feature_form_submit(&$form, &$form_state) {
  $ftype = $form_state['values']['ftype'];
  $ptype = $form_state['values']['ptype'];
  $function = ec_product_feature_get_function($ftype, 'admin_form');

  if (!$function || ($function && $form_state['storage']['ftype'])) {
    if ($ftype) {
      ec_product_feature_save($ptype, $form_state['values']);
    }
    $form_state['redirect'] = 'admin/config/store/products/types/' . $form_state['values']['ptype'] . '/features';
  }
  else {
    // Admin form exists so redirect to the admin form to handle the processing.
    // Do not save as the user could still hit cancel.
    $form_state['redirect'] = 'admin/config/store/products/types/' . $ptype . '/' . $ftype . '/add';
  }
}

/**
 * Allow the user to edit the feature settings on the product type
 */
function ec_product_admin_ptypes_feature_edit($form, &$form_state, $op, $ptype, $ftype) {
  $form = array();

  if ($function = ec_product_feature_get_function($ftype, 'admin_form')) {
    // get the definition of the product type and the current values of the feature settings
    $info = ec_product_get_types('type', $ptype);
    $feature = ($op == 'add' ? ec_product_feature_get('type', $ftype) : $info->features[$ftype]);
    $feature->ptype = $ptype;

    // build the form from the $info about this product type
    $form = $function($form_state, $feature);

    drupal_set_title(t('Settings for %name', array('%name' => ec_product_feature_get('name', $ftype))), PASS_THROUGH);

    $form['ftype'] = array(
      '#type' => 'value',
      '#value' => $ftype,
    );
    $form['ptype'] = array(
      '#type' => 'value',
      '#value' => $ptype,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('ec_product_admin_ptypes_feature_edit_submit'),
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#id' => 'cancel',
      '#submit' => array('ec_product_admin_ptypes_feature_edit_cancel'),
    );
  }
  else {
    drupal_set_message(t('The are no user definable settings for feature %name', array('%name' => ec_product_feature_get('name', $ftype))));
  }
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_admin_ptypes_feature_edit_validate(&$form, &$form_state) {
  if ($function = ec_product_feature_get_function($form_state['values']['ftype'], 'admin_validate')) {
    $function($form, $form_state);
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_admin_ptypes_feature_edit_submit(&$form, &$form_state) {
  ec_product_feature_save($form_state['values']['ptype'], $form_state['values']);
  $form_state['redirect'] = 'admin/config/store/products/types/' . $form_state['values']['ptype'] . '/features';
  drupal_set_message(t('Product feature %name has been updated', array('%name' => ec_product_feature_get('name', $form_state['values']['ftype']))));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_product_admin_ptypes_feature_edit_cancel(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/config/store/products/types/' . $form_state['values']['ptype'] . '/features';
}

/**
 * Overview of settings of a ptype.
 * @ingroup $form
 * Path: admin/config/store/products
 */
function ec_product_admin_ptypes_settings($form, &$form_state) {
  $form = array();

  $form['ec_product_convert'] = array(
    '#type' => 'select',
    '#title' => t('New node types product conversion setting'),
    '#default_value' => variable_get('ec_product_convert', 0),
    '#options' => array(
      0 => t('Never'),
      1 => t('Always'),
      2 => t('Optional'),
    ),
    '#description' => t('This setting will be applied to all new node types when they are created'),
  );

  $form['ec_product_types'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add new product types to nodes automatically'),
    '#default_value' => variable_get('ec_product_types', 1),
    '#description' => t('Set the default action for all new product types'),
  );

  $form['ec_product_cart_addition_by_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Users adds product to cart using link.'),
    '#default_value' => variable_get('ec_product_cart_addition_by_link', 1),
    '#description' => t('If this field is unchecked a form will be placed at the bottom of the product view which will allow the user select the quantity and other information.'),
  );

  $form['ec_product_cart_on_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display cart on node teaser display'),
    '#default_value' => variable_get('ec_product_cart_on_teaser', 1),
    '#description' => t('Show the cart links/form on node teasers.'),
  );

  return system_settings_form($form);
}

/**
 * Delete a product type.
 * @ingroup $form
 * Path: admin/config/store/products/types/.../delete
 */
function ec_product_admin_ptypes_delete($form, $form_state, $ptype) {
  if (!$info = ec_product_get_types('type', $ptype)) {
    drupal_not_found();
  }
  $form = array();

  if (db_query_range("SELECT COUNT(*) FROM {ec_product} WHERE ptype = :ptype", array(':ptype' => $info->ptype))->fetchField()) {
    drupal_set_message(t('You cannot delete product type %name as products still exist', array('%name' => $info->name)), 'error');
    drupal_goto('admin/config/store/products/types');
  }

  $form['info'] = array(
    '#type' => 'value',
    '#value' => $info,
  );

  return confirm_form($form, t('Are you sure you want to delete product type %name?', array('%name' => $info->name)), 'admin/config/store/products/types', t('This action cannot be undone'), t('Delete product type'), t('Cancel'));
}

/**
 * Submit product_admin_ptypes_delete()
 * @ingroup form
 */
function ec_product_admin_ptypes_delete_submit(&$form, &$form_state) {
  $info = $form_state['values']['info'];
  ec_product_ptypes_delete($info->ptype);
  drupal_set_message(t('Product type %name has been deleted', array('%name' => $info->name)));
  $form_state['redirect'] = 'admin/config/store/products/types';
}

/**
 * Delete a product type
 */
function ec_product_ptypes_delete($ptype) {
  $info = ec_product_get_types('type', $ptype, TRUE);

  db_delete('ec_product_ptypes')
    ->condition('ptype', $ptype)
    ->execute();
  module_invoke_all('product_types', $info, 'delete');
  menu_rebuild();
}

/**
 * Implements hook_product_types().
 * Which is also in the admin file.
 */
function ec_product_product_types($info, $op) {
  switch ($op) {
    case 'update':
      if (isset($info->old_ptype) && variable_get('ec_product_ptypes', 1)) {
        foreach (node_type_get_names() as $type => $description) {
          $ptypes = variable_get('ec_product_ptypes_' . $type, array());
          if (!in_array($info->ptype, $ptypes)) {
            $ptypes[] = $info->ptype;
          }
        }
      }
      break;

    case 'delete':
      foreach ($info->features as $ftype => $feature) {
        ec_product_feature_disable($info, $ftype);
      }
  }
}


/**
 * Confirm if the product feature should be deleted
 *
 * @param $info
 * @param $features
 *   Object, the feature
 * @ingroup form
 */
function ec_product_admin_ptypes_feature_delete($form, $form_state, $ptype, $ftype) {
  if (!$info = ec_product_get_types('type', $ptype)) {
    drupal_not_found();
  }
  if (!$feature = ec_product_feature_get('type', $ftype)) {
    drupal_not_found();
  }
  $form = array();

  $form['info'] = array(
    '#type' => 'value',
    '#value' => $info,
  );
  $form['feature'] = array(
    '#type' => 'value',
    '#value' => $feature,
  );

  return confirm_form($form, t('Are you sure you want to delete feature %feature_name from product type %name?',
    array('%feature_name' => $feature->name, '%name' => $info->name)),
    'admin/config/store/products/types/' . $ptype . '/features', t('This action cannot be undone'), t('Delete feature'),
    t('Cancel'));
}

/**
 * Confirm if the product feature should be deleted
 *
 * @param $info
 * @param $features
 *   Object, the feature
 * @ingroup form
 */
function ec_product_admin_ptypes_feature_delete_submit(&$form, &$form_state) {
  ec_product_feature_disable($form_state['values']['info'], $form_state['values']['feature']->ftype);
  $form_state['redirect'] = 'admin/config/store/products/types/' . $form_state['values']['info']->ptype . '/features';
}
