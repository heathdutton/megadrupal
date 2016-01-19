
DESCRIPTION
---------------
QQi connects you to over 700 million Chinese QQ accounts and
many of your favorite websites and useful tools. It's the best
place to stay in touch with friends and make new ones.
The latest international version download:
http://www.imqq.com

About users OpenAPI.(For example obtain user information,
dynamic synchronous, photos, log, share, etc).
In order to protect the security and privacy of user data.
Third-party websites to access user data before all
need explicit to users ask for authorization.

QQ login OAuth2.0 using OAuth2.0 standard protocol proceed user
authentication and obtaining user authorization.
Relative to the before OAuth1.0 agreement.
The certification process simpler and more secure.

OAuth 2.0(http://oauth.net/2/) is the next evolution of the OAuth protocol
which was originally created in late 2006.
OAuth 2.0 focuses on client developer simplicity while
providing specific authorization flows for web applications,
desktop applications, mobile phones, and living room devices.
This specification is being developed within the IETF
OAuth WG and is based on the OAuth WRAP proposal.

You can apply the QQ Login application for
your site by http://connect.qq.com/intro/login

The steps for applying for QQ the Login application:
1, You must have a QQ account, registered in http://www.imqq.com.
2, Login and submit some necessary information,
You will get the APP ID and KEY.
3, Reference prompted to verify your site.

Installation
------------
1. Move this folder into your modules directory.
2. Enable this module.

Configuration
---------------
Configure it at Administration >> Configuration >> Web services >> qq login.
Enter your APP ID and and KEY.
About scope, We only need to get_user_info.
Like this: http://drupal.org/files/20120324095527.png
