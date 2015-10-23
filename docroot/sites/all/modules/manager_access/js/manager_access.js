(function ($) {

    Drupal.behaviors.managerAccess = {
        attach: function (context, settings) {
            var table = $("#manager-access-fields-settings-form").find("table");

            table.find("tr").each(function (tr_n, tr_e) {
                var all = $(this).find("td:eq(0)");
                var first_col = $(this).find("td:eq(1)");
                var not_all = $(this).find("td:not(:first)");


                // generate All column
                var c = first_col.find("input[type=checkbox]").length;
                var all_html = '<fieldset class="manager-access-fields-all-fieldset">';
                for (i = 1; i <= c; i++) {
                    all_html += '<div class="manager-access-fields-all-checkboxes"><input class="users form-checkbox" type="checkbox" name="all" value="1"></div>'
                }
                all_html += "</fieldset>";
                all.html(all_html);


                // set checked/not checked all columns
                all.find("input[type=checkbox]").click(function () {
                    var n = all.find("input[type=checkbox]").index(this);
                    var checked = $(this).is(':checked');
                    table.find("tr").eq(tr_n).find("td:not(:first)").each(function () {
                        $(this).find("input[type=checkbox]:eq(" + n + ")").attr('checked', checked);
                    });
                });

                // check if all columns checked
                manager_access_check_checkboxes(tr_n, tr_e);
                not_all.each(function (i, e) {
                    $(e).find("input[type=checkbox]").click(function () {
                        manager_access_check_checkboxes(tr_n, tr_e);
                    });
                });
            });
        },

        completedCallback: function () {
            // Do nothing. But it's here in case other modules/themes want to override it.
        }
    }



    function manager_access_check_checkboxes(tr_n, tr_e){
        var c = $(tr_e).find("input[type=checkbox]").length
        for (i=0; i<c; i++){
            var checked = true;
            $(tr_e).find("td:not(:first)").each(function () {
                if (!$(this).find("input[type=checkbox]:eq(" + i + ")").is(':checked')) {
                    checked = false;
                }
            });

            // check/uncheck all
            $(tr_e).find("td:eq(0)").find("input[type=checkbox]:eq(" + i + ")").attr("checked", checked);
        }

    }

})(jQuery);
