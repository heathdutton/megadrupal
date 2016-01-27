README.txt
==========

Instruction
------------

Sinaweibo enable the Drupal site integrate with Sina Weibo (http://weibo.com).
Users can sign up, sign in with there weibo account. This module provide a 
tab below the user account edit menu, users can connect or disconnect with 
there weibo account there.

Installation
-------------

1. Upload the module to the /sites/all/modules/ folder of your site.
2. Get the sina weibo sdk from 
   http://libweibo.googlecode.com/files/weibo-phpsdk-v2-2011-12-16.zip
	 and extract the saetv2.ex.class.php in the module folder.
3. Get the consumer key and secret from sina weibo open platform 
   http://open.weibo.com/.
4. Enter the site url (http://yoursite.com/) and callback url 
   (http://yoursite.com/weibo/callback) for your website in sina weibo.
5. Input your sina weibo consumer key and secrect 
	 at /admin/config/people/sinaweibo

Updates
-------------

* Added the translation file: weibo.zh-hans.po
* Enable admin to config whether to allow users signup with weibo account.
* If user signup with weibo account, user can enter there email at /weibo/email
* Enable admin to config whether to post a message when users signup with weibo account.


Author
-------

Plusfront <admin at plusfront DOT com>
http://plusfront.com
