

                          The eXtensible Catalog

                                  NCIP

                              DOCUMENTATION

About NCIP
----------
National Information Standards Organization (NISO) Circulation Interchange
Protocol (NCIP) is a protocol for the exchange of messages between and among
computer-based applications to enable them to perform functions necessary to
lend and borrow items, to provide controlled access to electronic resources,
and to facilitate cooperative management of these functions.


About the NCIP Module
---------------------
The NCIP module is an extensible API that allows developers to create
applications that communicate via the NCIP protocol.

The NCIP module's API creates, configures, and manages NCIP applications,
connections, and messages. It also provides a simple user interface for listing
and modifying default NCIP settings as well as application and provider-specific
settings.

Although bundled with the XC Drupal Toolkit, the NCIP module is independent of
all other Drupal Toolkit modules.


API, Classes, and Objects
-------------------------
The NCIP module provides an API for creating and managing NCIP applications,
connections, and messages:
    - An application initiates or responds to connections to communicate with
      other applications
    - A connection is a collection of HTTP requests between two applications
    - A message is an XML document encoded according to UTF-8 specifications
      and streamed as-is over a connection

Each of these have their respective classes in the /ncip/lib folder, used to
build objects that interact with each other to implement the NCIP protocol. The
three classes are:
    - NCIPApplication
    - NCIPConnection
    - NCIPMessage

NCIPApplication objects manage a collection of connections and stores
specific properties of the application, such as the supported NCIP version,
“from” agency and system ID, and other message header options for authentication
and application profile type.

NCIPConnection objects belong to an NCIPApplication object and are of a
particular type specifying whether it is initiating or responding. These
objects facilitate the transport of messages to and from applications, and
store specific properties of the connected application, such as its “to” agency
and system ID and other message header options for authentication.

NCIPMessage objects should belong to an NCIPConnection object, but in rare
situations, they may not. These objects manage the content of an XML document,
allowing it to be edited in a more intuitive manner and transformed to and from
an XML string, SimpleXMLElement object, DOMDocument object, or NCIPMessage
object.


Implementation
--------------
On its own, the NCIP module does not implement any NCIP services (or verbs);
it only builds the foundation necessary to communicate over this protocol in
Drupal. Other modules must then take advantage of the NCIP module's features
and API to construct or parse messages sent or received to and from NCIP
applications. An example of such module is the NCIP Provider module, included
with the Drupal Toolkit.


Current Features
----------------
The module currently can create applications only with initiating connections
that expect a response after messages have been sent via HTTP. It supports all
current versions of the NCIP protocol.


Future Features
---------------
In the future, the NCIP module may support both an initiating and responding
connections, connect via TCP, or make use of implementation profiles.


Developer Information
---------------------
See the code documentation for more information on how to use the NCIP
module's API.


About the eXtensible Catalog
----------------------------
The NCIP module was designed and developed for use with the eXtensible Catalog.
It is part of a set of modules used to connect to other software toolkits,
specifically the eXtensible Catalog (XC) NCIP Toolkit, but can connect to other
NCIP providers as well.

The XC Drupal Toolkit receives user account information and circulation status
from an ILS by communicating back and forth over the NCIP protocol with the
XC NCIP Toolkit.

More information about the eXtensible Catalog can be found at the eXensible
Catalog website: http://www.extensiblecatalog.org
