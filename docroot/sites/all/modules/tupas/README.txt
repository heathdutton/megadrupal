Description
-----------

TUPAS Authentication module is a general tool for enabling strong session-based authentication in Drupal web applications.

TUPAS is an authentication service provided by the Federation of Finnish Financial Services (www.fkl.fi) and supported by most of the Finnish banks. Read more about TUPAS from https://myacc.tut.fi/tupas/docs/TUPAS_V22_eng.pdf.

The module grants a role to an authenticated user for a given period of time. Within that time, the user has a permission (created by the module upon installation) to access areas restricted to users with that permission.

The module comes with a test module (tupas_test) for testing the functionality and as a basic example of how to use the authentication module.


Installation
------------

1) Extract the .zip to your modules directory.
2) Enable the TUPAS authentication module (optionally you can enable the TUPAS test module as well to test the  functionality).


Usage
-----

1) Configuring the bank and general settings
	1.1) Bank settings
		1.1.1) Editing existing banks
			- There are some banks inserted upon installation with test values. Use them with the TUPAS test module
			- The banks can be enabled/disabled and their settings can be changed.
		1.1.2) Adding a new bank
			- Fill the values of the new bank form and click Save.
		1.1.3) Removing banks
			- Banks can be removed from the system by checking the Delete checkbox and clicking Save.
	1.2) General settings
		1.2.1) Session length
			- Insert session length in minutes. 0 for unlimited session that expires only on logout.
		1.2.2) Identification type
			- Type of the identification sent back from the bank. See the TUPAS documentation for details.
		1.2.3) Return handler
			- This is the most important part. In your module utilizing the TUPAS module, you must provide a menu item with a page callback and two page arguments. Enter the menu item name here. See the TUPAS test module code for details.
		1.2.4) Canceled transaction
			- Enter a menu item where the user will be redirecter after a canceled transaction.
		1.2.5) Rejected transaction 
			- Enter a menu item where the user will be redirecter after a rejected transaction (authentication failed).
		1.2.6) Expired authentication
			- Enter a menu item where the user will be redirecter after the authentication period has expired.
2) Using the module
	Much of the module's functionality is described in the source code of the modules. They are probably the best source for developer information at this point. Basically the flow goes like this:
		1. The bank forms are built by calling the form functions of the module for each enabled bank.
		2. The user clicks one of the bank buttons and authenticates with his/her credentials.
		3. The user is then redirected to the return handler page defined in 1.2.3. The return handler calls the tupas_return() function that parses the return data from the bank and determines if the authentication was successful or not. If it was, the return data is returned to the return handler and the user is granted a role for the given period of time.
		4. When the authentication period has passed (or the user logs out), the role is removed from the user.
		
		
Author
------
Lauri Kolehmainen <http://drupal.org/user/436736>
Juha Niemi <http://drupal.org/user/157732>
Sampo Turve <http://drupal.org/user/669530>

Credits
-------
Exove Ltd (www.exove.com)
Vesa Palmu / Moana (www.moana.fi)
The Finnish Red Cross (www.punainenristi.fi)
Mearra (www.mearra.com)
