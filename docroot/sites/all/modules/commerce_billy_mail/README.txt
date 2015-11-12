Extends the great "Commerce Billy" with mailing capabilities via Rules:

    Send out PDF invoices via email on creation (example)
    Send a copy to a predefined Shop email address
    ... and anything else possible via commerce billy pdf and Rules!

---------------------------------
Installation and configuration

    1. Use the usual Drupal Module installation procedure.
    2. Afterwards configure your module at: admin/commerce/config/billy-invoice/billy-mail
    3. Add the new action "Send commerce billy invoice mail." to the commerce order rule that shell trigger the mail sending. In most cases this is the Rule "Invoice order on completion" (Tech. name: "rules_commerce_billy_invoice_order_on_completion"). Select "commerce-order" in the dialog.
    4. Now your customers automatically receive invoice E-Mails with PDF E-Mail attachments when the order status is set to "completed" (or other rule conditions as you have configured).

---------------------------------
Typical issues
Receiving emails twice?
Check, if you have set your email address as cc recepient in the module settings.
Check your rules configration by enabling Show debug information => ALWAYS in Rules settings (admin/config/workflow/rules/settings)

No invoice emails sent?
See installation instructions above. If you don't set up a proper rule, no emails will be sent.

---------------------------------
Dependencies

    Commerce Billy
    MIME Mail
    Token (will become optional in the future and is currently missing in the .info file dependencies, sorry! I'll fix this ASAP.)

---------------------------------
Development proudly sponsored by German Drupal Friends & Companies:

webks: websolutions kept simple (http://www.webks.de)
and
DROWL: Drupalbasierte Lösungen aus Ostwestfalen-Lippe (http://www.drowl.de)
