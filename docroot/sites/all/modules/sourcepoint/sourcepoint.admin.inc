<?php
/**
 * @file
 * Administrative page callbacks for the Sourcepoint module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function sourcepoint_admin_settings_form($form_state) {

  $form['sourcepoint_api_key'] = array(
    '#title' => t('Sourcepoint Api Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('sourcepoint_api_key', ''),
    '#size' => 70,
    '#required' => TRUE,
  );

  $form['sourcepoint_get_method'] = array(
    '#type' => 'select',
    '#title' => t('Delivery Method'),
    '#options' => array(
      'script' => t('As a tag'),
      'inline' => t('Inline Script'),
    ),
    '#default_value' => variable_get('sourcepoint_get_method', ''),
    '#required' => TRUE,
    '#description' => t('Independent on option - The script will have the async param.'),
  );

  // Hold encoded script.
  $form['sourcepoint_encoded_script'] = array(
    '#title' => t('Sourcepoint Api Key'),
    '#type' => 'hidden',
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function sourcepoint_admin_settings_form_validate($form, &$form_state) {
  $api_key = $form_state['input']['sourcepoint_api_key'];
  $method = $form_state['input']['sourcepoint_get_method'];

  try {
    // /Get/Attach encoded script value to the form element.
    $sourcePoint = new SourcePointAsset($api_key, $method);
    $data = $sourcePoint->determine_asset();
    form_set_value($form['sourcepoint_encoded_script'], $data->encodedData, $form_state);
  } catch (Exception $e) {
    form_set_error('sourcepoint_api_key', t($e->getMessage()));
  }
}

/**
 * Class SourcePointAsset.
 */
class SourcePointAsset {
  /**
   * The Sourcepoint Service URL.
   */
  const SERVICE_URL_BASE = 'https://api.sourcepoint.com/script';

  /**
   * The API Key.
   */
  protected $apiKey;

  /**
   * The Service Url.
   */
  protected $serviceUrl;

  /**
   * Initial setup.
   */
  public function __construct($apiKey, $method) {
    if (empty($apiKey) || empty($method)) {
      throw new Exception('Missing arguments in SourcePointAsset().');
    }
    $this->apiKey = $apiKey;
    $this->serviceUrl = self::SERVICE_URL_BASE . "?delivery=$method";
  }

  public function determine_asset() {
    if (!function_exists('curl_init')) {
      throw new Exception('php-curl is required.');
    }

    $header = array(
      'Content-type: text/xml',
      "Authorization: Token token=$this->apiKey",
    );

    // Curl init.
    $ch = curl_init($this->serviceUrl);
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLINFO_HEADER_OUT => TRUE,
      CURLOPT_HTTPHEADER => $header,
    ));

    // Curl exec.
    $data = curl_exec($ch);

    // Check for Auth error (returned as JSON).
    $data_json = drupal_json_decode($data);
    if (!empty($data_json)) {
      throw new Exception('Errors: ' . PHP_EOL . implode(', ', $data_json));
    }

    // Check for Curl errors.
    if (curl_errno($ch)) {
      throw new Exception('Curl Request Error:' . curl_error($ch));
    }

    // Validate retrieved data.
    if (!$this->isValidScript($data)) {
      throw new Exception('Retrievet script dosen\'t seems to be a valid one');
    }

    $this->virginData = $data;
    $this->encodedData = $this->encodeAsset($data);

    return $this;
  }

  /**
   * Validate retrieved script.
   */
  private function isValidScript($data) {
    return preg_match('~<script.*</script>~i', $data);
  }

  private function encodeAsset($data = '') {
    return base64_encode($data);
  }

}
