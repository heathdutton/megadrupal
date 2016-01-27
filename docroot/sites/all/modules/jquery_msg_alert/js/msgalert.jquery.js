/*
 * jQuery Msg Alert
 *
 * Easily create alerts boxes for displaying messages to your users. 
 * This plugins uses jQuery UI Dialog and jQuery Timers to show any html into alerts.
 * Developed by Carlos Carvalhar (http://carvalhar.com) when working at PixFly (http://pixfly.com.br)
 *  
 * Depends:
 *	jQuery  (http://jquery.com)
 *  jQuery UI Core (http://jqueryui.com)
 *  jQuery Timers (http://plugins.jquery.com/project/timers)
 *
 */
(function ($) {
    $.fn.MsgAlert = function (options) {
        //Set the default values, use comma to separate the settings, example:  
        var defaults = {
            alertWidth: 200,
            alertHeight: 130,
            autoClose: true,
            wait: true, //wait sometime before pop an alert
            closeTime: 5000, //time to wait untill the alert closes by its own
            intervalTime: 500, //time between the creation of one alert and another one
            cssPosition: 'fixed', // css position, fixed is the default
            cssRight: 10,
			alertTitleDefault: 'Msg Alert', //will appear at the dialog title bar if title inst' set at the selector
            alertContentDefault: '<p>Hi there!</p>',
            className: 'my-alert',
            interSpaceHeight: 30, // space between one alert and the other
            initialSpaceHeight: 10 // space from the bottom of the page
        }
        var options = $.extend(defaults, options);
        var alertNumber = $('.msg-alert').length + 1; // because it stats with 0 when there isn't any alert
        return this.each(function () {
            var alertBottomSpace = options.initialSpaceHeight;
            var alertContent = $(this).html();
            var alertTitle = $(this).attr('title');
            //deleting default html
            $(this).remove();

            //creating the new div above the body content
            if (alertContent == "") {
                alertContent = options.alertContentDefault;
            }
            var newMsgAlert = $('<div>' + alertContent + '</div>');
            $(newMsgAlert).attr("id", "MsgAlert-" + alertNumber);
            $('body').append(newMsgAlert);
            //the title that will appear at the dialog title bar
            if (alertTitle == "") {
                alertTitle = options.alertTitleDefault;
            }

            $(newMsgAlert).dialog({
                autoOpen: false,
                draggable: false,
                hide: 'fast',
                modal: false,
                resizable: false,
                position: ['right', 'bottom'],
                dialogClass: options.className,
                title: alertTitle,
                close: function (event, ui) {
                    closeThisAlert(event, ui, $(newMsgAlert));
                },
                minHeight: 0,
                width: options.alertWidth,
                height: options.alertHeight
            });

            $(newMsgAlert).hide();
            $("#MsgAlert-" + alertNumber).parent().addClass(options.className);
            $("#MsgAlert-" + alertNumber).parent().addClass('msg-alert');
            $(newMsgAlert).show();

            //there is a bug here, when you close the alert and a new one is being created, some pixels are added
            for (i = 1; i < alertNumber; i++) {
                alertBottomSpace += $("#MsgAlert-" + i).parent().height() + options.interSpaceHeight;
            }

            //Using an interval to add new alerts with some time between them
            //setting wait:false will pop all alerts instantaneously 
            if (options.wait) {
                $("#MsgAlert-" + alertNumber).oneTime((options.intervalTime * alertNumber), function () {
                    showThisAlert($(this), alertBottomSpace)
                });
            } else {
                showThisAlert($("#MsgAlert-" + alertNumber), alertBottomSpace);
            }

            alertNumber++;

            function showThisAlert(myMsgAlert, alertBottom) {
                $(myMsgAlert).dialog('open');
                $(myMsgAlert).parent().css({
                    position: options.cssPosition,
                    bottom: alertBottom + "px",
                    right: options.cssRight + "px",
                    top: "auto",
                    left: "auto"
                });
                $(myMsgAlert).fadeIn("slow");
                if (options.autoClose) {
                    $(myMsgAlert).oneTime(options.closeTime, function () {
                        $(myMsgAlert).dialog('close');
                    });
                }
            }

            function closeThisAlert(event, ui, myMsgAlert) {
                //Fadeout and remove the alert html
                $(myMsgAlert).fadeOut("fast");
                $(myMsgAlert).remove();
                //changing ao ID from the alerts left
                $('.msg-alert').each(function (index) {
                    $(this).children('.ui-dialog-content').attr("id", "MsgAlert-" + (index + 1));
                });

                var totalAlerts = $('.msg-alert').length;
                for (i = totalAlerts; i > 0; i--) {
                    if (i == 1) {
                        // i == 1 means that it's the first alert, the one at the bottom
                        $("#MsgAlert-1").parent().animate({
                            bottom: options.initialSpaceHeight + "px"
                        }, 500);
                    } else {
                        var alertBottomSpace = options.initialSpaceHeight;
                        for (a = 1; a < i; a++) {
                            alertBottomSpace += $("#MsgAlert-" + a).parent().height() + options.interSpaceHeight;
                        }
                        $("#MsgAlert-" + i).parent().animate({
                            bottom: alertBottomSpace + "px"
                        }, 500);
                    }
                }
            }
        });
    };
})(jQuery);