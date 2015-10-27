<?php
/**
 * @file loginza.theme.inc
 *    Theme routines.
 */

/**
 * Loginza Iframe Widget theme.
 *
 * @param array $variables
 * @return string
 */
function theme_loginza_iframe($variables) {
  $widget = <<<LW
<iframe src="http://loginza.ru/api/widget?overlay=loginza&@providerslang=@lang&token_url=@token_url"
style="width:359px;height:180px;" scrolling="no" frameborder="no"></iframe>
LW;

  return _theme_loginza_widget($widget, $variables);
}

/**
 * Loginza Button Widget theme.
 *
 * @param array $variables
 * @return string
 */
function theme_loginza_button($variables) {
  $widget = <<<LW
<a href="http://loginza.ru/api/widget?@providerslang=@lang&token_url=@token_url" class="loginza">
    <img src="@image" alt="@caption" title="@caption"/>
</a>
LW;

  return _theme_loginza_widget($widget, $variables);
}

/**
 * Loginza Icons Widget theme.
 *
 * @param array $variables
 * @return string
 */
function theme_loginza_icons($variables) {
  $widget = <<<LW
@caption
<a href="https://loginza.ru/api/widget?@providerslang=@lang&token_url=@token_url" class="loginza">
    @icons
</a>
LW;

  return _theme_loginza_widget($widget, $variables);
}

/**
 * Loginza String Widget theme.
 *
 * @param array $variables
 * @return string
 */
function theme_loginza_string($variables) {
  $widget = <<<LW
<a href="http://loginza.ru/api/widget?@providerslang=@lang&token_url=@token_url" class="loginza">@caption</a>
LW;

  return _theme_loginza_widget($widget, $variables);
}

/**
 * Generic theme for all wodgets.
 *
 * @param string $widget
 * @param array $variables
 * @return string
 */
function _theme_loginza_widget($widget, $variables) {
  $values = array(
    '@token_url' => _loginza_param_token_url($variables['token_url']),
    '@providers' => _loginza_param_providers($variables['providers_set'], $variables['provider']),
    '@lang' => _loginza_param_lang($variables['lang']),
  );

  $keys = array_keys($variables);
  if (in_array('caption', $keys)) {
    $values['@caption'] = empty($variables['caption'])
        ? variable_get('loginza_caption_text', t('Log in with Loginza'))
        : $variables['caption'];
    $values['@caption'] = check_plain($values['@caption']);
  }

  $images = loginza_widgets_images();
  if (in_array('image', $keys)) {
    $values['@image'] = empty($variables['image']) ? $images['button_widget'] : $variables['caption'];
    $values['@image'] = file_create_url($values['@image']);
  }

  if (in_array('icons', $keys)) {
    $all_providers = loginza_providers_list();

    $providers_list = empty($variables['icons']) ? $variables['providers_set'] : array_unique($variables['icons']);
    if (empty($providers_list)) {
      $providers_set = array_filter(variable_get('loginza_providers_set', array()));
      $providers_icons = array_filter(variable_get('loginza_providers_icons', array()));
      $providers_list = empty($providers_set) ? $providers_icons : $providers_set;
    }
    if (empty($providers_list)) {
      $providers_list = array_keys($all_providers);
    }

    $icons = array();
    foreach ($providers_list as $provider) {
      $icons[] = theme('image', array(
        'path' => $images[$provider],
        'alt' => $all_providers[$provider],
        'title' => $all_providers[$provider],
      ));
    }
    $values['@icons'] = implode("\r\n", $icons);
  }

  drupal_add_js('http://loginza.ru/js/widget.js');
  return strtr($widget, $values);
}
