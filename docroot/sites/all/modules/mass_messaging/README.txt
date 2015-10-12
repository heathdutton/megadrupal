Required Modules:
-----------------
  - Rules: For executing the action (optional)
  - Views: For composing the mail lists
  - Mime Mail: For sending the mails (optional; if you are using another 
     send method, this is not required.)
  

Installation:
-------------
Install the Module as usual via the admin interface.
 
 
Configuration:
--------------
1. Setup a view with a Message display. Define the new fields.
   Define the mapping between the views fields and the message fields. 
      
2. Setup a rule which sends the mails by using the view created in
   the first step. (Optional; If you want to use the API directly it is 
   also possible.)
 