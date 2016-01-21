---------
Overview:
---------
Full featured stand-alone Drupal module to accept Perfect Money SCI payments on 
your website. Supports payments both directly via embedded payment form and 
using internal API & hooks.

Features:
    * embedded payment form for manual payments
    * supports API & hooks to interact with other modules
    * adjustable list of currencies
    * sample settings page for easy tuning Perfect Money to enable SCI
    * all payments are stored in stand-alone table
    * ability to browse payments in admin interface and taking actions on them 
      (deleting and enrolling)
    * ability to hide result url (path module required)
    * themeable "Payment success" and "Payment failed" pages
    * supports passing additional payment fields (via API)
    * MySQL and PostgreSQL support


------
Usage:
------
Installation, requirements and configuration information can be found within the 
INSTALL.txt file included with this module.


----
API:
----
Modules has internal API which can be used to process actions like creating, 
enrolling and deleting payments.

  ---------------
  Create payment:
  ---------------

  $result=perfectmoney_sci_api('insert', $params);

  Passed parameters:

  $params=array(
     'uid' => $user->uid,		 // owner of the payment, usually user who created it
     'created' => time(),		 // optional, time of payment creation
     'amount' => 12.95,			 // amount of payment
     'currency' => 'USD',		 // currency id
     'memo' => 'Payment to website',
     'payee_account' => 'U1111111', // optional
  );

  Returned array:

  $result['pid']  // payment_id
  $result['uid']
  $result['created']
  $result['amount']
  $result['currency']
  $result['memo']
  $result['payee_account']


  -------------
  Load payment:
  -------------

  $result=perfectmoney_sci_api('load', $pid);

  Passed parameters: $pid - payment ID.

  Returned array:

  $result['pid']  // payment_id
  $result['uid']
  $result['created']
  $result['enrolled'] // time of enrollment (0 if not enrolled yet)
  $result['amount']
  $result['currency']
  $result['memo']
  $result['payee_account']
  $result['payer_account'] //  empty if not enrolled yet
  $result['custom_field_you_passed_via_api'] // additional fields are also 
						returned

  ---------------
  Enroll payment:
  ---------------
  
  $result=perfectmoney_sci_api('enroll', $params);

  Passed parameters:

  $params=array(
     'pid' => $payment_id,		 // required
     'payer_account' => 'U2222222',	 // optional
     'time' => time(),			 // optional
  );

  Returned value: true - on success, false - on error.


  ---------------
  Delete payment:
  ---------------
  
  $result=perfectmoney_sci_api('delete', $pid);

  Passed parameters: $pid - payment ID.

  Returned value: true - on success, false - on error.


------
Hooks:
------
Module implements own hooks, which fire on the following actions:

function hook_perfectmoney_sci($op, $payment, $params=''){
   
   switch($op){
   case 'insert': 
      
      // fires BEFORE payment creation
      // $payment here is an array with payment info

      // hook MUST return passed $payment array with possible changes
      return array(
		'uid' => $payment['uid'],
		'created' => $payment['created'],
		'amount' => $payment['amount'],
		'currency' => $payment['currency'],
		'memo' => $payment['memo'].' add some text, for instance',
		'payee_account' => $payment['payee_account'],
      );

   break;
   case 'inserted': 
      
      // fires immediately AFTER payment created
      // $payment here is payment ID
      // $params here could be (or not be) an array with payment info
	   
   break;
   case 'enrolled': 
      
      // fires immediately after payment enrolled
      // $payment here is an array with payment info
	   
   break;
   case 'deleted': 
      
      // fires immediately after payment deleted
      // $payment here is payment ID
	   
   break;
   }

}

-------
Support
-------
If you experience a problem with this module or have a problem, file a
request or issue on the module queue at
http://drupal.org/project/issues/perfectmoney_sci. DO NOT POST IN THE FORUMS. 
Posting in the issue queues is a direct line of communication with the module author.