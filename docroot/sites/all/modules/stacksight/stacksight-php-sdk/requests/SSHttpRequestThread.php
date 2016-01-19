<?php

class SSHttpRequestThread extends SSHttpRequest implements SShttpInterface {
    public function sendRequest($data, $url = false){
        $opts = array(
            'http' => array(
                'method'  => 'POST',
                'header'  => "Content-type: application/json",
                'content' => json_encode($data)
            )
        );
        $context  = stream_context_create($opts);
        $response = ($url) ? file_get_contents(INDEX_ENDPOINT_01.$url, false, $context) : file_get_contents(INDEX_ENDPOINT_01.'/'.$data['index'].'/'.$data['eType'], false, $context);

        if ($response !== false) {
            return array('success' => true, 'message' => 'OK');
        } else {
            $error = error_get_last();
            return array('success' => false, 'message' => $error['message']);
        }
    }
}
