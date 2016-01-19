<?php

class SSHttpRequestMultiCurl extends SSHttpRequest implements SShttpInterface
{

    private $objects = array();
    private $ch = array();

    public function addObject($data, $url)
    {
        if (!empty($data)) {
            $this->objects[] = array(
                'data' => $data,
                'url' => $url
            );
        }
    }

    public function sendRequest($data = false, $url = false)
    {
        if (!empty($this->objects)) {
            $mh = curl_multi_init();
            $handles = array();

            foreach ($this->objects as $object) {
                $data = $object['data'];
                $url = $object['url'];
                $data_string = json_encode($data);
                $ch = ($url) ? curl_init(INDEX_ENDPOINT_01 . $url) : curl_init(INDEX_ENDPOINT_01 . '/' . $data['index'] . '/' . $data['eType']);
                curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
                curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
                curl_setopt($ch, CURLOPT_FRESH_CONNECT, true);
                curl_setopt($ch, CURLOPT_USERAGENT, 'api');
                curl_setopt($ch, CURLOPT_TIMEOUT, 1);
                curl_setopt($ch, CURLOPT_HEADER, 0);
                curl_setopt($ch, CURLOPT_RETURNTRANSFER, false);
                curl_setopt($ch, CURLOPT_FORBID_REUSE, true);
                curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
                curl_setopt($ch, CURLOPT_DNS_CACHE_TIMEOUT, 10);
                curl_multi_add_handle($mh, $ch);
                $handles[] = $ch;
            }
            $running = null;
            do {
                curl_multi_exec($mh, $running);
                usleep(250000);
            } while ($running > 0);
            for ($i = 0; $i < count($handles); $i++) {
                // get the content of the handle
                $output .= curl_multi_getcontent($handles[$i]);
                // remove the handle from the multi handle
                curl_multi_remove_handle($mh, $handles[$i]);
            }
            curl_multi_close($mh);
        }
    }
}
