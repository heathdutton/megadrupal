
--- DESCRIPTION ---

 Access code module allows site visitors to log in using an access code instead
 of entering username and password. The module aims at expediting the process of
 setting up and distributing account information to new users. It is intended to
 be used in cases where user accounts will most likely be accessed a few times
 only, and you don't want the users to go through the hassle of setting up a
 username and password.


 > Features

  * Access codes can be managed on the user edit form

  * A separate login form is provided to log in with an access code (user/ac)

  * An access link containing the code is also available (ac/[access code]),
   which logs in the visitor automatically when visited

  * Access codes can be set to expire


 > Security considerations

 Please keep in mind that using access codes as a way of authentication is
 significantly less secure than the traditional username/password
 authentication.


--- INSTALLATION ---

 Regular modul installation procedure.

  * Copy the access_code directory into the modules' directory.

  * Enable the module.


--- USAGE ---

  * To set an access code for a user, go to the user's profile edit form, and
    speficy the code in the "User's access code" field, and select the date of
    expiration.

  * After setting the code, you can either

     - give the code to the user, and they can log in on the 'Use access code'
       form (user/ac). The form is also accessible through the login block.

     - or give the access link to the user (the link will look like this:
       http://www.yoursite.com/ac/[access code]), and they will get
       automatically logged in when they click the link. (the link is shown on
       the user edit form)

  * To invalidate an access code, go to the user's profile edit form, and either
    remove the access code and leave the field blank, or set the expiration date
    to a past date. Alternatively, you can block the user altogether, if
    necessary.
