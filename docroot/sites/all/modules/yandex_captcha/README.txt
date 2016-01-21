Yandex.Captcha for Drupal
=========================
The Yandex.Captcha module uses the Yandex CleanWeb web service to
improve the CAPTCHA system. For more information on what Yandex CleanWeb is,
please visit: http://api.yandex.ru/cleanweb/

Requires - Drupal 7 with CAPTCHA module
License - GPL (see LICENSE)

The module is used ideas from the module "Image CAPTCHA Refresh".


MAINTAINER:
-----------

Andrei Abozau (http://drupal.org/user/108373)
Brilliant Solutions OU, Estonia, Tallinn (http://brilliant-solutions.eu)


DEPENDENCIES:
-------------

Yandex.Captcha depends on the CAPTCHA module.  
Link: https://drupal.org/project/captcha


INSTALLATION:
-------------

1. Download and unpack the Yandex.Captcha module directory in your modules 
   folder (this will usually be "sites/all/modules/").

2. Enable Yandex.Captcha and CAPTCHA modules in "Administer" -> "Modules".


CONFIGURATION:
--------------

1. Create account on Yandex or login to your account if you have:
       http://passport.yandex.ru

2. Go to the Yandex CleanWeb API page and create API key:
       http://api.yandex.ru/key/form.xml?service=cw

3. Go to Yandex.Captcha tab in the CAPTCHA administration page,
   fill API key field and select type of captcha.
       admin/config/people/captcha/yandex_captcha

4. Visit the Captcha administration page and set where you
   want the Yandex.Captcha form to be presented:
       admin/config/people/captcha
