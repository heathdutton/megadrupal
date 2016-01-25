<?php
/*
ACS Plugin for Wordpress
Copyright (c) 2011, Microsoft Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the organization nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MICROSOFT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//this class validates the SWT token received from ACS and extracts claims
class TokenValidator
{
    private $issuerLabel = "Issuer";
    private $expiresLabel = "ExpiresOn";
    private $audienceLabel = "Audience";
    private $hmacSHA256Label = "HMACSHA256";

    public function Validate($token, $audience, $issuer, $signingKey, $signingKeyOld)
    {
	    $tokenValues = $this->GetNameValues($token);
	    if (!$tokenValues)
	    {
		    throw new Exception('Response token contains no values');
	    }

        if ($this->IsExpired($tokenValues))
        {
            throw new Exception('Response token is expired: '.@$tokenValues[$this->expiresLabel]);
        }

        if (!$this->IsIssuerTrusted($tokenValues, $issuer))
        {
            throw new Exception('Response token contains an invalid issuer: '.@$tokenValues[$this->issuerLabel]);
        }

        if (!$this->IsAudienceTrusted($tokenValues, $audience))
        {
            throw new Exception('Response token contains an invalid audience: '.@$tokenValues[$this->audienceLabel]);
        }

        if (!$this->IsHMACValid($token, $signingKey, $signingKeyOld))
        {
            throw new Exception('Response token contains an invalid signature');
        }

        return true;
    }

    public function GetNameValues($token)
    {
        if (is_null($token) || $token == "")
        {
            return false;
        }

        $outValues = array();
        $inValues = explode("&", $token);
        foreach ($inValues as $value)
        {
	        $value = explode("=", $value);
	        $outValues[urldecode($value[0])] = urldecode($value[1]);
        }
        return $outValues;
    }

    public function GetClaims($token)
    {
	    $tokenValues = $this->GetNameValues($token);
	    unset($tokenValues[$this->issuerLabel]);
	    unset($tokenValues[$this->expiresLabel]);
	    unset($tokenValues[$this->audienceLabel]);
	    unset($tokenValues[$this->hmacSHA256Label]);
	    return $tokenValues;
    }

    private function IsAudienceTrusted($tokenValues, $audience)
    {
        if (strlen($tokenValues[$this->audienceLabel]) > 0)
        {
            if ($tokenValues[$this->audienceLabel] == $audience)
            {
                return true;
            }
        }

        return false;
    }

    private function IsIssuerTrusted($tokenValues, $issuer)
    {
        if (strlen($tokenValues[$this->issuerLabel]) > 0)
        {
            if ($tokenValues[$this->issuerLabel] == 'https://'.$issuer.'/')
            {
                return true;
            }
        }

        return false;
    }

    private function IsExpired($tokenValues)
    {
        if (strlen($tokenValues[$this->expiresLabel]) > 0)
        {
            if (intval(time()) > intval($tokenValues[$this->expiresLabel]))
            {
                return true;
            }
        }

        return false;
    }

    private function IsHMACValid($token, $signingKey, $signingKeyOld)
    {
	    $tokenHMAC = explode('&'.$this->hmacSHA256Label.'=', $token);
	    if (count($tokenHMAC) != 2)
        {
            return false;
        }
	    $swt = $tokenHMAC[0];
	    $tokenHMAC = explode('&', $tokenHMAC[1]);
	    $hmac = urldecode($tokenHMAC[0]);

		$signature = base64_encode(hash_hmac('sha256', $swt, base64_decode($signingKey), true));
        if ($hmac == $signature)
        {
            return true;
        }

        //check old key
        $signature = base64_encode(hash_hmac('sha256', $swt, base64_decode($signingKeyOld), true));
        if ($hmac == $signature)
        {
            return true;
        }

        return false;
    }

}

?>