<?php

class EcwidPlatform {

	static public function esc_attr($value)
	{
		return check_plain($value);
	}

	static public function esc_html($value)
	{
		return check_plain($value);
	}

	static public function get_price_label()
	{
		return t('Price');
	}

	static public function fetch_url($url)
	{
    $result = drupal_http_request($url);

    return array(
      'code' => $result->code,
      'data' => $result->data
    );
	}
}
