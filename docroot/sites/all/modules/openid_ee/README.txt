Estonian national smartcard ID card login using http://openid.ee OpenID provider.

Installation

1. Download the module and copy to your module directory (most likely sites/all/modules)
2. Enable the module (note: it also enables openid.module)
3. Navigate to admin/build/block and enable "Estonian ID card login" block

Note: the new block is only visible to non-logged users. To test it, run another browser where you are not logged in to your Drupal site.

Usage

When clicked on "ID-card" button it takes you to http://openid.ee authentication service provider and asks you to either insert your smartcard or Mobile ID. When authenticatication is successful, you will redirected back to your Drupal site.