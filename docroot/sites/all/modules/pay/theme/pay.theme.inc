<?php

/**
 * @file
 * Theme hooks and callbacks for the Payment API.
 */

/**
 * Implementation of hook_theme().
 */
function pay_theme_theme() {
  $path = drupal_get_path('module', 'pay') . '/theme';

  return array(
    // The all-purpose form renderer for payment forms.
    'pay_form' => array(
      'variables' => array('form' => array()),
      'template' => 'pay_form',
      'pattern' => 'pay_form__',
      'path' => $path,
    ),
    'pay_form_default' => array(
      'variables' => array('pay_form' => array()),
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_form_amount' => array(
      'variables' => array('pay_form' => array()),
      'template' => 'pay_form_amount',
      'pattern' => 'pay_form_amount__',
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_form_link' => array(
      'variables' => array('pay_form' => array(), 'text' => 'Make a payment'),
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_transaction' => array(
      'variables' => array('pay_transaction' => array()),
      'template' => 'pay_transaction',
      'pattern' => 'pay_transaction__',
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_cc_form' => array(
      'render element' => 'element',
      'template' => 'pay_cc_form',
      'pattern' => 'pay_cc_form__',
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_money' => array(
      'variables' => array('value' => 0, 'currency' => 'USD'),
      'file' => 'pay.theme.inc',
      'path' => $path,
    ),
    'pay_activity_list' => array(
      'variables' => array('activity' => array()),
    ),
  );
}

/**
 * Theme function for default form displays.
 */
function theme_pay_form_default($variables) {
  $pay_form = $variables['pay_form'];
  return drupal_get_form('pay_form', $pay_form);
}

/**
 * Theme function for links to payment forms.
 */
function theme_pay_form_link($variables) {
  $pay_form = $variables['pay_form'];
  $text = $variables['text'];
  if ($path = $pay_form->menu_path()) {
    $link = array(
      'title' => $text,
      'href' => $path,
    );
    return l($link['title'], $link['href'], $link);
  }
}

/**
 * A preprocess function for theme('pay_transaction').
 *
 * The $variables array initially contains the following arguments:
 *
 */
function template_preprocess_pay_transaction(&$variables) {
  $pay_transaction = $variables['pay_transaction'];
  $variables['pay_form'] = pay_form_load($pay_transaction->pfid);

  $variables['date'] = format_date($pay_transaction->created, 'short');
  $variables['user'] = user_load($pay_transaction->uid);
  $variables['notes'] = $pay_transaction->notes();
  $variables['id'] = $pay_transaction->pxid;

  if ($actions = $pay_transaction->available_actions()) {
    $links = array();
    foreach ($actions as $name => $info) {
      $href = 'pay/transaction/' . $pay_transaction->pxid . '/' . $name;
      $links[$name] = array(
        'href' => $href,
        'title' => t($info['title']),
      );
    }
    $variables['actions'] = theme_links($links, array('class' => 'pay-transaction-links'));
  }

  if ($activity = $pay_transaction->activity()) {
    $variables['activity'] = theme('pay_activity_list', array('activity' => $activity));
  }
}

/**
 * A preprocess function for theme('pay_cc_form').
 *
 * The $variables array initially contains the following arguments:
 *
 */
function template_preprocess_pay_cc_form(&$variables) {
  static $first_time = TRUE;

  $variables['pay'] = array();

  if ($variables['element']['cc_type']) {
    $path = drupal_get_path('module', 'pay') . '/theme/images/';
    foreach ($variables['element']['cc_type']['#options'] as $card => $label) {
      if (empty($card)) continue;
      $variables['element']['cc_type_image'][$card] = array(
        '#type' => 'markup',
        '#markup' => theme('image', array('path' => $path . $card . '.png', 'width' => $label)),
      );
    }
  }

  // Elements that will be specifically included by the theme template.
  $expected = array(
    'cc_type_image',
    'cc_type',
    'first_name',
    'last_name',
    'mail',
    'billto',
    'cc_number',
    'cc_ccv2',
    'cc_exp_month',
    'cc_exp_year',
  );

  $variables['pay']['prefix'] = $variables['pay']['suffix'] = array();
  $printed = FALSE;
  foreach (element_children($variables['element']) as $item) {
    if (in_array($item, $expected)) {
      $variables['pay'][$item] = drupal_render($variables['element'][$item]);
      $printed = TRUE;
    }
    else {
      // Add unrecognized items to either the 'prefix' or 'suffix' array.
      // If we've seen any 'expected' elements, use suffix: else use prefix.
      $key = $printed ? 'suffix' : 'prefix';
      $variables['pay'][$key][] = drupal_render($variables['element'][$item]);
    }
  }

  if ($first_time) {
    $variables['setup'] = $first_time;
    $first_time = FALSE;
  }
  else {
    $variables['setup'] = FALSE;
  }

  $variables['pay_form'] = $variables['pay'];
}

/**
 * Theme function for default money format.
 */
function theme_pay_activity_list($variables) {
  $activity_list = $variables['activity'];

  $hdrs = array(
    t('Date'),
    t('User'),
    t('Method'),
    t('Total'),
    t('Balance'),
    t('Comment'),
  );

  $rows = array();
  foreach ($activity_list as $activity) {
    $account = $activity->user();
    $currency = $activity->pay_transaction()->currency;

    if ($activity->result) {
      $comment = t('Success');
    }
    else {
      $comment = t('Failed');
    }

    $row = array(
      format_date($activity->timestamp, 'short'),
      $account->uid ? l($account->name, 'user/' . $account->uid) : '-',
      $activity->pay_method()->title(),
      theme('pay_money', array('value' => $activity->total, 'currency' => $currency)),
      theme('pay_money', array('value' => $activity->balance(), 'currency' => $currency)),
      $comment,
    );

    $rows[] = $row;
  }
  return theme('table', array('header' => $hdrs, 'rows' => $rows));
}

/**
 * Theme function for default money format.
 */
function theme_pay_money($variables) {
  $value = $variables['value'];
  $currency = $variables['currency'];
  //@todo: Add some logic for handling currencies?
  if ($currency == 'USD' || (!$currency)) {
    $symbol = '$';
  }
  return $symbol . (float) $value;
}
