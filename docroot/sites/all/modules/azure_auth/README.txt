
== Introduction ==

The Windows Azure Authentication module allows users to log in to your drupal
site using Windows Azure's federated login system. Currently, only Windows Live
and Google are supported as identity providers. Additionally, only Simple Web
Tokens are supported. SAML 2.0 support is planned.

Sponsored by R2integrated.
http://www.r2integrated.com

== Installation and Usage ==

1. Configure your Windows Azure account.
   Once your account is active:
   1.1  Create a namespace under the Service Bus.
   1.2  Open the management portal for the namespace.
   1.3  Add identity providers. Google and Windows Live ID are currently
        supported.
   1.4  Add a relying party application for your site. The return URL should be
        http://<your-host>/azure-auth/authenticate and the error URL should be
        http://<your-host>/azure-auth/authenticate/error. Token format should
        be SWT. Token encryption is not supported so leave Token encryption
        policy at "none". Select the identity providers you wish to use, and
        select a new rule group or create a new one. If you create a new one,
        you must generate claims for the identity providers, specifically the
        "nameidentifier" claim, as this is used to uniquely identify users.
        Generate or enter your token signing key.

2. Install the module.

3. Configure the module for your Windows Azure account at
   admin/config/people/azure-auth.
