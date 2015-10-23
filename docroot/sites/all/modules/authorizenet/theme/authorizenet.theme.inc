<?php

/**
 * Implements hook_theme().
 */
function authorizenet_theme_theme() {
  $path = drupal_get_path('module', 'authorizenet') . '/theme';
  return array(
    'authorizenet_payment_profile' => array(
      'arguments' => array('profile' => array()),
      'template' => 'authorizenet_payment_profile',
      'pattern' => 'authorizenet_payment_profile__',
      'file' => 'authorizenet.theme.inc',
      'path' => $path,
    ),
    'authorizenet_payment_profile_radios' => array(
      'arguments' => array('element' => array()),
      'file' => 'authorizenet.theme.inc',
      'path' => $path,
    ),
  );
}

/**
 * A preprocess function for theme('authorizenet_payment_profile').
 */
function template_preprocess_authorizenet_payment_profile(&$variables) {
  $profile = $variables['profile'];
  $variables['billto'] = $profile->billTo();

  $variables['first_name']  = $variables['billto']->firstName;
  $variables['last_name']   = $variables['billto']->lastName;
  $variables['company']     = $variables['billto']->company;
  $variables['street1']     = $variables['billto']->address;
  $variables['city']        = $variables['billto']->city;
  $variables['state']       = $variables['billto']->state;
  $variables['country']     = $variables['billto']->country;

  if ($card = $profile->creditCard()) {
    $variables['card_mask'] = $card->cardMask();
    $variables['expiration'] = $card->expirationTimestamp;
  }
}

/**
 * An alternative theme function for theming the radios for payment profiles.
 */
function theme_authorizenet_payment_profile_radios($variables) {
  $element = $variables['element'];
  $class = 'form-radios';
  if (isset($element['#attributes']['class'])) {
    $class .= ' ' . $element['#attributes']['class'];
  }
  foreach ($element['#options'] as $key => $value) {
    $class = $key ? 'saved-profile' : 'new-profile';
    $output .= '<div class="' . $class . '">';
    $output .= theme('radio', $element[$key]);
    $output .= '</div>';
  }
  return $output;
}
