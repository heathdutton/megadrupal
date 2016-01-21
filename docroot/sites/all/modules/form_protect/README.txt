Form Protect
============

Description
-----------

Form Protect is a tiny, simple, non-invasive spam protection for forms. The
protection is based on the assumption that spam bots are not running JavaScript
when loading targeted forms.

Comparing to other similar modules, this one is only redirecting the submit to a
blind page when JavaScript is disabled. Similar modules:
- Antibot [https://www.drupal.org/project/antibot]
- Captcha-free Form Protection [https://www.drupal.org/project/captcha_free]

Install & Config
----------------

Install the module as any other module. At admin/config/system/form-protect, set
the list of IDs for forms that need protection. You can choose to log the
blocked attempts.

Author
------
Claudiu Cristea
- https://www.drupal.org/u/claudiu.cristea
- Twitter: @claudiu_cristea
