<?php
/**
 * @file
 * API documentation for the Chinese Social Networks Authentication module.
 */

/**
 * Define Authentication providers available to the CSNA framework.
 *
 * The Chinese Social Networks Authentication (CSNA) module uses this hook to
 * gather information on provider authentication methods defined by enabled
 * modules. These methods depend on a variety of properties used by CSNA to
 * integrate with third party API providers based on the OAuth protocol.
 *
 * By default CSNA supports the following Authentication API providers through
 * sub-modules:
 *   - Sina weibo （新浪微博）, Chinese twitter like website.
 *   - Tencent （QQ）, biggest Chinese IM provider.
 *   - Renren（人人网）, biggest Chinese facebook like website.
 *   - Kaixin001（开心网）, secondary Chinese facebook like website.
 *
 * This hook allows other modules to define custom authentication third party
 * API providers by defining all the necessary information for CSNA's
 * implementation of the OAuth protocol.
 *
 * @return array
 *   An array of providers keyed by provider_id with the following properties:
 *   - title: The title or name of the defined provider.
 *   - description: Description text to appear on the admin page for the
 *     configuration of the required parameters for this specific provider,
 *     such as Authentication API KEY or Password.
 *   - display_title: The title to be displayed in the list of links of
 *     providers returned by theme_csna_providers, which displays in the user
 *     login/registration forms and block. For better usage, HTML is allowed
 *     for this property, for example to display an image or logo.
 *   - type: string identifying the type of OAuth protocol used. By default,
 *     CSNA provides a constant for 'oauth2', see CSNA_OAUTH2, but any other
 *     custom OAuth type could also be used and provided for this property.
 *   - authorize_uri: The URL used by providers to authenticate users through
 *     the OAuth protocol.
 *   - access_token_uri: The URL used by CSNA to get the authentication token
 *     returned by providers if operation was successful.
 *   - info_uri: This URL allows capturing User's information from third party
 *     authentication providers, such as username, or other useful information.
 *
 * @see csna_providers()
 * @see csna_weibo_csna_provider_info()
 */
function hook_csna_provider_info() {
  // Example taken from the csna_weibo sub-module.
  $providers = array();

  $providers['weibo'] = array(
    'title' => t('Weibo Sina'),
    'description' => t('Set your Sina Weibo authorized KEY and SECRET below.'),
    'display_title' => csna_get_weibo_button_image(),
    'type' => CSNA_OAUTH2,
    'authorize_uri' => 'https://api.weibo.com/oauth2/authorize',
    'access_token_uri' => 'https://api.weibo.com/oauth2/access_token',
    'info_uri' => 'https://api.weibo.com/2/users/show.json',
  );

  return $providers;
}

/**
 * Allows other modules to alter already defined list of providers.
 *
 * Any of the providers and any of their properties or values can be modified
 * by using an implementation of this hook.
 *
 * @param array $providers
 *   An associative array of providers keyed by the provider_id, as returned by
 *   the function csna_providers(). See hook_csna_provider_info for a detailed
 *   list of all the defined and required properties for each provider.
 *
 * @see hook_csna_provider_info()
 * @see csna_providers()
 */
function hook_csna_provider_info_alter(&$providers) {
  if (isset($providers['weibo'])) {
    $providers['weibo']['display_title'] = t("Login or register with your Sina Weibo account!");
  }
}

/**
 * Define providers custom authentication callback methods.
 *
 * The implementation of this hook is usually required when defining a Third
 * Party authentication OAuth API provider through hook_csna_provider_info.
 *
 * This hook callback is executed when returning from provider's API
 * authentication pages, with information about the operation, such as whether
 * it was successful or not. In case of success, most providers will return a
 * token or code that can be further used to verify or obtain user's
 * information through the $provider['info_uri'] defined URL.
 *
 * @param array $provider
 *   An array of properties for the corresponding provider defined through
 *   hook_csna_provider_info, see API hooks documentation for more information.
 * @param array $request
 *   A PHP predefined variable for HTTP Request variables: $_REQUEST, received
 *   from the third party authentication provider.
 *
 * @return array
 *   An array of properties including the following keys and values:
 *   - access_token: The access token returned by the provider through its
 *     access_token_uri URL to provider further access to user's information if
 *     authentication was successful.
 *   - uid: The ID of the user returned from the provider used to load user
 *     account through user_external_load (see core API).
 *   - name: The user name returned from the provider used to load user account
 *     account through user_load_by_name (see core API).
 *
 * @see csna_callback()
 * @see csna_kaixin_csna_provider_callback()
 */
function hook_csna_provider_callback($provider, $request) {
  // Example taken from the csna_kaixin sub-module.
  if (isset($request['code'])) {
    $parameters = array(
      'client_id' => $provider['key'],
      'client_secret' => $provider['secret'],
      'grant_type' => 'authorization_code',
      'redirect_uri' => $provider['callback'],
      'code' => $request['code'],
    );
    $http = drupal_http_request(url($provider['access_token_uri'], array('query' => $parameters)));
    $data = json_decode($http->data);
    $http = drupal_http_request(url($provider['info_uri'], array('query' => array('access_token' => $data->access_token))));
    $info = json_decode($http->data);
    if (!isset($info->uid)) {
      return FALSE;
    }
    return array(
      'access_token' => $data->access_token,
      'uid' => $info->uid,
      'name' => $info->name,
    );
  }
}
