Realex Payment Gateway
----------------------
A Payment gateway for realex payments.  To use this module you will need to
have an account set up with realex and have recieved a shared key as well as a
merchant ID.  Realex have the requirement that you initially use the account in
test mode to make sure that the process works. So first thing to do is requset
their test cards so that you can make some dummy payments (Note: Realex have
different test cards for normal transactions and 3D Secure ones and they only
give Visa 3D secure test cards, so 3D secure will have to be turned off to test
other types of cards).

Installation
------------
 - Download and enable the module
 - Go to the payment methods settings page at
 admin/commerce/config/payment-methods
 - Click the edit on the "Realex Integration" Rule and then again on the action
 for the rule.
 - Insert secret/merchant Id and decide whether to use 3D secure or not.

Future Works
------------
At the moment there is only integration with Realex's remote Realauth and
RealMPI(3D Secure) systems but hopefully in the future this will extend to all
Realex's features.
