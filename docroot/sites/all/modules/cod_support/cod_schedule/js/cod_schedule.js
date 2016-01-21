/**
 * Attaches the drag and drop behavior to scheduler
 */
(function ($) {
    Drupal.behaviors.cod_schedule = {
        attach: function (context) {
            $( ".sortable-sessions", context).sortable({
                items: "li:not(.unsortable)",
                connectWith: ".sortable-sessions",
                receive: function( event, ui ) {

                    console.info(ui);
                    $(ui.item).addClass( "unsortable" );

                    var timeslot = event.target;
                    var session = ui.item;
                    var timeslot_id = $(timeslot).attr('data-timeslot');
                    var session_nid = $(session).attr('data-nid');
                    var timeroom_id = $(timeslot).attr('id');

                    $(session).append('<span class="throbber"> Saving...</span>');
                    $.post( Drupal.settings.basePath + "ajax/cod-schedule/update-session", { session:session_nid, timeslot:timeslot_id, timeroom:timeroom_id }, function( data ) {
                        $(timeslot).attr('data-timeslot', data);
                        $(session).children('.throbber').remove();
                        $(ui.item).removeClass( "unsortable" );
                    }, "json");
                }
            }).disableSelection();
        }
    };

    Drupal.behaviors.cod_scheduler = {
        attach: function (context) {
            $("#schedule-items-filter", context).keyup(function() {
                var filter = $(this).val(), count = 0;
                $(".pane-cod-schedule-sessions-pane li").each(function() {
                    if ($(this).text().search(new RegExp(filter, "i")) < 0) {
                        $(this).hide();
                    }
                    else {
                        $(this).show();
                        count++;
                    }
                });

                var numberItems = count;
                //$("#filter-count").text("Number of Comments = "+count);
            });

            $("#schedule-items-type-select", context).change(function() {
                var type = $( this ).val();
                $.post( Drupal.settings.basePath + "ajax/cod-schedule/items/" + type, { }, function( data ) {
                    $('#cod-schedule-wrapper .panel-col-last .sortable-sessions').replaceWith(data);
                    Drupal.attachBehaviors('#cod-schedule-wrapper');
                }, "json");
            });
        }
    };

})(jQuery);
