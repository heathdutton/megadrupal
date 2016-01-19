<?php 

class SSHttpRequestCurl extends SSHttpRequest implements SShttpInterface {
    public function sendRequest($data, $url = false){
        $data_string = json_encode($data);
        $ch = ($url) ? curl_init(INDEX_ENDPOINT_01.$url) : curl_init(INDEX_ENDPOINT_01.'/'.$data['index'].'/'.$data['eType']);
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
        curl_setopt($ch, CURLOPT_FRESH_CONNECT, true);
//        curl_setopt($ch, CURLOPT_TIMEOUT_MS, 10);
        curl_setopt($ch, CURLOPT_USERAGENT, 'api');
        curl_setopt($ch, CURLOPT_TIMEOUT, 1);
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, false);
        curl_setopt($ch, CURLOPT_FORBID_REUSE, true);
        curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
        curl_setopt($ch, CURLOPT_DNS_CACHE_TIMEOUT, 10);

        curl_setopt($ch, CURLOPT_HTTPHEADER, array(
                'Content-Type: application/json',
                'Content-Length: ' . strlen($data_string))
        );

        curl_exec($ch);
    }
}
