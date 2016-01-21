<?php

class SSHttpRequestSockets extends SSHttpRequest implements SShttpInterface {

    public $timeout = 10;
    private $_socket;

    public $max_retry = 1;

    private $_state_socket = false;

    private $_socket_error = array();
    
    public function __construct(){
        $this->createSocket();
    }

    public function __destruct(){
        $this->closeSocket();
    }

    private function createSocket(){
        $flags = STREAM_CLIENT_ASYNC_CONNECT;
        if(!$this->_socket){
            if($this->_socket = @stream_socket_client($this->protocol . "://" . $this->host. ':' . $this->port, $errno, $errstr, $this->timeout, $flags)){
                stream_set_blocking($this->_socket, false);
                $this->_state_socket = true;
                $this->_socket_error = array();
            } else{
                $this->_socket_error = array(
                    'error_num' => $errno,
                    'error_message' =>  $errstr
                );
            }
        }

    }

    private function closeSocket(){
        if($this->_state_socket === true)
            fclose($this->_socket);
    }

    public function sendRequest($data, $url = false){
        if($this->_state_socket === true){
            if($url === false)
                $url = $this->api_path.'/'.$data['index'].'/'.$data['eType'];
            else
                $url = $this->api_path.$url;

            $content = json_encode($data);
            $req = "";
            $req.= "POST /$url HTTP/1.1\r\n";
            $req.= "Host: " . $this->host . "\r\n";
            $req.= "Content-Type: application/json\r\n";
            $req.= "Accept: application/json\r\n";
            $req.= "Content-length: " . strlen($content) . "\r\n";
            $req.= "\r\n";
            $req.= $content;

            if(!fwrite($this->_socket, $req)){
                $sended = false;
                for($i = 0; $i <= $this->max_retry; $i++){
                    usleep(200000);
                    if(fwrite($this->_socket, $req)){
                        $sended = true;
                        break;
                    }
                }
                if($sended === false){
                    $this->closeSocket();
                    usleep(200000);
                    $this->createSocket();
                    if(!fwrite($this->_socket, $req)){
                        $cURL = new SSHttpRequestCurl();
                        $cURL->sendRequest($data, $url);
                    }
                }
            }
        } else {
            if(!empty($this->_socket_error)){
                $error_num = $this->_socket_error['error_num'];
                $error_message = $this->_socket_error['error_message'];
                SSUtilities::error_log("#$error_num: $error_message", 'error_socket_connection');
            }
        }

    }
}
