== Introduction ==

The voipnumberfield.module provides an abstraction layer that facilitates the handling of the different kinds of "phone numbers" associated with Internet telephony calls. 

In particular, this module provides:

 - a new CCK VoIP Number field for storing landline, mobile phone numbers and SIP addresses.


== Installation ==

1. Extract voipnumberfield.module to your sites/all/modules directory

2. Enable the VoIP Number Field module in admin/build/modules


== Using VoIP Number CCK fields ==

Adding a VoIP Number field to an existing content type:

1. Choose any content type from admin/content/types and go to "manage fields"

2. Add a new field and select VoIP Number as its field type. 

3. Set field-specific options. For additional details, see the VoIP Number Options section below.  

4. Save

5. Create new content and you will see the VoIP Number!

6. Some of the options that can be saved along with VoIP Number:
*Number type. Select one of following options: PSTN number (default), SMS number, Mobile number, Work number, Home number, Fax number and 
SIP number(available only if "Allow SIP numbers" checkbox is checked for this field)

*Country. Some of the countries share their country code (for ex. USA, Canada and Puerto Rico are using same country code: +1), so its not possible to 
determine country by VoIP Number API. Therefore there is this optional field where you can select which country this number belongs to.

*Default number. VoIP Number Field associate its numbers with the node id. Since node can have multivalued VoIP Number fields or even multiple 
VoIP Number field, this option sets the default number to be used with this node id. This number then can be retrieved via 
VoIP Number API getDefaultNumberFromNid() function. This checkbox is unique per node and can be set only for one value regardles of number of 
VoIP Number fields in node.

Formatter options:

VoIP Number CCK fields come with the following formatters:

* Full number (E.164 format). Default formatter. Used to display the VoIP Number in E.164 format (for ex. +13477090908, or sip://user@domain.com)

* Full number (E.164 format) with country name. Used to display the VoIP Number in E.164 format prefixed by the country name (for ex. USA +13477090908)

* Local number. Used to display the VoIP Number without its country code with prefixed zero(for ex. 03477090908). 

* Full number (E.164 format) with additional data. Used to display VoIP Number in E.164 format together with additional data like number type.


== Including a default VoIP-based number in user profiles ==
!!!!!IMPORTANT!!!!!
This feature is deprecated. We recommend using VoIP User Number module instead (http://drupal.org/project/voipusernumber).


== VoIP Number Options ==

It is possible to configure the default VoIP Number settings at either the global level (by going to /admin/config/voipnumberfield) or on a field-by-field basis. 

* Allow SIP numbers. Disable if you don't want users to enter SIP addresses in VoIP Number Field.

Note: the following options are not applicable when using SIP numbers.

* Number length. Defines the maximum number of digits allowed in a VoIP Number. By default, this is set to 15, which is the current ITU standard.

* Country code settings:

** Allow all. Users will be allowed to select any value in the country code field
** Let user select from predefined list. You can determine which values will be allowed for users to choose from in the country code field
** Use default country code. You can set the one country code that will be used for the phone number. 


---
The VoIP Number Field module has been originally developed by Tamer Zoubi under the sponsorship of the MIT Center for Future Civic Media (http://civic.mit.edu).

