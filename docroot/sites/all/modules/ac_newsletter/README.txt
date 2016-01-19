ActiveCampaign Newsletter Signup
================================

Description
-----------

ActiveCampaign Newsletter Signup module provides Newsletter Signup Form
Integration with ActiveCampaign (http://www.activecampaign.com) using
ActiveCampaign API Php library (https://github.com/ActiveCampaign/
activecampaign-api-php).

Features
--------
The site administer can able to-

1. Enable/Disable the subscription form fields.

2. Make fields required.

3. Enable/Disable form fields placeholder and description.

4. Customize the fields placeholder, description and empty error message.

5. (Optionally) Enable Double Opt-in on the form.

6. (Optionally) Enable and Connect with BriteVerify API for the valid email
check.

Sponsored by DrupalGeeks.org (http://www.drupalgeeks.org/).

Dependencies
------------
Drupal 7 latest version
Libraries API

You need to download the ActiveCampaign API Php library from the URL (https://
github.com/ActiveCampaign/activecampaign-api-php).

Installation
------------
Download and install the Libraries API module and the ActiveCampaign
Newsletter Signup module as normal. Just like other modules. Install modules
on "sites/all/contrib/module/" folder. Then download the ActiveCampaign API
Php library.

Download ActiveCampaign API Php library

Unpack and rename the library directory to "activecampaign" and place it
inside the "sites/all/libraries" directory. Make sure the path to the library
file becomes: "sites/all/libraries/activecampaign/includes/ActiveCampaign.class
.php"

Download and Install using Drush: drush dl ac_newsletter && drush dl libraries
&& drush en ac_newsletter -y (Make sure that ActiveCampaign API Php library is
placed in the "sites/all/libraries" directory)

Configuration
-------------
1. Connect the module with ActiveCampaign Account. To Connect, Provide the
ActiveCampaign account API URL and API KEY and then click 'Test Connection'
button to confirm the connection.

2. Configure the lists for the site from the ActiveCampaign Account lists.

3. (Optional) Enable Double Opt-in checking for each subscription. To Enable
this, Create a form in ActiveCampaign with Double Opt-in enabled and provide
that form id.

4. (Optional) Enable and Connect BriteVerify API for valid email checking. To
Enable this, Provide a valid BriteVerify API KEY.

5. Configure the form specific lists on the block configuration (admin/
structure/block/manage/ac/ac_signup/configure).

6. Configure the form for enable/disable the form elements like First name
Field, last name field, subscription option, list option if more lists are
there, fields placeholder and field's description.

7. Customizing options for form fields placeholders, description, error
message.
