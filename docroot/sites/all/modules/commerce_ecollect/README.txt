CONTENTS OF THIS FILE
---------------------
* About this module.
* Installation.
* Usage.
* Why do we use rules to set payment parameters?.

ABOUT THIS MODULE
-------------
eCollect® (http://www.avisortech.com) is a colombian payment
gateway. With this module, you can integrate your store with it, 
because it is based on rules that define payment parameters. Useful
to change those parameters depending on several conditions.

INSTALLATION
-------------
- Download the NuSOAP library from this link, and place it
inside sites/all/libraries/nusoap.

You should see a PHP classs sites/all/libraries/nusoap/lib/nusoap.php.
  
- Enable the module.

USAGE
-------------
When module is enabled, you need to add a custom rule to set
right payment parameters. Go to admin/config/workflow/rules, and
create a new with your desired react on event. You can set
conditions, if you need. Now, add an action Set payment parameters,
which may be located inside group ECollect. Keep in mind that some
of these field values must be asked to Avisor Technologies. For more
information, please visit http://www.avisortech.com.

Warnings:
- If your rule is not invoked during checkout process,
then payment parameters will not be established. So, customers will
not be redirected to the payment gateway.

- Do not forget to setup permissions in admin/people/permissions to
allow anonymous users view orders statuses, if needed.

- You need to ensure that your default currency is the same that
you've contracted with Avisor Technologies. If not, you wouldn't
be redirected to the payment gateway.

WHY DO WE USE RULES TO SET PAYMENT PARAMETERS?
-------------
That's simple. Avisor gives you the possibility to have several
"payment pages" as you want. For example, you can setup a page to
pay events with following fields: Full name, Personal ID, Phone.
Also, a different page to pay courses with following fields: 
Full name, Personal ID, Phone, Customer ID, Course ID. Therefore,
you need to setup different values for each one. Tha's the trick!. 

Also (not tested yet) if you have a "multi-store website", you
could setup different values per shop.
