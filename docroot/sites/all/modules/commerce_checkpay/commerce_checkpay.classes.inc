<?php
/**
 * @file
 * CheckPay classes provided by PostNL and published under the GPLv2+ license.
 */

class StartTransactionRequest {
  public $Transaction;
  public $EntranceCode;
  public $ReturnURL;
  /**
   * Transform object to XML string.
   *
   * @param string $is_exec
   *   Param: $is_exec.
   * @param string $id
   *   Param: $id.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString($is_exec, $id) {
    if ($is_exec) {
      $return_str = '<soap:Body xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $id . '">';
    }
    else {
      $return_str = '<soap:Body wsu:Id="' . $id . '">';
    }

    $class_vars = get_class_vars(get_class($this));

    $return_str .= sprintf('<%s xmlns="http://nl.tntpost.parcels.eCommerce.onlineCod.datatypes">', get_class($this));
    foreach ($class_vars as $name => $value) {
      if (method_exists($this->$name, "ToXmlString")) {
        $return_str .= $this->$name->ToXmlString();
      }
      else {
        $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
      }
    }
    $return_str .= sprintf("</%s>", get_class($this));
    $return_str .= '</soap:Body>';

    return $return_str;
  }
}
class Transaction {
  public $Context;
  public $ClientNr;
  public $OrderNr;
  public $OrderDescription;
  public $OrderAmount;
  public $Buyer;
  public $PSP;
  public $PSPClientNr;
  /**
   * Transform object to XML string.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString() {

    $class_vars = get_class_vars(get_class($this));
    $return_str = sprintf("<%s>", get_class($this));
    foreach ($class_vars as $name => $value) {
      if (method_exists($this->$name, "ToXmlString")) {
        $return_str .= $this->$name->ToXmlString();
      }
      else {

        $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
      }
    }
    $return_str .= sprintf("</%s>", get_class($this));
    return $return_str;
  }
}
class PSP {
  public $Code;
  public $Description;
  /**
   * Transform object to XML string.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString() {

    $class_vars = get_class_vars(get_class($this));

    $return_str = sprintf("<%s>", get_class($this));
    foreach ($class_vars as $name => $value) {
      $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
    }
    $return_str .= sprintf("</%s>", get_class($this));
    return $return_str;
  }
}
class CodeDescription {
  public $Code;
  public $Description;
  /**
   * Constructor.
   *
   * @param array $aa
   *   Param: $aa.
   */
  public function __construct($aa = NULL) {
    if ($aa == NULL) {
      return;
    }
    foreach ($aa as $k => $v) {
      $this->$k = $aa[$k];
    };
  }
}
class Context {
  public $Country;
  public $Language;
  public $Currency;
  /**
   * Constructor.
   *
   * @param array $aa
   *   Param: $aa.
   */
  public function __construct($aa = NULL) {
    if ($aa == NULL) {
      return;
    }
    foreach ($aa as $k => $v) {
      $this->$k = $aa[$k];
    };
  }
  /**
   * Transform object to XML string.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString() {

    $class_vars = get_class_vars(get_class($this));

    $return_str = sprintf("<%s>", get_class($this));
    foreach ($class_vars as $name => $value) {
      $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
    }
    $return_str .= sprintf("</%s>", get_class($this));
    return $return_str;
  }
}
class Buyer {
  public $Name;
  public $City;
  public $EMailAddress;
  public $SMSPhoneNr;
  /**
   * Constructor.
   *
   * @param array $aa
   *   Param: $aa.
   */
  public function __construct($aa = NULL) {
    if ($aa == NULL) {
      return;
    }
    foreach ($aa as $k => $v) {
      $this->$k = $aa[$k];
    };
  }
  /**
   * Transform object to XML string.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString() {

    $class_vars = get_class_vars(get_class($this));

    $return_str = sprintf("<%s>", get_class($this));
    foreach ($class_vars as $name => $value) {
      $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
    }
    $return_str .= sprintf("</%s>", get_class($this));
    return $return_str;
  }

}

class StartTransactiontResponse {
  public $TransactionNr;
  public $PaymentURL;
  public $ErrorType;
  /**
   * Constructor.
   *
   * @param array $aa
   *   Param: $aa.
   */
  public function __construct($aa = NULL) {
    if ($aa == NULL) {
      return;
    }
    foreach ($aa as $k => $v) {
      $this->$k = $aa[$k];
    };
  }
  /**
   * Change xml string to object.
   *
   * @param string $xml_string
   *   A well-formed xml string.
   *
   * @return object
   *   An object of the xml string.
   */
  public function ParseFromXml($xml_string) {
    $parser = xml_parser_create();
    xml_parser_set_option($parser, XML_OPTION_CASE_FOLDING, 0);
    xml_parser_set_option($parser, XML_OPTION_SKIP_WHITE, 1);
    xml_parse_into_struct($parser, $xml_string, $values, $tags);
    xml_parser_free($parser);

    foreach ($tags as $key => $val) {
      if ($key == get_class($this)) {

        $molranges = $val;
        // Each contiguous pair of array entries are the
        // lower and upper range for each molecule definition.
        for ($i = 0; $i < count($molranges); $i += 2) {
          $offset = $molranges[$i] + 1;
          $len = $molranges[$i + 1] - $offset;
          $tdb[] = $this->parseEntity(array_slice($values, $offset, $len));
        }
      }
      else {
        continue;
      }
    }
    if (count($tdb) > 0) {
      return $tdb[0];
    }

  }
}
class ErrorType {
  public $Code;
  public $Description;

}
class TransactionInfo {
  public $TransactionNr;
  public $TransactionDt;
  public $Context;
  public $ClientNr;
  public $OrderNr;
  public $OrderDescription;
  public $OrderAmount;
  public $PSP;
  public $IntermediateParty;
  public $ExpirationDt;
  public $OrderPaymentStatus;
}
class TransactionInfoRequest {
  public $TransactionNr;
  public $ClientNr;
  /**
   * Transform object to XML string.
   *
   * @param string $is_exec
   *   Param: $is_exec.
   * @param string $id
   *   Param: $id.
   *
   * @return string
   *   An xml string.
   */
  public function ToXmlString($is_exec, $id) {
    if (!$is_exec) {
      $return_str = '<soap:Body wsu:Id="' . $id . '">';
    }
    else {
      $return_str = '<soap:Body xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $id . '">';
    }

    $class_vars = get_class_vars(get_class($this));

    $return_str .= sprintf('<%s xmlns="http://nl.tntpost.parcels.eCommerce.onlineCod.datatypes">', get_class($this));

    foreach ($class_vars as $name => $value) {
      if (method_exists($this->$name, "ToXmlString")) {
        $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name->ToXmlString(), $name);
      }
      else {
        $return_str .= sprintf("<%s>%s</%s>", $name, $this->$name, $name);
      }
    }
    $return_str .= sprintf("</%s>", get_class($this));
    $return_str .= '</soap:Body>';
    return $return_str;
  }

}
class TransactionInfoResponse {
  public $ErrorType;
  public $Transaction;
  /**
   * Constructor.
   *
   * @param array $aa
   *   Param: $aa.
   */
  public function __construct($aa = NULL) {
    if ($aa == NULL) {
      return;
    }
    foreach ($aa as $k => $v) {
      $this->$k = $aa[$k];
    };
  }
  /**
   * Change xml string to object.
   *
   * @param string $xml_string
   *   A well-formed xml string.
   *
   * @return object
   *   An object of the xml string.
   */
  public function ParseFromXml($xml_string) {
    $xml = simplexml_load_string($xml_string);
    return $xml;
  }
}
class SecurityXML extends SoapClient {
  /**
   * Constructor.
   *
   * @param mixed $wsdl
   *   The endpoint.
   * @param array $options
   *   Extra options for calling the web service.
   */
  public function __construct($wsdl, $options = NULL) {
    if ($options != NULL) {
      parent::__construct($wsdl, $options);
    }
    else {
      parent::__construct($wsdl);
    }
    $this->url = substr($wsdl, 0, strlen($wsdl) - 5);
    $this->time = time();
  }
  /**
   * Do SOAP request.
   *
   * @param string $request
   *   The XML SOAP request.
   * @param string $location
   *   The URL to request.
   * @param string $action
   *   The SOAP action.
   * @param int $version
   *   The SOAP version.
   *
   * @return mixed
   *   The XML SOAP response.
   */
  public function __doRequest($request, $location, $action, $version) {
    $this->action = $action;
    $request = $this->FormatSecurityMessage();
    $ret = parent::__doRequest($request, $location, $action, $version);
    // Update internal __last_request variable.
    $this->__last_request = $request;

    return $ret;
  }

  public $action;
  public $time;
  public $bodyEntity;
  public $url;
  public $actionId = 'Id-7de19a0a-8083-4e97-af3a-34f65c913598';
  public $messageId = 'Id-9322f595-2511-4c3f-bd82-843ef0fad864';
  public $messageUid = '09da5698-ea78-487e-9f24-10c2248a884d';
  public $replyToId = 'Id-3489eab2-f7e5-4bbe-8968-d9da28fd5c80';
  public $toId = 'Id-023899bd-f211-4c1b-bda3-42cdadd217cf';
  public $TimestampId = 'Timestamp-9073a7ac-aa94-48da-a01b-205432a45e8f';
  public $bodyId = 'Id-fbd10a6f-33fc-4d93-8e91-a07a64ad5e74';
  /**
   * Get action.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   XML string containing the action value.
   */
  public function GetAction($is_exec) {
    if ($is_exec) {
      return '<wsa:Action xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $this->actionId . '">' . $this->action . '</wsa:Action>';
    }
    else {
      return '<wsa:Action wsu:Id="' . $this->actionId . '">' . $this->action . '</wsa:Action>';
    }
  }
  /**
   * Get message ID.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   XML string containing the message ID value.
   */
  public function GetMessageID($is_exec) {
    if ($is_exec) {
      return '<wsa:MessageID xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $this->messageId . '">urn:uuid:' . $this->messageUid . '</wsa:MessageID>';
    }
    else {
      return '<wsa:MessageID wsu:Id="' . $this->messageId . '">urn:uuid:' . $this->messageUid . '</wsa:MessageID>';
    }
  }
  /**
   * Get replyTo.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   XML string containing the reply to value.
   */
  public function GetReplyTo($is_exec) {
    if ($is_exec) {
      return '<wsa:ReplyTo xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $this->replyToId . '"><wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address></wsa:ReplyTo>';
    }
    else {
      return '<wsa:ReplyTo wsu:Id="' . $this->replyToId . '"><wsa:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:Address></wsa:ReplyTo>';
    }
  }
  /**
   * Get to.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   XML string containing the 'to' value.
   */
  public function GetTo($is_exec) {
    if ($is_exec) {
      return '<wsa:To xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $this->toId . '">' . $this->url . '</wsa:To>';
    }
    else {
      return '<wsa:To wsu:Id="' . $this->toId . '">' . $this->url . '</wsa:To>';
    }
  }
  /**
   * Get timestamp.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   XML string containing current timestamp.
   */
  public function GetTimestamp($is_exec) {
    date_default_timezone_set('UTC');
    if ($is_exec) {
      return '<wsu:Timestamp xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="' . $this->TimestampId . '"><wsu:Created>' . $this->FormatTime($this->time) . '</wsu:Created><wsu:Expires>' . $this->FormatTime($this->time + 5 * 60) . '</wsu:Expires></wsu:Timestamp>';
    }
    else {
      return '<wsu:Timestamp wsu:Id="' . $this->TimestampId . '"><wsu:Created>' . $this->FormatTime($this->time) . '</wsu:Created><wsu:Expires>' . $this->FormatTime($this->time + 5 * 60) . '</wsu:Expires></wsu:Timestamp>';
    }
  }
  /**
   * Format time.
   *
   * @param string $time
   *   Param: $time.
   *
   * @return string
   *   Time in ISO 8601 format.
   */
  public function FormatTime($time) {
    return substr(date('c', $time), 0, 19) . 'Z';
  }
  /**
   * Get the signed info required in the XML request.
   *
   * @param bool $is_exec
   *   Param: $is_exec.
   *
   * @return string
   *   The signed information of the XML request.
   */
  public function GetSignedInfo($is_exec) {
    if ($is_exec) {
      $signed_info = '<SignedInfo xmlns="http://www.w3.org/2000/09/xmldsig#">';
    }
    else {
      $signed_info = '<SignedInfo>';
    }
    if ($is_exec) {
      $signed_info .= '<ds:CanonicalizationMethod xmlns:ds="http://www.w3.org/2000/09/xmldsig#" Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></ds:CanonicalizationMethod>';
    }
    else {
      $signed_info .= '<ds:CanonicalizationMethod Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" />';
    }
    if ($is_exec) {
      $signed_info .= '<SignatureMethod Algorithm="http://www.w3.org/2000/09/xmldsig#rsa-sha1"></SignatureMethod>';
    }
    else {
      $signed_info .= '<SignatureMethod Algorithm="http://www.w3.org/2000/09/xmldsig#rsa-sha1" />';
    }
    // Digest action.
    $signed_info .= '<Reference URI="#' . $this->actionId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->GetAction(TRUE), TRUE)) . '</DigestValue></Reference>';
    // Digest MessageID.
    $signed_info .= '<Reference URI="#' . $this->messageId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->GetMessageID(TRUE), TRUE)) . '</DigestValue></Reference>';
    // Digest ReplyTo.
    $signed_info .= '<Reference URI="#' . $this->replyToId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->GetReplyTo(TRUE), TRUE)) . '</DigestValue></Reference>';
    // Digest To.
    $signed_info .= '<Reference URI="#' . $this->toId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->GetTo(TRUE), TRUE)) . '</DigestValue></Reference>';
    // Digest TimeStamp.
    $signed_info .= '<Reference URI="#' . $this->TimestampId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->GetTimestamp(TRUE), TRUE)) . '</DigestValue></Reference>';

    // Digest Body.
    $signed_info .= '<Reference URI="#' . $this->bodyId . '"><Transforms>';
    if ($is_exec) {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#"></Transform>';
    }
    else {
      $signed_info .= '<Transform Algorithm="http://www.w3.org/2001/10/xml-exc-c14n#" />';
    }
    $signed_info .= '</Transforms>';
    if ($is_exec) {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1"></DigestMethod>';
    }
    else {
      $signed_info .= '<DigestMethod Algorithm="http://www.w3.org/2000/09/xmldsig#sha1" />';
    }
    $signed_info .= '<DigestValue>' . base64_encode(sha1($this->bodyEntity->ToXmlString(TRUE, $this->bodyId), TRUE)) . '</DigestValue></Reference>';

    $signed_info .= '</SignedInfo>';

    return $signed_info;
  }
  /**
   * Format the full signed XML message.
   *
   * @return string
   *   Signed XML string.
   */
  public function FormatSecurityMessage() {
    $message = '<?xml version="1.0" encoding="UTF-8"?>';
    $message .= '<soap:Envelope xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd">';
    $message .= "<soap:Header>";
    $message .= $this->GetAction(FALSE);
    $message .= $this->GetMessageID(FALSE);
    $message .= $this->GetReplyTo(FALSE);
    $message .= $this->GetTo(FALSE);
    $message .= '<wsse:Security soap:mustUnderstand="1">';
    $message .= $this->GetTimestamp(FALSE);
    $message .= '<wsse:BinarySecurityToken ValueType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-x509-token-profile-1.0#X509v3" EncodingType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" wsu:Id="SecurityToken-6f5a67e5-173d-4245-b4d1-3542a7715be4">' . $this->GetPublicKey() . '</wsse:BinarySecurityToken>';
    $message .= '<Signature xmlns="http://www.w3.org/2000/09/xmldsig#">';
    $message .= $this->GetSignedInfo(FALSE);
    $message .= '<SignatureValue>' . $this->Signature($this->GetSignedInfo(TRUE)) . '</SignatureValue>';
    $message .= '<KeyInfo><wsse:SecurityTokenReference><wsse:Reference URI="#SecurityToken-6f5a67e5-173d-4245-b4d1-3542a7715be4" ValueType="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-x509-token-profile-1.0#X509v3" /></wsse:SecurityTokenReference></KeyInfo></Signature></wsse:Security></soap:Header>';
    $message .= $this->bodyEntity->ToXmlString(FALSE, $this->bodyId);
    $message .= '</soap:Envelope>';
    return $message;
  }
  /**
   * Sign a string using a private key.
   *
   * @param string $sourcestr
   *   Source to be signed.
   *
   * @return string
   *   Signed string.
   */
  public function Signature($sourcestr) {
    // Load payment method instance.
    $payment_method = commerce_payment_method_instance_load('commerce_checkpay|commerce_payment_commerce_checkpay');

    $file_name = $payment_method['settings']['certs_private_key'];
    $fp = fopen($file_name, 'r');
    if (!$fp) {
      return FALSE;
    }
    $priv_key = fread($fp, 8192);
    fclose($fp);

    // Get private key.
    $pkeyid = openssl_get_privatekey($priv_key, $payment_method['settings']['key_password']);

    // Compute signature.
    $signature = '';
    openssl_sign($sourcestr, $signature, $pkeyid, OPENSSL_ALGO_SHA1);
    // Free the key from memory.
    openssl_free_key($pkeyid);

    return base64_encode($signature);
  }
  /**
   * Get public key.
   *
   * @return string
   *   A string containing the public key.
   */
  public function GetPublicKey() {
    // Load payment method instance.
    $payment_method = commerce_payment_method_instance_load('commerce_checkpay|commerce_payment_commerce_checkpay');

    $file_name = $payment_method['settings']['certs_public_cert'];
    $fp = fopen($file_name, "r");
    if (!$fp) {
      return FALSE;
    }
    $cert = fread($fp, 8192);
    fclose($fp);

    $data = openssl_x509_read($cert);

    if (!openssl_x509_export($data, $data)) {
      return FALSE;
    }

    $data = str_replace("-----BEGIN CERTIFICATE-----", "", $data);
    $data = str_replace("-----END CERTIFICATE-----", "", $data);
    $data = str_replace("\n", "", $data);

    return $data;

  }
}
