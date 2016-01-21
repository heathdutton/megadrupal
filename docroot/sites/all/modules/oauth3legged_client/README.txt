README.txt
--------------------

DESCRIPTION
--------------------
The Oauth3legged_client module is designed to be a component of 3 legged oauth 
in multiple servers. This includes getting auth_token from oauth1.0a server 
(Services 3.x module) and saving the token in user profile. If you want to get 
and save oauth token from oauth provider in seperate server and save it per 
user basic --then this is the module for you.

One of the strengths of Oauth3legged_client is that it is installed on serpated 
server. So that it unlock the architecture that oauth should be in production 
scenario.

INSTALLATION
--------------------
To install this module, do the following:
1. Extract the tar ball that you downloaded from Drupal.org.
   or drush dl oauth3legged_client
2. Enable the module
3. Install php oauth plugin if required.


CONFIGURATION
--------------------
To configure this module do the following:
1. Go to People -> Permissions (admin/people/permissions) and find the relevant 
   module permissions underneath the "administer Oauth provider consumer." 
   section. If you are not logged in as user #1, you must give at least one role 
   (probably the administrator role) the 'administer Oauth provider consumer' permission to 
   configure this module.
2. Create required field in user profile according to configuration.
3. Go to Configuration -> Oauth consumer configuration (admin/config/services/
   oauth3legged_client) and configure the module settings per your requirements. 
   Add the field names created at 2 here. You will need key & secret generated 
   by oauth provider server.
4. Using a web service client / mobile client (iOS in my case) to test 
   connections.

P.S. You need to configure oauth provider callback URL back to the server 
installed this module with E.G.
URL: dios://<Oauth_consumer_server(server have this module)>/
     oauth3legged_client/oauthstart/authenticated/token
*authenticated/token is a temp fix to let this module knows the request right 
now is coming back from oauth server. dios is your custom URL schema.

FAQ
--------------------
Q: How to configure oauth provider ?
A: Please checkout services 3.x's oauth implementation.

Q: What is dios in the callback URL ?
A: This is a custom URL schema for your client going to oauth (iOS client in my 
   case) and trigger an action. It can be other things, just custom as your 
   requirement.

Q: Why I cannot directly redirect with http schema back to the oauth consumer 
   server ?
A: Some client like ios, it need to open an UIWebview to have the oauth process 
   while it need a trigger to close it. If the rediect call directly goes to 
   http, it is hard to find another way to close the oauth process. Or please 
   kindly suggest in the forum if you figured out a better way :)
