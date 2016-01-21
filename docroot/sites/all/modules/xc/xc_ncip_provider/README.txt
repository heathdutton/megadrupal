

                          The eXtensible Catalog

                             XC NCIP Provider

                               DOCUMENTATION

About NCIP
----------
National Information Standards Organization (NISO) Circulation Interchange
Protocol (NCIP) is a protocol for the exchange of messages between and among
computer-based applications to enable them to perform functions necessary to
lend and borrow items, to provide controlled access to electronic resources,
and to facilitate cooperative management of these functions.


About the XC NCIP Provider Module
---------------------------------
The XC NCIP Provider module provides an API and user interface to implement
common NCIP services (or verbs) through the NCIP module.

The XC NCIP Provider module allows:
    - Setting an identity for the NCIP application that will connect to
      NCIP providers (agency and system ID and authentication, authentication
      profile type, etc.)
    - Connecting to multiple NCIP providers
    - Sending test XML requests to the NCIP providers from within Drupal for
      specific NCIP services
    - Getting the live circulation status, location, and call number for an
      item within an ILS and have it appear in search results
    - Creating customized login forms for users to log in to the Drupal
      interface using a username and password supported by the institution's
      ILS or LDAP server
    - Fetching account information from an institution's ILS or LDAP server
      for display with the Drupal interface


Current Features
----------------
The XC NCIP Provider module currently supports the following NCIP services:
    - AuthenticateUser
    - LookupUser
    - LookupItem
    - LookupVersion
    - RecallItem
    - RenewItem
    - RequestItem
    - XCGetAvailability


Developer Information
---------------------
See the code documentation for more information on how to implement these
services.


Future Features
---------------
In the future, the XC NCIP Provider module will likely support more services
and take more advantage of the NCIP module's API.


About the eXtensible Catalog
----------------------------
The XC NCIP Provider module was designed and developed for use with the
eXtensible Catalog and is part of a set of modules used to connect to other
software toolkits, specifically the eXtensible Catalog (XC) NCIP Toolkit. It
can, however, connect to other NCIP providers as well.

The XC Drupal Toolkit receives user account information and circulation status
from an ILS by communicating back and forth over the NCIP protocol with the
XC NCIP Toolkit.

More information about the eXtensible Catalog can be found at the eXensible
Catalog website: http://www.extensiblecatalog.org

