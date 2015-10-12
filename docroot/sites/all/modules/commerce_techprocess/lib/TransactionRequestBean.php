<?php

require_once 'RequestValidate.php';
require_once 'AES.php';

/**
 * Version 1.0
 */
class TransactionRequestBean
{

    private $tilda = "~";

    private $separator = "|";

    private $requestType = "";

    private $merchantCode = "";

    private $merchantTxnRefNumber = "";

    private $ITC = "";

    private $amount = "";

	private $accountNo = "";

    private $currencyCode = "";

    private $uniqueCustomerId = "";

    private $returnURL = "";

    private $s2SReturnURL = "";

    private $TPSLTxnID = "";

    private $shoppingCartDetails = "";

    private $txnDate = "";

    private $email = "";

    private $mobileNumber = "";

    private $socialMediaIdentifier = "";

    private $bankCode = "";

    private $customerName = "";

    private $reqst = null;

    private $webServiceLocator = "NA";

    private $MMID = "";

    private $OTP = "";

    private $key;

    private $iv;

    static $mkd;

    private $blockSize = 128;

    private $mode = "cbc";

    private $logPath = "";

    static $currDate;

    private static $rqst_kit_vrsn = 1;

    private $custId = "";

    private $cardId = "";

    private $cardNo = "";

    private $cardName = "";

    private $cardCVV = "";

    private $cardExpMM = "";

    private $cardExpYY = "";

    private $timeOut = 30;

    /**
     * @return the $tilda
     */
    public function getTilda()
    {
        return $this->tilda;
    }

	/**
     * @return the $separator
     */
    public function getSeparator()
    {
        return $this->separator;
    }

	/**
     * @return the $requestType
     */
    public function getRequestType()
    {
        return $this->requestType;
    }

	/**
     * @return the $merchantCode
     */
    public function getMerchantCode()
    {
        return $this->merchantCode;
    }

	/**
     * @return the $accountNo
     */
    public function getAccountNo()
    {
        return $this->accountNo;
    }

	/**
     * @return the $merchantTxnRefNumber
     */
    public function getMerchantTxnRefNumber()
    {
        return $this->merchantTxnRefNumber;
    }

	/**
     * @return the $ITC
     */
    public function getITC()
    {
        return $this->ITC;
    }

	/**
     * @return the $amount
     */
    public function getAmount()
    {
        return $this->amount;
    }

	/**
     * @return the $currencyCode
     */
    public function getCurrencyCode()
    {
        return $this->currencyCode;
    }

	/**
     * @return the $uniqueCustomerId
     */
    public function getUniqueCustomerId()
    {
        return $this->uniqueCustomerId;
    }

	/**
     * @return the $returnURL
     */
    public function getReturnURL()
    {
        return $this->returnURL;
    }

	/**
     * @return the $s2SReturnURL
     */
    public function getS2SReturnURL()
    {
        return $this->s2SReturnURL;
    }

	/**
     * @return the $TPSLTxnID
     */
    public function getTPSLTxnID()
    {
        return $this->TPSLTxnID;
    }

	/**
     * @return the $shoppingCartDetails
     */
    public function getShoppingCartDetails()
    {
        return $this->shoppingCartDetails;
    }

	/**
     * @return the $txnDate
     */
    public function getTxnDate()
    {
        return $this->txnDate;
    }

	/**
     * @return the $email
     */
    public function getEmail()
    {
        return $this->email;
    }

	/**
     * @return the $mobileNumber
     */
    public function getMobileNumber()
    {
        return $this->mobileNumber;
    }

	/**
     * @return the $socialMediaIdentifier
     */
    public function getSocialMediaIdentifier()
    {
        return $this->socialMediaIdentifier;
    }

	/**
     * @return the $bankCode
     */
    public function getBankCode()
    {
        return $this->bankCode;
    }

	/**
     * @return the $customerName
     */
    public function getCustomerName()
    {
        return $this->customerName;
    }

	/**
     * @return the $reqst
     */
    public function getReqst()
    {
        return $this->reqst;
    }

	/**
     * @return the $webServiceLocator
     */
    public function getWebServiceLocator()
    {
        return $this->webServiceLocator;
    }

	/**
     * @return the $MMID
     */
    public function getMMID()
    {
        return $this->MMID;
    }

	/**
     * @return the $OTP
     */
    public function getOTP()
    {
        return $this->OTP;
    }

	/**
     * @return the $key
     */
    public function getKey()
    {
        return $this->key;
    }

	/**
     * @return the $iv
     */
    public function getIv()
    {
        return $this->iv;
    }

	/**
     * @return the $mkd
     */
    public static function getMkd()
    {
        return TransactionRequestBean::$mkd;
    }

	/**
     * @return the $blockSize
     */
    public function getBlockSize()
    {
        return $this->blockSize;
    }

	/**
     * @return the $mode
     */
    public function getMode()
    {
        return $this->mode;
    }

	/**
     * @return the $logPath
     */
    public function getLogPath()
    {
        return $this->logPath;
    }

	/**
     * @return the $currDate
     */
    public static function getCurrDate()
    {
        return TransactionRequestBean::$currDate;
    }

	/**
     * @return the $rqst_kit_vrsn
     */
    public static function getRqst_kit_vrsn()
    {
        return TransactionRequestBean::$rqst_kit_vrsn;
    }

	/**
     * @return the $custId
     */
    public function getCustId()
    {
        return $this->custId;
    }

	/**
     * @return the $cardId
     */
    public function getCardId()
    {
        return $this->cardId;
    }

	/**
     * @return the $cardNo
     */
    public function getCardNo()
    {
        return $this->cardNo;
    }

	/**
     * @return the $cardName
     */
    public function getCardName()
    {
        return $this->cardName;
    }

	/**
     * @return the $cardCVV
     */
    public function getCardCVV()
    {
        return $this->cardCVV;
    }

	/**
     * @return the $cardExpMM
     */
    public function getCardExpMM()
    {
        return $this->cardExpMM;
    }

	/**
     * @return the $cardExpYY
     */
    public function getCardExpYY()
    {
        return $this->cardExpYY;
    }

	/**
     * @return the $timeOut
     */
    public function getTimeOut()
    {
        return $this->timeOut;
    }

	/**
     * @param string $tilda
     */
    public function setTilda($tilda)
    {
        $this->tilda = $tilda;
    }

	/**
     * @param string $separator
     */
    public function setSeparator($separator)
    {
        $this->separator = $separator;
    }

	/**
     * @param string $requestType
     */
    public function setRequestType($requestType)
    {
        $this->requestType = $requestType;
    }

	/**
     * @param string $merchantCode
     */
    public function setMerchantCode($merchantCode)
    {
        $this->merchantCode = $merchantCode;
    }

	/**
     * @param string $accountNo
     */
    public function setAccountNo($accountNo)
    {
        $this->accountNo = $accountNo;
    }

	/**
     * @param string $merchantTxnRefNumber
     */
    public function setMerchantTxnRefNumber($merchantTxnRefNumber)
    {
        $this->merchantTxnRefNumber = $merchantTxnRefNumber;
    }

	/**
     * @param string $ITC
     */
    public function setITC($ITC)
    {
        $this->ITC = $ITC;
    }

	/**
     * @param string $amount
     */
    public function setAmount($amount)
    {
        $this->amount = $amount;
    }

	/**
     * @param string $currencyCode
     */
    public function setCurrencyCode($currencyCode)
    {
        $this->currencyCode = $currencyCode;
    }

	/**
     * @param string $uniqueCustomerId
     */
    public function setUniqueCustomerId($uniqueCustomerId)
    {
        $this->uniqueCustomerId = $uniqueCustomerId;
    }

	/**
     * @param string $returnURL
     */
    public function setReturnURL($returnURL)
    {
        $this->returnURL = $returnURL;
    }

	/**
     * @param string $s2SReturnURL
     */
    public function setS2SReturnURL($s2SReturnURL)
    {
        $this->s2SReturnURL = $s2SReturnURL;
    }

	/**
     * @param string $TPSLTxnID
     */
    public function setTPSLTxnID($TPSLTxnID)
    {
        $this->TPSLTxnID = $TPSLTxnID;
    }

	/**
     * @param string $shoppingCartDetails
     */
    public function setShoppingCartDetails($shoppingCartDetails)
    {
        $this->shoppingCartDetails = $shoppingCartDetails;
    }

	/**
     * @param string $txnDate
     */
    public function setTxnDate($txnDate)
    {
        $this->txnDate = $txnDate;
    }

	/**
     * @param string $email
     */
    public function setEmail($email)
    {
        $this->email = $email;
    }

	/**
     * @param string $mobileNumber
     */
    public function setMobileNumber($mobileNumber)
    {
        $this->mobileNumber = $mobileNumber;
    }

	/**
     * @param string $socialMediaIdentifier
     */
    public function setSocialMediaIdentifier($socialMediaIdentifier)
    {
        $this->socialMediaIdentifier = $socialMediaIdentifier;
    }

	/**
     * @param string $bankCode
     */
    public function setBankCode($bankCode)
    {
        $this->bankCode = $bankCode;
    }

	/**
     * @param string $customerName
     */
    public function setCustomerName($customerName)
    {
        $this->customerName = $customerName;
    }

	/**
     * @param NULL $reqst
     */
    public function setReqst($reqst)
    {
        $this->reqst = $reqst;
    }

	/**
     * @param string $webServiceLocator
     */
    public function setWebServiceLocator($webServiceLocator)
    {
        $this->webServiceLocator = $webServiceLocator;
    }

	/**
     * @param string $MMID
     */
    public function setMMID($MMID)
    {
        $this->MMID = $MMID;
    }

	/**
     * @param string $OTP
     */
    public function setOTP($OTP)
    {
        $this->OTP = $OTP;
    }

	/**
     * @param field_type $key
     */
    public function setKey($key)
    {
        $this->key = $key;
    }

	/**
     * @param field_type $iv
     */
    public function setIv($iv)
    {
        $this->iv = $iv;
    }

	/**
     * @param field_type $mkd
     */
    public static function setMkd($mkd)
    {
        TransactionRequestBean::$mkd = $mkd;
    }

	/**
     * @param number $blockSize
     */
    public function setBlockSize($blockSize)
    {
        $this->blockSize = $blockSize;
    }

	/**
     * @param string $mode
     */
    public function setMode($mode)
    {
        $this->mode = $mode;
    }

	/**
     * @param string $logPath
     */
    public function setLogPath($logPath)
    {
        $this->logPath = $logPath;
    }

	/**
     * @param field_type $currDate
     */
    public static function setCurrDate($currDate)
    {
        TransactionRequestBean::$currDate = $currDate;
    }

	/**
     * @param number $rqst_kit_vrsn
     */
    public static function setRqst_kit_vrsn($rqst_kit_vrsn)
    {
        TransactionRequestBean::$rqst_kit_vrsn = $rqst_kit_vrsn;
    }

	/**
     * @param string $custId
     */
    public function setCustId($custId)
    {
        $this->custId = $custId;
    }

	/**
     * @param string $cardId
     */
    public function setCardId($cardId)
    {
        $this->cardId = $cardId;
    }

	/**
     * @param string $cardNo
     */
    public function setCardNo($cardNo)
    {
        $this->cardNo = $cardNo;
    }

	/**
     * @param string $cardName
     */
    public function setCardName($cardName)
    {
        $this->cardName = $cardName;
    }

	/**
     * @param string $cardCVV
     */
    public function setCardCVV($cardCVV)
    {
        $this->cardCVV = $cardCVV;
    }

	/**
     * @param string $cardExpMM
     */
    public function setCardExpMM($cardExpMM)
    {
        $this->cardExpMM = $cardExpMM;
    }

	/**
     * @param string $cardExpYY
     */
    public function setCardExpYY($cardExpYY)
    {
        $this->cardExpYY = $cardExpYY;
    }

	/**
     * @param int $timeOut
     */
    public function setTimeOut($timeOut)
    {
        $this->timeOut = $timeOut;
    }

	/**
     * This function encrypts request params details
     * @return string
     */
    public function getEncryptedData()
    {
        try {

            $clientMetaData = "";
            $requestValidate = new RequestValidate();

            if (!$requestValidate->isBlankOrNull($this->ITC)) {
                $clientMetaData .= "{itc:" . $this->ITC . "}";
            }
            if (!$requestValidate->isBlankOrNull($this->email)) {
                $clientMetaData .= "{email:" . $this->email . "}";
            }
            if (!$requestValidate->isBlankOrNull($this->mobileNumber)) {
                $clientMetaData .= "{mob:" . $this->mobileNumber . "}";
            }
            if (!$requestValidate->isBlankOrNull($this->uniqueCustomerId)) {
                $clientMetaData .= "{custid:" . $this->uniqueCustomerId . "}";
            }
            if (!$requestValidate->isBlankOrNull($this->customerName)) {
                $clientMetaData .= "{custname:" . $this->customerName . "}";
            }

            $this->strReqst = "";
            if (!$requestValidate->isBlankOrNull($this->requestType)) {
                $this->strReqst .= "rqst_type=" . $this->requestType;
            }

            $this->strReqst .= "|rqst_kit_vrsn=1.0." . self::$rqst_kit_vrsn;

            if (!$requestValidate->isBlankOrNull($this->merchantCode)) {
                $this->strReqst .= "|tpsl_clnt_cd=" . $this->merchantCode;
            }

			if (!$requestValidate->isBlankOrNull($this->accountNo)) {
                $this->strReqst .= "|accountNo=" . $this->accountNo;
            }

            if (!$requestValidate->isBlankOrNull($this->merchantTxnRefNumber)) {
                $this->strReqst .= "|clnt_txn_ref=" . $this->merchantTxnRefNumber;
            }

            if (!$requestValidate->isBlankOrNull($clientMetaData)) {
                $this->strReqst .= "|clnt_rqst_meta=" . (string) $clientMetaData;
            }

            if (!$requestValidate->isBlankOrNull($this->amount)) {
                $this->strReqst .= "|rqst_amnt=" . $this->amount;
            }

            if (!$requestValidate->isBlankOrNull($this->currencyCode)) {
                $this->strReqst .= "|rqst_crncy=" . $this->currencyCode;
            }

            if (!$requestValidate->isBlankOrNull($this->returnURL)) {
                $this->strReqst .= "|rtrn_url=" . $this->returnURL;
            }

            if (!$requestValidate->isBlankOrNull($this->s2SReturnURL)) {
                $this->strReqst .= "|s2s_url=" . $this->s2SReturnURL;
            }

            if (!$requestValidate->isBlankOrNull($this->shoppingCartDetails)) {
                $this->strReqst .= "|rqst_rqst_dtls=" . $this->shoppingCartDetails;
            }

            if (!$requestValidate->isBlankOrNull($this->txnDate)) {
                $this->strReqst .= "|clnt_dt_tm=" . $this->txnDate;
            }

            if (!$requestValidate->isBlankOrNull($this->bankCode)) {
                $this->strReqst .= "|tpsl_bank_cd=" . $this->bankCode;
            }

            if (!$requestValidate->isBlankOrNull($this->TPSLTxnID)) {
                $this->strReqst .= "|tpsl_txn_id=" . $this->TPSLTxnID;
            }

            if (!$requestValidate->isBlankOrNull($this->custId)) {
                $this->strReqst .= "|cust_id=" . $this->custId;
            }

            if (!$requestValidate->isBlankOrNull($this->cardId)) {
                $this->strReqst .= "|card_id=" . $this->cardId;
            }
            if (!$requestValidate->isBlankOrNull($this->mobileNumber)) {
                $this->strReqst .= "|mob=" . $this->mobileNumber;
            }

            if (($this->requestType == "TWC") || ($this->requestType == "TRC") || ($this->requestType == "TIC")) {

                $cardInfoBuff = "";
                $cardInfoBuff .= "card_Hname=" . $this->cardName;
                $cardInfoBuff .= "|card_no=" . $this->cardNo;
                $cardInfoBuff .= "|card_Cvv=" . $this->cardCVV;
                $cardInfoBuff .= "|exp_mm=" . $this->cardExpMM;
                $cardInfoBuff .= "|exp_yy=" . $this->cardExpYY;

                $aes = new AES($cardInfoBuff, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aes->require_pkcs5();
                $cardInfoStr = $aes->encryptHex();

                $aesObj = new AES($cardInfoStr, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aesObj->require_pkcs5();
                $cardInfo = $aesObj->encryptHex();

                $this->strReqst .= "|card_details=" . $cardInfo;

            } else if ($this->requestType == "TCC") {

                $cardInfoBuff = "";
                $cardInfoBuff .= "|card_Cvv=" . $this->cardCVV;

                $aes = new AES($cardInfoBuff, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aes->require_pkcs5();
                $cardInfoStr = $aes->encryptHex();

                $aesObj = new AES($cardInfoStr, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aesObj->require_pkcs5();
                $cardInfo = $aesObj->encryptHex();

                $this->strReqst .= "|card_details=" . $cardInfo;

            } else if ($this->requestType == "TWI") {

                $impsInfoBuff = "";
                $impsInfoBuff .= "mmid=" . $this->MMID;
                $impsInfoBuff .= "|mob_no=" . $this->mobileNumber;
                $impsInfoBuff .= "|otp=" . $this->OTP;

                $aes = new AES($impsInfoBuff, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aes->require_pkcs5();
                $impsInfoStr = $aes->encryptHex();

                $aesObj = new AES($impsInfoStr, $this->key, $this->blockSize, $this->mode, $this->iv);
                $aesObj->require_pkcs5();
                $impsInfo = $aesObj->encryptHex();

                $this->strReqst .= "|imps_details=" . $impsInfo;

            } else if ($this->requestType == "TIO") {
				$this->strReqst .=  "|otp=" . $this->OTP;
            }

            $this->strReqst .= "|hash=" . sha1($this->strReqst);

            $aesObj = new AES($this->strReqst, $this->key, $this->blockSize, $this->mode, $this->iv);
            $aesObj->require_pkcs5();
            $encryptedData = $aesObj->encrypt();

        } catch ( Exception $ex ) {
            echo $ex->getMessage();
            return;
        }

        return $encryptedData;
    }

    /**
     * This function returns transaction token url
     * @return string
     */
    public function getTransactionToken()
    {
        set_time_limit((int)$this->timeOut);
        if ($this->webServiceLocator != null && $this->webServiceLocator != "" && $this->webServiceLocator != "NA") {

            $requestValidate = new RequestValidate();

            $params = array();
            $params['pReqType'] = $this->requestType;
            $params['pMerCode'] = $this->merchantCode;
            $params['pEncKey'] = $this->key;
            $params['pEncIv'] = $this->iv;

            $errorResponse = $requestValidate->validateRequestParam($params);

            if ($errorResponse) {
                return $errorResponse;
            }

            $encryptedData = $this->getEncryptedData();

            if (!$encryptedData) {
                return;
            }

            try {
                $postData = $encryptedData . "|" . $this->getMerchantCode() . "~";
                $client = new SoapClient($this->webServiceLocator,
                    array(
                            "trace" => 1,
                            "exceptions" => 1
                    ));
                $response = $client->getTransactionToken(array(
                        'msg' => $postData
                ));
            } catch ( Exception $ex ) {
                echo "Error while getting transaction token : " . $ex->getMessage();
                return;
            }

            return isset($response->getTransactionTokenReturn) ? $response->getTransactionTokenReturn : NULL;
        } else {
            return "ERROR065";
        }
    }
}
