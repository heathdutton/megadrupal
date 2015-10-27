<?php

/**
 * @return array
 */
function yamoney_admin_settings() {
  $form = array();

  // General settings
  $form['yamoney_all'] = array(
    '#type' => 'fieldset',
    '#title' => t('Base options'),
	 '#description' => t('Любое использование Вами программы означает полное и безоговорочное принятие Вами условий лицензионного договора, размещенного по адресу <a href="https://money.yandex.ru/doc.xml?id=527132">https://money.yandex.ru/doc.xml?id=527132</a> (далее – «Лицензионный договор»). Если Вы не принимаете условия Лицензионного договора в полном объёме, Вы не имеете права использовать программу в каких-либо целях.')
  );  
  $form['yamoney_all']['yamoney_ip'] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed IPs for callbacks'),
    '#default_value' => variable_get('yamoney_ip', '0.0.0.0'),
    '#description' => t('The list of IP addresses which has access to payment callbacks. One per line.<br/>0.0.0.0 means allow from all.')
  );
  $form['yamoney_all']['yamoney_mode'] = array(
    '#type' => 'select',
    '#title' => t('Payment mode'),
    '#options' => array(
      'test' => t('Test mode'),
      'live' => t('Live mode')
    ),
    '#default_value' => variable_get('yamoney_mode', 'test'),
  );

  $form['yamoney_all']['yamoney_payment_method'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enabled payment methods'),
    '#options' => yamoney_get_payment_methods(),
    '#default_value' => variable_get('yamoney_payment_method', array_keys(yamoney_get_payment_methods())),
  );

  $form['yamoney_all']['yamoney_default_payment_method'] = array(
    '#type' => 'radios',
    '#title' => t('Default payment method'),
    '#options' => yamoney_get_payment_methods(),
    '#default_value' => variable_get('yamoney_default_payment_method', YAMONEY_DEFAULT_PAYMENT_METHOD),
  );

  // Shop settings
  $shop_states = array(
    // Hide the settings when the Yandex shop checkbox is disabled.
    'invisible' => array(
      ':input[name="yamoney_shop"]' => array('checked' => FALSE),
    ),
  );
  $form['yamoney_shop_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yandex shop settings'),
  );
  $form['yamoney_shop_setting']['yamoney_shop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Select if you have a shop in Yandex.Money'),
    '#default_value' => variable_get('yamoney_shop', 0),
  );
  $form['yamoney_shop_setting']['yamoney_shop_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop ID'),
    '#description' => t('You shop ID. If you have any shops in you Yandex account.'),
    '#default_value' => variable_get('yamoney_shop_id', ''),
    '#size' => 2,
    '#states' => $shop_states,
  );
  $form['yamoney_shop_setting']['yamoney_scid'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop SCID'),
    '#default_value' => variable_get('yamoney_scid', ''),
    '#size' => 5,
    '#states' => $shop_states,
  );
  $form['yamoney_shop_setting']['yamoney_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Shop security key'),
    '#description' => t('You shop password security key. Not you payment password.'),
    '#default_value' => variable_get('yamoney_secret', ''),
    '#size' => 17,
    '#states' => $shop_states,
  );

  // Purse settings
  $form['yamoney_purse_setting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yandex purse settings'),
  );
  $form['yamoney_purse_setting']['yamoney_receiver'] = array(
    '#type' => 'textfield',
    '#title' => t('Purse number'),
    '#description' => t('Your Yandex.Money purse number.'),
    '#default_value' => variable_get('yamoney_receiver', ''),
    '#size' => 14,
  );

  $form['yamoney_purse_setting']['yamoney_formcomment'] = array(
    '#type' => 'textfield',
    '#title' => t('Pay comment'),
    '#description' => t('Your Yandex.Money pay comment.'),
    '#default_value' => variable_get('yamoney_formcomment', ''),
  );

  $form['yamoney_texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text for success and fail payment pages'),
  );
  $success = variable_get('yamoney_success_text', array('value' => '', 'format' => ''));
  $form['yamoney_texts']['yamoney_success_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Text for success page'),
    '#default_value' => $success['value'] ? $success['value'] : '',
    '#format' => $success['format'] ? $success['format'] : '',
  );
  $fail = variable_get('yamoney_fail_text', array('value' => '', 'format' => ''));
  $form['yamoney_texts']['yamoney_fail_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Text for fail page'),
    '#default_value' => $fail['value'] ? $fail['value'] : '',
    '#format' => $fail['format'] ? $fail['format'] : '',
  );
  $form['#submit'][]='send_statistics';
  return system_settings_form($form);
}
/**
 * @param array $form
 * @param array $form_state
 */
function send_statistics($form, &$form_state) {
		global $base_url;
		$path = drupal_get_path('module', 'yamoney') . '/' . 'yamoney.info';
		$info = drupal_parse_info_file($path);
		$headers = array();
		$headers[] = 'Content-Type: application/x-www-form-urlencoded';
		$form_data=$form_state['input'];
		$array = array(
			'url' => $base_url,
			'cms' => 'drupal',
			'version' => VERSION,
			'ver_mod' => $info['version'],
			'yacms' => false,
			'email' => variable_get('site_mail', ''),
			'shopid' => (isset($form_data['yamoney_shop_id']))?$form_data['yamoney_shop_id']:0,
			'settings' => array(
				'kassa' => (isset($form_data['yamoney_shop']))?true:false,
				'p2p' => (isset($form_data['yamoney_shop']))?false:true
			)
		);

		$key_crypt = gethostbyname($_SERVER['HTTP_HOST']);
		$crypto= new Crypto();
		$crypto->setKey($key_crypt);
		$array_crypt = $crypto->encrypt($array);

		$url = 'https://statcms.yamoney.ru/';
		$curlOpt = array(
			CURLOPT_HEADER => false,
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_SSL_VERIFYPEER => false,
			CURLOPT_SSL_VERIFYHOST => false,
			CURLINFO_HEADER_OUT => true,
			CURLOPT_POST => true,
		);

		$curlOpt[CURLOPT_HTTPHEADER] = $headers;
		$curlOpt[CURLOPT_POSTFIELDS] = http_build_query(array('data' => $array_crypt));

		$curl = curl_init($url);
		curl_setopt_array($curl, $curlOpt);
		$rbody = curl_exec($curl);
		$errno = curl_errno($curl);
		$error = curl_error($curl);
		$rcode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		curl_close($curl);
}
/**
 * @param array $form
 * @param array $form_state
 */
function yamoney_admin_settings_validate($form, &$form_state) {
  if (!$form_state['values']['yamoney_payment_method']) {
    form_set_error('yamoney_payment_method', t('You do not choose the type of payment online!'));
  }

  if ($form_state['values']['yamoney_ip']) {
    $ips = explode("\n", $form_state['values']['yamoney_ip']);
    foreach ($ips as $ip) {
      $ip = trim($ip);
      if (empty($ip)) {
        continue;
      }
      if (!filter_var($ip, FILTER_VALIDATE_IP)) {
        form_set_error('yamoney_ip', t('IP is not correct: %ip.<br/>Please insert correct server IP or insert 0.0.0.0 to default!', array(
          '%ip' => $ip,
        )));
      }
    }
  }
}

class Crypto {
	protected $key = '';
	protected $cipher = MCRYPT_RIJNDAEL_256;
	protected $cipher_mode = MCRYPT_MODE_CBC;

	public function setKey($key)
	{
		$this->key = hash('sha256', $key, true);
	}

	public function encrypt($data)
	{
		$data = serialize($data);
		$init_size = mcrypt_get_iv_size($this->cipher, $this->cipher_mode);
		$init_vect = mcrypt_create_iv($init_size, MCRYPT_RAND);
		$str = $this->randomString(strlen($this->key)).$init_vect.mcrypt_encrypt($this->cipher, $this->key, $data, $this->cipher_mode, $init_vect);
		return base64_encode($str);
	}

	public function decrypt($data)
	{
		$data = base64_decode($data);
		$data = substr($data, strlen($this->key));
		$init_size = mcrypt_get_iv_size($this->cipher, $this->cipher_mode);
		$init_vect = substr($data, 0, $init_size);
		$data = substr($data, $init_size);
		$str = mcrypt_decrypt($this->cipher, $this->key, $data, $this->cipher_mode, $init_vect);
		return unserialize($str);
	}

	protected function randomString($len)
	{
		$str = '';
		$pool = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
		$pool_len = strlen($pool);
		for ($i = 0; $i < $len; $i++) {
			$str .= substr($pool, mt_rand(0, $pool_len - 1), 1);
		}
		return $str;
	}
}