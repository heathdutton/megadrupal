Install
=============

1) Go to http://vk.com/editapp?act=create and create STANDALONE application.

2) Extract module in sites/all/modules directory.

3) Go to module settings page /admin/modules and enable VKXP (VKontakte CrossPoster) module.

4) Go to vkxp settings page /admin/config/services/vkxp and copy there application ID, application secret key (from application settings page) and group/user ID.

5) Save configuration.

6) Go to "Recieve access token" page/admin/config/services/vkxp/access_token and click "Get code" link.

7) In a new window allow access for your application.

8) From url in a new window copy param #code. For example, if URL in a new window like this "https://oauth.vk.com/blank.html#code=b1c51bbaeeb2ddd51c" you should copy only "b1c51bbaeeb2ddd51c".

9) Paste copied param to textfield "Code" in "Recieve access token" form and click "Get access token".

10) Configure node type that should be crossposted on /admin/structure/types/manage/[NODE_TYPE] page.

Russian documentation
=====================

Documentation in Russian can be found here http://drupalace.ru/vkontakte-crossposter-2-voskreshenie.

Usage
=============

When creating or editing node, just check "Post this node to vk.com" and data will be automatically sent to VK.


External usage
==============

If you are authorized on vk server (if you get access token after 5th step of installation) you may make queries to vk api using this function:

vkxp_query($api_method, $post_fields, $requert_url);

About VK api you can read here http://vk.com/developers.php#devstep2.

Also possible to change VK queries by using hook_vkxp_query_alter(). Read about it in vkxp.api.php


CREDITS
=============

Module was developed by Maslouski Yauheni (http://drupalace.ru).
Module development was not sponsored by anyone. It was created for the love of Drupal.
