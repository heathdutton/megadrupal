
-- THE PROBLEM --

As a Drupal developer, it's very likely that you have saved passwords for
several of your websites. This makes it easier when coming back to it after a
while. However, there are several situations where this is not desirable:

* Whenever you try to edit any user account (including yours), your password
is already populating one of the fields, even though the purpose of the field
is to change it.

* Whenever you want to add a new user through the admin interface, not just
your password, but also your username are now populating the fields of a
soon-to-be different user.

As a developer, you probably have learned to deal with it. But the truth is
that this is a major problem when you are developing the website for someone
else. Any other user that has permissions to administer user accounts will
face the same problems. Unacceptable!


-- THE SOLUTION --

What this module does is very simple: it clears the password.

Gone are the days of manually clearing that field whenever doing the slightest
change to your account. User admins don't need to worry anymore about
accidentally changing their users' passwords to that of their own.

Through simple jQuery, this module clears the field for you whenever it deems
appropriate. Note that this module won't erase the password from your
browser's configuration, it just clears the field to improve usability.


-- CONFIGURATION --

You can enable/disable the functionality on the Account Settings page
(admin/config/people/accounts).


-- CREDITS --

* Developed by Victor Kareh (vkareh) - http://www.vkareh.net
* Sponsored by Switchback - http://www.switchbackcms.com
