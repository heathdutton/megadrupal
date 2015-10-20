README.txt
---------------
The Jabber Module integrates Drupal with any xmpp service.
Whether you want to integrate with Google Talk as we do
at Babson College or with AIM, or any other instant messenger
service that uses XMPP, Droogle will integrate your users
to enable XMPP communication on your Drupal site.  The module
will create a roster ("friends list") from your current friends
list for your JID you sign in with, and as well it will construct
a list from the organic groups you are a member of within your
Drupal installation.  If you are authorized to speak with the Jids
in your list the names will turn grey, green or red if the person
is afk, available or away respectively.  At Babson we use Google
for education and there is a setting so users in your domain
do not need authorization to chat.

INSTALL
--------------
1.)  Install Jabber like any other Drupal module,
  Put the jabber module in your modules folder and then 
  enable the module in admin/modules (or preferably use
  drush to download and enable your module :)

2.)  Jabber currently also requires the profile2 module with
  the following fields:  In the Main profile (Machine name: main)
  you will need to create field_first_name, field_last_name, and
  field_google_password.

3.)  Realize Jabber will be pulling your user's JIDs from the 
  standard Drupal email address field associated with each user's
  profile.  So on GTalk my username (jid) might be username@gmail.com
  or on AIM username@aol.com so then my Drupal email address would
  have to be one of these usernames (jids) and then I would need to 
  input my Google password into the profile field configured above 
  for my user.

4.)  This module utilizes the strophe javascript library and other 
  jquery and jquery ui files. I just commited this Drupal 6 version
  to github:  https://github.com/barnettech/jabber  You can download
  the javascript files/libraries from this github project and thus
  guarantee you get the same versions of the files I know work. Or you
  can just download your copies from their source on the internet.

  Here are the files that will need to be in the jabber modules js
  directory:

  Crypto        flensed.js      jquery-ui-1.8.18.custom.min.js  swfobject.js
  checkplayer.js      gab.js        jquery.ui.core.js   updateplayer.swf
  flXHR.js      jabber.js     jquery.ui.dialog.js
  flXHR.swf     jabber_gab.js     strophe.flxhr.js
  flXHR.vbs     jquery-1.7.1.min.js   strophe.js

  jabber.js and jabber_gab.js I wrote myself and so there are no
  rights issues and so come with the module.  The rest cannot be posted
  on Drupal.org directly.

  Also the module looks for the sites/all/libraries/jquery.ui/ui directory to
  find lots of jquery.ui javascript files.  I will add a TODO: so you can configure
  the name of this directory where the files are kept.
  You can see my jquery.ui folder structure on github:
  https://github.com/barnettech/jabber and there you will see the file
  jqueryui.tar.gz and all the correct files with the correct working versions
  are in that gzipped tar file.

5.5.)  Enable the 2 jabber blocks on any page(s) of your site you like (probably within
  the organic groups context?  or on a user profile page?).  The 2 blocks
  are named the Jabber Block and Jabber Chat Block, and the first shows your
  roster, and the 2nd is the chat area itself.  Both must be enabled 
  on the same page to function together.

5.7.)  Also there is a page called at /jabber which I've used for testing but
  could be used if you like as the main chat page for the user.

5.9.)  FYI you can enable presense anywhere a username shows up on your site by
  wrapping it like this:
  <div class='username-gmail-com toggle-presence online'>
    John Dov
  </div>

  The jid (username/email address) of the user must have the dashes instead of the
  username@gmail.com format as you see above when constructing the class name.

  Then if jabber is enabled on that page with the jabber blocks being present, then 
  the username will turn green, red, or grey no matter where it is on the page.

6.)  I fully intend to support this module, and be in touch with any
  questions.  Also checkout the Droogle module which integrates Drupal
  with Google Docs (Google Drive as its now called) and Droogle comes 
  with backend APIs to manage Google users.

7.)  IMPORTANT:  configure your production BOSH server url at admin/settings/jabber
  XMPP via HTML requires a BOSH server to translate requests, HTML does not speak
  XMPP natively, right now the module uses by default
  the TEST server http://bosh.metajack.im:5280/xmpp-httpbind and is thanks to
  the author of the Strophe library, it is not for production use.  You will
  need to setup your own punjab server to translate HTML to XMPP, punjab is
  a flavor of server chosen to use any federated XMPP server, if you want to
  setup your own Jabber server (not using Gtalk, AIM, Facebook, etc, 
  or an existing federated service it does not need to be punjab) -- See the
  Book "Professional XMPP" for more information on choosing and setting up a 
  server ... it is an excellent book, and the author is very receptive to
  helping others. There is an excellent google group dedicated to support of this topic:
  https://groups.google.com/forum/?fromgroups#!forum/strophe
