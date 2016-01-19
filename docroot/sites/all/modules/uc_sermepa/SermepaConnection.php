<?php


class SermepaConnection{

  private $transactionParams;

  // Public api

  public function setTransactionParam( $paramName, $value ){
    $this->transactionParams[ $paramName] = $value ;
  }

  public function getRequestData(){
    return base64_encode( json_encode($this->transactionParams) );
  }

  public function getSignature( $encryptionKey ){
    $key = base64_decode($encryptionKey);
    $dsMerchantParameters = $this->getRequestData();
    $newKey = $this->encryptOrderCode( $key, $this->getOrderCode() );
    $dsMerchanSignature = hash_hmac('sha256', $dsMerchantParameters, $newKey, true);
    return base64_encode($dsMerchanSignature);
  }

  public function getResponseSignature( $encryptionKey, $responseData ){
    $key = base64_decode($encryptionKey);
    $dsMerchantParameters = $this->decodeResponse( $responseData );
    $this->transactionParams = json_decode($dsMerchantParameters, true);
    $newKey = $this->encryptOrderCode( $key, $this->getOrderFromResponse() );
    $dsMerchanSignature = hash_hmac('sha256', $responseData, $newKey, true);
    return strtr(base64_encode($dsMerchanSignature), '+/', '-_');
  }

  public function decodeResponse( $responseData ){
    return base64_decode(strtr($responseData, '-_', '+/'));
  }


  // Helper Functions

  private function getOrderCode(){
    if( empty($this->transactionParams['DS_MERCHANT_ORDER']) ){
      return $this->transactionParams['Ds_Merchant_Order'];
    } else {
      return $this->transactionParams['DS_MERCHANT_ORDER'];
    }
  }

  private function getOrderFromResponse(){
    if( empty($this->transactionParams['Ds_Order']) ){
			return $this->transactionParams['DS_ORDER'];
		} else {
			return $this->transactionParams['Ds_Order'];
		}
  }

  private function encryptOrderCode( $key, $orderCode ){
    $iv = implode(array_map("chr", array(0,0,0,0,0,0,0,0)));
    $ciphertext = mcrypt_encrypt(MCRYPT_3DES, $key, $orderCode, MCRYPT_MODE_CBC, $iv);
    return $ciphertext;
  }

}
