## CONTENTS OF THIS FILE

* Introduction
* Recommended modules
* Installation
* Configuration
  - Anonymous publishing CL
  - Anonymous publishing LR
  - Anonymous publishing PET
* Troubleshooting
  - No e-mail
* Spam protection
* Known glitches
* Maintainers


# INTRODUCTION

The **Anonymous publishing** module increases your control over anonymous
publishing on a site.

* For a full description of the module, visit the [project page][1].

* To submit bug reports and feature suggestions, or to track changes
  visit the project's [issue tracker][2].

* For community documentation, visit the [documentation page][3].
  
**Anonymous publishing** may lower the threshold for authorship and entry
to a site.  It may also be a requirement for certain sites that deal
with sensitive issues that anonymous publishing is allowed.

In Drupal, the administrator may grant the anonymous user the right to
create new content (so you don't need this module to allow anonymous
users to create content). However, allowing anonymous publishing may
have the side-effect of making the site wide open to spammers.  This
module may be used to mitigate that.  Also, it implements a privacy
enhancing technology (PET) that allows authenticated users publish
anonymously.

The major features of **Anonymous publishing** are:

1. Users may publish content without first registering an account at
   the site, provided they supply a vaild e-mail address and click on
   an activation link sent them in a verification e-mail (some call
   this the “craigslist model”). To use this feature enable the
   **Anonymous publishing CL** submodule.

2. “Lazy” registration, where the user creates the content first and
   can then “claim” the content after registering on the site. If you
   enable this submodule, there will be a tab named “Claim anonymous
   posts” on the profile page of users with “unclaimed” content they
   created on the site before they registered. To use this feature
   enable the **Anonymous publishing LR** submodule.

3. Authenticated users may publish content that appear to be published
   by Anonymous (i.e. they need to be authenticated and logged in to
   publish, but the content they tag as anonymous will never be
   publicly associated with their user name or other identity). To use
   this feature enable the **Anonymous publishing PET** submodule.

More specifically, it may:

- let anonymous publishers self-activate;
- retain self-activated e-mail addresses for future use;
- send e-mail to administrators when anonymous content is created;
- provide simple moderation of content published anonymously;
- let the administrator block abusers based on e-mail & IP address;
- track spambots and let the administrator block them,
- provide flood control;
- purge records that links the identifier to specific content;
- let the administrator customize the verification e-mail;
- allow authenticated users to publish as the anonymous user;
- associate a persistent alias with generated accounts.


## RECOMMENDED MODULES

* [Advanced Help][4]:<br>
  When this module is enabled, display of the project's `README.md`
  will be rendered when you visit
  `help/anonymous_publishing/README.md`.
* [Markdown filter][5]:<br>
  When this module is enabled, display of the project's `README.md`
  will be rendered with the markdown filter.


# INSTALLATION

1. Install as you would normally install a contributed drupal
   module. See: [Installing modules][6] for further information.

2. Enable the **Anonymous publishing** module on the *Modules* list
   page.  The database tables will be created automagically for you at
   this point.

3. Check the *Status report* to see if it necessary to run the
   database update script.

4. Enable the submodules providing the features you want to use.

5. Proceed to configure each of the submodules, as described in the
   configuraton section below.


# CONFIGURATION

By itself, the parent module does nothing. You need to enable at least
one of the project's submodules to be able to use the feature's of
this project.

The forms to configure the **Anonymous publishing CL** and **Anonymous
publishing PET** submodules are found by navigating to:

    Admin » Configuration » People

**Anonymous publishing LR** does not have a configuration form.

Below is a description of how to configure all three submodules.

## Anonymous publishing CL

To access the **Anonymous publishing CL** administration form, you need to
be granted the right to administer anonymous publishing.

After installing and activating the module navigate to:

    Admin » Configuration » People » Anonymous publishing CL

There are seven tabs:

1. **Main settings**: All the main options for this module.
2. **Message templates**: Customize verificaton e-mail sent out.
3. **Moderation**: Moderate anonymously published content.
4. **Verified**: Block (and unblock) verified e-mail addresses.
5. **Unverified**: Ban unverified users' IP addresses.
6. **Spambots**: Ban reported spambots' IP addresses.
7. **Privacy**: Privacy enhancing settings.


### Main settings

You first need to select the content type (or types) that you will
allow anonymous users to post.  You may also enable anonymous
publishing for comments.

If you want to allow users that are not logged in to create content,
you must also give permission for the anonymous user to create
content.  This is by done navigating to:

    Admin » People » Permissions.

The rest of the settings on the settings page will only have an effect
on the type or types (including comments) selected here.

Here is a brief description of the options:

- Allow self-activation<br/>
  If you check this option, content from anonymous publishers that has
  not been previously validated will be automatically published when
  the anonymous publisher verifies the e-mail.  If you leave this
  un-checked, content from un-validated anonymous publishers will be
  flagged as verified when the user verify the email-address, but it
  will not be published until approved by a administrator.  The
  setting has no effect if the email address is already validated.
  (See description of the “Moderation” panel below for details about
  how activation works.)

- Make self-activation sticky.<br/>
  If this option is set, users that have self-activated will not be
  asked to activate again as long as their e-mail address appears in
  the list of verified addresses (see “Verified” below).  If you
  remove this option, users will have to re-verify their email-address
  again to self-activate each post.  This is more secure, but also but
  more of a burden on the user.

- Skip comment approval<br/>
  This is greyed out, as this setting is managed by the Drupal core
  comment module.  Go to `Admin » People » Permissions` to set
  it.  The core “Skip comment approval” setting retains it standard
  meaning and its status is only included in this panel for
  information purposes (previous setting).  Note that if you allow
  self-activation, but don't also check “Skip comment approval”,
  self-activated comments will not be published when the user
  activates.  They will just be flagged as verified and not be
  published until they are approved by a moderator.  To avoid
  confusion, make sure this setting is in-sync with the setting Allow
  self-activation.

- Send e-mail to the administrator when anonymous content is created.<br/>
  Checking this will automatically send an e-mail to the administrator
  e-mail address whenever anonymous content is created.  You may
  use this to make sure the administrator becomes aware of possible
  problems (such as spam), or to speed up the moderation process
  (if you do not allow self-activation).

- Use IP-address for blocking.<br/>
  By default, the “blocking” box only applies to the e-mail used for
  authentication. The module records the IP-address used to
  authenticate, but this normally only used for flood control
  purposes.  When this option is set, the module will also block the
  corresponding IP-address.  Note that setting this option may result
  in false positives (as one IP-address may be shared between several
  users over time), so use this option with caution.

- Associate a generated alias with contents published anonymously.<br/>
  An unique persistent alias on the format 'userN', where N is some
  unique number, is automatically generated when an user that is not
  logged in verifies his or her email. If you enable this setting,
  this persistent alias will be used as byline for any content that is
  associated with that email address.  While this setting is disabled,
  the default name used to indicate anonymous users will be used as
  byline.  NOTE: Purging the information that links e-emails/aliases
  to anonymously published content will delete all information linking
  aliases to activation email addresses. After purging, the alias will
  no longer be used as a byline.

- Allow registered e-mails to be used for anonymous posts.<br/>
  By default, if a user has already registered and created a regualar
  account on the website, that e-mail can no longer be used for
  anonymous posts.  If you want to allow regular users to be able to
  publish as the anonymous user role, enable the **Anonymous
  publishing PET** sub-module.  However, you may override this
  behaviour by setting this option.  This security implications, as
  somebody familiar with your users will be able to guess the email of
  a registered user and use this to post harmful content which the
  regular user may be blamed for.  It is recommended to turn of
  stickyness for self-activation if you enable this option.

- Administrator's e-mail address:<br/>
  If you opt to send e-mail to the administrator when anonymous
  content is created, you need to provide a vaild e-mail address for
  the administrator.

- Verification e-mail address field weight:<br/>
  To control where on create content form the verification e-mail
  address field is placed, you may specify a weight for this field.

- Number of hours to retain anonymous posts before auto-deletions
  removes it:<br/>
  Spammers often creates contents on sites that allow them to do so,
  but almost never act on the verification e-mail.  This settings
  can be used to automatically delete anonymous posts if the e-mail
  has not yet ben verified after the number of hours you set here.

- Number of anonymous posts allowed from a single user e-mail/ip
  allowed within an hour:<br/>
  For flood control, you may set the number of anonymous posts allowed
  from a single e-mail-address or ip-address within an hour from
  1 - 98. Use 99 for no limit.

Remember to press “Save configuration” when done.


### Message templates

In this panel, there are four fields that let the administrator
customize the e-mail message sent to non-authenticated users when they
create content. The first field is the subject, the rest of the fields
may go in the body. Other settings determine what fields are used.

There is also two templates (subject and body) for the e-mail sent to
the administrator.


### Moderation

This panel shows all the comtent published anonymously that have been
verified by e-mail.  It lets the administrator publish or unpublish.

The moderation workflow of anonymous publishing is tied to the option
“Allow self-activation”.  If you check this option, the user is
allowed to selv-activate content by verifing his or her e-mail address.
If you leave this option unchecked, content created by unverified
anonymous publishers will not become activated until it is approved by
a moderator.

If you do not check the options “Allow selv-activation”, the content
will not be published when the anonymous user verifies is or her
email.  In addition, the moderator must activate the content for it to
be published.


### Verified

This panels list all the verified e-mails and their current status.

Also listed is the alias associated with each email.  The alias is
always generated, but will only be shown publicly if you check
“Associate a generated alias with contents published anonymously”
under main settings.

Also listed is IP-address associated with each email.  The IP-address
listed will be used for blocking if you mark the e-mail as blocked and
you've checked “Use IP-address for blocking” under main settings.

E-mail addresses that are blocked from anonymous publishing is shown
with a check-mark in the column “blocked”.

To block, set a check-mark in the row of the e-mail address.  To
unblock, remove this check-mark. To make changes take effect, press
“Execute”.

Note that a checkmark in the “blocked” column will only prevent the
user from posting anonymously.  No other part of the site's
functionality will be affected. Unlike the *ban IP* blocking action
you may take in the “Unverified” and “Spambots” panels, this blocking
is handled by this module, not by Drupal.

This status of blocked is only meant to be used to block abusive human
users from publishing anonymously, not to ban spambots.


### Unverified

This panels list all the e-mails IP-addresses that has been associated
with anonymous publishing that has not yet verified by e-mail and how
long it has remained unverified.

This panel provides a chortcut to admins that want to delete
unpublished spams posts and at the same time ban the IP-address used
to post it.  Placing a mark in the “delete+ban IP” column to the right
of an posting listed here will delete the posting and add the
IP-address used to post it to the Drupal's `{blocked_ips}` table.
Banned IP-addresses will not have access to any part of site at all.

To unban an IP address, navigate to:

    Admin » Configuration » People » IP address blocking

and press “delete” for the IP-address you want to unblock.


### Spambots

The spambots panel shows the IP-address of the “Top 10” spambots
targeting the site, along with some simple statistics.  You can ban an
IP address by placing a check-mark in the “ban IP” column.  When you
press “Execute”, the IP-address will is moved to the list of
IP-addresses blocked by Drupal.

To unban an IP address, follow the same procedure as suggested above.


### Privacy

While no e-mail address or username is made avialable to outsiders,
the e-mail address and IP-address associated with content is by default
retained indefinitely, and can be extracted from the database.  If
your site is used to publish sensitive material, you may want to limit
the period the record that links e-mails and IP-addresses to specific
content is retained.

For a very sensitive site, you may want to set this to “Delete ASAP”
to delete at next cron run.  But you may also opt to retain for a
limited time (from an hour up to 1 month) to give you some time to
pick up the e-mail addresses or IP-addresses of spammers and block
them.  The purging of e-mail adresses and IP-addresses is done by
cron, so you need to run cron at least as often as half the maximum
period set to be sure identifiers are purged within the time limit.

The button “Purge now” bypasses cron and purges the identifiers
instantly.


### Other administration

If you want users that are not logged in to be able to create content,
you also need to navigate to

    Admin » People » Permissions

and check the following for the anonymous user:

- View published content

Then for each of the content type(s) you want to allow anonymous
publishing for, check the following for the Anonymous user:

- Create new content

To allow the anonymous user to post comments, grant the following
permissions:

- View comments
- Post comments
- Skip comment approval

To get rid of the “(Not verified)” string that appears next to the
user name of any content posted by the anonymous user, navigate to

    Admin » Appearance » Settings » Global settings

and uncheck the following:

- User verification status in comments


## Anonymous publishing LR

If you enable the **Anonymous publishing LR** submodule, “lazy”
registration (where people can post first, and register later) is
enabled.

When this submodule is enabled, when a user that already has published
content using **Anonymous publishing CL** registers on the site with
the same e-mail address as he or she used when creating anonymous
postings, he or her will see a tab named “Claim anonymous posts” on
the “my account” profile page. This tab will allow the user to “claim”
the posts they created as an anonymous publisher for his/her profile
(i.e. if “claimed”, these posts will be owned by their logged in
profile).

Note that since this feature uses the e-mail addresses to link the old
content to the new user, it is incompatible with the privacy functions
to purge e-mail addresses available in the panel under the “Privacy”
tab.

There is no administrative interface to this submodule.  To make the
tab appear on the user's profile page. enable the submodule.  To make
the tab disappear, disable this submodule.



## Anonymous publishing PET

To access the **Anonymous publishing PET** administration form, you
need to be granted the right to administer anonymous publishing.

After installing and activating the module navigate to:

    Admin » Configuration » People » Anonymous publishing PET

There are three tabs:

1. **Main settings**: All the main options for this module.
2. **Real names**: List real user names of content published as anomymous.
3. **Privacy**: Privacy enhancing settings.


### Main settings

Under this tab, you check the content types where you want
authenticated users to be able to publish as the anonymous user role.
You may also enable this featire for comments.

The option “Allow authenticated users to publish as Anonymous” must
also be checked to have this feature available on the site.


### Real names

This panels lists the real user names and subject lines of content
that is published as anonymous.  It allows the administrator to
identify authenticated users that abuse the ability to post as
anonymous.

The rentention period of the user names listed here is controlled by
the settings under the privacy tab.


### Privacy

While no e-mail address or real user names are made avialable to
outsiders, the e-mail address and real user name associated with
content is by default retained indefinitely, and can be extracted from
the database.  If your site is used to publish sensitive material, you
may want to limit the period the record these records are retained.

For a very sensitive site, you may want to set this to “Delete ASAP”,
but you may also opt to retain for a limited time (from an hour up to
1 month) to give you some time to spot users abusing the privilege to
publish as the anonymous user.  The purging is done by cron, so you
need to run cron at least as often as half the maximum period set to
be sure identifiers are purged within the time limit.

The button “Purge now” bypasses cron and purges the identifiers
instantly.


# TROULESHOOTING

## No e-mail

The **Anonymous publishing** module uses Drupal's DefaultMailSystem to
send out verification/activation e-mails (refer to the API
documentation at api.drupal.org for the function
[`drupal_mail_system`][7] for details.  It uses the method `mail` to
send the mail, and prints out the message:

> A link and further instructions have been sent to your e-mail
address.

after after the mail was successfully accepted for by the mail for
delivery.

If the mail system rejects the message, it prints out:

> Error mailing activation/verification link.

If you get the error message, then there is probably something wrong
with the configuration of mail on your Drupal site.   

Here are some things to check if you do not receive an e-mail after
posting as anonymous, despite the fact that you're told that a link
and further instructions have been sent to your e-mail address:

- The first thing you should check is that Drupal can send e-mail at
  all. You can try using the Contact module (part of core) or you can
  request a password reset by logging out and clicking the “Request
  new password” link in the login block.

- Next check that verification/activation emails are not stopped by
  some spam-filter at the receiver end.  The sender of the emails sent
  by **Anonymous publishing** is the site's email address. You can
  inspect this at: `Admin » Configuration » System » Site
  information`.  Search your-spam folder for recent emails sent from
  this address.  Also note that there are some mail services
  (e.g. *mail.yahoo.com*) that have spam filters that consider the
  verification/activation e-mails from this module as spam and
  silently delete them. For testing, use a mail service where you
  control what is filtered.

- When testing this, make sure that the e-mail address given by the
  anonymous poster is valid, and goes to a mailbox you've access to.

- Make sure that the site's email address is valid. Look for bounced
  verification/activation emails in the mailbox belonging to the
  site's email address.


# SPAM PROTECTION

If you allow anonymous publishing your site will probably be targeted
by spammers, both of the human kind, and 'bots.  There is already some
built-in 'bot protection, and very few spammers activate.  These
built-in features may be suffiscient to keep spam at bay.  If you need
more protection against spam, projects such as [**CAPTCHA**][8],
[**Riddle**][9], [**Spambot**][10] (and many others) may be good
companions.


# KNOWN GLITCHES

- The core comment module allows anonymous publishers to pick their
  own non-persistent byline when they post a comment.  This conflicts
  with the persistent alias used as a byline by this module, so this
  feature in Drupal is disabled when this module is enabled.

- When you associate a persistent alias with generated accounts, the
  alias will not appear when there is no content directly associated
  with the display (e.g. on the forum landing page).  Instead the
  system default name for the anonymous user will appear.

# MAINTAINERS

**Anonymous publishing** was first created by [dropcube][11] (Ronny
López).<br/>
The current maintainer is [gisle][12] (Gisle Hannemyr).

Any help with development (patches, reviews, comments) are welcome.

Development has been sponsored by [Hannemyr Nye Medier AS][13].

[1]: https://drupal.org/project/anonymous_publishing
[2]: https://drupal.org/project/issues/anonymous_publishing
[3]: https://drupal.org/node/2024005
[4]: https://www.drupal.org/project/advanced_help
[5]: https://www.drupal.org/project/markdown
[6]: https://drupal.org/documentation/install/modules-themes/modules-7
[7]: https://api.drupal.org/api/drupal/includes!mail.inc/function/drupal_mail_system/7
[8]: https://drupal.org/project/captcha
[9]: https://drupal.org/project/riddle
[10]: https://drupal.org/project/spambot
[11]: https://www.drupal.org/user/37031
[12]: https://www.drupal.org/u/gisle
[13]: http://hannemyr.com/hnm/