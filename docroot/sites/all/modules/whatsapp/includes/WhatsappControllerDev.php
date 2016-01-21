<?php

/**
 * @file
 * Whatsapp controller class implementation.
 */

require_once 'lib/whatsapp.protocol.class.inc';
require_once 'lib/rc4.class.inc';

/**
 * Class implementation.
 */
class WhatsappController {

  /**
   * Constant declarations.
   */
  const undefined = 01;
  const missing = 02;
  
  /**
   * Phone Country Code without '+' or '00'.
   */
  protected $phoneCountryCode;

  /**
   *  Phone number without country code.
   */
  protected $phoneNumber;

  /**
   * Account user name.
   */
  protected $userName;

  /**
   * .
   */
  protected $userStatus;

  /**
   * Used in registration calls, and for login if you are trying to use
   * an existing account that is setup on a physical device.
   *
   * WhatsApp has recently deprecated using IMEI/MAC to generate the account's
   * password in updated versions of their clients.
   * Typically this var should contain the phone's IMEI if your account
   * is setup on a Nokia or an Android device, or the phone's WLAN's
   * MAC Address for iOS devices. If you are not trying to use existing
   * credentials or want to register, you can leave this var blank
   * or set it to some random text.
   */
  protected $identity;

  /**
   * .
   */
  protected $password;

  /**
   * ISO Country Code, 2 Digit.
   */
  protected $countryCode;

  /**
   * ISO 639-1 Language Code: two-letter codes. 
   */
  protected $langCode;

  /**
   * Destination where the messages will be sent to.
   */
  protected $destination;

  /**
   * Debug mode value.
   */
  protected $debug;

  /**
   * .
   */
  protected $whatsAppHost = 'c.whatsapp.net';

  /**
   * .
   */
  protected $whatsAppServer = 's.whatsapp.net';

  /**
   * .
   */
  public $whatsAppGroupServer = "g.us";

  /**
   * .
   */
  protected $whatsAppPort = 5222;

  /**
   * .
   */
  protected $whatsAppVer = '2.8.7';

  /**
   * .
   */
  protected $timeout = array('sec' => 2, 'usec' => 0);

  /**
   * .
   */
  protected $device = 'iPhone';

  /**
   * .
   */
  protected $incompleteMessage = '';

  /**
   * .
   */
  protected $disconnectedStatus = 'disconnected';

  /**
   * .
   */
  protected $connectedStatus = 'connected';

  /**
   * .
   */
  protected $loginStatus;

  /**
   * .
   */
  protected $accountInfo;

  /**
   * .
   */
  protected $messageQueue = array();

  /**
   * .
   */
  protected $outQueue = array();

  /**
   * .
   */
  protected $lastId = FALSE;

  /**
   * .
   */
  protected $lastSent = FALSE;

  /**
   * .
   */
  protected $msgCounter = 1;

  /**
   * .
   */
  protected $socket;

  /**
   * .
   */
  protected $KeepAlive = FALSE;

  /**
   * .
   */
  protected $writer;

  /**
   * .
   */
  protected $reader;

  /**
   * .
   */
  protected $inputKey;

  /**
   * .
   */
  protected $outputKey;

  /**
   * .
   */
  protected $newMsgBind = FALSE;

  /**
   * .
   */
  protected $whatsAppReqHost = 'v.whatsapp.net/v2/code';

  /**
   * .
   */
  protected $whatsAppRegHost = 'v.whatsapp.net/v2/register';

  /**
   * .
   */
  protected $whatsAppCheHost = 'v.whatsapp.net/v2/exist';

  /**
   * .
   */
  protected $whatsAppUserAgent = 'WhatsApp/2.3.53 S40Version/14.26 Device/Nokia302';

  /**
   * .
   */
  protected $whatsAppToken = 'PdA2DJyKoUrwLw1Bg6EIhzh502dF9noR9uFCllGk1354754753509';

  /**
   * Empty constructor.
   */
  function __construct() {
    $this->setUp();
  }

  /**
   * Sets up the oject to be initialized.
   * 
   * @return WhatsappController
   *   Returns itself.
   * 
   * @throws ErrorException
   */
  public function setUp() {
    // Set default values if they have not been set.
    if (empty($this->phoneCountryCode) || empty($this->phoneNumber)) {
      $phone = variable_get('whatsapp_sitewide_phone', '');
      $dissectPhone = $this->get_container('util')->dissect($phone);
      $this
        ->setPhoneCountryCode($dissectPhone['cc'])
        ->setPhoneNumber($dissectPhone['phone']);
    }
    if (empty($this->identity)) {
      $identifier = variable_get('whatsapp_sitewide_identifier', '');
      $this->setIdentity($identifier);
    }
    if (empty($this->phoneCountryCode) ||
        empty($this->phoneNumber) ||
        empty($this->identity)) {
      throw new ErrorException(t('Missing parameter required parameter.'), WhatsappController::missing, WATCHDOG_ERROR);
    }
    // Set default values if they have not been set.
    if (empty($this->userName)) {
      $this->setUserName(variable_get('whatsapp_sitewide_username', variable_get('site_name')));
    }
    if (empty($this->userStatus)) {
      $this->setUserStatus(variable_get('whatsapp_sitewide_status', 'Using Drupal WhatsApp!'));
    }
    if (empty($this->countryCode)) {
      $this->setCountryCode('US');
    }
    if (empty($this->langCode)) {
      $this->setLangCode('en');
    }
    if (empty($this->debug)) {
      $this->setDebug(FALSE);
    }
    return $this;
  }

  /**
   * Initializes the object.
   */
  public function init() {
    // Exclude setters from parent constructor.
    $dict = $this->get_container('util')->getDictionary();
    $this->writer = new BinTreeNodeWriter($dict);
    $this->reader = new BinTreeNodeReader($dict);
    $this->loginStatus = $this->disconnectedStatus;
  }

  /**
   * Prepares the connection to send a node.
   * 
   * @return WhatsappController
   *   Returns itself.
   */
  public function prepare() {
    if (empty($this->password)) {
      $credentials = $this->checkCredentials();
      if ($credentials->status == 'ok') {
        $this->setPassword($credentials->pw);
      }
      else {
        throw new ErrorException(t('Your credentias are invalids.'), WhatsappController::undefined, WATCHDOG_ERROR);
        if ($this->getDebug()) {
          watchdog('WhatsApp', t('Your credentias are invalids.'), array(), WATCHDOG_DEBUG);
        }
      }
    }
    if ($this->loginStatus == $this->disconnectedStatus) {
      $this->Connect();
      $this->Login();
    }
    return $this;
  }

  /**
   * .
   */
  public function Connect() { 
    $Socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    socket_connect($Socket, $this->whatsAppHost, $this->whatsAppPort);
    $this->socket = $Socket;
    socket_set_option($this->socket, SOL_SOCKET, SO_RCVTIMEO, $this->timeout);
  }

  /**
   * .
   */
  public function Login() {
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
    do {
      $this->processInboundData($this->readData());
    } while (($cnt++ < 100) && (strcmp($this->loginStatus, $this->disconnectedStatus) == 0));
    $this->sendNickname();
    $this->sendUserStatus();
    $this->SendPresence();
  }

  /**
   * .
   */
  protected function addFeatures() {
    $child = new ProtocolNode('receipt_acks', NULL, NULL, '');
    $parent = new ProtocolNode('stream:features', NULL, array($child), '');

    return $parent;
  }

  /**
   * .
   */
  protected function addAuth() {
    $authHash = array();
    $authHash['xmlns'] = 'urn:ietf:params:xml:ns:xmpp-sasl';
    $authHash['mechanism'] = 'WAUTH-1';
    $authHash['user'] = $this->phoneCountryCode . $this->phoneNumber;
    $node = new ProtocolNode('auth', $authHash, NULL, '');

    return $node;
  }

  /**
   * .
   */
  public function encryptPassword() {
    return base64_decode($this->password);
  }

  /**
   * .
   */
  protected function authenticate() {
    $key = $this->get_container('util')->pbkdf2('sha1', $this->encryptPassword(), $this->challengeData, 16, 20, true);
    $this->inputKey = new KeyStream($key);
    $this->outputKey = new KeyStream($key);
    $array = $this->phoneCountryCode . $this->phoneNumber . $this->challengeData . time();
    $response = $this->outputKey->encode($array, 0, strlen($array), false);

    return $response;
  }

  /**
   * .
   */
  public function addOutQueue($node) {
    $this->outQueue[] = $node;
  }

  /**
   * .
   */
  protected function addAuthResponse() {
    $resp = $this->authenticate();
    $respHash = array();
    $respHash['xmlns'] = 'urn:ietf:params:xml:ns:xmpp-sasl';
    $node = new ProtocolNode('response', $respHash, NULL, $resp);

    return $node;
  }

  /**
   * .
   */
  protected function sendData($data) {
    socket_send($this->socket, $data, strlen($data), 0);
  }	

  /**
   * .
   */
  protected function sendNode($node) {
    // @todo: Add debug.
    //$this->DebugPrint($node->NodeString("tx  ") . "\n");
    $this->sendData($this->writer->write($node));
  }

  /**
   * .
   */
  protected function readData() {
    $buff = '';
    $ret = socket_read($this->socket, 1024);
    if ($ret) {
      $buff = $this->incompleteMessage . $ret;
      $this->incompleteMessage = '';
    }
    else if ($this->KeepAlive) {
      // Reconnect on socket close.
      // @todo: Add sleeper, attemps and debug.
      $this->prepare();
    }

    return $buff;
  }

  /**
   * .
   */
  protected function processChallenge($node) {
    $this->challengeData = $node->_data;
  }

  /**
   * .
   */
  protected function sendMessageReceived($msg) {
    $requestNode = $msg->getChild('request');
    $receivedNode = $msg->getChild('received');
    if ($requestNode != NULL || $receivedNode != NULL) {
      $recievedHash = array();
      $recievedHash['xmlns'] = 'urn:xmpp:receipts';
      $receivedNode = new ProtocolNode('received', $recievedHash, NULL, '');

      $messageHash = array();
      $messageHash['to'] = $msg->getAttribute('from');
      $messageHash['type'] = 'chat';
      $messageHash['id'] = $msg->getAttribute('id');
      $messageHash['t'] = time();
      $messageNode = new ProtocolNode('message', $messageHash, array($receivedNode), '');
      $this->sendNode($messageNode);
    }
  }

  /**
   * .
   */
  protected function processInboundData($data) {
    try {
      $node = $this->reader->nextTree($data);
      while ($node != NULL) {
        // @todo: Add debug.
        //$this->DebugPrint($node->NodeString("rx  ") . "\n");
        if (strcmp($node->_tag, 'challenge') == 0) {
          $this->processChallenge($node);
        }
        else if (strcmp($node->_tag, 'success') == 0) {
          $this->loginStatus = $this->connectedStatus;
          $this->accountInfo = array(
            'status'=>$node->getAttribute('status'),
            'kind'=>$node->getAttribute('kind'),
            'creation'=>$node->getAttribute('creation'),
            'expiration'=>$node->getAttribute('expiration')
          );
        }
        if (strcmp($node->_tag, 'message') == 0) {
          array_push($this->messageQueue, $node);
          $this->sendMessageReceived($node);
          if ($node->hasChild('x') && $this->lastId==$node->getAttribute('id')) {
            $this->sendNext();
          }
          if($this->newMsgBind && $node->getChild('body')) {
            $this->newMsgBind->process($node);
          }
        }
        if (strcmp($node->_tag, 'iq') == 0 && strcmp($node->_attributeHash['type'], 'get') == 0 && strcmp($node->_children[0]->_tag, 'ping') == 0) {
          $this->Pong($node->_attributeHash['id']);
        }
        if (strcmp($node->_tag, 'iq') == 0 && strcmp($node->_attributeHash['type'], 'result') == 0 && strcmp($node->_children[0]->_tag, 'query') == 0) {
          array_push($this->messageQueue, $node);
        }
        $node = $this->reader->nextTree();
      }
    }
    catch (IncompleteMessageException $e) {
      $this->incompleteMessage = $e->getInput();
    }
  }

  /**
   * .
   */
  public function sendNext() {
    if (count($this->outQueue)>0) {
      $msgnode = array_shift($this->outQueue);
      $msgnode->refreshTimes();
      $this->lastId = $msgnode->getAttribute('id');
      $this->lastSent = $msgnode->getAttribute('id');
      $this->sendNode($msgnode);
    }
    else {
      $this->lastId = false;
    }
  }

  /**
   * .
   */
  public function sendComposing($msg) {
    $comphash = array();
    $comphash['xmlns'] = 'http://jabber.org/protocol/chatstates';
    $compose = new ProtocolNode('composing', $comphash, NULL, '');
    $messageHash = array();
    $messageHash['to'] = $msg->getAttribute('from');
    $messageHash['type'] = 'chat';
    $messageHash['id'] = time() . '-' . $this->msgCounter;
    $messageHash['t'] = time();
    $this->msgCounter++;
    $messageNode = new ProtocolNode('message', $messageHash, array($compose), '');
    $this->sendNode($messageNode);
  }

  /**
   * Pull from the socket, and place incoming messages in the message queue.
   */
  public function PollMessages() {
    $this->processInboundData($this->readData());
  }

  /**
   * Drain the message queue for application processing.
   */
  public function GetMessages() {
    $ret = $this->messageQueue;
    $this->messageQueue = array();

    return $ret;
  }

  /**
   * .
   */
  public function WaitforReceipt() {
    $received = false;
    do {
      $this->PollMessages();
      $msgs = $this->GetMessages();
      foreach ($msgs as $m) {
        // Process inbound messages.
        if ($m->_tag == 'message') {
          if ($m->getChild('received') != NULL){
            $received = TRUE;
          }
        }
      }
    } while(!$received);
  }

  /**
   * .
   */
  public function SendPresence($type = 'available') {
    $presence = array();
    $presence['type'] = $type;
    $presence['name'] = $this->userName;
    $node = new ProtocolNode("presence", $presence, NULL, '');
    $this->sendNode($node);
  }

  /**
   * .
   */
  protected function SendMessageNode($to, $node) {
    $serverNode = new ProtocolNode('server', NULL, NULL, '');
    $xHash = array();
    $xHash['xmlns'] = 'jabber:x:event';
    $xNode = new ProtocolNode('x', $xHash, array($serverNode), '');
    $notify = array();
    $notify['xmlns'] = 'urn:xmpp:whatsapp';
    $notify['name'] = $this->userName;
    $notnode = new ProtocolNode('notify', $notify, NULL, '');
    $request = array();
    $request['xmlns'] = 'urn:xmpp:receipts';
    $reqnode = new ProtocolNode('request', $request, NULL, '');
    $msgid = time() . '-' . $this->msgCounter;
    $whatsAppServer = $this->whatsAppServer;
    if (strpos($to, "-") !== FALSE) {
      $whatsAppServer = $this->whatsAppGroupServer;
    }
    $messageHash = array();
    $messageHash['to'] =  $to . "@" . $whatsAppServer;
    $messageHash['type'] = "chat";
    $messageHash['id'] = $msgid;
    $messageHash['t'] = time();
    $this->msgCounter++;
    $messsageNode = new ProtocolNode('message', $messageHash, array($xNode, $notnode,$reqnode,$node), '');
    if (!$this->lastId) {
      $this->lastId = $msgid;
      $this->lastSent = $msgid;
      $this->sendNode($messsageNode);
    }
    else {
      $this->outQueue[] = $messsageNode;
    }
  }

  /**
   * Sends a message.
   * 
   * This method is a wrapper that allows performing some setup and data caching
   * 
   * @param string $message
   *   Short text message.
   * 
   * @return WhatsappController
   *   Returns itself.
   */
  public function sendMessage($message) {
    if ($this->prepare()) {
      $this->Message($this->getDestination(), $message);
      return $this;
    }
    return FALSE;
  }

  /**
   * .
   */
  public function Message($to, $txt) {
    $bodyNode = new ProtocolNode('body', NULL, NULL, $txt);
    $this->SendMessageNode($to, $bodyNode);
  }

  /**
   * .
   */
  public function MessageImage($to, $url, $file, $size, $icon) {
    $mediaAttribs = array();
    $mediaAttribs['xmlns'] = 'urn:xmpp:whatsapp:mms';
    $mediaAttribs['type'] = 'image';
    $mediaAttribs['url'] = $url;
    $mediaAttribs['file'] = $file;
    $mediaAttribs['size'] = $size;

    $mediaNode = new ProtocolNode('media', $mediaAttribs, NULL, $icon);
    $this->SendMessageNode($to, $mediaNode);
  }

  /**
   * .
   */
  public function Location($msgid, $to, $long, $lat) {
    $whatsAppServer = $this->whatsAppServer;

    $mediaHash = array();
    $mediaHash['type'] = 'location';
    $mediaHash['longitude'] = $long;
    $mediaHash['latitude'] = $lat;
    $mediaHash['xmlns'] = 'urn:xmpp:whatsapp:mms';
    $mediaNode = new ProtocolNode('media', $mediaHash, NULL, NULL);

    $messageHash = array();
    $messageHash['to'] = $to . '@' . $whatsAppServer;
    $messageHash['type'] = 'chat';
    $messageHash['id'] = $msgid;
    $messageHash['author'] = $this->phoneCountryCode . $this->phoneNumber . '@' . $this->whatsAppServer;

    $messsageNode = new ProtocolNode('message', $messageHash, array($mediaNode), '');
    $this->sendNode($messsageNode);
  }

  /**
   * .
   */
  public function sendUserStatus() {
    $msgid = time() . '-' . $this->msgCounter;
    $this->sendStatusUpdate($msgid, $this->userStatus);
  }

  /**
   * .
   */
  public function sendStatusUpdate($txt) {
    $bodyNode = new ProtocolNode('body', NULL, NULL, $txt);
    $serverNode = new ProtocolNode('server', NULL, NULL, '');
    $xHash = array();
    $xHash['xmlns'] = 'jabber:x:event';
    $xNode = new ProtocolNode('x', $xHash, array($serverNode), '');

    $msgid = time() . '-' . $this->msgCounter;
    $messageHash = array();
    $messageHash['to'] = 's.us';
    $messageHash['type'] = 'chat';
    $messageHash['id'] = $msgid;
    $messsageNode = new ProtocolNode('message', $messageHash, array($xNode, $bodyNode), '');
    $this->sendNode($messsageNode);
  }

  /**
   * .
   */
  public function sendNickname() {
    $messageHash = array();
    $messageHash['name'] = $this->userName;
    $messsageNode = new ProtocolNode('presence', $messageHash, NULL, '');
    $this->sendNode($messsageNode);
  }

  /**
   * .
   */
  public function RequestLastSeen($to) {
    $whatsAppServer = $this->whatsAppServer;

    $queryHash = array();
    $queryHash['xmlns'] = 'jabber:iq:last';
    $queryNode = new ProtocolNode('query', $queryHash, NULL, NULL);

    $msgid = time() . '-' . $this->msgCounter;
    $messageHash = array();
    $messageHash['to'] = $to . '@' . $whatsAppServer;
    $messageHash['type'] = 'get';
    $messageHash['id'] = $msgid;
    $messageHash['from'] = $this->phoneCountryCode . $this->phoneNumber . '@' . $this->whatsAppServer;

    $messsageNode = new ProtocolNode("iq", $messageHash, array($queryNode), '');
    $this->sendNode($messsageNode);
  }

  /**
   * .
   */
  public function Pong($msgid) {
    $whatsAppServer = $this->whatsAppServer;

    $messageHash = array();
    $messageHash['to'] = $whatsAppServer;
    $messageHash['id'] = $msgid;
    $messageHash['type'] = 'result';
       
    $messsageNode = new ProtocolNode('iq', $messageHash, NULL, '');
    $this->sendNode($messsageNode);
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

  /**
   * .
   */  
  protected function getResponse($host, $query) {
    // Build the url.
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

    return drupal_json_decode($response);
  }


  // Setters and getters.

  /**
   * Global setter.
   * 
   * @param string $key
   *   Name of the property.
   * @param mixed $value
   *   Value to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  protected function set($key, $value) {
    // Check if the specified property is defined.
    if (!property_exists($this, $key)) {
      throw new ErrorException(t('The property %property is not defined.', array('%property' => $key)), WhatsappController::undefined, WATCHDOG_ERROR);
    }
    if ($this->getDebug()) {
      watchdog('WhatsApp', t('Variable %var has been set to: %val.', array('%var' => $key, '%val' => $value)), array(), WATCHDOG_DEBUG);
    }
    $this->{$key} = $value;
    return $this;
  }

  /**
   * Setter for WhatsApp::phoneCountryCode property
   * 
   * @param $phoneCountryCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setPhoneCountryCode($phoneCountryCode) {
    return $this->set('phoneCountryCode', $phoneCountryCode);
  }

  /**
   * Getter for WhatsApp::phoneCountryCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getPhoneCountryCode() {
    return $this->phoneCountryCode;
  }

  /**
   * Setter for WhatsApp::phoneNumber property
   * 
   * @param $phoneNumber
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setPhoneNumber($phoneNumber) {
    return $this->set('phoneNumber', $phoneNumber);
  }

  /**
   * Getter for WhatsApp::phoneNumber property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getPhoneNumber() {
    return $this->phoneNumber;
  }

  /**
   * Setter for WhatsApp::userName property
   * 
   * @param $userName
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setUserName($userName) {
    return $this->set('userName', $userName);
  }

  /**
   * Getter for WhatsApp::userName property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getUserName() {
    return $this->userName;
  }

  /**
   * Setter for WhatsApp::userStatus property
   * 
   * @param $userStatus
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setUserStatus($userStatus) {
    return $this->set('userStatus', $userStatus);
  }

  /**
   * Getter for WhatsApp::userStatus property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getUserStatus() {
    return $this->userStatus;
  }

  /**
   * Setter for WhatsApp::identity property
   * 
   * @param $identity
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setIdentity($Identity) {
    return $this->set('identity', $identity);
  }

  /**
   * Getter for WhatsApp::identity property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getIdentity() {
    return $this->identity;
  }

  /**
   * Setter for WhatsApp::password property
   * 
   * @param $password
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setPassword($password) {
    return $this->set('password', $password);
  }

  /**
   * Getter for WhatsApp::password property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getPassword() {
    return $this->password;
  }

  /**
   * Setter for WhatsApp::countryCode property
   * 
   * @param $countryCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setCountryCode($countryCode) {
    return $this->set('countryCode', $countryCode);
  }

  /**
   * Getter for WhatsApp::countryCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getCountryCode() {
    return $this->countryCode;
  }

  /**
   * Setter for WhatsApp::langCode property
   * 
   * @param $langCode
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setLangCode($langCode) {
    return $this->set('langCode', $langCode);
  }

  /**
   * Getter for WhatsApp::langCode property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getLangCode() {
    return $this->langCode;
  }

  /**
   * Setter for WhatsApp::destination property
   * 
   * @param $destination
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDestination($destination) {
    return $this->set('destination', $destination);
  }

  /**
   * Getter for WhatsApp::destination property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDestination() {
    return $this->destination;
  }

  /**
   * Setter for WhatsApp::debug property
   * 
   * @param $debug
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDebug($debug) {
    return $this->set('debug', $debug);
  }

  /**
   * Getter for WhatsApp::debug property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDebug() {
    return $this->debug;
  }

  /**
   * Setter for WhatsApp::whatsAppHost property
   * 
   * @param $whatsAppHost
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppHost($whatsAppHost) {
    return $this->set('whatsAppHost', $whatsAppHost);
  }

  /**
   * Getter for WhatsApp::whatsAppHost property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppHost() {
    return $this->whatsAppHost;
  }

  /**
   * Setter for WhatsApp::whatsAppServer property
   * 
   * @param $whatsAppServer
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppServer($whatsAppServer) {
    return $this->set('whatsAppServer', $whatsAppServer);
  }

  /**
   * Getter for WhatsApp::whatsAppServer property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppServer() {
    return $this->whatsAppServer;
  }

  /**
   * Setter for WhatsApp::whatsAppPort property
   * 
   * @param $whatsAppPort
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppPort($whatsAppPort) {
    return $this->set('whatsAppPort', $whatsAppPort);
  }

  /**
   * Getter for WhatsApp::whatsAppPort property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppPort() {
    return $this->whatsAppPort;
  }

  /**
   * Setter for WhatsApp::whatsAppVer property
   * 
   * @param $whatsAppVer
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppVer($whatsAppVer) {
    return $this->set('whatsAppVer', $whatsAppVer);
  }

  /**
   * Getter for WhatsApp::whatsAppVer property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppVer() {
    return $this->whatsAppVer;
  }

  /**
   * Setter for WhatsApp::timeout property
   * 
   * @param $timeout
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setTimeout($timeout) {
    return $this->set('timeout', $timeout);
  }

  /**
   * Getter for WhatsApp::timeout property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getTimeout() {
    return $this->timeout;
  }

  /**
   * Setter for WhatsApp::device property
   * 
   * @param $device
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDevice($device) {
    return $this->set('device', $device);
  }

  /**
   * Getter for WhatsApp::device property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDevice() {
    return $this->device;
  }

  /**
   * Setter for WhatsApp::incompleteMessage property
   * 
   * @param $incompleteMessage
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setIncompleteMessage($incompleteMessage) {
    return $this->set('incompleteMessage', $incompleteMessage);
  }

  /**
   * Getter for WhatsApp::incompleteMessage property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getIncompleteMessage() {
    return $this->incompleteMessage;
  }

  /**
   * Setter for WhatsApp::disconnectedStatus property
   * 
   * @param $disconnectedStatus
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setDisconnectedStatus($disconnectedStatus) {
    return $this->set('disconnectedStatus', $disconnectedStatus);
  }

  /**
   * Getter for WhatsApp::disconnectedStatus property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getDisconnectedStatus() {
    return $this->disconnectedStatus;
  }

  /**
   * Setter for WhatsApp::connectedStatus property
   * 
   * @param $connectedStatus
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setConnectedStatus($connectedStatus) {
    return $this->set('connectedStatus', $connectedStatus);
  }

  /**
   * Getter for WhatsApp::connectedStatus property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getConnectedStatus() {
    return $this->connectedStatus;
  }

  /**
   * Setter for WhatsApp::loginStatus property
   * 
   * @param $loginStatus
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setLoginStatus($loginStatus) {
    return $this->set('loginStatus', $loginStatus);
  }

  /**
   * Getter for WhatsApp::loginStatus property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getLoginStatus() {
    return $this->loginStatus;
  }

  /**
   * Setter for WhatsApp::accountInfo property
   * 
   * @param $accountInfo
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setAccountInfo($accountInfo) {
    return $this->set('accountInfo', $accountInfo);
  }

  /**
   * Getter for WhatsApp::accountInfo property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getAccountInfo() {
    return $this->accountInfo;
  }

  /**
   * Setter for WhatsApp::messageQueue property
   * 
   * @param $messageQueue
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setMessageQueue($messageQueue) {
    return $this->set('messageQueue', $messageQueue);
  }

  /**
   * Getter for WhatsApp::messageQueue property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getMessageQueue() {
    return $this->messageQueue;
  }

  /**
   * Setter for WhatsApp::outQueue property
   * 
   * @param $outQueue
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setOutQueue($outQueue) {
    return $this->set('outQueue', $outQueue);
  }

  /**
   * Getter for WhatsApp::outQueue property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getOutQueue() {
    return $this->outQueue;
  }

  /**
   * Setter for WhatsApp::lastId property
   * 
   * @param $lastId
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setLastId($lastId) {
    return $this->set('lastId', $lastId);
  }

  /**
   * Getter for WhatsApp::lastId property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getLastId() {
    return $this->lastId;
  }

  /**
   * Setter for WhatsApp::lastSent property
   * 
   * @param $lastSent
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setLastSent($lastSent) {
    return $this->set('lastSent', $lastSent);
  }

  /**
   * Getter for WhatsApp::lastSent property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getLastSent() {
    return $this->lastSent;
  }

  /**
   * Setter for WhatsApp::msgCounter property
   * 
   * @param $msgCounter
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setMsgCounter($msgCounter) {
    return $this->set('msgCounter', $msgCounter);
  }

  /**
   * Getter for WhatsApp::msgCounter property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getMsgCounter() {
    return $this->msgCounter;
  }

  /**
   * Setter for WhatsApp::socket property
   * 
   * @param $socket
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setSocket($socket) {
    return $this->set('socket', $socket);
  }

  /**
   * Getter for WhatsApp::socket property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getSocket() {
    return $this->socket;
  }

  /**
   * Setter for WhatsApp::KeepAlive property
   * 
   * @param $KeepAlive
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setKeepAlive($KeepAlive) {
    return $this->set('KeepAlive', $KeepAlive);
  }

  /**
   * Getter for WhatsApp::KeepAlive property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getKeepAlive() {
    return $this->KeepAlive;
  }

  /**
   * Setter for WhatsApp::writer property
   * 
   * @param $writer
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWriter($writer) {
    return $this->set('writer', $writer);
  }

  /**
   * Getter for WhatsApp::writer property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWriter() {
    return $this->writer;
  }

  /**
   * Setter for WhatsApp::reader property
   * 
   * @param $reader
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setReader($reader) {
    return $this->set('reader', $reader);
  }

  /**
   * Getter for WhatsApp::reader property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getReader() {
    return $this->reader;
  }

  /**
   * Setter for WhatsApp::inputKey property
   * 
   * @param $inputKey
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setInputKey($inputKey) {
    return $this->set('inputKey', $inputKey);
  }

  /**
   * Getter for WhatsApp::inputKey property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getInputKey() {
    return $this->inputKey;
  }

  /**
   * Setter for WhatsApp::outputKey property
   * 
   * @param $outputKey
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setOutputKey($outputKey) {
    return $this->set('outputKey', $outputKey);
  }

  /**
   * Getter for WhatsApp::outputKey property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getOutputKey() {
    return $this->outputKey;
  }

  /**
   * Setter for WhatsApp::newMsgBind property
   * 
   * @param $newMsgBind
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setNewMsgBind($newMsgBind) {
    return $this->set('newMsgBind', $newMsgBind);
  }

  /**
   * Getter for WhatsApp::newMsgBind property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getNewMsgBind() {
    return $this->newMsgBind;
  }

  /**
   * Setter for WhatsApp::whatsAppReqHost property
   * 
   * @param $whatsAppReqHost
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppReqHost($whatsAppReqHost) {
    return $this->set('whatsAppReqHost', $whatsAppReqHost);
  }

  /**
   * Getter for WhatsApp::whatsAppReqHost property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppReqHost() {
    return $this->whatsAppReqHost;
  }

  /**
   * Setter for WhatsApp::whatsAppRegHost property
   * 
   * @param $whatsAppRegHost
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppRegHost($whatsAppRegHost) {
    return $this->set('whatsAppRegHost', $whatsAppRegHost);
  }

  /**
   * Getter for WhatsApp::whatsAppRegHost property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppRegHost() {
    return $this->whatsAppRegHost;
  }

  /**
   * Setter for WhatsApp::whatsAppCheHost property
   * 
   * @param $whatsAppCheHost
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppCheHost($whatsAppCheHost) {
    return $this->set('whatsAppCheHost', $whatsAppCheHost);
  }

  /**
   * Getter for WhatsApp::whatsAppCheHost property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppCheHost() {
    return $this->whatsAppCheHost;
  }

  /**
   * Setter for WhatsApp::whatsAppUserAgent property
   * 
   * @param $whatsAppUserAgent
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppUserAgent($whatsAppUserAgent) {
    return $this->set('whatsAppUserAgent', $whatsAppUserAgent);
  }

  /**
   * Getter for WhatsApp::whatsAppUserAgent property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppUserAgent() {
    return $this->whatsAppUserAgent;
  }

  /**
   * Setter for WhatsApp::whatsAppToken property
   * 
   * @param $whatsAppToken
   *   The property to set.
   * 
   * @return WhatsappController
   *   Return itself.
   * 
   * @throws ErrorException
   */
  public function setWhatsAppToken($whatsAppToken) {
    return $this->set('whatsAppToken', $whatsAppToken);
  }

  /**
   * Getter for WhatsApp::whatsAppToken property
   * 
   * @return string
   *   Return the requested property.
   */
  public function getWhatsAppToken() {
    return $this->whatsAppToken;
  }
}
