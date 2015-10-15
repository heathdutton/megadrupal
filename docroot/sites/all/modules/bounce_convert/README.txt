================================================================================
 INTRODUCTION
================================================================================

This module was originally authored, and is currently maintained by: 
- Jordan Magnuson <https://drupal.org/user/269983>
- Zafar Faizi <https://drupal.org/user/1877634>

Bounce Convert allows you to run "exit-intent" campaigns to show modal webforms
to visitors who are leaving your website (determined by mouse cursor position 
and velocity). This allows you to get an extra pageview from visitors that would 
otherwise have been lost, and providesan opportunity to make offers, collect 
email addresses, poll visitors, or ask them why they are leaving. One common 
technique is to provide a special offer which requires the user to sign up for 
your newsletter, thereby "capturing" the visitor as they are leaving, instead 
of losing them.

This module was created to provide a Drupal-based alternative to expensive
web services like Exit Monitor <exitmonitor.com> and Bounce Exchange 
<bounceexchange.com>.

Unlike other Drupal-based solutions (like the Bounce Reasons module
<drupal.org/project/reasonsbounce>), Bounce Convert treats campagins as nodes, 
and allows you to collect and track impression and conversion data for every 
campaign that you run.

================================================================================
 Screencast
================================================================================

A brief introductory screencast is available at <http://youtu.be/ilIfhD7J9ek>.

================================================================================
 DEPENDENCIES
================================================================================

This module requires the following modules be installed:

- CTools <https://drupal.org/project/ctools>
- Modal Forms <https://drupal.org/project/modal_forms>
- Webform <https://drupal.org/project/webform>
  - (Tested only with Webform 4.x... may or may not work with 3.x)
- Charts <https://drupal.org/project/charts>

================================================================================
 INSTALLATION
================================================================================

This module has no special installation requirements other than requiring that 
you first install the listed dependency modules. For general instruction on 
how to install and update Drupal modules see 
<http://drupal.org/getting-started/install-contrib>.

================================================================================
 CREATING YOUR FIRST BOUNCE CONVERT CAMPAIGN
================================================================================

*Every Bounce Convert campaign is tied to a single Webform node.*
Before creating your first Bounce Convert campaign, you will need to create
a Webform node to be the basis for the campaign. This webform could consist of a 
single email field if you are attempting to collect the visitor's  email address 
for your newsletter, or it could be short survey, etc. To create a Webform node, 
go to Content -> Add Content -> Webform from within your Drupal admin area 
(node/add/webform). 

Once you have created your Webform, you can create your first Bounce Convert 
campaign by going to Content -> Add Content -> Bounce Convert Campaign from 
within your Drupal admin area (node/add/bounce-convert-campaign). Give your
campaign a title, choose the webform you want to use as a basis, and fill
in the rest of the required fields. To test your campaign, we suggest you set
a cookie expiration time of 0, so that the campaign will always be shown.

After saving your campaign, try moving your mouse above the browser plane (for
example, by moving your mouse as if to click your browser's "back" button). The
modal popup should then be displayed. 

================================================================================
 THEMING
================================================================================

Your webform and modal popup can be themed in the same way that you would
theme any other Drupal elements. Generally, getting the modal to popup, then 
inspecting the markup with a tool like Firebug and adding some targeted css to 
your theme's style.css file is all that is needed.

Sample css can be found at 
<https://gist.github.com/JordanMagnuson/ca86c19f7a05e0bff4ba>

================================================================================
 TROUBLESHOOTING
================================================================================

If your modal popup is not being displayed as expected:

1. Make sure that you have created a Bounce Convert Campaign node.
2. Make sure that you are moving your mouse cursor above the browser plane (for
   example, by moving your mouse as if to click your browser's "back" button).
3. Make sure that your campaign is set to ACTIVE and PUBLISHED (two separate
   settings).
4. Check your campaign's page visitility and role visibility settings. For
   testing, we suggest you remove all page rules and role visibility roles.
5. Make sure that the cookie expiration time has passed, or the popup will
   not be shown again for your browser. For testing, set cookie expiration to
   0 (zero).
   
================================================================================
 WARNING ABOUT MULTIPLE ACTIVE CAMPAIGNS
================================================================================   
   
It is possible to have multiple Bounce Convert campaigns running at once, but
if you have multiple active campaigns running at the same time, make sure that
the visibility settings for the active campaigns do not overlap. The module
is not designed to show more than one campaign to the same user on the same 
page, and will break if you try to do so.   
   
================================================================================
 CAMPAIGN REPORTS
================================================================================

You can get report data for all your Bounce Convert campaigns by visiting
Admin -> Reports -> Bounce Convert within your Drupal admin area 
(admin/reports/bounce-convert). Click on a campaign to get graphical
impression and conversion data, as well as to see a list of webform submissions
for that campaign.
