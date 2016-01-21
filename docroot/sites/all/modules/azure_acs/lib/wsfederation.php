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

//this class handles and validates the WS-Federation response from ACS
class TokenReponseHandler
{

	public function HandleResponse($response, $audience, $tokenType)
	{
		$xml = new SimpleXMLElement(stripslashes($response));
		$xml->registerXPathNamespace('t', 'http://schemas.xmlsoap.org/ws/2005/02/trust');
		$xml->registerXPathNamespace('wsu', 'http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd');
		$xml->registerXPathNamespace('wsp', 'http://schemas.xmlsoap.org/ws/2004/09/policy');
		$xml->registerXPathNamespace('wsse', 'http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd');
		$xml->registerXPathNamespace('addr', 'http://www.w3.org/2005/08/addressing');

		$addressQuery = $xml->xpath('//t:RequestSecurityTokenResponse/wsp:AppliesTo/addr:EndpointReference/addr:Address');
		if (!count($addressQuery) || $addressQuery[0] != $audience)
		{
			throw new Exception('The realm in the token response was not valid. Realm: '.@$addressQuery[0]);
		}

		$expiresQuery = $xml->xpath('//t:RequestSecurityTokenResponse/t:Lifetime/wsu:Expires');
		if (!count($expiresQuery))
		{
			throw new Exception('The expiration time was missing in the token response.');
		}
		else
		{
			//2010-08-01T00:56:52.804Z
			preg_match('/(\\d{4})-(\\d{2})-(\\d{2})T(\\d{2}):(\\d{2}):(\\d{2}).*/', $expiresQuery[0], $n);

			if (count($n) != 7)
			{
				throw new Exception('The expiration time was invalid in the token response.');
			}

			$timestamp = gmmktime($n[4],$n[5],$n[6],$n[2],$n[3],$n[1]);

			if (time() > $timestamp)
			{
				throw new Exception('The token response has expired.');
			}
		}

		$tokenTypeQuery = $xml->xpath('//t:RequestSecurityTokenResponse/t:TokenType');
		if (!count($tokenTypeQuery) || $tokenTypeQuery[0] != $tokenType)
		{
			throw new Exception('Invalid token type received: '.@$tokenTypeQuery[0]);
		}

		$tokenQuery = $xml->xpath('//t:RequestSecurityTokenResponse/t:RequestedSecurityToken/wsse:BinarySecurityToken');
		if (count($tokenQuery))
		{
			return base64_decode($tokenQuery[0]);
		}
		throw new Exception('Response token was missing or invalid.');
	}
}

?>