Implements WAYF login

See http://wayf.dk/ for details about becoming identity- or service-provider.

The SPorto SAML class was originally written by Mads Freek.
See https://code.google.com/p/sporto/ for details

Author: Tom Helmer Hansen, tom@adapt.dk, 2013

Configuration
==================

- WAYF

1) Goto WAYF.dk self-service JANUS https://janus.wayf.dk and create 
   a new sp connection.
   
   Connection ID: should be in URL format eg. http://wayf.adapt.dk
   Type should be : SAML 2.0 SP
    
2) Configure metadata: 
   AssertionConsumerService:0:Binding : urn:oasis:names:tc:SAML:2.0:bindings:HTTP-POST
   AssertionConsumerService:0:Location : http://<your-hostname>/wayf/consume
   certData : Your public key, a self-generated cert will work in test-mode
   description:da, description:en, name:da and name:en ... some text to show to 
   users

   Support for Logout needs to have:
     SingleLogoutService:0:Binding : urn:oasis:names:tc:SAML:2.0:bindings:HTTP-Redirect
     SingleLogoutService:0:Location : http://<your-hostname>/wayf/logout

   NB! you can add up to 3 AssertionConsumerServices and SingleLogoutService by adding (plus icon) :

     AssertionConsumerService:1:Binding
     AssertionConsumerService:1:Location
   
     AssertionConsumerService:2:Binding
     AssertionConsumerService:2:Location

   .. this way your sp can have a test/dev/production location

   Tip: You can test the validity of you configuration by selecting
   "Export metadata" in the "Export" tab.

- Drupal 

1) Install the module

2) Goto admin/config/people/wayf

   * WAYF bridge

   Service mode should match your status on WAYF.dk
   The location of WAYF's single sign on page and their public certificate
   are fetched by the module.

   * Service provider
     
     Connection ID, the ID you entered in JANUS

     AssertionConsumerService:0:Location, on of the locations you entered in 
     JANUS.
     
     Private key, your private key for the same certificate you entered the 
     public key for in JANUS. The private key should start with 
     -----BEGIN RSA PRIVATE KEY----- and end with -----END RSA PRIVATE KEY-----

   * Field mappings

   The module can map textfields from users to attributes released from
   WAYF. If you don't have any fields for users goto 
   admin/config/people/accounts/fields and add some text fields.. 
   These singular attributes can be mapped:

    urn:oid:2.5.4.4                   Last name
    urn:oid:2.5.4.42                  First name
    urn:oid:2.5.4.3                   Nickname
    urn:oid:2.5.4.10                  Organisation nickname
    urn:oid:1.3.6.1.4.1.5923.1.1.1.5  Primary user affiliation

   * User mappings

   Local security domain (scope) can be set if you have local users created eg. 
   with LDAP synchronization.

   eduPersonPrincipalName is the The "NetID" of the person for the purposes of 
   inter-institutional authentication. It is represented in the form 
   "user@scope" where scope defines a local security domain. When setting a 
   local scope users will be mapped by their username alone ( the part before 
   @scope )

   Autocreate scoped users, with this setting checked users will be auto-created
   with eduPersonPrincipalNameas their username. 

   * Display settings

   Here you can enable the login icon on the default Drupal login form.
   There's a range of icons to choose between.

3) There's a content pane "WAYF.dk login" under widgets you can add to you pages 
   with the page manager. The content pane is only visible if current user is 
   anonymous (user 0).
