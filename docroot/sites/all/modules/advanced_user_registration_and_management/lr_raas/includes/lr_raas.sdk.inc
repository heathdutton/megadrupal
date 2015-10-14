<?php
/**
 * @file
 * User Registration SDK.
 */

// Define LoginRadius Raas domain.
define('RAAS_DOMAIN', 'https://api.loginradius.com');
define('RAAS_API_KEY', trim(variable_get('lr_social_login_apikey')));
define('RAAS_SECRET_KEY', trim(variable_get('lr_social_login_apisecret')));

module_load_include('inc', 'lr_social_login', 'includes/lr_social_login.loginradius');
/**
 * Class for RaaS SDK.
 *
 */
class LoginradiusRaasSDK {
  /**
   * Create User.
   *
   * @param $params
   * @return mixed
   */
  public function raasCreateUser($params) {
    $url = RAAS_DOMAIN . "/raas/v1/user?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY;
    return $this->raasGetResponseFromRaas($url, json_encode($params), 'application/json');
  }

  /**
   * Update User.
   *
   * @param $params
   * @param $uid
   * @return mixed
   */
  public function raasUpdateUser($params, $uid) {
    $url = RAAS_DOMAIN . "/raas/v1/user?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&userid=" . $uid;
    return $this->raasGetResponseFromRaas($url, json_encode($params), 'application/json');
  }

  /**
   * Block User.
   *
   * @param $params
   * @param $uid
   * @return mixed
   */
  public function raasBlockUser($params, $uid) {
    $url = RAAS_DOMAIN . "/raas/v1/user/status?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&uid=" . $uid;
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * Update password.
   *
   * @param $params
   * @param $uid
   * @return mixed
   */
  public function raasUpdatePassword($params, $uid) {
    $url = RAAS_DOMAIN . "/raas/v1/user/password?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&userid=" . $uid;
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * created Raas Profile.
   *
   * @param $params
   * @return mixed
   */
  public function createRaasProfile($params) {
    $url = RAAS_DOMAIN . "/raas/v1/account/profile?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY;
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * Set Password.
   *
   * @param $params
   * @param $uid
   * @return mixed
   */
  public function raasSetPassword($params, $uid) {
    $url = RAAS_DOMAIN . "/raas/v1/user/password?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&userid=" . $uid . "&action=set";
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * Link account.
   *
   * @param $uid
   * @param $provider
   * @param $providerid
   * @return mixed
   */
  public function raasLinkAccount($uid, $provider, $providerid) {
    $url = RAAS_DOMAIN . "/raas/v1/account/link?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY;
    $params = array(
      'accountid' => $uid,
      'provider' => $provider,
      'providerid' => $providerid,
    );
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * Unlink Account.
   *
   * @param $uid
   * @param $provider
   * @param $providerid
   * @return mixed
   */
  public function raasUnlinkAccount($uid, $provider, $providerid) {
    $url = RAAS_DOMAIN . "/raas/v1/account/unlink?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY;
    $params = array(
      'accountid' => $uid,
      'provider' => $provider,
      'providerid' => $providerid,
    );
    return $this->raasGetResponseFromRaas($url, $params);
  }

  /**
   * Delete User.
   *
   * @param $uid
   * @return mixed
   */
  public function RaasUserDelete($uid) {
    $url = RAAS_DOMAIN . "/raas/v1/user/delete?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&uid=" . $uid;
    return $this->raasGetResponseFromRaas($url);
  }

  /**
   * Get Custom field of raas.
   *
   * @return string
   */
  public function raasGetCustomFields() {
    $url = RAAS_DOMAIN . "/api/v2/userprofile/fields?apikey=" . RAAS_API_KEY . "&apisecret=" . RAAS_SECRET_KEY;
    $response = $this->raasGetResponseFromRaas($url);
    return isset($response->CustomFields) ? $response->CustomFields : '';
  }

  /**
   * Get only Raas Profile Data.
   *
   * @param $uid
   * @return mixed
   */
  public function raasGetRaasProfile($uid) {
    $url = RAAS_DOMAIN . "/raas/v1/account?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&accountid=" . $uid;
    $user_profile = $this->raasGetResponseFromRaas($url);
    foreach ($user_profile as $provider_profile) {
      if (isset($provider_profile->Provider) && strtolower($provider_profile->Provider) == 'raas') {
        return $provider_profile;
      }
    }
  }

  /**
   * Get all linked account.
   *
   * @param $uid
   * @return mixed
   */
  public function raasGetLinkedAccount($uid) {
    $url = RAAS_DOMAIN . "/raas/v1/account?appkey=" . RAAS_API_KEY . "&appsecret=" . RAAS_SECRET_KEY . "&accountid=" . $uid;
    return $this->raasGetResponseFromRaas($url);
  }

  /**
   * Get Response from API client.
   *
   * @param $validate_url
   * @param string $post
   * @param string $content_type
   * @return mixed
   */
  public function raasGetResponseFromRaas($validate_url, $post = '', $content_type = 'application/x-www-form-urlencoded') {

    $obj = new LoginRadius();
    return json_decode($obj->loginradiusGetApiResponse($validate_url, $post, $content_type));
  }
}
