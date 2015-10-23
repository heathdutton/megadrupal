
-- SUMMARY --

Add Drupal as a webapp in Ubuntus Unity.

Current set of Features:
* It adds a Shortcut to ubuntus launcher.
* Allows integration for menu shortcuts in the Unity HUD.



-- REQUIREMENTS --

Uniy Webapps must be installed in ubuntu
http://www.omgubuntu.co.uk/2012/07/how-to-install-ubuntus-new-web-apps-feature


-- INSTALLATION --

1. Install as usual, see http://drupal.org/node/70151 for further information.

2. After install make shure to set permissions for users that should have access.
admin/people/permissions%23module-unity_webapp

-- CONFIGURATION --

nothing there yet
it's possible to add menu links by returning an array in hook 
hook_unity_webapp_addlinks()

see unity_webapp.api.php for example usage
