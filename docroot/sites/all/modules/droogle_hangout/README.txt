INTRODUCTION
------------
Drupal Hangout is written specifically with Google Education and
Business accounts in mind so that organizations can effectively
communicate using Google Hangout right from within their Drupal site.
Droogle Hangout provides an API connection between Google and Drupal.

It uses Google's calendar API to create a calendar event and grab the
Hangout link that gets generated when a calendar event is created. See
the Google Configuration section to ensure users have a Google Hangout
link enabled for their calendars.

After Droogle Hangout is enabled and configured, users will see a
Hangout icon next any username displayed in the site.  Clicking on the
Hangout icon will prefill a popup window with the username you wish to
create a Hangout with.  You can click on the Hangout button for other
users to add more users to the popup window.  Once you are done adding
users for your Hangout, you simply click the Start Hangout button and a
Hangout is created.  A new window will open with the Hangout session
started along with a link to the Hangout.  Additionally, an email is
sent to the invitees of the Hangout with the link to the Hangout.

GOOGLE CONFIGURATION
--------------------
Droogle Hangout uses OAuth2 which Google uses for it's API.  As an
introduction to how Google's API works please read
https://developers.google.com/accounts/docs/OAuth2.

In order to make Droogle Hangout work you will need to set up a project
in the Google Developers Console <https://console.developers.google.com/>
to obtain OAuth2 credentials.  If you have not done so already then
choose Create Project.  Otherwise, in the left sidebar under your
project name, click on APIs & auth > APIs.
 * Turn on the Calendar API.
 * In the APIs & auth section click Credentials.
 * Under the OAuth section, Create new Client ID.
   - Choose Web application.
   - Under Authorized Javascript Origins enter your website url.
   - Under Authorized Redirect URI, enter the following:
     https://developers.google.com/oauthplayground
     http://YOUR-WEBSITE-DOMAIN/droogle_hangout_create_hangout
 * Click Create Client ID button.

You should now have a Client ID and a Client Secret.  You will need
these under the configuration section.

Create a Refresh Token.
A refresh token allows your application to obtain new access tokens.
Go to https://developers.google.com/oauthplayground/
 * Settings > Access Type = Offline
 * Use your own OAuth credentials = Checked
 * Enter in your Client ID and Secret that you created in the steps
   above.
In oauthplayground, select the api's to authorize.
Choose Calendar api and press Authorize APIs button
After granting access, press Exchange Authorization code for tokens button
See https://www.youtube.com/watch?v=hfWe1gPCnzc for details instructions
if you have any trouble.

Note:
You will also need to ensure that users of your site have the Hangout
link enabled in their calendar settings:
http://googleappsupdates.blogspot.com/2014/03/new-google-calendar-events-will-now_26.html


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

Requirements
------------
 * The JQuery Update dependency requires that JQuery be updated to at
   least 1.10 and JQuery UI to be set.
   - This can be set at Administration > Configuration > JQuery Update.
   - If you have trouble getting JQuery UI to load choose an option
     under JQuery and JQuery UI CDN such as Google.
 * The Libraries dependency is necessary because you'll need to download
   the Google API PHP Client from GitHub
   - https://github.com/google/google-api-php-client
   - The folder should be named google-api-php-client and copied into
     the site/all/libraries folder.

CONFIGURATION
-------------
 * Configure user permissions in Administration > People > Permissions:
   - Grant the Administer Droogle Hangout permission to any roles that
     should have the ability to administer Droogle Hangout.
 * Enter the Google Console credentials in Administration > Configuration >
   Droogle Hangout Settings.
   - Enter the email address for the account that is used for Google
     Console credentials.
   - Enter the Google Console Client ID.
   - Enter the Google Client Secret.
   - Enter the Google Refresh Token.
 * Create custom css rules to make the Hangout windows match your site's
   theme under Administration > Configuration > Droogle Hangout
   Settings.
   - Under Custom CSS Rules for Droogle Hangout you can modify the
     following rules by adding them in the Custom CSS for Hangout popup
     window textarea:
     #content #block-droogle-hangout-droogle-hangout-invites h2,
     #block-droogle-hangout-droogle-hangout-invites h2 {
       background-color: #2A93D3;
     }
     .slant-45 {
       border-right: 20px solid #2A93D3;
     }
     #hangout-invite-popup {
       border: solid 5px #2A93D3;
     }
     .hangout-btn-bubble {
       background-color: #525050;
     }
     .hangout-btn-bubble:after {
       border-right: 8px solid #525050;
     }
     #google-popup-logo {
       background: url(http://YOURSITE.com/sites/all/modules/custom/droogle_hangout/images/hangout_logo_blue.png) no-repeat 0 0 transparent;
       /* You can replace png file with hangout_logo_green.png,
       hangout_logo_red.png, or hangout_logo_yellow.png */
     }
     #close-hangout-invite-popup:hover {
       background-color: #666666
     }
     .hangout-sent {
       border: solid 3px #2A93D3;
       border-top: solid 20px #2A93D3;
     }
     .hangout-sent a:visited,
     .hangout-sent a {
       background-color: #2A93D3;
     }

SCHEDULING A HANGOUT
--------------------
 * To allow users the ability to schedule a hangout you need to add
   Trent Richardson's datetimepicker add-on.
   http://trentrichardson.com/examples/timepicker/
   - Create a folder labeled datetimepicker to be placed in
     sites/all/libraries.
   - Add the following javascript files to the folder:
      * jquery-ui-sliderAccess.js
      * jquery-ui-timepicker-addon.js
   - For your convenience, you can get the folder and javascript files
     at https://github.com/hurley-drupal/datetimepicker
