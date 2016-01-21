Description
-----------
This is a wrapper for the USPS PHP API by Vince Gabriel to make it easier to use
within Drupal.


Installation
------------
1) Place this module directory in your "modules" folder (this will usually be
   "sites/all/modules/"). Don't install your module in Drupal core's "modules"
   folder, since that will cause problems and is bad practice in general. If
   "sites/all/modules" doesn't exist yet, just create it.

2) Download the USPS PHP API from 
   https://github.com/VinceG/USPS-php-api/archive/master.zip
   and place it in sites/all/libraries as a folder named usps.
   The folder structure should be sites/all/files/usps/*.php.

3) Enable the module.

3) Visit "admin/config/services/usps" and enter and save your USPS API
   username.


FAQ
---
Q: Do I need a USPS API account?
A: Yes.

Q: How do I get a USPS API account?
A: You can sign up for one here:
   https://www.usps.com/business/web-tools-apis/welcome.htm
   by clicking the register now button.

Q: How do I get my account approved for production use?
A: The easiest way I've found to do this is to just go directly to the testing
   urls in https://www.usps.com/business/web-tools-apis/address-information-v3-
   1d.htm#_Toc131231398 . Just make sure you replace the user id with the
   username they sent you. You'll also be using the sub-domain testing and not
   production. The email says production, don't listen to it. Took a lengthy
   phone call with the USPS to figure that one out since it was not mentioned
   anywhere in the email or the documentation. After you've visited these two
   pages, call the ICCC to get your account changed to production access. THEN
   you can use the production subdomiain and the regular dll file.


Array Data Structure for USPS functions
---------------------------------------
Here is an example of an address array that you would pass
usps_api_integration_verify_address().
// Example Data
$address = array(
  'name' => 'Big Corporation',
  'apt' => 'Suite 1337',
  'address' => '123 Big Easy St',
  'city' => 'Los Angeles',
  'state' => 'CA',
  'zip' => 90034,
);


Future Plans
------------
For future versions I would like to incorporate this into things like the
addressfield module so that it can automatically verify an address when a user
enters it instead of this working by calling functions only.


Author
------
Michael Andrew Roberts ~ http://www.michaelandrewroberts.com/

I didn't do the hard work, that was all done by Vincent Gabriel and a few
others.
https://github.com/VinceG/USPS-php-api
http://vadimg.com/
