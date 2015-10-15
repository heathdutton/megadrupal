/*
 * Send a checkout request to Affirm API.
 */

(function () {

    /*
     * Organises and sends content to the Affirm server.
     */
    Drupal.behaviors.commerce_affirm_checkout = {

        attach: function (context, settings) {

            var settings = Drupal.settings.commerce_affirm;

            affirm.checkout({
                "merchant": {
                    "user_confirmation_url": settings.ConfirmUrl,
                    "user_confirmation_url_action": "POST",
                    "user_cancel_url": settings.CancelUrl
                },
                "config": {
                    "financial_product_key": settings.FinancialProductKey
                },
                "billing": {
                    "name": {
                        "full": settings.BillingFullName,
                        "first": settings.BillingFirstName,
                        "last": settings.BillingLastName
                    },
                    "address": {
                        "line1": settings.BillingAddressLn1,
                        "line2": settings.BillingAddressLn2,
                        "city": settings.BillingAddressCity,
                        "state": settings.BillingAddressState,
                        "zipcode": settings.BillingAddressPostCode,
                        "country": settings.BillingAddressCountry
                    },
                    "email": settings.Email,
                    "phone_number": settings.BillingTelephone
                },
                "items": settings.items,
                "shipping": {
                    "name": {
                        "full": settings.ShippingFullName,
                        "first": settings.ShippingFirstName,
                        "last": settings.ShippingLastName
                    },
                    "address": {
                        "line1": settings.ShippingAddressLn1,
                        "line2": settings.ShippingAddressLn2,
                        "city": settings.ShippingAddressCity,
                        "state": settings.ShippingAddressState,
                        "zipcode": settings.ShippingAddressPostCode,
                        "country": settings.ShippingAddressCountry
                    },
                    "email": settings.Email,
                    "phone_number": settings.ShippingTelephone
                },
                metadata: {
                    shipping_type: settings.ShippingType
                },

                "shipping_amount": settings.ShippingTotal,
                "tax_amount": settings.TaxAmount,
                "total": settings.ProductsTotal
            });

            // submit and redirect to checkout flow
            affirm.checkout.post();
        }
    }

})();
