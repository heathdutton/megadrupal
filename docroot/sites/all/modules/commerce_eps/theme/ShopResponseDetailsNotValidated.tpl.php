<?php
/**
 * @file ShopResponseDetailsNotValidated.tpl.php
 *
 * Returns an unsuccessful payment confirmation message, see step III-8 of chapter 7 "EPS Ablauf".
 *
 * @link http://www.stuzza.at/en/download/294-eps-pflichtenheft-inter-scheme-v1-0-01-09-2014.html
 */
?>
<epsp:EpsProtocolDetails xmlns:epsp="http://www.stuzza.at/namespaces/eps/protocol/2013/02"
                         xmlns:atrul="http://www.stuzza.at/namespaces/eps/austrianrules/2013/02"
                         xmlns:epi="http://www.stuzza.at/namespaces/eps/epi/2013/02"
                         xmlns:eps="http://www.stuzza.at/namespaces/eps/payment/2013/02"
                         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                         xsi:schemaLocation="http://www.stuzza.at/namespaces/eps/protocol/2013/02/EPSProtocol-V25.xsd"
                         SessionLanguage="DE">
  <epsp:ShopResponseDetails>
    <epsp:ErrorMsg><?php print $error_msg; ?></epsp:ErrorMsg>
    <epsp:SessionId><?php print $session_id; ?></epsp:SessionId>
  </epsp:ShopResponseDetails>
</epsp:EpsProtocolDetails>