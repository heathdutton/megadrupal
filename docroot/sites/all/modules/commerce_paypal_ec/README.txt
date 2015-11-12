DESCRIPTION
-----------

This module is an extension of Commerce Paypal module that currently integrates
Paypal with the implementation methods: Website Payments Standard and Website
Payments Pro. The main differences of Paypal Express Checkout VS the other
implementation methods are:

- Is globally available unlike Website Payments Pro that is restricted
  for few countries.

- Is really easy to open a Bussines Account that provide the option of
  Paypal Express Checkout.

- The redirection to Paypal Payment page doesn't have all the parameters
  sent via POST, this implementation method promote the use of an API to
  configure the behavior of the Paypal Payment Page. All the requests
  with sensitive information travel via HTTPS.

- Paypal Express is the recommeded method from Paypal Reps. is
  considered more advanced solution than the Standard version.

- Allows more customizations than the Standard version for example: add
  a logo and custom header colors in Paypal page, decide if creating a
  Paypal account is mandatory or not for the payer, autofill the payer
  address in Paypal Form, decide the main payment form at Paypal (Paypal
  account or Credit Card), decide if Paypal confirmed shipping address
  is mandatory or not, decide if display the shopping cart items in
  payment page and other few options.

- Many of the backend operations that with Standard method are executed
  in Paypal backend could be integrated in the backend of Drupal using
  the API calls. In this module we intagrate with the Commerce payment
  tranasctions the option to execute the capture for the Paypal payments
  of type Authorization. In similar manner could be integrated the
  refunds, sale reports, etc.

- Doesn't require the use of IPN (instant payment notifications) because
  the reception of all the related transaction info is instantly when
  receiving the response of the API calls.


WORKFLOW
--------

The Paypal Payment page compounds three steps to get the payment:

1. SetExpressCheckout this step allows to configure the Payment page
   and retrieves a Token that is used to redirect the client to Paypal
   page.

2. GetExpressCheckoutDetails once the user add all the payment
   information in Paypal site he continue and comeback to vendor site,
   this method is called to get all the details of the transaction to do
   some validations.

3. DoExpressCheckoutPayment this call execute the Payment.

The Paypal API could be implemented via SOAP or NVP protocols. This module
implementes the second because is easier and as this API only allow few methods
is overcomplicated to add a SOAP layer for only few calls tha we need to
process.

INSTALLATION
------------

Pending to write.

CREDITS
-------

- Thanks to Ryan Szrama that do the implementation of Paypal Website Payment
  Standard that was the example I followed to learn the Commerce Paypal Payment
  API.  

- The development of this module was sponsored by Bluespark Labs.

