
DESCRIPTION

The Kaje™ Picture Passwords service is a proof-of-knowledge replacement for
typed passwords. 

PICTURE PASSWORD QUICK INSTALL

Install and enable like any other Drupal module. There are no depending modules.
To complete the configuration of Kaje™ Picture Passwords on your site, you'll
need to create your Kaje Admin account here:
https://kaje.authenticator.com/index.php/admin
When prompted for the DOMAIN and REDIRECT values, use the same domain name,
namely that of your site.

Once you have created your account and verified ownership of your domain, you'll
be given a Requesting Party ID and Secret. Enter those two at the Picture
Passwords configuration page, admin/config/system/picture-password, and you're
good to go!

FREQUENTLY ASKED QUESTIONS

Q: Can I implement Kaje on a site that does NOT use SSL (HTTPS)?
A: No. For security, Kaje requires that your website be SSL-enabled.

Q: Is the use of the Kaje™ Picture Passwords service free?
A: The Kaje™ Picture Passwords service is free for the first 10,000 successful
   proofs of knowledge.

Q: Do I need to sign up for anything in order to implement Kaje on my site?
A: As a site administrator you will need to create a Requesting Party Admin
   account. Sign-up at https://kaje.authenticator.com/index.php/admin is free
   and only requires a valid email address.

Q: Where can I find additional information about Kaje™ Picture Passwords?
A: Website: http://picturepassword.info
   YouTube: https://www.youtube.com/watch?v=vMB5RhJIHz8

Q: Where do I obtain a Requesting Party ID and Secret for testing purposes, e.g.
   https://localhost?
A: This is not available. You start out with 10,000 free PoKs, which can be used
   for testing.

Q: What Kaje-related information does this module store on my site's database?
A: Apart from the Requesting Party ID and Secret entered by the site 
   administrator, the Picture Passwords module stores one Kaje user ID for each 
   Drupal user that has a Kaje account.

Q: Can I create an overview of users on my site that have Kaje accounts?
A: Yes, using the Views module, create a View of Users and various Kaje-related
   user fields will be available for display. No need to use a Relationship.

Q: What user data does Kaje store on its system?
A: Kaje maintains complete anonymity of users. It receives no user data other
   than the unique random code that your site sends (and the original picture
   password that is set up by the user).

Q: What data does Kaje send back?"
A: For each request, Kaje sends back only the unique random code and a status
   code. Kaje also provides statistical data in the Admin panel, about login
   attempts and successes, etc.

Q: What site data does Kaje store on its system?
A: Kaje retains data for each Requesting Party, including the site's number of
   successes and failures, and timestamps of the most recent. Cloud server web
   logs may temporarily contain additional web traffic information but Kaje
   does not use this information, in the interests of privacy.

Q: How does Kaje maintain privacy, anonymity, and security?
A: Kaje is structured to provide maximum privacy, anonymity, and security.
   Further details are available on the Requesting Party admin site.
   - Kaje uses the highest level of encryption and security best
     practices, and every step in the process has been structured and vetted
     to maintain that security.
   - Kaje has no way to determine who a user is.
   - All data communications are done in an anonymized, encrypted environment.
   - The user's interactions with Kaje never travel through the Requesting
     Party's servers.
   - Since the user's identity and the password are maintained by two
     different entities (the Requesting Party, and Kaje), hacking someone's
     Picture Password would require compromising two different servers run by
     two different companies in two different places on the Internet.
   - Kaje's SAAS runs in the cloud, further complicating that process.

Q: What is Entropy, and how does that apply to passwords?
A: Information Entropy is one measure of how many bits of information are
   contained in a message. It is not the whole story, but it is useful for
   comparisons. Higher entropy is better. The NIST paper "Picture Password: A
   Visual Login Technique for Mobile Devices" discusses this in detail, for an
   earlier and less secure picture password implementation example.
   http://csrc.nist.gov/publications/nistir/nistir-7030.pdf

Q: Can anyone offer Picture Passwords for the Web?
A: Kaje™ Picture Passwords for the Web are covered by several patents either
   accepted or pending. Other implementations would be restricted to
   non-infringing methods."
