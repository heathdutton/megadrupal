<?php

    /*
    * Name: Http Helper
    */
    require dirname(__FILE__) . '/../helpers/EasyHttp.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Error.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Curl.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Cookie.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Encoding.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Fsockopen.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Proxy.php';
    require dirname(__FILE__) . '/../helpers/EasyHttp/Streams.php';

    /*
    * Name: Constants
    *   - Aliases to payment types
    */
    define("ECOM_ROOT_URL", "https://www.etsemoney.com/hp/v2");

    class HostedPayments {

        /*
        * Name: Properties
        */ 
        private $props;
        public  $store;
        
        /*
        * Name: Constructor
        * @requestId (string)
        *   - Merchant ID
        */
        function __construct( $apiKey ) {

            if( !function_exists('curl_version') )
                throw new Exception("Hosted Payments requires CURL. Please have your system administrator enable CURL in your pnp.ini", 1);
        
            if(!isset($apiKey))
                throw new Exception("Your API key was not provided.", 1);

            $this->props = array();
            $this->store = new Storage($this);

            $this->set('apiKey', $apiKey);

        }

        /*
        * Name: the set function
        */
        public function set( $key, $value ) {

            $this->props[$key] = $value;
            return $this;

        }

        /*
        * Name: the get function
        */    
        public function get( $key ) {


            /*
            * Name: Get Session
            *  - Quickly provides a means to retreive an HP_SESSION_ID if the getter contains "session"
            */
            if (strpos($key, "session") !== false) {

                    /*
                    * Name: Create session
                    *  - uses session start, but checks to see if a session has already been started
                    */
                    if( session_id() == '' ) {
                        session_start();
                    }

                    return $_SESSION["HP_SESSION_ID"];

            }

            if(array_key_exists($key,$this->props))
                return $this->props[$key];
            else
                return null;

        }

        /*
        * Name: the send function 
        * - populates target url, makes request, and returns json with session id
        */
        public function send(){

            $targetUrl = "";

            /* - if action property exists, then populate target url */
            if(array_key_exists('action',$this->props))
                $targetUrl = (ECOM_ROOT_URL . ($this->props["action"] == "session" ? "/sessions" : "/transactions"));
                
            /*
            * Name: HTTP request
            *  - uses EasyHttp to make POST to create session id
            */
            $http = new EasyHttp();
            $result = $http->request( $targetUrl, 
                array(
                  'method' => 'POST',
                  'blocking' => true,
                  'headers' => array(),
                  'body' => http_build_query( $this->props )
                )
            );


            /*
            * Name: Trims and Decodes
            *  - toJSON and Whitespace cleanup
            */
            try {

                if ( !isset($result->errors) ) {
               
                    $decoded_result = json_decode(
                        trim($result["body"])
                    );


                    /*
                    * Name: ID exist
                    *  - Doesnt create session if ID was returned on the response.
                    */
                    if ( array_key_exists('id', $decoded_result) && $this->props["action"] != "verify" ) {

                        /*
                        * Name: Create session
                        *  - uses session start, but checks to see if a session has already been started
                        */
                        if( session_id() == '' ) {
                            session_start();
                        }

                        $_SESSION["HP_SESSION_ID"] = $decoded_result->id;

                    }

                    return $decoded_result;

                } else {

                   return (object)array(
                        "message" => $result->errors, 
                        "id" => 0, 
                        "status" => "error"
                    );
                   
                }

            } catch( Exception $e ){

                throw new Exception( 'The response from the server was malformed. ' . $e->message, 1 );
            
            }
                   
        }
    }

    class Storage {

        /*
        * Name: Properties
        */ 
        private $ref;

        /*
        * Name: Constructor
        * @requestId (string)
        *   - Merchant ID
        */
        function __construct( $context ) {

            if(!isset($context))
                throw new Exception("Your payment context was not passed to Storage class.", 1);

            $this->ref = $context;

        }

        /*
        * Name: storage set function
        */
        public function set( $key, $value ) {

            $this->ref->set('store', array($key=>$value));
            return $this->ref;

        }
        
        /*
        * Name: storage get function
        */    
        public function get( $key ) {

            if( array_key_exists($key, $this->ref->get('store')) ) {
                $res = $this->ref->get('store');
                return $res[$key]; 
            } else
                return null;

        }

    }