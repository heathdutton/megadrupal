/**
 * @file
 * Handles AJAX submission for AvaTax address validation.
 */

(function($) {
    function commerce_avatax_attach_events() {
        $('input.checkout-continue').unbind('click').click(function(event) {
            if ($('form.commerce_checkout_form').hasClass('commerce_avatax_revalidate_addr')) {
                if (Drupal.settings.commerce_avatax.commerce_avatax_address_do_country_validate
                        && Drupal.settings.commerce_avatax.commerce_avatax_address_validate_countries) {
                    var enabled_countries = Drupal.settings.commerce_avatax.commerce_avatax_address_validate_countries;
                    var selected_country = $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' select.country').val();
                    if ($.inArray(selected_country, enabled_countries) < 0) {
                        return true;
                    }
                }
                event.preventDefault();
                $('input.address-validate-btn').trigger('mousedown');
                return false;
            }
            return true;
        });
    }

    function commerce_avatax_handle_errors(errors) {
        for(var index in errors) {
            var parts = index.split('][');
            var name = parts[0] + "[" + parts.slice(1).join("][") + "]";
            var $el = $('input[name="' + name + '"]');
            $el.addClass('error');
            $el.parent().append('<div class="inline error message">' + errors[index] + '</div>');
        }
    }

    Drupal.behaviors.avataxAddressValidate = {
        attach: function(context, settings) {
            $('form.commerce_checkout_form').addClass('commerce_avatax_revalidate_addr');
            commerce_avatax_attach_events();
        }
    };
    Drupal.ajax.prototype.commands.afterAddressValidation = function(ajax, response, status) {
        $('.error.message').remove();
        $('.error').removeClass('error');
        if (response.errors) {
            var continue_btn = $('input.checkout-continue.checkout-processed');
            $(continue_btn[0]).show().attr('disabled', false);
            $(continue_btn[1]).remove();
            $('span.checkout-processing').addClass('element-invisible');
            commerce_avatax_handle_errors(response.errors);
            return;
        }
        if (response.validation_result === false) {
            // This should never happen!
            var continue_btn = $('input.checkout-continue.checkout-processed');
            $(continue_btn[0]).show().attr('disabled', false);
            $(continue_btn[1]).remove();
            $('span.checkout-processing').addClass('element-invisible');
            commerce_avatax_attach_events();
            return;
        }
        if (response.validation_result.result != 'valid') {
            var continue_btn = $('input.checkout-continue.checkout-processed');
            $(continue_btn[0]).show().attr('disabled', false);
            $(continue_btn[1]).remove();
            $('span.checkout-processing').addClass('element-invisible');
            var buttons = [];
            if (response.validation_result.result == 'needs correction') {
                buttons = [
                    {
                        text: "Use recommended",
                        click: function() {
                            $('form.commerce_checkout_form').removeClass('commerce_avatax_revalidate_addr');
                            var $continue_btn = $('input.checkout-continue');
                            $continue_btn.clone().insertAfter($continue_btn).attr('disabled', true).next().removeClass('element-invisible');
                            $continue_btn.hide();
                            var selected = jQuery('#address_validation_wrapper .form-type-radios.form-item-addresses input[name="addresses"]').val();
                            var address = response.validation_result.suggestions[selected];
                            $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' select.country').val(address.country);
                            $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' input.thoroughfare').val(address.line1);
                            $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' input.permise').val(address.line2);
                            $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' input.locality').val(address.city);
                            if ($('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' select.administrative-area')) {
                                $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' select.administrative-area').val(address.state);
                            }
                            $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' input.postal-code').val(address.postal_code);
                            $(this).dialog("close");
                            $('form.commerce_checkout_form').submit();
                        }
                    },
                    {
                        text: "Use as entered",
                        click: function() {
                            $('form.commerce_checkout_form').removeClass('commerce_avatax_revalidate_addr');
                            var $continue_btn = $('input.checkout-continue');
                            $continue_btn.clone().insertAfter($continue_btn).attr('disabled', true).next().removeClass('element-invisible');
                            $continue_btn.hide();
                            $(this).dialog('close');
                            $('form.commerce_checkout_form').submit();
                        }
                    },
                    {
                        text: "Enter again",
                        click: function() {
                            $(this).dialog('close');
                            $('input.checkout-continue').unbind('click').click(function() {
                                var $this = $(this);
                                $this.clone().insertAfter($this).attr('disabled', true).next().removeClass('element-invisible');
                                $this.hide();
                            });
                            commerce_avatax_attach_events();
                        }
                    }
                ]
            }
            else if (response.validation_result.result == 'invalid') {
                buttons = [
                    {
                        text: "Let me change the address",
                        click: function() {
                            $(this).dialog("close");
                            $('input.checkout-continue').unbind('click').click(function() {
                                var $this = $(this);
                                $this.clone().insertAfter($this).attr('disabled', true).next().removeClass('element-invisible');
                                $this.hide();
                                commerce_avatax_attach_events();
                            });
                        }
                    },
                    {
                        text: "Use the address anyway",
                        click: function() {
                            $('form.commerce_checkout_form').removeClass('commerce_avatax_revalidate_addr');
                            var $continue_btn = $('input.checkout-continue');
                            $continue_btn.clone().insertAfter($continue_btn).attr('disabled', true).next().removeClass('element-invisible');
                            $continue_btn.hide();
                            $('form.commerce_checkout_form').submit();
                        }
                    }
                ]
            }
            $("#address_validation_wrapper").html(response.validation_result.msg);
            $("#address_validation_wrapper").dialog({
                height: 500,
                width: 800,
                modal: true,
                title: Drupal.t('Confirm your shipping address'),
                resizable: false,
                draggable: false,
                buttons: buttons,
                dialogClass: 'no-close',
                closeOnEscape: false
            });
            if (!$("#address_validation_wrapper").dialog('isOpen')) {
                $("#address_validation_wrapper").dialog('open');
            }
        }
        else {
            if (Drupal.settings.commerce_avatax.commerce_avatax_autocomplete_postal_code) {
                var address = response.validation_result.suggestions[0];
                $('fieldset.' + Drupal.settings.commerce_avatax.commerce_avatax_address_validation_profile + ' input.postal-code').val(address.postal_code);
            }
            $('form.commerce_checkout_form').submit();
        }
    };
}(jQuery));
