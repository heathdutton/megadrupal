<?php
/**
 * @file
 * Storefront Form-Specific Customizations.
 * Customize the search bar
 * - Change the label element to say 'Search'
 * - Make title invisible
 * - Set a default 'search' value for text field
 * - Add extra onblur attributes to text box
 */

/**
 * Add specific form overrides.
 */
function storefront_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = t('Search');
    $form['search_block_form']['#title_display'] = 'invisible';
  }
}

/**
 * Customize the Checkout panes without affecting other elements in the checkout
 * forms.
 *
 * To remove the 'or' and control the text of the 'cancel' link copy and
 * paste the following into the function below. Be advised that these were
 * initially set this way after usability testing.
 *
 *  $form['buttons']['cancel']['#prefix'] = '';
 *  $form['buttons']['back']['#prefix'] = '';
 *  $form['buttons']['cancel']['#value'] = 'Go Back';
 */
function storefront_form_commerce_checkout_form_alter(&$form, &$form_state, $form_id) {

  foreach (commerce_checkout_panes(array('enabled' => TRUE)) as $pane_id => $pane_info) {
    if (isset($form[$pane_id])) {
      $form[$pane_id]['#attributes']['class'][] = 'checkout-pane';

      // Some panes like address book use the prefix/suffix.
      // Making sure not to overwrite those.
      if (!empty($form[$pane_id]['#prefix'])) {
        $fprefix = $form[$pane_id]['#prefix'];
        $form[$pane_id]['#prefix'] = $fprefix . '<div class="' . $pane_id . '-pane checkout-pane-wrapper">';
      } else {
        $form[$pane_id]['#prefix'] = '<div class="' . $pane_id . '-pane checkout-pane-wrapper">';
      }
      if (!empty($form[$pane_id]['#suffix'])) {
        $fsuffix = $form[$pane_id]['#suffix'];
        $form[$pane_id]['#suffix'] = '</div>' . $fsuffix;
      } else {
        $form[$pane_id]['#suffix'] = '</div>';
      }
    }
  }
}

/**
 * Customize 'view cart' pane to add a wrapper div and a 'Continue Shopping'
 * link.
 */
function storefront_form_views_form_commerce_cart_form_default_alter(&$form, &$form_state, $form_id) {
  $form['output']['#prefix'] = '<div class="cart-pane checkout-pane-wrapper">';
  $form['line_item_summary']['#suffix'] = '</div>';
  $form['actions']['#prefix'] = '<fieldset class="checkout-buttons">
  <div class="return">' . l(t('Continue Shopping'), '') . '</div>';
  $form['actions']['checkout']['#value'] = t('Proceed to Checkout');
  $form['actions']['#suffix'] = '</fieldset>';
}

/**
 * Customize the 'review' pane to change the final button to 'Complete my
 * order'.
 */
function storefront_form_commerce_checkout_form_review_alter(&$form, &$form_state, $form_id) {
  $form['buttons']['continue']['#value'] = t('Complete my order');
}
