/*
 * Proccess all timeago span tags.
 */

(function ($) {
    Drupal.behaviors.hap_timeago_render = {
        attach: function (context, settings) {
            if (typeof Drupal.settings.hap_timeago == "object") {
                var current = new Date();
                for(var nodeId in Drupal.settings.hap_timeago) {
                    date = new Date(Drupal.settings.hap_timeago[nodeId]*1000);
                    $("span.time-ago-" + nodeId).html(timeAgo(current,date).toString());
                }
            }
        }
    }
})(jQuery);


function timeAgo(current, previous) {

    var msPerMinute = 60 * 1000;
    var msPerHour = msPerMinute * 60;
    var msPerDay = msPerHour * 24;
    var msPerMonth = msPerDay * 30;
    var msPerYear = msPerDay * 365;

    var elapsed = current - previous;

    if (elapsed < msPerMinute) {
        return  Drupal.formatPlural(Math.round(elapsed/1000), '1 second ago', '@count seconds ago');
    }

    else if (elapsed < msPerHour) {
        return  Drupal.formatPlural(Math.round(elapsed/msPerMinute), '1 minute ago', '@count minutes ago');
    }

    else if (elapsed < msPerDay ) {
        return Drupal.formatPlural(Math.round(elapsed/msPerHour ), '1 hour ago', '@count hours ago');
    }

    else if (elapsed < msPerMonth) {
        return Drupal.formatPlural(Math.round(elapsed/msPerDay), '1 day ago', '@count days ago');
    }

    else if (elapsed < msPerYear) {
        return Drupal.formatPlural(Math.round(elapsed/msPerMonth), '1 month ago', '@count months ago');
    }

    else {
        return Drupal.formatPlural(Math.round(elapsed/msPerYear ), '1 year ago', '@count years ago');
    }
}
