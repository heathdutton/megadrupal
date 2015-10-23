<?php

/**
 * Ubercart Cart Links Builder Form
 */
function uc_cartlinksbuilder_builder_form($form, $form_state, $product_nid = NULL) {
  if (module_exists('ctools')) {
    ctools_include('dependent');
  }

  if ($form_state['submitted']) {
    $form['results'] = array(
        '#type' => 'fieldset',
        '#title' => t('Generated URL'),
        '#description' => t('Copy and paste the following to use the cart link that was generated'),
    );

    $form['results']['cart_url'] = array(
        '#type' => 'item',
        '#title' => t('Generated URL'),
        '#markup' => $form_state['storage']['cart_link'],
    );

    if ($form_state['storage']['cart_html']) {
      $form['results']['cart_html'] = array(
          '#type' => 'textarea',
          '#title' => t('Generated HTML for Link Tag'),
          '#default_value' => $form_state['storage']['cart_html'],
          '#description' => t('Copy and paste this link code into a web page, Drupal block, email blast, etc.'),
          '#disabled' => FALSE,
          '#resizable' => FALSE,
          '#rows' => 3,
          '#cols' => 80,
      );

      $form['results']['link_preview'] = array(
          '#type' => 'item',
          '#title' => t('HTML Link Preview'),
          '#markup' => $form_state['storage']['cart_html'],
      );
    }

    //$form['hr'] = array('#value' => '<hr />');

    // $product_nid = $form_state['values']['product_nid'];
    $default_product = $form_state['values']['product_nid'];
  }
  else {
    if ($product_nid) {
      $product_node = node_load($product_nid);
      $default_product = sprintf('%s [nid:%d]', $product_node->title, $product_nid);
    }
  }



  $form['product_nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Select a Product'),
      //'#process' => array('ctools_dependent_process'),
      //'#dependency' => array('radio:ref_type_custom' => array('user')),
      '#autocomplete_path' => 'uc-cartlinksbuilder/autocomplete',
      '#required' => TRUE,
      '#default_value' => $default_product,
  );

  $form['qty'] = array(
      '#type' => 'textfield',
      '#title' => t('Quantity'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['qty'] : 1,
  );

  $form['dest'] = array(
      '#type' => 'radios',
      '#title' => t('Redirect to this destination when link is clicked'),
      '#options' => array(
          'cart' => t('Shopping Cart Summary'),
          'checkout' => t('Checkout Page'),
          'home' => t('Home Page'),
          'custom' => t('Custom Destination (Please Specify)'),
      ),
      '#default_value' => ($form_state['values']) ? $form_state['values']['dest'] : 'cart',
  );

  $form['custom_destination'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter a Custom Destination'),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('radio:dest' => array('custom')),
      '#default_value' => ($form_state['values']) ? $form_state['values']['custom_destination'] : '',
  );

  $form['link_options'] = array(
      '#tree' => FALSE,
      '#type' => 'fieldset',
      '#title' => t('HTML Link Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );
  $form['link_options']['make_hyperlink'] = array(
      '#type' => 'checkbox',
      '#title' => t('Also Generate HTML Link Code'),
      '#description' => t('In addition to generating a raw URL, checking this option will give you the HTML codes for a hyperlink'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['make_hyperlink'] : FALSE,
  );
  $form['link_options']['link_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Link Label'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['link_label'] : t('Click here'),
          //'#process' => array('ctools_dependent_process'),
          //'#dependency' => array('edit-make-hyperlink' => array(TRUE)),
  );
  $form['link_options']['link_action'] = array(
      '#type' => 'radios',
      '#title' => t('Link Action'),
      '#prefix' => '<div id="edit-link-action-wrapper"><div id="edit-link-action">',
      '#suffix' => '</div></div>',
      '#options' => array(
          '_blank' => t('Open in New Window'),
          '_self' => t('Open in Same Window'),
      ),
      '#default_value' => ($form_state['values']) ? $form_state['values']['link_action'] : '_self',
          //'#process' => array('ctools_dependent_process'),
          //'#dependency' => array('edit-make-hyperlink' => array(TRUE)),
  );


  $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Additional Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
  );

  $form['options']['empty_cart'] = array(
      '#type' => 'checkbox',
      '#title' => t('Empty existing cart when this link is clicked'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['empty_cart'] : FALSE,
  );
  $form['options']['hide_default_message'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide default "Added to Cart" message when this link is clicked'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['hide_default_message'] : FALSE,
  );
  $form['options']['display_message'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display a message when link is clicked'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['display_message'] : FALSE,
  );
  $form['options']['message'] = array(
      '#type' => 'textfield',
      '#title' => t('ID of Message to Display'),
      '#description' => t('You may set these messages up on the Cart Links Settings page (!link)', array('!link' => l(t('click here'), 'admin/store/settings/cart_links'))),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-display-message' => array(TRUE)),
      '#default_value' => ($form_state['values']) ? $form_state['values']['message'] : '',
  );

  $form['options']['link_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Link ID'),
      '#description' => t('You can leave this field blank if you don\'t know what to do with it.'),
      '#default_value' => ($form_state['values']) ? $form_state['values']['link_id'] : '',
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Generate Link'),
  );

  return $form;
}

/**
 * Submission handler for Ubercart Cart Links Builder Form (uc_cartlinksbuilder_builder_form())
 */
function uc_cartlinksbuilder_builder_form_submit($form, &$form_state) {
  $v = $form_state['values'];
  $actions = array();

  if ($v['empty_cart']) {
    $actions[] = 'e';
  }

  $matches = array();
  preg_match('/\[nid:(\d+)\]/', $v['product_nid'], $matches);
  $product_nid = $matches[1];

  $product = sprintf('p%s_q%d', $product_nid, $v['qty']);
  $actions[] = $product;

  if ($v['hide_default_message']) {
    $actions[] = 'm0';
  }

  if ($v['display_message'] && $v['message']) {
    $actions[] = 'm' . $v['message'];
  }

  if ($v['link_id']) {
    $actions[] = 'i' . $v['link_id'];
  }

  $url = sprintf('cart/add/%s', implode('-', $actions));
  switch ($v['dest']) {
    case 'home':
      $destination = '<front>';
      break;
    case 'cart':
      $destination = 'cart';
      break;
    case 'checkout':
      $destination = 'cart/checkout';
      break;
    case 'custom':
      $destination = $v['custom_destination'];
      break;
  }

  if ($destination) {
    $query = array('destination' => $destination);
  }

  $url = url($url, array('query' => $query, 'absolute' => TRUE));

  if ($v['make_hyperlink']) {
    $html = l($v['link_label'], $url, array('attributes' => array('target' => $v['link_action'])));
  }

  $form_state['rebuild'] = TRUE;
  $form_state['storage']['cart_link'] = $url;
  $form_state['storage']['cart_html'] = $html;
}