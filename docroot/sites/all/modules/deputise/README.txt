README.txt
==========

This module aims to allow site admins to grant granular administration
privileges to "Deputies".

Site admins with the appropriate permissions can select which modules are 
visible to 'Deputised' users (who have the normal permissions to use the 
modules settings form). So the Deputies can change which modules are enabled,
but certain modules can be hidden from them.

Any hidden modules which are enabled remain enabled when Deputised users make
changes.

A common use case is to hide the PHP filter module, removing the ability to  
switch on the evaluation of PHP code in input filtered content.

This might be particularly useful in a managed hosted environment where site
owners should have a high level of control, but do not have access to the user
#1 account.

There are both D6 and D7 versions of the module.
