
INTRODUCTION
------------

The purpose of Login Notify is to alert you when someone logs in to your
account from an unrecognized browser. It also gives you the opportunity to
"lock out" a browser if you wish.

The goal of this is to minimize the damage that can be done by a malicious user
in the event that someone obtains your password, or through some other means
manages to log in to your account.

This module was inspired by a similar Facebook feature introduced in 2010.
  http://www.facebook.com/blog/blog.php?post=389991097130


INSTALLATION
------------

Install this module using the typical process:

1. Copy the module to the sites/SITENAME/modules directory.

2. Enable the module in admin/modules or via drush.

3. Configure the settings in admin/config/people/login-notify to enable login
   notification and select the roles that require it. The default settings are
   "All roles except those selected below" with all roles unchecked. This will
   require login notification for all users with roles other than "anonymous
   user" and "authenticated user".


HOW IT WORKS
------------

When someone logs in to the site for the first time from an unrecognized
browser they are required to give the browser a unique name before they can
proceed. An email is then sent to the email address on record for the account.
This email contains a link that allows you to "lock out" the browser, which
will forcefully log them out the next time they access the site from your
account.

Browsers are recognized by storing an additional cookie that persists even
after logging out. If that cookie doesn't exist, or if it doesn't match a valid
record in the {login_notify_browsers} table, the browser is unrecognized.

Browser records in the {login_notify_browsers} table are unique for each user
account, so if someone uses multiple accounts on the same site they'll have to
name their browser once for every account.

Despite the name of the module (Login Notify), it does not implement
hook_user_login(). Instead it implements hook_init(). There are two reasons for
this:

1. In case someone accesses site without going through the login process. For
   example, by hijacking the session cookie.

2. In the event that a browser is "locked out" it needs to be logged out
   immediately on the next page load without waiting until the next time they
   log in.

From a developer standpoint the module can better be thought of as "Browser
Authentication" rather than "Login Notification".
