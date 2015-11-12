
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Current Maintainers: Jeremy Lichtman <jeremy.l@myplanetdigital.com>
                     Nick Vahalik <nick.vahalik@commerceguys.com>

CyberSource SASOP (Secure Acceptance Silent Order POST) integration for the 
Drupal Commerce payment and checkout system. Currently supports credit card 
payments on the checkout form via SASOP.


INSTALLATION
------------

Install the CyberSource SASOP module. Then enable the Rule for CyberSource 
SASOP via Store > Configuration >  Payment settings and edit the payment 
action to use your CyberSource API credentials.

You MUST configure the Merchant POST URL for your account. Instructions can be
found here: 

http://apps.cybersource.com/library/documentation/dev_guides/Secure_Acceptance_SOP/html/wwhelp/wwhimpl/common/html/wwhelp.htm#href=creating_profile.05.4.html&single=true

You will need to set the Merchant POST URL in your account to:
   
https://<your site>/commerce_cybersource_sasop/notification

or 
   
http://<your site>/commerce_cybersource_sasop/notification 

(if running an insecure site is your thing).
