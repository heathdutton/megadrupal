jQuery(document).ready(function () {
    //handleResponse(true, "");
    jQuery("#fade, #lr-loading").click(function () {
        jQuery('#fade, #lr-loading').hide();
    });

});
function show_birthdate_date_block() {
    var maxYear = new Date().getFullYear();
    var minYear = maxYear - 100;
    if (jQuery('body').on) {
        jQuery('body').on('focus', '.loginradius-raas-birthdate', function () {
            jQuery('.loginradius-raas-birthdate').datepicker({
                dateFormat: 'mm-dd-yy',
                maxDate: new Date(),
                minDate: "-100y",
                changeMonth: true,
                changeYear: true,
                yearRange: (minYear + ":" + maxYear)
            });
        });
    } else {
        jQuery(".loginradius-raas-birthdate").live("focus", function () {
            jQuery('.loginradius-raas-birthdate').datepicker({
                dateFormat: 'mm-dd-yy',
                maxDate: new Date(),
                minDate: "-100y",
                changeMonth: true,
                changeYear: true,
                yearRange: (minYear + ":" + maxYear)
            });
        });
    }

}
function unLinkAccount(name, id) {
    handleResponse(true, "");
    if (confirm('Are you sure you want to unlink!')) {
        jQuery('#fade').show();
        var array = {};
        array['value'] = 'accountUnLink';
        array['provider'] = name;
        array['providerId'] = id;
        var form = document.createElement('form');
        var key;
        form.action = '';
        form.method = 'POST';
        for (key in array) {
            var hiddenToken = document.createElement('input');
            hiddenToken.type = 'hidden';
            hiddenToken.value = array[key];
            hiddenToken.name = key;
            form.appendChild(hiddenToken);
        }
        document.body.appendChild(form);
        form.submit();
    }
    else {
        jQuery('#fade').hide();
    }
}


function handleResponse(isSuccess, message, show, status) {
    status = status ? status : "status";
    if (status == "error" && window.LoginRadiusSSO) {
        LoginRadiusSSO.init(raasoption.appName);
        LoginRadiusSSO.logout(window.location);
    }
    if (typeof show != 'undefined' && !show) {
        jQuery('#fade').show();
    }
    if (message != null && message != "") {
        jQuery('#lr-loading').hide();
        jQuery('.messageinfo').text(message);
        jQuery(".messages").show();
        jQuery('.messageinfo').show();
        jQuery(".messages").removeClass("error status");
        jQuery(".messages").addClass(status);
        if (isSuccess) {
            jQuery('form').each(function () {
                this.reset();
            });
        }
    } else {
        jQuery(".messages").hide();
        jQuery('.messageinfo').hide();
        jQuery('.messageinfo').text("");
    }
}
function linking() {
    jQuery(".lr-linked-data, .lr-unlinked-data").html('');
    jQuery(".lr-linked").each(function () {
        jQuery(".lr-linked-data").append(jQuery(this).html());
    });
    jQuery(".lr-unlinked").each(function () {
        jQuery(".lr-unlinked-data").append(jQuery(this).html());
    });
    var linked_val = jQuery('.lr-linked-data').html();
    var unlinked_val = jQuery('.lr-unlinked-data').html();
    if (linked_val != '') {

        jQuery(".lr-linked-data").prepend('Connected Account<br>');
    }
    if (unlinked_val != '') {
        jQuery(".lr-unlinked-data").prepend('Choose Social Account to connect<br>');
    }
    jQuery('#interfacecontainerdiv').hide();
}
LoginRadiusRaaS.$hooks.setProcessHook(function () {

    // console.log("start process", '');
    jQuery('#lr-loading').show();
}, function () {
    if (jQuery('.lr_account_linking') && jQuery('#interfacecontainerdiv').text() != '') {
        linking();
    }

    //  jQuery('#lr-loading').hide();
});
LoginRadiusRaaS.$hooks.socialLogin.onFormRender = function () {
    jQuery('#lr-loading').hide();
    jQuery('#social-registration-form').show();
    show_birthdate_date_block();
    //ShowformbyId("social_registration_from");
};
function callSocialInterface() {
    LoginRadiusRaaS.CustomInterface(".interfacecontainerdiv", raasoption);
    jQuery('#lr-loading').hide();
}
function initializeLoginRaasForm() {
//initialize Login form
    LoginRadiusRaaS.init(raasoption, 'login', function (response) {
        handleResponse(true, "");
        raasRedirect(response.access_token);
    }, function (response) {
        if (response[0].description != null) {
            handleResponse(false, response[0].description, "", "error");
        }
    }, "login-container");
    jQuery('#lr-loading').hide();

}
function initializeRegisterRaasForm() {
    LoginRadiusRaaS.init(raasoption, 'registration', function (response, data) {
        handleResponse(true, "An email has been sent to " + jQuery("#loginradius-raas-registration-emailid").val() + ".Please verify your email address.");
    }, function (response) {
        if (response[0].description != null) {
            handleResponse(false, response[0].description, "", "error");
        }
    }, "registeration-container");
    jQuery('#lr-loading').hide();
}
function initializeResetPasswordRaasForm(raasoption) {
    //initialize reset password form and handel email verifaction
    var vtype = $SL.util.getQueryParameterByName("vtype");
    if (vtype != null && vtype != "") {
        LoginRadiusRaaS.init(raasoption, 'resetpassword', function (response) {
            handleResponse(true, "Password reset successfully");
            window.location = raasoption.emailVerificationUrl;
        }, function (response) {
            handleResponse(false, response[0].description, "", "error");
        }, "resetpassword-container");

        if (vtype == "reset") {
            LoginRadiusRaaS.init(raasoption, 'emailverification', function (response) {
                handleResponse(true, "");

                var reset_div = jQuery('a.active[href*="/user"]');
                reset_div.html('Reset Password');

                jQuery('#resetpassword-container').show();
                jQuery('#login-container').hide();
            }, function (response) {
                handleResponse(false, response[0].description, "", "error");
            });
        } else {
            LoginRadiusRaaS.init(raasoption, 'emailverification', function (response) {
                //On Success this callback will call
                handleResponse(true, "Your email has been verified successfully");
                //   ShowformbyId("login_form");
            }, function (response) {
                // on failure this function will call ‘errors’ is an array of error with message.
                handleResponse(false, response[0].description, "", "error");
            });
        }
    }
    jQuery('#lr-loading').hide();
}
function initializeSocialRegisterRaasForm() {
    //initialize social Login form
    LoginRadiusRaaS.init(raasoption, 'sociallogin', function (response) {
        if (response.isPosted) {
            handleResponse(true, "An email has been sent to " + jQuery("#loginradius-raas-social-registration-emailid").val() + ".Please verify your email address.");
            jQuery('#social-registration-form').hide();
        } else {
            handleResponse(true, "", true);
            raasRedirect(response);
        }
    }, function (response) {
        if (response[0].description != null) {
            handleResponse(false, response[0].description, "", "error");
            jQuery('#social-registration-form').hide();
        }
    }, "social-registration-container");

    jQuery('#lr-loading').hide();

}

function initializeForgotPasswordRaasForms() {
    //initialize forgot password form
    LoginRadiusRaaS.init(raasoption, 'forgotpassword', function (response) {

        handleResponse(true, "An email has been sent to " + jQuery("#loginradius-raas-forgotpassword-emailid").val() + " with reset Password link.");
    }, function (response) {
        if (response[0].description != null) {
            handleResponse(false, response[0].description, "", "error");
        }
    }, "forgotpassword-container");
    jQuery('#lr-loading').hide();
}
function initializeAccountLinkingRaasForms() {
    LoginRadiusRaaS.init(raasoption, "accountlinking", function (response) {
        handleResponse(true, "");
        raasRedirect(response);
    }, function (response) {
        jQuery('#fade').hide();
        if (response[0].description != null) {
            handleResponse(false, response[0].description, "", "error");
        }
    }, "interfacecontainerdiv");
    jQuery('#lr-loading').hide();
}
function initializeChangePasswordRaasForms() {
    initializeAccountLinkingRaasForms();
    LoginRadiusRaaS.passwordHandleForms("setpasswordbox", "changepasswordbox", function (israas) {
//var password_div = jQuery('a[href*="/changepassword"]');
        var password_div = jQuery('#page-title');
        if (israas) {
            password_div.html('Change Password');
            jQuery("#changepasswordbox").show();
        } else {
            password_div.html('Set Password');
            jQuery("#setpasswordbox").show();
        }
    }, function () {
        document.forms['setpassword'].action = window.location;
        document.forms['setpassword'].submit();
    }, function () {

    }, function () {
        document.forms['changepassword'].action = window.location;
        document.forms['changepassword'].submit();
    }, function () {

    });
    jQuery('#lr-loading').hide();
}
function raasRedirect(token, name) {
    if (window.redirect) {
        redirect(token, name);
    }
    else {
        var token_name = name ? name : 'token';
        var source = typeof lr_source != 'undefined' && lr_source ? lr_source : '';

        jQuery('#fade').show();
        var form = document.createElement('form');
        form.action = LocalDomain;
        form.method = 'POST';

        var hiddenToken = document.createElement('input');
        hiddenToken.type = 'hidden';
        hiddenToken.value = token;
        hiddenToken.name = token_name;
        form.appendChild(hiddenToken);
        if (source == 'wall_post' || source == 'friend_invite') {
            var hiddenToken = document.createElement('input');
            hiddenToken.type = 'hidden';
            hiddenToken.value = lr_source;
            hiddenToken.name = 'lr_source';
            form.appendChild(hiddenToken);
        }
        document.body.appendChild(form);
        form.submit();
    }
}
