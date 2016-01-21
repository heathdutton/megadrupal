MODULE NAME
-------------------------------------------------------------------------------
User Popup Info




DEPENDENCY
-------------------------------------------------------------------------------
This module requires jQuery version 1.5 to work properly and give you the best.
It is therefore, dependent on 'jQuery Update' module. You can download it from
here :
    'https://www.drupal.org/project/jquery_update'
After downloading and enabling jQuery Update module, go to 
    '/admin/config/development/jquery_update'
and set Default jQuery Version to 1.5(RECOMMENDED).




DESCRIPTION AND USAGE
-------------------------------------------------------------------------------
With this module, you can see a mini profile view of a user when mouse is
hovered over his/her profile link anywhere on the website. The effect is
similar to what you get when you hover mouse over your friend's name on social
networking sites. This module can detect 'realname' of a user and can display
it, if it exists.

On popup, by default, you can see user name, user status, user member
since, and user's picture or a default picture. You can now extend this
information by selecting which user fields to display, via module
configuration. 

You also have a feature to theme user profile popup, just create 
*.tpl file named "render_user_popup.tpl.php" and place it in your theme, or
template folder in your theme. Read the sample *.tpl file shipped with this
module to get further information.

To know more about what you are going to get in future from this
module, scroll below to FUTURE RELEASES section.




CONFIGURATION
-------------------------------------------------------------------------------
You can configure module settings at :
    '/user-popup-info-config'

These settings are :
    1. How should user links be detected
        a. Manually
            To detect user profile link manually, you need to add a class 
            'user-popup' with this link,

        b. Automatically
            This module can automatically detect user profile links on any
            given page giving the site administrator to sit back and relax.
            However, this approach may overload the server with AJAX requests.
            It is advisable to use this option only in case you do not have
            any server issues,

    2. User picture width
        Mention in numerical terms the width of user picture to show in mini
        profile popup,

    3. User picture height
        Mention in numerical terms the height of user picture to show in mini
        profile popup,

    4. User alias name (to support user aliases)
        To support user aliases, mention this textfield value as :
            a. trailing with a '/'
            b. enter only what every user alias URL has in common after site's
               base URL.

    5. Links that should be excluded
        Mention all those relative paths (be it comma separated or one path per
        line) on which you do not want this module to operate. Please apply a
        leading slash '/' before every path. This can be any system or custom
        path on your site. By default, links that are excluded are : user with
        id 1, /user/logout, and other similar user links that are of no use to
        this module.

    6. Additional fields that must be included in mini profile popup
        This setting automatically detects fields on user entity, and displays
        them in checkboxes for admin to select. Whichever field is selected,
        is shown in the mini profile popup as it is along with its label.

    7. Show default user info
        By default, the mini profile popup comes pre-populated with information
        like - user profile pic, user name, status, member since. This info can
        be disabled by setting 'No'.




FUTURE RELEASES
-------------------------------------------------------------------------------
The future releases of this module will provide :
- integration with Profile2 module




CREDITS
-------------------------------------------------------------------------------
Gauravjeet Singh < gauravjeet007@gmail.com >
D.O. username < gauravjeet_singh >

-------------------------------------------------------------------------------
