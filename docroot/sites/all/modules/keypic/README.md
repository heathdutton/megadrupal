We use captcha module as module which provide API. System admin will decide
which form will be protected by which challenge.

Thing we do is: implement captcha API to provide KeyPic challenge. So these
features are already implemented:

> Login protection
> Registration protection
> Change User/Password protection
> Forums protection

keypic_comment.module do this, and alter to comment forms to display spam %.

> Comments protection
> In Comments approvation show a new column called "Keypic spam" and show % of
> spamming and button to report smap and delete it. (use the provided
> reportSpam() function)

keypic_user.module do this, it also alter to user admin forms to display spam %.

> In new users registration show a new column called "Keypic spam" and show % of
> spamming and button to report spam and delete it. (use the provided
> reportSpam() function)

Admin can config which role can skip captcha. We do not need any back door.

> Provide a backdoor system to escape Login protection.
> Just in case of false positive spam, it is needed this kind of backdoor

About:
> After completing the plugin you have to provide it to keypic managers to
> try it on some preinstalled platform installation.

My dev site is http://keypic.dev.drupal.vc/
