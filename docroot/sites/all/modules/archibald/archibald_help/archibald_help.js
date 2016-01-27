(function ($) {

    Drupal.behaviors.archibald_help = {
      attach: function (context, settings) {
       archibald_help_handler();
      }
    };

    /**
     * switch to check if the page will loaded or we will vcalled by a ajax request
     * @var boolean
     */
    var archibald_help_entry_handler_first_load = true;
    /**
     * handle the form identifers
     */
    function archibald_help_handler() {
        if(archibald_help_entry_handler_first_load == true) {

            $("*[archibald_help_ident]").each(function() {
                if($('div[archibald_help_identifier="'+$(this).attr("archibald_help_ident")+'"]').length > 0) {
                    return;
                }
                var help_elm = $("<div class='archibald_help_elm' archibald_help_identifier='"+$(this).attr("archibald_help_ident")+"'></div>");
                help_elm.click(function() {
                    Drupal.CTools.Modal.show(archibald_modal_defaults);
                    $.ajax({
                        url: '/admin/help/archibald/'+ $(this).attr("archibald_help_identifier"),
                        dataType: 'json',
                        success: function(response, status) {
                            for (var i in response) {
                                if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                                    Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
                                }
                            }
                        }
                    });
                });

                var elm = "";
                var for_search = $(this).attr("archibald_help_selector");
                //console.log(for_search);
                if(for_search == undefined || for_search == "") {
                    elm = $("#"+$(this).attr("id")+" > div > label");

                    if(elm.length <= 0) {
                        elm = $('label[for="'+$(this).attr("id")+'"]');
                    }
                }
                else {
                    var tmpArr = for_search.split("|");
                    elm = $('label['+tmpArr[0]+'="'+tmpArr[1]+'"]');
                }
                //console.log(elm);
                $(elm).css("display", "inline");
                $(elm).after(help_elm);
                $(help_elm).after($("<div></div>"));

            });
            archibald_help_entry_handler_first_load = false;
        }
    }

})(jQuery);
