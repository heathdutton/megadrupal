<?php

class RequestValidate
{
    const BLANK_REQUEST_TYPE = 'ERROR008';
    const INVALID_REQUEST_TYPE = 'ERROR002';
    const BLANK_MER_CODE = 'ERROR027';
    const INVALID_KEY = 'ERROR067';
    const INVALID_IV = 'ERROR068';
    const BLANK_PG_RESPONSE = 'ERROR069';

    protected $requestTypes = array('T','S','O','R','TIC','TIO','TWC','TRC','TCC','TWI');


    /**
     * This function validates the mandatory fields in the request.
     * @param array $requestParams
     * @return array $errorArray
     */
    public function validateRequestParam($requestParams = array())
    {
        if (!isset($requestParams['pReqType'])  || $this->isBlankOrNull($requestParams['pReqType'])) {
            return self::BLANK_REQUEST_TYPE;
        } else if (!in_array ($requestParams['pReqType'], $this->requestTypes)) {
            return self::INVALID_REQUEST_TYPE;
        }
        if (!isset($requestParams['pMerCode']) || $this->isBlankOrNull($requestParams['pMerCode'])) {
            return self::BLANK_MER_CODE;
        }
        if (!isset($requestParams['pEncKey']) || $this->isBlankOrNull($requestParams['pEncKey'])) {
            return self::INVALID_KEY;
        }
        if (!isset($requestParams['pEncIv']) || $this->isBlankOrNull($requestParams['pEncIv'])) {
            return self::INVALID_IV;
        }
        return false;
    }

    /**
     * This function validates the mandatory fields in the response.
     * @param array $responseParams
     * @return array $errorArray
     */
    public function validateResponseParam($responseParams = array())
    {
        if (!isset($responseParams['pRes']) || $this->isBlankOrNull($responseParams['pRes'])) {
            return self::BLANK_PG_RESPONSE;
        }
        if (!isset($responseParams['pEncKey']) || $this->isBlankOrNull($responseParams['pEncKey'])) {
            return self::INVALID_KEY;
        }
        if (!isset($responseParams['pEncIv']) || $this->isBlankOrNull($responseParams['pEncIv'])) {
            return self::INVALID_IV;
        }

        return false;
    }

    /**
     * This function checks if a value is blank, null or 'NA' and returns true, else returns false.
     * @param string $param
     * @return boolean
     */
    public function isBlankOrNull($param = null)
    {
        if (empty($param) || $param == "NA") {
            return true;
        }
        return false;
    }
}
