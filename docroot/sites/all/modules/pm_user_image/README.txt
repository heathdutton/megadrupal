
-- SUMMARY --

  "PM User Image" module helps to show the user profile image in 
 
   user auto complete text box in private message module.

   Whenever a user wants to send a private message to another user, 

   he / she types the name of user in text box

   and this module shows the user image along with the user name.

-- REQUIREMENTS --

* Private Message module

* Enable the clean url


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions

* GO to admin/config/messaging/privatemsg/pm-user-image
  
  * set the image height and width property. If not set,

    default 50px by 50px will be used.
  
  * If Image Path type is "External" then provide the full url for 
  
    default image path like - http://www.xyz.com/imagepath/---
  
  * If image path is "Internal" , then upload the image
