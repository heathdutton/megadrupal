== Introduction ==

The voipnumber.module provides an abstraction layer that facilitates the handling of the different kinds of "phone numbers" associated with Internet telephony calls. 

In particular, this module provides:

 - a new CCK VoIP Number field with specific widgets for storing landline and mobile phone numbers, SIP addresses, IM usernames and "unknown" number types 

 - a default VoIP Number that might be included directly into user profiles

 - an API that enables access to the different VoIP Numbers associated with individual users and content types

 - A "VoIP Number Phone" and a "VoIP Number SMS" module to facilitate the integration with the telephone numbers already provided by the Phone module (http://drupal.org/project/phone) and the SMS Framework (http://drupal.org/project/smsframework)


== Installation ==

1. Extract voipnumber.module to your sites/all/modules directory

2. Enable the VoIP Number module in admin/build/modules

3. Optionally enable "VoIP Number Phone" and/or "VoIP Number SMS" modules to use telephone numbers already defined by the Phone modules and the SMS Framework, respectively


== Using VoIP Number CCK fields ==

Adding a VoIP Number field to an existing content type:

1. Choose any content type from admin/content/types and go to "manage fields"

2. Add a new field and select VoIP Number as its field type. Choose one of the widgets according to your needs (landline, mobile, SIP, IM username or unknown)

3. Set field-specific options. For additional details, see the VoIP Number Options section below.  

4. Save

5. Create new content and you will see the VoIP Number!


Formatter options:

VoIP Number CCK fields come with the following formatters:

* Full number (default). Used to display the VoIP Number in full (for ex. +13477090908)

* Full number with country name. Used to display the VoIP Number prefixed by the country name (for ex. USA +13477090908)

* Number without country code. Used to display the VoIP Number without its country code (for ex. 77090908). 


== Including a default VoIP-based number in user profiles ==

1. Enable the "use default voipnumber" permission (from admin/user/permissions) for the roles you want to have access to VoIP Number

2. Now users will have a default VoIP Number field in their user profile


== VoIP Number Options ==

It is possible to configure the default VoIP Number settings at either the global level (by going to /admin/settings/voipnumber) or on a field-by-field basis. 

Note: the following options are only applicable to landline and mobile widgets.

* Number length. Defines the maximum number of digits allowed in a VoIP Number. By default, this is set to 15, which is the current ITU standard.

* Country code settings:

** Allow all. Users will be allowed to select any value in the country code field
** Let user select from predefined list. You can determine which values will be allowed for users to choose from in the country code field
** Use default country code. You can set the one country code that will be used for the phone number. Users will not have the option to changing it. 


== The VoIP Number API ==

For API usage see voipnumber.inc

Other functions:
* voipnumber_get_code($country) 
  Returns the country code associated with the given country name.


Hooks:

The VoIP Number module provides hook_get_voip_numbers() to enable other modules to add their own custom numbers to the VoIP Number API.

hook_get_voip_numbers($id, $type, $op):
$type - can be user, node, uids, nids for returing numbers by specific user, specific node, list of user ids by specific number and 
        list of node ids by specific number.
$op - is either uid, nid or number, depending on value of $type

For example of implementation check VoIP Phone and/or VoIP SMS modules.


---
The VoIP Number module has been originally developed by Tamer Zoubi under the sponsorship of the MIT Center for Future Civic Media (http://civic.mit.edu).

