/**
 * @file
 * Javascrip file for User Navigation module.
 */

(function ($)
{
    Drupal.behaviors.userNav = {
        attach: function (context, settings) {
            var elt = document.getElementById("clock");
            var month = new Array();
            month[0] = Drupal.t('Jan');
            month[1] = Drupal.t('Feb');
            month[2] = Drupal.t('Mar');
            month[3] = Drupal.t('Apr');
            month[4] = Drupal.t('May');
            month[5] = Drupal.t('Jun');
            month[6] = Drupal.t('Jul');
            month[7] = Drupal.t('Aug');
            month[8] = Drupal.t('Sep');
            month[9] = Drupal.t('Oct');
            month[10] = Drupal.t('Nov');
            month[11] = Drupal.t('Dec');

            // Run again in 1 second.
            window.setInterval(function () {
                var now = new Date();
                var dt = "<div class='clock-date'>" + month[now.getMonth()] + " " + (now.getDay() + 1) + ", " + now.getFullYear() + "</div>";
                elt.innerHTML = dt + "<div class='clock-time'>" + now.toLocaleTimeString() + "</div>";
            }, 1000);

            $('.user-nav-slide').once('user-nav', function () {
                $(".user-menu-container").css("margin-left", "-195px");
                $(".user-menu-container").addClass("closed");
                $(".user-nav-slide").click(function () {
                    if ($(".user-menu-container").hasClass("closed")) {
                        $(".user-menu-container").animate({"margin-left": "0px"});
                        $(".user-menu-container").removeClass("closed");
                    } else {
                        $(".user-menu-container").animate({"margin-left": "-195px"});
                        $(".user-menu-container").addClass("closed");
                    }
                });
            });
        }
    };
}(jQuery));
