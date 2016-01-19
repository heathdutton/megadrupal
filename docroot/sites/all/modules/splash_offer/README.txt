SUMMARY:
Allows the management of one or more "splash offers", which are modal popups
with an accept or reject form. If a user accepts the offer then a new page will
open with a provided accept url. If they reject the offer, the modal will close.
(If they select Do not Show Again, then a cookie is set so as to avoid futher
display.) You may define the who, when and where of the splash offers with
precision controls. Factors such as: user role, site url, device, and whether
the offer has been previously viewed (cookies), cookie duration, etc. are
configurable for each splash offer created.

The module was borne as a way to advertise a website companion app and
offer a button to download, but it's generalized nature has the potential for
many other applications.


REQUIREMENTS:
* Javascript is required.
* This project creates a new entity type and thus depends on the Entity API:

http://drupal.org/project/entity


ADDITIONAL FUNCTIONALITY:
* Device detection is supported if you install the optional module:

http://drupal.org/project/mobile_detect


INSTALLATION:
* Download and unzip this module into your modules directory.
* Goto Administer > Site Building > Modules and enable this module.


CONFIGURATION:
* To create you first offer visit: Structure > Splash Offers > Add a Splash
  Offer; your role will need to have Administer Splash Offer permission.
* Some degree of css styling is required to achieve more than the vanilla shell
  you get with this module. Refer to splash_offer.tpl.php for more info. You
  should override the module's css in your theme or custom module.
* See also splash_offer.api.inc for module hooks.
* HINT: Leave cookies disabled during development and theming of the splash
  offer.  Otherwise you'll have to keep clearing cookies to see the popup.


USAGE:
* After creating your splash offer, you may disable it temporarily by editing
  the Other fieldset and unchecking the Active checkbox.


API:
* See also splash_offer.api.inc for module hooks.


TROUBLESHOOTING:

"I can't see the splash offer anymore!"
* Make sure your browser has JS enabled
* Check to see if you have 'Use cookies?' enabled in the 'Repeat Viewing'
  section, if so uncheck that.
* Try checking both 'anonymous user' and 'authenticated user' for the roles
  setting.
* Trying setting page visibility to 'All pages except those listed' and remove
  any values in the 'Specify pages' text area.
* Make sure 'Always trigger, regardless of device' is checked.
* If you still can't see the offer, view the html source and look for a div
  called '.entity-splash-offer'; if you see that then check your css to make
  sure it's correct.

"I have multiple offers, but I only every see the first one"
* The module is designed to only show one offer per page load, even if you've
  defined multiple offers. So if you do not have cookies enabled, you will only
  ever see the first offer. Try enabling cookies.
* Offers are presented lowest weight first, which can be set in the 'Other'
  fieldset of the edit form.  Try adjusting the weights.


--------------------------------------------------------
CONTACT:
In the Loft Studios
Aaron Klump - Web Developer
PO Box 29294 Bellingham, WA 98228-1294
aim: theloft101
skype: intheloftstudios

http://www.InTheLoftStudios.com
