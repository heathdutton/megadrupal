# mailcheck

Get fast, client-side suggestions for mistyped email addresses.

## What is it?
An integration of [mailcheck.js](https://github.com/tableau-mkt/mailcheck) with forms in Drupal (supports user registration, webforms)

## What's it do?

  Someone enters `user@gmaul.com`, mailcheck asks, Did you mean `user@gmail.com`?

### Reduces Email Bounces

Kicksend [reports of a 50% decrease](http://blog.kicksend.com/how-we-decreased-sign-up-confirmation-email-bounces-by-50/) in sign up confirmation email bounces. Try it out on [Kicksend's join form](http://kicksend.com/join).

## Configuration Options
- Enable on webform forms
- Enable on user registration form
- Custom suggestion message (multilingual)
- Custom domains and top-level domains lists
- Custom distance threshold (note: this feature is in [tableau-mkt/mailcheck](https://github.com/tableau-mkt/mailcheck) fork on GitHub)
- Shake gesture on typos

## Installation
1. Download and enable the module
2. Download the latest version of [mailcheck.js](https://github.com/tableau-mkt/mailcheck/tree/master/src) and save it in your Drupal site at `sites/all/libraries/mailcheck/mailcheck.js`
4. Configure at `admin/config/people/mailcheck`
5. See magic happen!

## Authors
- **7.x-1.x** release created by [@MartinElvar](https://twitter.com/#!/MartinElvar)
- **7.x-2.x** release by [jtwalters](https://www.drupal.org/user/1052318)
