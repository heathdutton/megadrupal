<?php

include 'includes/AuthnRequest.php';

$authn_request = new MiniOrangeAuthnRequest();
$authn_request->initiateLogin();
