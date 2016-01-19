CONTENTS OF THIS FILE
---------------------
  
* Introduction
* Installation
* Configuration
* FAQ
* Maintainers

INTRODUCTION
------------

The NoMoreCaptchas module by Oxford BioChronometrics uses biochronometric
behavior to determine if the thing knocking on your door is a human or a
bot. We send all bots away and let humans in.

INSTALLATION
------------

* Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

* Please register (it's free!) at http://www.NoMoreCaptchas.com/register
   to keep those bots away! Fill that form with appropriate options.

* System will send email with License key to your email address which
   you entered in the form.
 
* Enter your License key in Administration » People » NoMoreCaptchas
  (admin/config/people/xb_nomorecaptchas) and click on Validate License
  key button. Check info preloaded on form!

* You're secured now!

FAQ
---

Q: If I'm using the free version, do I still have to register?
A: Yes. In order to ensure your site is protected, we must open access
   to our cloud-based service to your site. In order to do that, we need
   to know you want to connect to us. Please take a moment to register.
   It really takes just a minute or two!

Q: What makes NoMoreCaptchas different?
A: We don't use traditional Captcha, or problem-solving methods to make a
   user prove he or she is human. Most people find that annoying, and user
   studies show that 74% of the time, people will abandon a site that makes
   them solve a Captcha code. We also don't use blacklists and whitelists
   to cross check users. Those databases are as fallible as the humans who
   maintain them and can be wrong. And if you're the one who has been
   accidentally listed as spam, Spamhaus and the other 5 leading spam
   databases have a long, opaque procedure for getting yourself off of them.
   It's a frustrating experience. And even if you're fine, cross-checking
   database lists creates a lag for the user. What we use is a user's e-DNA,
   or Electronically Defined Natural Attributes, to determine whether the user
   is a bot or a human. Since even the best bots only poorly mimic a few human
   attributes, we detect them and block them from your site. And users don't
   need to do anything but be themselves, so they don't slow down and your
   engagement goes up.

Q: What exactly is biochronometric protection?
A: Bio means life and chrono means time. We devised unique signatures based
   to determine whether someone is acting alive over time based on hundreds
   of individual user inputs that people automatically generate just simply
   by being alive. The time we need to figure out if you're alive or a bot
   is just fractions of a second. And since bots can't fake being alive, we
   can be sure we block them and let only real humans enjoy your site.

Q: Is there a free version?
A: Yes. You can install and use NoMoreCaptchas for free for personal use and
   enjoy protection for up to 2,500 bot attacks per month. That is plenty for
   a small, personal site or blog.
   
   If your site is not for personal use, NoMoreCaptchas is free for the first
   30 days. After that, or even if you run a personal site and desire a
   greater level of protection and want more functionality in your dashboard,
   we have a variety of subscription services available. You can read about
   them at http://nomorecaptchas.com/pricing/

Q: Who shouldn't use NoMoreCaptchas?
A: Spammers, scammers, bots, trolls and other Internet bottom-dwellers really
   should avoid NoMoreCaptchas. We're out to make their lives difficult, so
   they really don't like us.

Q: What do I need to install NoMoreCaptchas?
A: Installing NoMoreCaptchas is easy. All you need are the module zip file
   and your License Key.

   To get your License Key, register your site with us at
   http://nomorecaptchas.com/register/. As soon as you do, we'll send you a
   license key and download the module from drupal.org.

   Registering allows us to create a unique key for your site that will make
   your NoMoreCaptchas keeps spam bots away and only allows humans through
   the door.

Q: Is NoMoreCaptchas compatible with BuddyPress?
A: Yes. We have tested it up to the latest release (BuddyPress 2.0.1) and it
   is compatible.

Q: Which pages does NoMoreCaptchas protect?
A: NoMoreCaptchas prevents spambots from registering or logging in to your
   site, it also protects your contact forms and public comment sections. So
   the login page is protected and, if you use BuddyPress' registration
   page, that is also protected - and now so are your contact forms.

   In other words, it protects the pages where you would normally place a
   Captcha code.

Q: How do I set up protection for my contact forms?
A: We have optimized NoMoreCaptchas to work with Contact Form, the most 
   widely used contact form plugin. All you have to do is place the 
   NoMoreCaptchas code in the same place as your Contact Form shortcode and
   you're all set.We posted the instructions for you here:
   http://nomorecaptchas.com/configure-contact-form-7. If you use a different
   contact form plugin, please contact us at support@nomorecaptchas.com and
   we can help you set it up.

Q: How can I tell if NoMoreCaptchas is working?
A: Seeing NoMoreCaptchas in action is believing, and there's no better place
   to see it happen than your Dashboard, located at 
   http://nomorecaptchas.com/customer-dashboard/. If you have a free version
   of NoMoreCaptchas, you'll see the last 400 actions. If you're one of your
   subscribers (thanks!) you'll be able to customize your view.

   To log in to your NoMoreCaptchas Dashboard, visit the Customer Dashboard
   page (http://nomorecaptchas.com/customer-dashboard/) and enter your
   Authenticating Code and the domain you registered.

Q: I'm still getting spam, what's happening?
A: If NoMoreCaptchas is properly installed (see above) and you are able to
   see your dashboard, there are a few possible issues:

   You may not have registered. If you believe you did (or if you're a
   subscriber, if know you are up to date on your payment but are not listed
   as active), please contact us at support@nomorecaptchas.com

   If that's not the case, then your License Key was not properly entered
   into the control panel. Please be sure to copy and paste it directly
   from the email we sent you. If you no longer have your License Key,
   please contact us at support@nomorecaptchas.com so we can send you a new
   one.

   There may have been some spam registrations pending in your queue. Please
   make sure you have eliminated anyone who got in before you installed
   NoMoreCaptchas.

   The only other reason you might be getting spam is if an actual human
   decided to register on your site and spam you personally. Believe it or
   not, some people have that kind of time on their hands. That's not the
   way most spammers operate, so it's probably someone you recently beat in
   a game of WoW or didn't like something you said on Reddit.

Q: Is NoMoreCaptchas an accessible (508-compliant) solution?
A: Yes. Section 508 of the Americans with Disabilities Act (ADA) requires
   websites to be built in such a way that the blind, visually impaired and
   others with disabilities are able to use the site without issue. Most
   Captcha solutions are very difficult for those with visual impairments,
   and the audio solutions are a clumsy workaround at best. Since
   NoMoreCaptchas doesn't require users to do anything but be human, the
   path is made equally accessible to everyone.

MAINTAINERS
-----------

Current maintainers:
* Shekhar Aryan (nomorecaptcha) - https://drupal.org/user/3082253
* Kamran Zafar (kamranzafar) - https://drupal.org/user/344376
