SMSGatewayHub
=============

This module allows you to send SMS using
[smsgatewayhub](http://www.smsgatewayhub.com) service.

### Installation
Follow the usual installation [steps](http://www.drupal.org/node/120641).

### Usage
- Create an account in [smsgatewayhub](http://www.smsgatewayhub.com). You will
get username and password.
Make sure you are allowed to send
[promotional SMS](http://www.smsgatewayhub.com/promotional-sms).
[Transactional SMS](http://www.smsgatewayhub.com/transactional-sms) can be sent on request.
- Get sender Id from [smsgatewayhub](http://login.smsgatewayhub.com)
- Enter the credentials in setting page `admin/config/services/smsgatewayhub`

### Features
- Integration with [rules](http://www.drupal.org/project/rules), it provides
rules events and action.
- API hooks like, execute operations before or after sending SMS. Or, you can
alter SMS message using hook.

Read `smsgatewayhub.api.php` for API documentation.
