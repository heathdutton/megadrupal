Adds the Baidu Analytics[http://tongji.baidu.com/] web statistics tracking
system to your website. Baidu Analytics (百度统计 Baidu Tongji, in Chinese) is
Baidu search engine's web analytics platform.


Features:
---------
The module allows you to add the following statistics features to your site:
* Selectively track/exclude certain users, roles and pages.
* Monitor what type of links are tracked (downloads, outgoing and mailto).
* Monitor what files are downloaded from your pages.
* Custom variables support with tokens.
[http://tongji.baidu.com/open/api/more?p=guide_setCustomVar]
* Custom code snippets.
* Site Search support.
* DoNotTrack support (non-cached content only).
* Drupal messages tracking.
* Modal dialog tracking (Colorbox[http://drupal.org/project/colorbox]).
* Access denied (403) and Page not found (404) tracking.
* Cache the Baidu Analytics code on your local server for improved page loading
times.


Implementation:
---------------
Module's code was greatly inspired and adapted from the excellent Google
Analytics[https://drupal.org/project/google_analytics] module (7.x-1.x branch).
Several features provided by Google Analytics API had to be adjusted since
Baidu Analytics has its own (slightly different) ways, methods and API to
provide a similar set of features.
The Baidu Analytics module currently supports two types of JavaScript code for
script inclusion: Standard (Legacy) and Asynchronous (Recommended), as provided
on Baidu Analytics code tracker page (screenshot: https://drupal.org/files/proj
ect-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg).

For more information about the main differences with the Google Analytics
module, please check the project page.


Installation and configuration:
-------------------------------
1 - Download the module and copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).
[https://drupal.org/documentation/install/modules-themes/modules-7]

2 - Configuration:
a. After successful installation, browse to the Baidu Analytics Settings form
page under:
Home » Administration » Configuration » System » Baidu Analytics
Path: admin/config/system/baidu_analytics or use the "Configure" link displayed
on the Modules management page.

To start using Baidu Analytics fill in the Web Property ID (Site Tracker ID) as
described in field's help text (first field of the admin settings form). If you
don't already have a Web Property ID, feel free to register your site with
Baidu Analytics, or if you already have registered your site, go to your Baidu
Analytics tracker code page to extract the ID inside the JavaScript code
provided (see screenshot indicated above in the previous paragraph).
[http://tongji.baidu.com/]
[http://tongji.baidu.com/open/api/more?p=guide_overview]

b. Configure all other settings: enable/disable tracking of messages
/links/search, visibility, roles, users, etc... and save the form. Please make
sure *Cache is not enabled before testing* that all settings work properly.


Useful Resources:
-----------------
For any questions and problems, you may find some help on the Baidu Analytics
official site:
* Baidu Analytics Module Documentation: [https://drupal.org/node/2076737]
* Unfortunately, there isn't much English documentation or literature on Baidu
Analytics, so if you would like to know more about its Features in general, we
would recommend the article: A Thorough Guide to Baidu Analytics (Baidu Tongji)
[http://www.east-west-connect.com/baidu-analytics-guide].
* Baidu Analytics Forum: [http://tieba.baidu.com/f?kw=%B0%D9%B6%C8%CD%B3%BC%C6]
For general discussions, inquiries, future developments or API changes, etc...
* Baidu Analytics Help Center and help on Baidu Tuiguang:
[http://yingxiao.baidu.com/support/tongji/index.html]
[http://yingxiao.baidu.com/support/fc/index.html]
* Baidu Analytics Chinese API Documentation:
[http://tongji.baidu.com/open/api/more?p=guide_overview]


Future developments:
--------------------
*Module is subject to changes and restrictions from the Baidu Analytics
Javascript API.*
[http://tongji.baidu.com/open/api/more?p=guide_overview]

For more information about Baidu Analytics future plans and developments,
please check the project page or the issue queue (link in the next paragraph).
[https://drupal.org/project/baidu_analytics]

Efforts on documentation (especially in Chinese), tutorials/presentations,
module's tranlations, more simpletests, more code snippets and of course the
usual bug fixes.


Contributions are welcome!!
---------------------------
Feel free to follow up in the issue queue for any contributions, bug reports,
feature requests. Tests, feedback or comments (and of course, patches) in
general are highly appreciated.
[https://drupal.org/project/issues/baidu_analytics]


Credits:
--------
Hopefully, with the help of the community, for testing, reviewing and
reporting, new features, patches or tests added to this module might as well be
ported/adapted to be back with the Google Analytics module.

This module was sponsored by DAVYIN[http://www.davyin.com/] | 上海戴文
[http://www.davyin.cn/].


Maintainers:
------------
DYdave [https://drupal.org/user/467284]
