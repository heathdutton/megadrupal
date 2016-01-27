<?php

/**
 * @file
 * WhatsApp Class allows you to do all the previous in your own app.
 * This class allows you to login and use the WhatsApp service and provides
 * you with all capabilities of an official WhatsApp client,
 * allowing you to create a full-fledged custom Whatsapp client..
 *
 * Based on Yowsup python library and WhatsAPI php library.
 *
 * Rewritten by:
 *   - Mateu Aguiló (e0ipso) <>
 *   - Manuel García (facine) <manu[at] facine [dot] es>
 */

require 'whatsapp.protocol.class.inc';
require 'whatsapp.utils.inc';
require 'rc4.class.inc';

class WhatsApp {
  /*
   * Property declaration.
   */
  public $phoneCountryCode;
  public $phoneNumber;
  public $userName;
  public $userStatus;
  public $identity;
  public $password;
  public $countryCode;
  public $langCode;

  public $debug;

  public $whatsAppHost = 'c.whatsapp.net';
  public $whatsAppServer = 's.whatsapp.net';
  public $whatsAppGroupServer = "g.us";
  public $whatsAppPort = 5222;
  public $whatsAppVer = '2.8.7';
  public $timeout = array('sec' => 2, 'usec' => 0);
  public $device = 'iPhone';
  public $incompleteMessage = '';

  public $disconnectedStatus = "disconnected";
  public $connectedStatus = "connected";
  public $loginStatus;
  public $accountinfo;

  public $messageQueue = array();
  public $outQueue = array();
  public $lastId = FALSE;
  public $lastSent = FALSE;
  public $msgCounter = 1;
  public $socket;
  public $writer;
  public $reader;

  public $inputKey;
  public $outputKey;

  public $newMsgBind = FALSE;

  public $whatsAppReqHost = 'v.whatsapp.net/v2/code';
  public $whatsAppRegHost = 'v.whatsapp.net/v2/register';
  public $whatsAppCheHost = 'v.whatsapp.net/v2/exist';

  public $whatsAppUserAgent = 'WhatsApp/2.3.53 S40Version/14.26 Device/Nokia302';
  public $whatsAppToken = 'PdA2DJyKoUrwLw1Bg6EIhzh502dF9noR9uFCllGk1354754753509';

  /*
   * Declaration of constructor methods for classes.
   *
   * @param integer $phone_country_code
   *   Phone Country Code without '+' or '00'.
   * @param integer $phone_number
   *   Phone number whit country code.
   * @param string $identity
   *   Used in registration calls, and for login if you are trying to use
   *   an existing account that is setup on a physical device.
   *   WhatsApp has recently deprecated using IMEI/MAC to generate the account's
   *   password in updated versions of their clients.
   *   Typically this var should contain the phone's IMEI if your account
   *   is setup on a Nokia or an Android device, or the phone's WLAN's
   *   MAC Address for iOS devices. If you are not trying to use existing
   *   credentials or want to register, you can leave this var blank
   *   or set it to some random text.
   * @param string $user_name
   *   Account user name.
   * @param string $country_code
   *   ISO Country Code, 2 Digit. Default 'US'.
   * @param string $lang_code
   *   ISO 639-1 Language Code: two-letter codes. Default 'en'.
   * @param boolean $debug
   *   Debug mode value, TRUE or FALSE. Default FALSE.
   */
  function __construct($phone_country_code, $phone_number, $identity, $user_name = NULL, $country_code = 'US', $lang_code = 'en', $debug = FALSE) {
    $this->phoneCountryCode = $phone_country_code;
    $this->phoneNumber = $phone_number;
    $this->userName = $user_name;
    $this->identity = strtolower($identity);
    $this->countryCode = $country_code;
    $this->langCode = $lang_code;
    $dict = getDictionary();
    $this->writer = new BinTreeNodeWriter($dict);
    $this->reader = new BinTreeNodeReader($dict);
    $this->loginStatus = $this->disconnectedStatus;
    $this->debug = $debug;
  }

  protected function DebugPrint($debugMsg) {
    if ($this->debug) {
      print_r($debugMsg);
    }
  }

  protected function getResponse($host, $query) {
    $url = $host . '?';
    foreach ($query as $key => $value) {
      $url .= $key . '=' . $value . '&';
    }
    rtrim($url, '&');

    // Open connection.
    $ch = curl_init();

    // Configure the connection.
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_USERAGENT, $this->whatsAppUserAgent);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Accept: text/json'));
    // This makes CURL accept any peer!
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

    // Get the response.
    $response = curl_exec($ch);

    // Close the connection.
    curl_close($ch);

    $response = json_decode($response);

    $this->DebugPrint($response);

    return $response;
  }

  /*      ###################
   *      # WITHOUT REWRITE #
   */     ###################
    protected function addFeatures()
    {
        $child = new ProtocolNode("receipt_acks", NULL, NULL, "");
        $parent = new ProtocolNode("stream:features", NULL, array($child), "");
        return $parent;
    }

    protected function addAuth()
    {
        $authHash = array();
        $authHash["xmlns"] = "urn:ietf:params:xml:ns:xmpp-sasl";
        $authHash["mechanism"] = "WAUTH-1";
        $authHash["user"] = $this->phoneCountryCode . $this->phoneNumber;
        $node = new ProtocolNode("auth", $authHash, NULL, "");
        return $node;
    }
    
    public function encryptPassword()
    {
        return base64_decode($this->password);
    	if(stripos($this->_imei, ":") !== false){
            $this->_imei = strtoupper($this->_imei);
            return md5($this->_imei.$this->_imei);
    	}
        else
        {
            return md5(strrev($this->_imei));
        }
    }

    protected function authenticate()
    {
        $key = pbkdf2("sha1", $this->encryptPassword(), $this->challengeData, 16, 20, true);
        $this->inputKey = new KeyStream($key);
        $this->outputKey = new KeyStream($key);
        $array = $this->phoneCountryCode . $this->phoneNumber.$this->challengeData.time();
        $response = $this->outputKey->encode($array, 0, strlen($array), false);
        return $response;
    }
    
    public function setNewMessageBind($bind)
    {
        $this->newMsgBind = $bind;
    }
    
    public function addOutQueue($node)
    {
        $this->outQueue[] = $node;
    }

    protected function addAuthResponse()
    {
        $resp = $this->authenticate();
        $respHash = array();
        $respHash["xmlns"] = "urn:ietf:params:xml:ns:xmpp-sasl";
        $node = new ProtocolNode("response", $respHash, NULL, $resp);
        return $node;
    }

    protected function sendData($data)
    {
	socket_send( $this->socket, $data, strlen($data), 0 );
    }	
    
    protected function sendNode($node)
    {
        $this->DebugPrint($node->NodeString("tx  ") . "\n");
        $this->sendData($this->writer->write($node));
    }

    protected function readData()
    {
        $buff = "";
        $ret = socket_read( $this->socket, 1024 );
        if ($ret)
        {
            $buff = $this->incompleteMessage . $ret;
            $this->incompleteMessage = "";
        }
        return $buff;
    }
    
    protected function processChallenge($node)
    {
        $this->challengeData = $node->_data;
    }
    
    protected function sendMessageReceived($msg)
    {
        $requestNode = $msg->getChild("request");
        $receivedNode = $msg->getChild("received");
        if ($requestNode != null || $receivedNode != null)
        {
            $recievedHash = array();
            $recievedHash["xmlns"] = "urn:xmpp:receipts";
            $receivedNode = new ProtocolNode("received", $recievedHash, null, "");

            $messageHash = array();
            $messageHash["to"] = $msg->getAttribute("from");
            $messageHash["type"] = "chat";
            $messageHash["id"] = $msg->getAttribute("id");
            $messageHash["t"] = time();
            $messageNode = new ProtocolNode("message", $messageHash, array($receivedNode), "");
            $this->sendNode($messageNode);
        }
    }
    
    protected function processInboundData($data)
    {
        try
        {
            $node = $this->reader->nextTree($data);
            while ($node != null)
            {
                $this->DebugPrint($node->NodeString("rx  ") . "\n");
                if (strcmp($node->_tag, "challenge") == 0)
                {
                    $this->processChallenge($node);
                }
                else if (strcmp($node->_tag, "success") == 0)
                {
                    $this->loginStatus = $this->connectedStatus;
                    $this->accountinfo = array('status'=>$node->getAttribute('status'),'kind'=>$node->getAttribute('kind'),'creation'=>$node->getAttribute('creation'),'expiration'=>$node->getAttribute('expiration'));
                }
                if (strcmp($node->_tag, "message") == 0)
                {
                    array_push($this->messageQueue, $node);
                    $this->sendMessageReceived($node);
                    if($node->hasChild('x') && $this->lastId==$node->getAttribute('id'))
                        $this->sendNext();
                    if($this->newMsgBind && $node->getChild('body'))
                        $this->newMsgBind->process($node);
                }
                if (strcmp($node->_tag, "iq") == 0 AND strcmp($node->_attributeHash['type'], "get") == 0 AND strcmp($node->_children[0]->_tag, "ping") == 0)
                {
                    $this->Pong($node->_attributeHash['id']);
                }
                if (strcmp($node->_tag, "iq") == 0 AND strcmp($node->_attributeHash['type'], "result") == 0 AND strcmp($node->_children[0]->_tag, "query") == 0)
                {
                    array_push($this->messageQueue, $node);
                }
                $node = $this->reader->nextTree();
            }
        }
        catch (IncompleteMessageException $e)
        {
            $this->incompleteMessage = $e->getInput();
        }
    }

    public function sendNext()
    {
        if(count($this->outQueue)>0)
        {
            $msgnode = array_shift($this->outQueue);
            $msgnode->refreshTimes();
            $this->lastId = $msgnode->getAttribute('id');
            $this->lastSent = $msgnode->getAttribute('id');
            $this->sendNode($msgnode);
        }else
            $this->lastId = false;
    }
    
    public function sendComposing($msg)
    {
        $comphash = array();
        $comphash['xmlns'] = "http://jabber.org/protocol/chatstates";
        $compose = new ProtocolNode("composing", $comphash, null, "");
        $messageHash = array();
        $messageHash["to"] = $msg->getAttribute("from");
        $messageHash["type"] = "chat";
        $messageHash["id"] = time().'-'.$this->msgCounter;
        $messageHash["t"] = time();
        $this->msgCounter++;
        $messageNode = new ProtocolNode("message", $messageHash, array($compose), "");
        $this->sendNode($messageNode);
    }
    
    public function accountInfo(){
    	if(is_array($this->accountinfo)){
            print_r($this->accountinfo);
    	}
    	else{
            echo "No information available";
    	}
    }
    
    public function Connect(){ 
        $Socket = socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
        socket_connect( $Socket, $this->whatsAppHost, $this->whatsAppPort );
        $this->socket = $Socket;
        socket_set_option($this->socket, SOL_SOCKET, SO_RCVTIMEO, $this->timeout);
    }

    public function Login()
    {
        $resource = "$this->device-$this->whatsAppVer-$this->whatsAppPort";
        $data = $this->writer->StartStream($this->whatsAppServer, $resource);
        $feat = $this->addFeatures();
        $auth = $this->addAuth();
        $this->sendData($data);
        $this->sendNode($feat);
        $this->sendNode($auth);

        $this->processInboundData($this->readData());
        $data = $this->addAuthResponse();
        $this->sendNode($data);
        $this->reader->setKey($this->inputKey);
        $this->writer->setKey($this->outputKey);
        $cnt = 0;
        do
        {
            $this->processInboundData($this->readData());
        } while (($cnt++ < 100) && (strcmp($this->loginStatus, $this->disconnectedStatus) == 0));
        $this->sendNickname();
        $this->SendPresence();
    }

    # Pull from the socket, and place incoming messages in the message queue
    public function PollMessages()
    {
        $this->processInboundData($this->readData());
    }
    
    # Drain the message queue for application processing
    public function GetMessages()
    {
        $ret = $this->messageQueue;
        $this->messageQueue = array();
        return $ret;
    }
    
    public function WaitforReceipt()
    {
        $received = false;
        do{
            $this->PollMessages();
            $msgs = $this->GetMessages();
            foreach ($msgs as $m)
            {
                # process inbound messages
                if($m->_tag == "message"){
                    if($m->getChild('received')!=null){
                        $received = true;
                    }
                }
                //print($m->NodeString("") . "\n");
            }
        }while(!$received);
        //echo "Received node!!\n";
    }
    
    public function SendPresence($type="available")
    {
        $presence = array();
        $presence['type'] = $type;
        $presence['name'] = $this->userName;
        $node = new ProtocolNode("presence", $presence, null, "");
        $this->sendNode($node);
    }
    
    protected function SendMessageNode($to, $node)
    {
        $serverNode = new ProtocolNode("server", null, null, "");
        $xHash = array();
        $xHash["xmlns"] = "jabber:x:event";
        $xNode = new ProtocolNode("x", $xHash, array($serverNode), "");
        $notify = array();
        $notify['xmlns'] = 'urn:xmpp:whatsapp';
        $notify['name'] = $this->userName;
        $notnode = new ProtocolNode("notify", $notify, null, "");
        $request = array();
        $request['xmlns'] = "urn:xmpp:receipts";
        $reqnode = new ProtocolNode("request", $request, null, "");
        $msgid = time().'-'.$this->msgCounter;
        $whatsAppServer = $this->whatsAppServer;
        if (strpos($to, "-") !== FALSE) {
            $whatsAppServer = $this->whatsAppGroupServer;
        }
        $messageHash = array();
        $messageHash["to"] = $to . "@" . $whatsAppServer;
        $messageHash["type"] = "chat";
        $messageHash["id"] = $msgid;
        $messageHash["t"] = time();
        $this->msgCounter++;
        $messsageNode = new ProtocolNode("message", $messageHash, array($xNode, $notnode,$reqnode,$node), "");
        if(!$this->lastId){
            $this->lastId = $msgid;
            $this->lastSent = $msgid;
            $this->sendNode($messsageNode);
        }else
            $this->outQueue[] = $messsageNode;
    }

    public function Message($to, $txt)
    {
        $bodyNode = new ProtocolNode("body", null, null, $txt);
        $this->SendMessageNode($to, $bodyNode);
    }

    public function MessageImage($to, $url, $file, $size, $icon)
    {
        $mediaAttribs = array();
        $mediaAttribs["xmlns"] = "urn:xmpp:whatsapp:mms";
        $mediaAttribs["type"] = "image";
        $mediaAttribs["url"] = $url;
        $mediaAttribs["file"] = $file;
        $mediaAttribs["size"] = $size;

        $mediaNode = new ProtocolNode("media", $mediaAttribs, null, $icon);
        $this->SendMessageNode($to, $mediaNode);
    }

    public function Location($msgid, $to, $long, $lat)
    {
        $whatsAppServer = $this->whatsAppServer;

        $mediaHash = array();
        $mediaHash['type'] = "location";
        $mediaHash['longitude'] = $long;
        $mediaHash['latitude'] = $lat;
        $mediaHash['xmlns'] = "urn:xmpp:whatsapp:mms";
        $mediaNode = new ProtocolNode("media", $mediaHash, null, null);

        $messageHash = array();
        $messageHash["to"] = $to . "@" . $whatsAppServer;
        $messageHash["type"] = "chat";
        $messageHash["id"] = $msgid;
        $messageHash["author"] = $this->phoneCountryCode . $this->phoneNumber . "@" . $this->whatsAppServer;

        $messsageNode = new ProtocolNode("message", $messageHash, array($mediaNode), "");
        $this->sendNode($messsageNode);
    }
    
    public function sendStatusUpdate($msgid, $txt)
    {
        $bodyNode = new ProtocolNode("body", null, null, $txt);
        $serverNode = new ProtocolNode("server", null, null, "");
        $xHash = array();
        $xHash["xmlns"] = "jabber:x:event";
        $xNode = new ProtocolNode("x", $xHash, array($serverNode), "");
        $messageHash = array();
        $messageHash["to"] = 's.us';
        $messageHash["type"] = "chat";
        $messageHash["id"] = $msgid;
        $messsageNode = new ProtocolNode("message", $messageHash, array($xNode, $bodyNode), "");
        $this->sendNode($messsageNode);
    }
    
    public function Pong($msgid)
    {
        $whatsAppServer = $this->whatsAppServer;

        $messageHash = array();
        $messageHash["to"] = $whatsAppServer;
        $messageHash["id"] = $msgid;
        $messageHash["type"] = "result";
       
       	$messsageNode = new ProtocolNode("iq", $messageHash, null, "");
        $this->sendNode($messsageNode);
    }
    
    public function sendNickname()
    {
        $messageHash = array();
        $messageHash["name"] = $this->userName;
        $messsageNode = new ProtocolNode("presence", $messageHash, null, "");
        $this->sendNode($messsageNode);
    }
    
    public function RequestLastSeen($msgid, $to)
    {

    	$whatsAppServer = $this->whatsAppServer;

    	$queryHash = array();
    	$queryHash['xmlns'] = "jabber:iq:last";
    	$queryNode = new ProtocolNode("query", $queryHash, null, null);

    	$messageHash = array();
    	$messageHash["to"] = $to . "@" . $whatsAppServer;
    	$messageHash["type"] = "get";
    	$messageHash["id"] = $msgid;
    	$messageHash["from"] = $this->phoneCountryCode . $this->phoneNumber . "@" . $this->whatsAppServer;

    	$messsageNode = new ProtocolNode("iq", $messageHash, array($queryNode), "");
    	$this->sendNode($messsageNode);
    }

  /*      ###################
   *      # WITHOUT REWRITE #
   */     ###################

  /*
   * Get the last sent message id.
   *
   * @param string $id
   *   Last sent message id.
   */
  public function getLastSent() {
    return $this->lastSent;
  }

  /*
   * Request a registration code from WhatsApp.
   *
   * @param string $method
   *   Accepts only 'sms' or 'voice' as a value.
   *
   * @return object
   *   An object with server response.
   *   - status: Status of the request (sent/fail).
   *   - reason: Reason of the status (e.g. too_recent/missing_param/bad_param).
   *   - length: Registration code lenght.
   *   - method: Used method.
   *   - retry_after: Waiting time before requesting a new code.
   */
  public function requestCode($method = 'sms') {
    // Build the token.
    $token = md5($this->whatsAppToken . $this->phoneNumber);

    // Build the url.
    $host = 'https://' . $this->whatsAppReqHost;
    $query = array(
      'cc' => $this->phoneCountryCode,
      'in' => $this->phoneNumber,
      'lc' => $this->countryCode,
      'lg' => $this->langCode,
      'mcc' => '000',
      'mnc' => '000',
      'method' => $method,
      'id' => $this->identity,
      'token' => $token,
      'c' => 'cookie',
    );

    return $this->getResponse($host, $query);
  }

  /*
   * Register account on WhatsApp using the provided code.
   *
   * @param integer $code
   *   Numeric code value provided on requestCode().
   *
   * @return object
   *   An object with server response.
   *   - status: Account status.
   *   - login: Phone number with country code.
   *   - pw: Account password.
   *   - type: Type of account.
   *   - expiration: Expiration date in UNIX TimeStamp.
   *   - kind: Kind of account.
   *   - price: Formated price of account.
   *   - cost: Decimal amount of account.
   *   - currency: Currency price of account.
   *   - price_expiration: Price expiration in UNIX TimeStamp.
   */
  public function registerCode($code) {
    // Build the url.
    $host = 'https://' . $this->whatsAppRegHost;
    $query = array(
      'cc' => $this->phoneCountryCode,
      'in' => $this->phoneNumber,
      'id' => $this->identity,
      'code' => $code,
      'c' => 'cookie',
    );

    return $this->getResponse($host, $query);
  }

  /*
   * Check if account credentials are valid.
   *
   * WARNING: WhatsApp now changes your password everytime you use this.
   * Make sure you update your config file if the output informs about
   * a password change.
   *
   * @return object
   *   An object with server response.
   *   - status: Account status.
   *   - login: Phone number with country code.
   *   - pw: Account password.
   *   - type: Type of account.
   *   - expiration: Expiration date in UNIX TimeStamp.
   *   - kind: Kind of account.
   *   - price: Formated price of account.
   *   - cost: Decimal amount of account.
   *   - currency: Currency price of account.
   *   - price_expiration: Price expiration in UNIX TimeStamp.
   */
  public function checkCredentials() {
    // Build the url.
    $host = 'https://' . $this->whatsAppCheHost;
    $query = array(
      'cc' => $this->phoneCountryCode,
      'in' => $this->phoneNumber,
      'id' => $this->identity,
      'c' => 'cookie',
    );

    return $this->getResponse($host, $query);
  }

  /*
   * Set account password.
   *
   * @param string $password
   *   Account password.
   */
  public function setPassword($password) {
    $this->password = $password;
  }

}
